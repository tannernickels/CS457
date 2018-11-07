#include "serverData.h"

serverData::serverData(){
    // USERS IO
    FileIO usersTXT("db/users.txt", "r");
    users = usersTXT.readUsersTXT();
    // BANNER IO
    FileIO bannerTxt("db/banner.txt", "r");
    banner = bannerTxt.readFull();

}

bool serverData::tryGetActiveUser(string& username){
    string curr;
    for(auto& user : activeUsers){
        curr = user.getUsername();
        if(curr.compare(username)==0){
            return true;
        } 
    }
    return false;
}

chatUser& serverData::getActiveUser(string& username){
    string curr;
    for(auto& user : activeUsers){
        curr = user.getUsername();
        if(curr.compare(username)==0){
            return user;
        } 
    }
   
}

void serverData::removeActiveUser(string& username){
    string curr;
    for(vector<chatUser>::iterator it = activeUsers.begin(); it != activeUsers.end(); ++it){
        if(username.compare(it -> getUsername()) == 0){
            activeUsers.erase(it);
            break; 
        }
    }
    
}