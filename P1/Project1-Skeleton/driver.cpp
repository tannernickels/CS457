#include <iostream>
#include <string> 
#include <tuple> 
#include <thread> 
#include <vector> 
#include <memory> 
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include "CommandLookup.h"
#include "chatUser.h"

using namespace std;

bool ready = true;
vector<chatUser>  activeUsers;     // vector of active user
map <string, string> users;        // empty map container for usrs
string uname;    


void messageHandler(string msg, chatUser user, shared_ptr<cs457::tcpUserSocket> clientSocket){
     if(msg.at(0) == '/'){
        CommandLookup cl;
        Command command = cl.find(msg);
        user.onEvent(command, msg);
    }
    else{
        cout << "message was not a command.." << endl;
        clientSocket.get()->sendString(msg); // relay message back to client to confirm acknowledgement 
    }
}


int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket,int id)
{
    cout << "Client " << id << " Unique Socket ID: " << clientSocket.get()->getUniqueIdentifier() << endl;
    cout << "Waiting for message from Client Thread " << id << std::endl << std::endl;
    string msg;
    ssize_t val;
    bool cont =true ;  
    chatUser user; // create user object
    activeUsers.push_back(user);  // add user to list of active users
    user.setSocket(clientSocket);

    bool notAuthenticated = true;
    while(notAuthenticated){
        tie(msg,val) = clientSocket.get()->recvString();
        if(msg.substr(0,3) == "-u "){ // process username
            string username = msg.substr(3, msg.size());
            cout << "username: " << username << endl;
            uname = username;
            if(users.find( username ) != users.end()){
                clientSocket.get()->sendString("validUser");
            }
            else{
                clientSocket.get()->sendString("invalidUser");
            }
        }
        else{ //process password
            cout << "pwd: " << msg << endl;
            if(msg == users[uname]){
                clientSocket.get()->sendString("authenticated!");
                break;
            }
            else{
                clientSocket.get()->sendString("notAuthenticated");
            }
        }
    }

    while (cont) 
    {
        tie(msg,val) = clientSocket.get()->recvString();
        if (msg.substr(0,4) == "EXIT"){
            cout << "[Client " << id << "] exits chat server" << endl;
            cont = false; 
            break;
        }
        cout << "[Client "<< id <<"] " << msg << "     (value return = " << val << ")" << endl;
        string s =  "[SERVER REPLY] The client is sending message:" + msg  + "\n"; 

        // WHY ARE WE USING THREADS HERE?? why not use -- clientSocket.get()->sendString(s);
        //thread childT1(&cs457::tcpUserSocket::sendString,clientSocket.get(),s,true);
        //thread childT2(&cs457::tcpUserSocket::sendString,clientSocket.get(),msg,true);
        //thread childT3(&cs457::tcpUserSocket::sendString,clientSocket.get(),"\n",true);
        
        thread eventHandler(messageHandler, msg, user, clientSocket);
        eventHandler.join();

        //childT1.join();
        //childT2.join(); 
        //childT3.join(); 
        //clientSocket.get()->sendString(msg); 
        //clientSocket.get()->sendString("\n"); 
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
    
    return 1; 
}

int main(int argc, char * argv[])
{
    cout << "Initializing Socket" << std::endl; 
    cs457::tcpServerSocket mysocket(2000);
    cout << "Binding Socket" << std::endl; 
    mysocket.bindSocket(); 
    cout << "Listening Socket" << std::endl; 
    mysocket.listenSocket(); 
    cout << "Waiting to Accept Socket\n" << std::endl;
    int id = 0; 
    vector<unique_ptr<thread>> threadList; 
  
    // insert elements into users map -- replace with fileIO
    users.insert(pair <string, string> ("dcdennis", "123")); 
    users.insert(pair <string, string> ("tnickels", "456 user false")); 


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