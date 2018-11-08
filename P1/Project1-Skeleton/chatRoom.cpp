#include "chatRoom.h"

void chatRoom::joinChannel(chatUser& user, string& password){
    
    if(password == this->password){
        addUser(user);
        user.joinChatRoom(this->channel_name);
        cout << user.getUsername() << " joined " << this->channel_name << endl;
        user.writeToSocket("you have joined the channel: " + this->channel_name);
    }
    else{
        user.writeToSocket("you entered an invalid password for the channel: " + this-> channel_name);
    }

    printChannelData();

}

void chatRoom::sendMessageToChannel(string& message, chatUser& sender){
    cout << "SENDING MESSAGE TO CHANNEL" << endl;
    printChannelData();
    string name_of_sender = sender.getUsername();
    cout << to_string(users.size()) << endl;
    for(auto& user: users) {
        //if(user.getUsername() != name_of_sender){
            cout << this -> getChannelName() << endl;
            cout << "SENDER " << sender.getUsername() << endl;
            user.writeToSocket(sender.getUsername()+": "+ message);
            cout << "RECIPIENT " << user.getUsername() << endl;
        //}
    }
}

void chatRoom::printChannelData(){
    cout << channel_name << endl;
    cout << "Channel address: " << &(*this) << endl;
    cout << "Channel users: " << endl;
    for(unsigned int i = 0; i < users.size(); i++){
        cout << users[i].getUsername() << endl;
    }
}