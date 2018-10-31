#include "chatUser.h"

void chatUser::onEvent(Command cmd, string& msg){

     switch(cmd){
        case AWAY: std::cout << "execute AWAY()" << std::endl; break;
        case CONNECT: std::cout << "execute CONNECT()" << std::endl; break;
        case DIE: std::cout << "execute DIE()" << std::endl; break;
        case HELP: std::cout << "execute HELP()" << std::endl; break;
        case INFO: std::cout << "execute INFO()" << std::endl; break;
        case INVITE: std::cout << "execute INVITE()" << std::endl; break;
        case ISON: std::cout << "execute ISON()" << std::endl; break;
        case JOIN: std::cout << "execute JOIN()" << std::endl; break;
        case KICK: std::cout << "execute KICK()" << std::endl; break;
        case KILL: std::cout << "execute KILL()" << std::endl; break;
        case KNOCK: std::cout << "execute KNOCK()" << std::endl; break;
        case LIST: std::cout << "execute LIST()" << std::endl; break;
        case MODE: std::cout << "execute MODE()" << std::endl; break;
        case NICK: std::cout << "execute NICK()" << std::endl; break;
        case NOTICE: std::cout << "execute NOTICE()" << std::endl; break;
        case OPER: std::cout << "execute OPER()" << std::endl; break;
        case PART: std::cout << "execute PART()" << std::endl; break;
        case PASS: std::cout << "execute PASS()" << std::endl; break;
        case PING: std::cout << "execute PING()" << std::endl; break;
        case PONG: std::cout << "execute PONG()" << std::endl; break;
        case PRIVMSG: std::cout << "execute PRIVMSG()" << std::endl; break;
        case QUIT: std::cout << "execute QUIT()" << std::endl; break;
        case RESTART: std::cout << "execute RESTART()" << std::endl; break;
        case RULES: std::cout << "execute RULES()" << std::endl; break;
        case SETNAME: std::cout << "execute SETNAME()" << std::endl; break;
        case SILENCE: std::cout << "execute SILENCE()" << std::endl; break;
        case TIME: std::cout << "execute TIME()" << std::endl; break;
        case TOPIC: std::cout << "execute TOPIC()" << std::endl; break;
        case USER: std::cout << "execute USER()" << std::endl; break;
        case USERHOST: std::cout << "execute USERHOST()" << std::endl; break;
        case USERIP: std::cout << "execute USERIP()" << std::endl; break;
        case USERS: std::cout << "execute USERS()" << std::endl; break;
        case VERSION: std::cout << "execute VERSION()" << std::endl; break;
        case WALLOPS: std::cout << "execute WALLOPS()" << std::endl; break;
        case WHO: std::cout << "execute WHO()" << std::endl; break;
        case WHOIS: std::cout << "execute WHOIS()" << std::endl; break;
        case INVALID: std::cout << "invalid command" << endl; break;
        default: std::cout << "ERROR IN COMMAND PROCESSING" << std::endl; break;
    }


}