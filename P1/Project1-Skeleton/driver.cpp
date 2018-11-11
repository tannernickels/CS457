#include <iostream>
#include <string> 
#include <tuple> 
#include <thread> 
#include <vector> 
#include <memory> 
#include <future>
#include <stdio.h>
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include "CommandLookup.h"
#include "chatUser.h"
#include "FileIO.h"
#include "serverData.h"
#include "eventHandler.h"
#include "server.h"

using namespace std;

//GLOBALS
server server;
bool ready = true;
char s_in[1500];

void to_lowercase(string& s){
    for (int i = 0; i < s.size(); i++){
        if(s[i] == ' ') break;
        s[i] = tolower(s[i]);
    }
}

bool isLeaving(string& msg, int& id, ssize_t val){
    if(val == -1 || val == 0)
        return false;
    else if (msg.substr(0,4) == "exit" || msg.substr(0,5) == "/quit"){ // replace for /QUIT functionality     
        cout << "[Client " << id << "] exits chat server" << endl;
        return false;  
    }
    else return true;
}

// HANDLES CLIENT CONNECTIONS
int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket, int id)
{
    cout << "Client " << id << " Unique Socket ID: " << clientSocket.get()->getUniqueIdentifier() << endl;
    cout << "Waiting for message from Client Thread " << id << std::endl << std::endl;
    string msg;
    ssize_t val;
    bool cont =true ;  

    std::future<chatUser> newActiveUser = std::async(& server::authenticateUser, &server, clientSocket, id);
    chatUser dumbUser = newActiveUser.get();
    

    while (cont) 
    {
        tie(msg,val) = clientSocket.get()->recvString();
        to_lowercase(msg);
        cont = isLeaving(msg, id, val);

        cout << "[Client "<< id <<"] " << msg << "     (value return = " << val << ")" << endl;
        if(!msg.empty() && val != -1 && val != 0){
        eventHandler consumer;
        thread taskConsumer(&eventHandler::processTask, &consumer, msg, std::ref(dumbUser), std::ref(server));
        taskConsumer.join();

        cout << "waiting for another message\n" << endl; 
        }
    }

    clientSocket.get()->sendString("goodbye\n"); 
    
    clientSocket.get()->closeSocket(); 
    server.removeActiveUser(dumbUser.getUsername());
    return 1; 
}

// TODO: add server shell functionality
void startShell(){
    while(true){
        string data;
        getline(cin, data);
        cout << "[Server] " << data << endl;
        // TODO: add more shell functionality
        if(data == "USERS")
        {
            server.listOnlineUsers();
        }
    
    }
}

int main(int argc, char * argv[])
{

    thread interactiveServer(startShell);

    cout << "Initializing Socket" << std::endl; 
    cs457::tcpServerSocket mysocket(2000);
    cout << "Binding Socket" << std::endl; 
    mysocket.bindSocket(); 
    cout << "Listening Socket" << std::endl; 
    mysocket.listenSocket(); 
    cout << "Waiting to Accept Socket\n" << std::endl;
    int id = 0; 
    vector<unique_ptr<thread>> threadList; 

    while (ready)
    { 
        shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val; 
        tie(clientSocket,val) = mysocket.acceptSocket();
        cout << "value for accept is " << val << std::endl; 
        cout << "Socket Accepted -- Client " << id << " has connected to the server" << std::endl; 
        unique_ptr<thread> t = make_unique<thread>(cclient,clientSocket,id); 
        threadList.push_back(std::move(t)); 
        
        id++; //not the best way to go about it. 
       // threadList.push_back(t);    
    }

    for (auto& t: threadList)
    {
        t.get()->join();
    }
    
        
    cout << "Server is shutting down after one client" << endl; 
    return 0; 
}