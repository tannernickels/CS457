#include "serverData.h"

serverData::serverData(){
    // USERS IO
    FileIO usersTXT("db/users.txt");
    users = usersTXT.readUsersTXT();
    // BANNER IO
    FileIO bannerTxt("db/banner.txt");
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