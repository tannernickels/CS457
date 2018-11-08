#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include "FileIO.h" 
#include "chatUser.h"
#include "chatRoom.h"
using std::map;
using std::make_pair;

class serverData{

    public:
        serverData();

        //ACCESSORS
        inline vector<chatUser>& getActiveUsers(){return this -> activeUsers;}
        inline map<string, string>& getUsers(){return this -> users;}
        inline string getBanner(){return this -> banner;}
      
        bool tryGetChatRoom(string& name);
        chatRoom& getChatRoom(string& name); //{ return chatRooms[name]; }
        string getChannelDescription(string attributes);
        string getChannelPass(string attributes);


        vector<string> getUserData(string& username);
        chatUser& getActiveUser(string& username);
        bool tryGetActiveUser(string& username);
        

        //MUTATORS
        inline void addActiveUser(chatUser& user){activeUsers.push_back(user);}
        inline void addNewlyRegisteredUser(string& username, string& userData){this -> users.insert(make_pair(username, userData));}
        inline void updateBanner(string& banner){this -> banner = banner;}
        void removeActiveUser(string& username);
        void initializeChatRooms(map<string, string> channels);

    private:
        /* SERVER DATA */
        vector<chatUser>  activeUsers;     // vector of active user
        map <string, string> users;        // empty map container for usrs
        string banner;
        map <string, string> channels;
        map <string, chatRoom> chatRooms;
        vector<chatRoom> cRooms;


};


#endif