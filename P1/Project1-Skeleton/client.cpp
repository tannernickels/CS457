//Client Code loosely based off https://simpledevcode.wordpress.com/2016/06/16/client-server-chat-in-c-using-sockets/
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector> 
using namespace std;

//GLOBALS
string hostname;
string username;
int serverPort;
string configFile;
string testFile;
string logFile;

int clientSd; // global client socket
char msg[1500]; //create a message buffer 
int bytesRead, bytesWritten = 0; // bookkeeping 

bool isRunning = true;


// Creates client socket and establishes connection to server
void configureSocketAndServerConnection(char *serverIp, int port){

    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);

    // configure client socket
    clientSd = socket(AF_INET, SOCK_STREAM, 0);

    //try to connect...
    int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0){
        cout<<"Error connecting to socket!"<<endl; 
        exit(-1);
    }
    else
        cout << "Connected to the server!" << endl;
}

// Sender Thread Functionality
void outgoing(){
    while(isRunning){
        sleep(1);
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        if(data == "EXIT")
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
        }
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        cout << "Awaiting server response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
    }
}

// Receiver Thread Functionality
void incoming(){
    while(isRunning){
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "goodbye\n"))
        {
            cout << "Server Acknowledges Client Termination" << endl;
            isRunning = false;
        }
        else{
            cout << "Server: " << msg << endl;
        }
    }
}

// Login Thread Functionality
void auth(){
    cout << endl;
    cout << "LOGIN [" << username << "]" << endl;
    bool notAuthenticated = true;
    /* Send Username to Server */
    string syn = "-u " + username;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, syn.c_str());
    bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
    memset(&msg, 0, sizeof(msg));//clear the buffer
    bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
    if(!strcmp(msg, "validUser"))
    {
        cout << "ENTER PASSWORD: ";
    }
    else if(!strcmp(msg, "invalidUser")){
        cout << "You are not a registered user!" << endl;
        isRunning = false;
        notAuthenticated = false;
    }

    while(notAuthenticated){
        /* password input */
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        /* Send password to Server */
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        memset(&msg, 0, sizeof(msg));//clear the buffer

        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "authenticated!"))
        {
            cout << "\nLOGIN SUCCESSFUL: Welcome "<< username << "\n" << endl;
            break;
        }
        else if(!strcmp(msg, "notAuthenticated")){
            cout << "Incorrect Password..try again" << endl;
            cout << "ENTER PASSWORD: ";
        }
        else{
            cout << "SERVER: " << msg << endl;
        }
    }
    
}

int main(int argc, char *argv[])
{

    //parse arguments
    int opt;
    while((opt = getopt(argc, argv, "h:u:p:c:t:L")) != EOF){
        switch(opt){
            case 'h': hostname = optarg; cout << "hostname: " << hostname << endl; break;
            case 'u': username = optarg; cout << "username: " << username << endl; break;
            case 'p': serverPort = stoi(optarg); cout << "server port: " << serverPort << endl; break;
            case 'c': configFile = optarg; cout << "config file: " << configFile << endl; break;
            case 't': testFile = optarg; cout << "run test file: " << testFile << endl; break;
            case 'L': logFile = optarg; cout << "log file name: " << logFile << endl; break;
            case '?': fprintf(stderr, "usage is \n -h hostname \n -u username \n -p server port \n -c configuration file \n -t run test file \n -L log_file_name\n"); exit(-1);
            default: cout << endl; abort();
        }
    }
    // Establish Connection To Server
    char* hname = &hostname[0u];
    configureSocketAndServerConnection(hname, serverPort);

    // Bookkeeping for client termination 
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    // Send Username to Server -- for authentication into servers system and possible registration
    thread login(auth);
    login.join();

    /// Start sender and receiver threads
    vector<unique_ptr<thread>> threadList; 
    unique_ptr<thread> sender = make_unique<thread>(outgoing);
    unique_ptr<thread> receiver = make_unique<thread>(incoming);
    threadList.push_back(std::move(sender)); 
    threadList.push_back(std::move(receiver)); 

    // BLOCKING CALL until client has "EXIT" process
    while(isRunning){}  

    // CLEAN UP CLIENT SESSION 
    for (auto& t: threadList)
    {
        t.get()->detach(); 
    }

    // Print session statistics
    gettimeofday(&end1, NULL);
    close(clientSd);
    cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << 
    " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec- start1.tv_sec)
      << " secs" << endl;
    cout << "Connection closed" << endl;

    // CLIENT TERMINATION
    return 0;    
}