#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include "FileIO.h" 
#include "chatUser.h"

class serverData{

    public:
        serverData();

        //ACCESSORS
        inline vector<chatUser>& getActiveUsers(){return this -> activeUsers;}
        inline map<string, string>& getUsers(){return this -> users;}
        inline string getBanner(){return this -> banner;}

        chatUser& getActiveUser(string& username);
        bool tryGetActiveUser(string& username);
        
        //MUTATORS
        inline void addActiveUser(chatUser& user){activeUsers.push_back(user);}
        inline void addNewlyRegisteredUser(string& username, string& userData){this -> users.insert(make_pair(username, userData));}
        inline void updateBanner(string& banner){this -> banner = banner;}


    private:
        /* SERVER DATA */
        vector<chatUser>  activeUsers;     // vector of active user
        map <string, string> users;        // empty map container for usrs
        string banner;


};


#endif