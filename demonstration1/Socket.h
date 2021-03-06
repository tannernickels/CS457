#pragma once

#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <strings.h>
#include <string> 
#include <tuple> 
#include <unistd.h>
#include <map> 
#include <assert.h>
#include <vector> 

namespace cs457
{
    using namespace std; 

    //https://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number
    int roundUpMultipleOfTwo(int numToRound, int multiple) 
    {
        //positive only 
          assert(multiple && ((multiple & (multiple - 1)) == 0));
          return (numToRound + multiple - 1) & -multiple;
    }
    
    class tcpUserSocket
    {
        public:
            tcpUserSocket()
            {
               
            };
            void setSocket(int socket){ userSocket=socket;}; 
            struct sock_addr * getAddressPointer(){return ((struct sock_addr *) &userAddress);}; 
            socklen_t getLenghtPointer()
            {
                socklen_t len = sizeof(userAddress);  
                return len;
            };
            int getSocket(){return userSocket;}; 
            int closeSocket(){return close(userSocket);}; 
            std::tuple<string,ssize_t> recvString(int bufferSize=1024)
            {
                char stringBuffer[bufferSize]; 
                ssize_t recvMsgSize = recv(userSocket, stringBuffer, bufferSize, 0); 
                return make_tuple(string(stringBuffer),recvMsgSize);     
            };
        

            ssize_t sendString(const string & data)
            {
                //https://stackoverflow.com/questions/7352099/stdstring-to-char
                if (data.size() == 0) return 0;                 
                int bufferSize = data.size()+1; 
                vector<char> stringBuffer(data.c_str(), data.c_str() + data.size() + 1u);
                return send(userSocket, &stringBuffer[0], bufferSize, 0);
            }
            string getUniqueIdentifier(){ return "modifythislater";};



        private:
        struct sockaddr_in userAddress; 
        int userSocket; 
    };

    class tcpServerSocket
    {

    public:

        tcpServerSocket(uint portNumber): port(portNumber), address("")
        {
           init(); 
           setSocketOptions();
        };     
        tcpServerSocket(string networkAddress, uint portNumber): address(networkAddress), port(portNumber)
        {
           init(); 
           setSocketOptions();
        };
        int bindSocket()
        {
            return bind(serverSocket,(struct sockaddr *)&serverAddress,sizeof(serverAddress));    
        }
        int listenSocket()
        {
            return listen(serverSocket,14);
        }
        tuple<tcpUserSocket,int> acceptSocket()
        {
            cs457::tcpUserSocket userSocket; 
            socklen_t len = userSocket.getLenghtPointer();
            int client_fd = accept(serverSocket,(struct sockaddr *)userSocket.getAddressPointer(),&len);
            userSocket.setSocket(client_fd); 
            return make_tuple(userSocket,client_fd); 
        }
    private: 
        void init()
        {
            const char * cstr = address.c_str();
            int val =0;  
            bzero(&serverAddress,sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            if (address == "")
                addr.s_addr =  htonl(INADDR_ANY);
            else
            {
                val = inet_aton(cstr,&addr);
                serverAddress.sin_addr = addr;
                string addresscpy(inet_ntoa(addr));
                address = addresscpy;  
            }
       
            serverAddress.sin_port = htons(port);
        }

        void setSocketOptions()
        {
          int optval = 1;
          setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, 
	                (const void *)&optval , sizeof(int));
        }
        uint port; 
        string address; 
        //
        int serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        struct sockaddr_in serverAddress; 
        struct in_addr addr;
        /* Internet address */
        struct in_addr{
            in_addr_t s_addr; /*32 bit IPv4 network byte ordered address*/
        };
        struct sockaddr_in {
            uint8_t sin_len; /* length of structure (16)*/
            sa_family_t sin_family; /* AF_INET*/
            in_port_t sin_port; /* 16 bit TCP or UDP port number */
            struct in_addr sin_addr; /* 32 bit IPv4 address*/
            char sin_zero[8]; /* not used but always set to zero */
        };
        //struct sockaddr {
        //        uint8_t sa_len;
        //        sa_family_t sa_family; /* address family: AD_xxx value */
        //        char sa_data[14];
        //};


    };

    class tcpUserManager
    {
        public:
            tcpUserManager(){};
            void addUserSocket(tcpUserSocket & userSocket)
            {
                 users[userSocket.getUniqueIdentifier()] = userSocket; 
            }
            
        private: 

        std::map<string,tcpUserSocket> users;

        //?
        //printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));
        //printf("port is: %d\n", (int) ntohs(client_addr.sin_port));
    };



}

