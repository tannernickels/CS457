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
#include <time.h>  

using namespace std;



class server{

    public:

        void onEvent(Command cmd, vector<string>& args, chatUser& user);
        chatUser authenticateUser(shared_ptr<cs457::tcpUserSocket> clientSocket, int id);
        string argToString(vector<string>& args);
        inline void removeActiveUser(string& username) { server_data.removeActiveUser(username); } // allows driver to removeActiveUser

        // Events
        void die();
        void msg(vector<string>& args, chatUser& user);
        void privmsg(vector<string>& args, chatUser& user);
        void join(vector<string>& args, chatUser& user);
        void list(vector<string>& args, chatUser& user);
        void userip(vector<string>& args, chatUser& user);
        void _time_(chatUser& user);

    private:
        serverData server_data;
};


#endif