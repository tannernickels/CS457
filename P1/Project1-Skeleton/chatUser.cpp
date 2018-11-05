#include "chatUser.h"

void chatUser::setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket){
    connection = clientSocket;
}

void chatUser::onEvent(Command cmd, vector<string>& args){

     switch(cmd){
        case AWAY: std::cout << "execute AWAY()" << std::endl; break;
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

void chatUser::nick(vector<string>& args){
    cout << "executing NICK()" << endl; 
    if(args.size()==1){
        nickName = args[0];
        cout << "[Client "<< id << "] Registered the nickname: " << nickName << endl;
    } else{
        usage(-1);
    }
}

void chatUser::info(){
    cout << "executing INFO()" << endl; 
    connection.get()->sendString("this IRC server is made possible by YUNG T and BIG DEV");
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
        default:
            return code;
    }
}


