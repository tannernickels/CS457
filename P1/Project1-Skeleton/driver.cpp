#include <iostream>
#include <string> 
#include <tuple> 
#include <thread> 
#include <vector> 
#include <memory> 
#include <future>
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include "CommandLookup.h"
#include "chatUser.h"
#include "FileIO.h"
#include "serverData.h"
#include "eventHandler.h"

using namespace std;


serverData server_data;

//GLOBALS
bool ready = true;


// EventHandler Thread Functionality -- handles messages sent from a particular user/client
void messageHandler(string msg, chatUser user, shared_ptr<cs457::tcpUserSocket> clientSocket){
     if(msg.at(0) == '/'){
        CommandLookup cl;
        vector<string> args = cl.parseArguments(msg);
        Command command = cl.getCommand(args);
        user.onEvent(command, args);
        //eventHandler eh;
        //eh.processTask(msg, user);
    }
    else{
        cout << "message was not a command.." << endl;
        clientSocket.get()->sendString(msg); // relay message back to client to confirm acknowledgement 
    }
}


// Authenticator Thread Functionality
chatUser authenticateUser(shared_ptr<cs457::tcpUserSocket> clientSocket, int id){
    bool notAuthenticated = true;
    string msg;
    ssize_t val;
    cout << "Authenticating Client " << id << endl;
    string uname;
    map <string, string> users = server_data.getUsers();

    while(notAuthenticated){
        tie(msg,val) = clientSocket.get()->recvString();
        if(msg.substr(0,3) == "-u "){ // process username
            string username = msg.substr(3, msg.size());
            cout << "username[ " << username;
            uname = username;
            if(users.find( username ) != users.end()){
                cout << " ] is valid" << endl;
                clientSocket.get()->sendString("validUser");
            }
            else{
                cout << " ] is invalid" << endl;
                clientSocket.get()->sendString("invalidUser");
            }
        }
        else{ //process password
            cout << "pwd[ " << msg;
            /* AUTHENTICATION SUCCESSFUL -- password is correct */
            if(msg == users[uname].substr(0, users[uname].find(' '))){
                cout << " ] is correct" << endl;
                clientSocket.get()->sendString("authenticated!");
                clientSocket.get()->sendString(server_data.getBanner()); // send banner
                chatUser user(uname, clientSocket); // create active user
                server_data.addActiveUser(user);   // update server data
                //dumbUser = user;
               // break;
                return user;
            }
            /* password is incorrect */
            else{
                cout << " ] is incorrect" << endl;
                clientSocket.get()->sendString("notAuthenticated");
            }
        }
    }
    cout << "[Client " << id<< "]: " << uname << " was successfully authenticated\n" << endl;
}

int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket,int id)
{
    cout << "Client " << id << " Unique Socket ID: " << clientSocket.get()->getUniqueIdentifier() << endl;
    cout << "Waiting for message from Client Thread " << id << std::endl << std::endl;
    string msg;
    ssize_t val;
    bool cont =true ;  

    std::future<chatUser> newActiveUser = std::async(&authenticateUser, clientSocket, id);
    chatUser dumbUser = newActiveUser.get();
    

    while (cont) 
    {
        tie(msg,val) = clientSocket.get()->recvString();
        if (msg.substr(0,4) == "EXIT"){ // replace for /QUIT functionality
            
            cout << "[Client " << id << "] exits chat server" << endl;
            cont = false; 
            break;
        }
        cout << "[Client "<< id <<"] " << msg << "     (value return = " << val << ")" << endl;
        string s =  "[SERVER REPLY] The client is sending message:" + msg  + "\n"; 
        
        thread eventHandler(messageHandler, msg, dumbUser, clientSocket);
        eventHandler.join();
       // eventHandler eh;
        //thread eventHandler(eh.processTask, msg, dumbUser);
        //eventHandler.join();

        if (msg.substr(0,6) == "SERVER")
        {
            thread childTExit(&cs457::tcpUserSocket::sendString,clientSocket.get(),"GOODBYE EVERYONE",false);
            thread childTExit2(&cs457::tcpUserSocket::sendString,clientSocket.get(),"\n",false);
            ready = false;   
            cont = false;   
            childTExit.join(); 
            childTExit2.join();
        }
        else
        {
            cout << "waiting for another message\n" << endl; 
        }
    }

    clientSocket.get()->sendString("goodbye\n"); 
    
    clientSocket.get()->closeSocket(); 
    server_data.removeActiveUser(dumbUser.getUsername());
    return 1; 
}

// TODO: add server shell functionality
void startShell(){
    while(true){
        string data;
        getline(cin, data);
        cout << "[Server] " << data << endl;
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