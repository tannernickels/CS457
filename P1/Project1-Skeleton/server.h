#ifndef SERVER_H
#define SERVER_H

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

using namespace std;



class server{

    public:

        void onEvent(Command cmd, vector<string>& args);
        chatUser authenticateUser(shared_ptr<cs457::tcpUserSocket> clientSocket, int id);

        inline void removeActiveUser(string& username) { server_data.removeActiveUser(username); } // allows driver to removeActiveUser

        // Events
        void die();

        void privmsg(vector<string>& args);

    private:
        serverData server_data;
};


#endif