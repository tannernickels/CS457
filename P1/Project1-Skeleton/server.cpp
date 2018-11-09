#include "server.h"


void server::onEvent(Command cmd, vector<string>& args, chatUser& user){
    switch(cmd){
    
        case CONNECT: std::cout << "execute CONNECT()" << std::endl; break;
        case DIE:   die(); 
                    break;
        case INVITE: std::cout << "execute INVITE()" << std::endl; break;
        case ISON: std::cout << "execute ISON()" << std::endl; break;
        case JOIN:  join(args, user); 
                    break;
        case KICK: std::cout << "execute KICK()" << std::endl; break;
        case KILL: std::cout << "execute KILL()" << std::endl; break;
        case KNOCK: std::cout << "execute KNOCK()" << std::endl; break;
        case LIST:  list(args, user); 
                    break;
        case MODE: std::cout << "execute MODE()" << std::endl; break;
        case MSG:   msg(args, user); 
                    break;
        
        case NOTICE: std::cout << "execute NOTICE()" << std::endl; break;
        case OPER: std::cout << "execute OPER()" << std::endl; break;
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
        case TOPIC: std::cout << "execute TOPIC()" << std::endl; break;
        
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
    // TODO: send every active user the goodbye string so that they can terminate before the server shuts down
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

// HANDLES CHAT ROOM MESSAGING
void server::msg(vector<string>& args, chatUser& user){
    std::cout << "execute MSG()" << std::endl; 
    string name = args[0];
    args.erase(args.begin());
    string message = argToString(args);
    cout << "[MSG] " << message << endl;
   
    if(server_data.tryGetChatRoom(name)){
        chatRoom& room = this->server_data.getChatRoom(name);
        room.sendMessageToChannel(message, user);
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