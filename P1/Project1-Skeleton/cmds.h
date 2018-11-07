#ifndef COMMANDS_H
#define COMMANDS_H

// GLOABL COMMAND ENUM REFERENCE
enum Command {AWAY, CONNECT, DIE, HELP,
                INFO, INVITE, ISON, JOIN,
                KICK, KILL, KNOCK, LIST,
                MODE, NICK, NOTICE, OPER,
                PART, PASS, PING, PONG,
                PRIVMSG, QUIT, RESTART, RULES,
                SETNAME, SILENCE, TIME, TOPIC,
                USER, USERHOST, USERIP, USERS,
                VERSION, WALLOPS, WHO, WHOIS, INVALID};
enum Level {SLUGS, CHANNELOP, SYSOP, ADMIN};
enum Handler {SERVER, CHATUSER};
enum taskType {COMMAND=true, MESSAGE=false};

#endif