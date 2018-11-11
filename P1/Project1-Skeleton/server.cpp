#include "server.h"


void server::onEvent(Command cmd, vector<string>& args, chatUser& user){
    switch(cmd){
    
        case CONNECT: std::cout << "execute CONNECT()" << std::endl; break;
        case DIE:   die(); 
                    break;
        case INVITE:    invite(args, user);
                        break;
        case ISON: std::cout << "execute ISON()" << std::endl; break;
        case JOIN:  join(args, user); 
                    break;
        case KICK:  kick(args, user);
                    break;
        case KILL:  kill(args, user);
                    break;
        case KNOCK: knock(args, user); 
                    break;
        case LIST:  list(args, user); 
                    break;
        case MODE: std::cout << "execute MODE()" << std::endl; break;
        case MSG:   msg(args, user); 
                    break;
        
        case NOTICE:    notice(args, user);
                        break;
        case OPER:      oper(args, user);
                        break;
        case PART: std::cout << "execute PART()" << std::endl; break;
        case PASS: std::cout << "execute PASS()" << std::endl; break;
       
        case PRIVMSG:   privmsg(args, user); 
                        break;
        case QUIT: std::cout << "execute QUIT()" << std::endl; break;
        case RESTART: std::cout << "execute RESTART()" << std::endl; break;
        case RULES: std::cout << "execute RULES()" << std::endl; break;
        
        case SILENCE: std::cout << "execute SILENCE()" << std::endl; break;
        case TIME:  _time_(user);
                    break;
        case TOPIC: topic(args, user); 
                    break;
        
        case USERHOST: std::cout << "execute USERHOST()" << std::endl; break;
        case USERIP:    userip(args, user); 
                        break;
        case USERS: std::cout << "execute USERS()" << std::endl; break;
        case VERSION: std::cout << "execute VERSION()" << std::endl; break;
        case WALLOPS: std::cout << "execute WALLOPS()" << std::endl; break;
        case WHO: std::cout << "execute WHO()" << std::endl; break;
        case WHOIS: std::cout << "execute WHOIS()" << std::endl; break;
        case INVALID: std::cout << "invalid command" << endl; break;
        default: std::cout << "ERROR IN COMMAND PROCESSING" << std::endl; break;
    }
}


// Authenticator Thread Functionality
chatUser server::authenticateUser(shared_ptr<cs457::tcpUserSocket> clientSocket, int id){
    bool notAuthenticated = true;
    string msg;
    ssize_t val;
    cout << "Authenticating Client " << id << endl;
    string uname;
    map <string, string> users = server_data.getUsers();

    while(notAuthenticated){
        tie(msg,val) = clientSocket.get()->recvString();
        if(msg == "guest"){ // process guest request
            cout << "Logging in new guest to irc server" << endl;
            string guest_response = "You are Guest-" + std::to_string(id) + "!";
            clientSocket.get()->sendString(guest_response);
            clientSocket.get()->sendString(server_data.getBanner()); // send banner
            string guest_username = "Guest-" + std::to_string(id);
            chatUser guest(guest_username, clientSocket, id); // create active user
            string lvl = "user";
            guest.setLevel(lvl); // set level of guest
            string IPV4 = clientSocket.get()->getIP();
            guest.setIP(IPV4); // set IPV4 Address of user
            server_data.addActiveUser(guest);   // update server data
            return guest;
        }
        else if(msg.substr(0,3) == "-u "){ // process username
            string username = msg.substr(3, msg.size());
            cout << "username[ " << username;
            uname = username;
            if(users.find( username ) != users.end()){
                cout << " ] is valid" << endl;
                clientSocket.get()->sendString("validUser");
            }
            else{
                cout << " ] is invalid" << endl;
                clientSocket.get()->sendString("invalidUser");
            }
        }
        else{ //process password
            cout << "pwd[ " << msg;
            /* AUTHENTICATION SUCCESSFUL -- password is correct */
            if(msg == users[uname].substr(0, users[uname].find(' '))){
                cout << " ] is correct" << endl;
                clientSocket.get()->sendString("authenticated!");
                clientSocket.get()->sendString(server_data.getBanner()); // send banner
                chatUser user(uname, clientSocket, id); // create active user
                user.setLevel(server_data.getUserData(uname)[1]); // set level of user
                string IPV4 = clientSocket.get()->getIP();
                user.setIP(IPV4); // set IPV4 Address of user
                server_data.addActiveUser(user);   // update server data
                return user;
            }
            /* password is incorrect */
            else{
                cout << " ] is incorrect" << endl;
                clientSocket.get()->sendString("notAuthenticated");
            }
        }
    }
    cout << "[Client " << id<< "]: " << uname << " was successfully authenticated\n" << endl;
}


void server::die(){
    std::cout << "executing DIE()" << std::endl; 
    thread notifyAll(&serverData::notifyActiveUserOfServerShutDown, &server_data);
    notifyAll.join();
    cout << "******** SHUTING DOWN ********" << endl;
    exit(0);
}

void server::privmsg(vector<string>& args, chatUser& user){
    string name = args[0];
    args.erase(args.begin());
    string message = argToString(args);

    if(server_data.tryGetActiveUser(name)){
        chatUser recipient = server_data.getActiveUser(name);
        recipient.writeToSocket("<"+ user.getUsername() +  "> " + message);
    }
    else{
        user.writeToSocket("User[ " + name + " ] is either offline or does not exist");
    }
    
}


void server::msg(vector<string>& args, chatUser& user){
    // HANDLES CHAT ROOM MESSAGING
    std::cout << "execute MSG()" << std::endl; 
    string name = args[0];
    args.erase(args.begin());
    string message = argToString(args);
    cout << "[MSG] " << message << endl;
   
    if(server_data.tryGetChatRoom(name)){
        chatRoom& room = this->server_data.getChatRoom(name);
        room.sendMessageToChannel(message, user, false);
    }
    else{
        user.writeToSocket("The channel[ " + name + " ] does not exist");
    }
    
    
}

void server::join(vector<string>& args, chatUser& user){
    std::cout << "execute JOIN()" << std::endl; 
    string name = args[0];
    string pwd = args[1];
    
    if(server_data.tryGetChatRoom(name)){
        chatRoom& room = this->server_data.getChatRoom(name);
        room.joinChannel(user, pwd);
    }
    else{
        user.writeToSocket("The channel[ " + name + " ] does not exist");
    }
    
}


void server::list(vector<string>& args, chatUser& user){
    std::cout << "execute LIST()" << std::endl; 
    string list = server_data.getListOfChannels();
    user.writeToSocket(list);
}


string server::argToString(vector<string>& args){
    string message = "";
    for (unsigned int i = 0; i < args.size(); i++){
        if(i==args.size()-1){
            message += args[i] + "\n";
        }else{
            message += args[i] + " ";
        }
    }
    return message;
}

void server::userip(vector<string>& args, chatUser& user){
    std::cout << "execute USERIP()" << std::endl; 
    string username = args[0];
    
    if(server_data.tryGetActiveUser(username)){
        chatUser& target = server_data.getActiveUser(username);
        string targetsIP = target.getIP();
        user.writeToSocket("IP Address of " + username + ": " + targetsIP);
    }
    else{
        user.writeToSocket(username + " is not online or does not exist");
    }

}

void server::_time_(chatUser& user){
    std::cout << "execute TIME()" << std::endl;
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    string response = "The current local time and date is " + std::string(asctime(timeinfo)); 
    user.writeToSocket(response);
}

void server::notice(vector<string>& args, chatUser& user){
    // command is implemented at privmsg
    std::cout << "execute NOTICE()" << std::endl;
    privmsg(args, user);
}

void server::oper(vector<string>& args, chatUser& user){
    // According to the commands pdf: OPer attempts to give a user OPER privileges..
    // Although if you are an operator then you should already have been given those privileges up logging in.
    // So this command will validate if a user has OPER privilegs or not.  Their level should have been set upon login.
    std::cout << "execute OPER()" << std::endl;
    vector<string> data = server_data.getUserData(user.getUsername());
    if(data[1] == "channelop")
        user.writeToSocket("You have IRC Operator privileges.");
    else
        user.writeToSocket("You are not a IRC Operator");
}

void server::topic(vector<string>& args, chatUser& user){
    std::cout << "execute TOPIC()" << std::endl;
    string channel_name = args[0];
    string set_topic_to = args[1]; //optional parameter that is used to set the topic for a channel
    if(server_data.tryGetChatRoom(channel_name)){
        chatRoom& room = this->server_data.getChatRoom(channel_name);
        string topic = room.getChannelDescription();
        user.writeToSocket(topic);
    }
    else{
        user.writeToSocket("The channel[ " + channel_name + " ] does not exist");
    }
    
    //TODO: if args[1] is not empty then we need to change the chatRoom description and update channels.txt.
}

void server::invite(vector<string>& args, chatUser& user){
    // INVITE <nickname> <channel>
    std::cout << "execute INVITE()" << std::endl; 
    string uname = args[0];
    string channel = args[1];
    string message = user.getUsername() + " has invited you to join " + channel;

    if(server_data.tryGetChatRoom(channel)){ // check that the specified chatRoom exists
        chatRoom& room = this->server_data.getChatRoom(channel);
        if(room.isValidUser(user)){ // check that user has already joined the channel he is attempting to invite another user to
            if(server_data.tryGetActiveUser(uname)){ // check that the specified recipient is a valid active user
                chatUser recipient = server_data.getActiveUser(uname);
                if(room.isValidUser(recipient)){
                    user.writeToSocket(uname + " has already joined #" + channel);
                }
                else{
                    string pwd = room.getChannelPassword();
                    if(pwd == "@"){
                        string invitation = message + ", the chat room does not have a password \n";
                        recipient.writeToSocket(invitation);
                    }
                    else{
                        string invitation = message + ", the chat room password is " + pwd + "\n";
                        recipient.writeToSocket(invitation);
                    }
                }
            }
            else{
                user.writeToSocket("User[ " + uname + " ] is either offline or does not exist");
            }
        }
        else user.writeToSocket("Please join the channel before inviting another player to the chat room.");
    }
    else{
        user.writeToSocket("The channel[ " + channel + " ] does not exist");
    }
}

void server::kick(vector<string>& args, chatUser& user){
    std::cout << "execute KICK()" << std::endl;
    bool authenticated = false;
    // this command can only be issued by a channel operator
    Level privilege =  user.getLevel();  
    switch(privilege){
        case CHANNELOP: authenticated = true; break;
        default: user.writeToSocket("You do no have operator privileges to kick another user from the chat"); break;
    }
    if(authenticated){
        // KICK <channel> <client> [<message>]
        string channel = args[0];
        string uname = args[1];
        args.erase(args.begin()); args.erase(args.begin()); // remove channel and uname -> toString message args
        string message = argToString(args);
        if(server_data.tryGetChatRoom(channel)){ // check that the specified chatRoom exists
            chatRoom& room = this->server_data.getChatRoom(channel);
            if(room.isValidUser(user)){ // check that user has already joined the channel he is attempting to kcik another user to
                if(server_data.tryGetActiveUser(uname)){ // check that the specified recipient is a valid active user
                    chatUser recipient = server_data.getActiveUser(uname);
                    if(room.isValidUser(recipient)){ // check that the recipient is in the specified channel
                        room.removeUser(recipient); 
                        recipient.writeToSocket("You have been removed from #" + room.getChannelName());
                        if(!message.empty()) // send user the message if valid
                            recipient.writeToSocket("<" + user.getUsername() + "> " + message);
                    }
                    else{
                        user.writeToSocket("User[ " + uname + " ] is not in the channel");
                    }
                }
                else{
                    user.writeToSocket("User[ " + uname + " ] is either offline or does not exist");
                }
            }
            else user.writeToSocket("Please join the channel before attempting to kick another player from a chat room.");
        }
        else{
            user.writeToSocket("The channel[ " + channel + " ] does not exist");
        }

    }
}

void server::kill(vector<string>& args, chatUser& user){
    std::cout << "execute KILL()" << std::endl;
    // KILL <client> <comment> : Forcibly removes <client> from the network. [13] is command may only be issued by IRC operators. Defined in RFC
    string uname = args[0];
    args.erase(args.begin()); // remove client
    string comment = argToString(args);

    bool authenticated = false;
    // this command can only be issued by a channel operator
    Level privilege =  user.getLevel();  
    switch(privilege){
        case CHANNELOP: authenticated = true; break;
        default: user.writeToSocket("You do no have operator privileges to kick another user from the chat"); break;
    }
    if(authenticated){
        if(server_data.tryGetActiveUser(uname)){ // check that the specified recipient is a valid active user
            chatUser recipient = server_data.getActiveUser(uname);
            recipient.writeToSocket(user.getUsername() + " has removed you from the server"); //notify user 
            recipient.writeToSocket("<" + user.getUsername() + "> " + comment); //send comment
            sleep(1);
            recipient.writeToSocket("GOODBYE EVERYONE"); // remove client
            sleep(2);
            recipient.socketPointer().get()->closeSocket();  // close socket
            this -> removeActiveUser(uname); // clean up server data
            
        }   
        else{
            user.writeToSocket("User[ " + uname + " ] is either offline or does not exist");
        }
    }
    else{
        
    }
    

}

void server::knock(vector<string>& args, chatUser& user){
    std::cout << "execute KNOCK()" << std::endl;
    //KNOCK <channel> [<message>] -> Sends a NOTICE to an invitation-only <channel> with an optional <message>, requesting an invite
    string channel_name = args[0];
    args.erase(args.begin());
    string message = argToString(args);

    if(server_data.tryGetChatRoom(channel_name)){
        chatRoom& room = this->server_data.getChatRoom(channel_name);
        string respones = string(user.getUsername()) + " has requested an invite to #" + string(room.getChannelName()) + "\n<" + string(user.getUsername()) + "> " + message;
        room.sendMessageToChannel(respones, user, true);
    }
    else{
        user.writeToSocket("The channel[ " + channel_name + " ] does not exist");
    }

}