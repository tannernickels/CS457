#include "chatUser.h"

void chatUser::setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket){
    connection = clientSocket;
}

void chatUser::onEvent(Command cmd, vector<string>& args){

     switch(cmd){
        case AWAY:  away(args);
                    break;
        case HELP: std::cout << "execute HELP()" << std::endl; break;
        case INFO:  info();
                    break;
        case NICK:  nick(args);
                    break;
        case PING: std::cout << "execute PING()" << std::endl; break;
        case PONG: std::cout << "execute PONG()" << std::endl; break;
        case SETNAME: std::cout << "execute SETNAME()" << std::endl; break;
        case USER: std::cout << "execute USER()" << std::endl; break;
        case INVALID: std::cout << "invalid command" << endl; break;
        default: std::cout << "ERROR IN COMMAND PROCESSING" << std::endl; break;
    }

}
void chatUser::away(vector<string>& args){
    if(args.size()==0){
        isAway = false;
    }else{
        isAway = true;
        for (unsigned int i = 0; i < args.size(); i++){
            if(i==args.size()-1){
                away_msg += args[i];
            }else{
                away_msg += args[i] + ' ';
            }
        }
    }
}

void chatUser::info(){
    cout << "executing INFO()" << endl; 
    connection.get()->sendString("This IRC server is made possible by YUNG T and BIG DEV");
}

void chatUser::nick(vector<string>& args){
    cout << "executing NICK()" << endl; 
    if(args.size()==1){
        nickName = args[0];
        cout << "[Client "<< id << "] Registered the nickname: " << nickName << endl;
    } else{
        usage(-1);
    }
}

void chatUser::set_name(vector<string>& args){
    return;
}

int chatUser::usage(int code){
    string u = "USAGE(" + to_string(code) + "): ";
    switch(code){
        case -1:
            cout << u << "incorrect number of arguments" << endl;
            connection.get() -> sendString(u + "incorrect number of arguments");
            return code;
        case -2:
            cout << u << "incorrect argument error" << endl;
            connection.get() -> sendString(u + "incorrect argument error");
            return code;
        default:
            return code;
    }
}


