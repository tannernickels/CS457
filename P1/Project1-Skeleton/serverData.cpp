#include "serverData.h"

serverData::serverData(){
    // USERS IO
    FileIO usersTXT("db/users.txt", "r");
    users = usersTXT.readAttributesToMap();
    // BANNER IO
    FileIO bannerTxt("db/banner.txt", "r");
    banner = bannerTxt.readFull();
    // CHANNELS IO
    FileIO channelsTxt("db/channels.txt", "r");
    channels = channelsTxt.readAttributesToMap();
    initializeChatRooms(channels);

}

vector<string> serverData::getUserData(string& username){
    StringMapIt it = users.find(username);
    vector<string> userData;
    string dataString = "";
    if(it!=users.end()){
        dataString = it->second;
    }
    istringstream iss(dataString);
    string curr;
    do{
        iss >> curr;
        if(curr.size()!=0){
            userData.push_back(curr);
        }
    }while(iss);
    return userData;
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
    //FIX, use trymethod
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

string serverData::getChannelDescription(string attributes){
    return attributes.substr(0, attributes.find(' '));	
}
string serverData::getChannelPass(string attributes){
    return attributes.substr(attributes.find(' ') + 1, attributes.size());
}

// populates map<channel_name, chatRoom> chatRooms
void serverData::initializeChatRooms(map<string, string> channels){

    for ( const auto &pair : channels ) {

        string channel_name = pair.first;
        string channel_description = getChannelDescription(pair.second);
        string channel_password = getChannelPass(pair.second); 
        
        chatRoom room(channel_name, channel_description, channel_password);
        chatRooms[channel_name] = room;
        cRooms.push_back(room);

    }
}

bool serverData::tryGetChatRoom(string& name){
    map<string, chatRoom>::iterator it = chatRooms.find(name);
    cout << "Searching " << &chatRooms << " for chat room at address: " << &(*it) << endl;
    if (it == chatRooms.end())return false;
    else return true;
}

chatRoom& serverData::getChatRoom(string& name){
    //map<string, chatRoom>::iterator it = chatRooms.find(name);
    //cout << "Retrieving from " << &chatRooms << " chat room at address: " << &(*it) << endl;
    //return it->second;
    //cout << "Retrieving from " << &chatRooms << " chat room at address: " << &(*chatRooms[name]) << endl;
    
    //chatRoom& room = chatRooms.at(name);
    //return room;
    for(int i=0; i < cRooms.size(); i++){
        if(cRooms[i].getChannelName() == name){
            cout << "FOUND YOUR CHAT ROOM: " << cRooms[i].getChannelName() << " at " << &(cRooms[i]) << endl;
            return cRooms[i];
        }
    }
}