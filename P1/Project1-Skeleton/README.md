# CS457 Project 1 - IRC Chat System 
####  By Devin Dennis and Tanner Nickels 
****
**Chat Client and Chat Server**
**2,221 total lines of code** [ *.cpp and *.h files ]

## Installation
****

Install Project from GitHub
Source code is located under CS457/P1/Project1-Skeleton/
```bash 
git clone https://github.com/tannernickels/CS457.git
```



## Usage
****

Server and Client code is compiled under the comp script.  
(1) The server can be run with ./run
(2) The client can be run with ./run_client  -h <hostname> -u <username> -p <server port> -c <configuration file> -t <run test file> -L <log_file_name>
All of the arguments are optional when running the script above, there are some preset defaults to help you out.
The IRC system will authenticate you as a guest if no username is specified otherwise you will be prompted to enter your password.


(1) Usage[server]: takes no arguments
(2) Usage[client]:  -h <hostname> -u <username> -p <server port> -c <configuration file> -t <run test file> -L <log_file_name>
Usage[run test]: -h <host_address> -u <username> <test_file_name>
Usage[run tests on two clients]: -h <host_address> -u1 <username>  -u2 <username>  -k1 <password 1>  -k2 <password 2>  -t1 <test_file_name1>  -t2 <test_file_name2>

```bash
# example:
comp                       # compiles the server and client code
scripts/run                # starts the server
scripts/run_client         # starts the client
```

## Architecture 
*****

Class  | Responsibility
------------- | -------------
driver | This is where the main execution block of the server is being carried out.  This class accepts client connections and multi-threading of those socket connections. When one of the client threads receives a message the "request" is processed by a consumer thread which calls the eventHandler class.
eventHandler | This class processes what type of "request" was sent to the server.  A task is divided into either a message (1) or command (2) . A message (1) is simply relayed back to the client to confirm the connection is active.  A command (2) will be processed by either the chatUser class or server class.  Depending on the type of command it makes more sense for the chain of responsibility to be divided into different components.
server | This is where the server commands are carried out (onEvent()).  This class holds an instance of the server_data class where the server can easily access and mutate data for any given command.  
chatUser |  This is where chat user commands are carried out (onEvent()).  This class is a wrapper that holds all data relevant to a specific user.  
server_data | When this class is constructed it reads in the text files from db/ and populates their contents into private data structures.  The class implements various accessors and mutators for the server to use.  
FileIO | Wrapper class for ifstream file reading. Provides useful methods for extracting various data formats from different files. Opens the file depending on "r" or "w" to read or write to file, and "a" to append to a file.
CommandLookup | This class is used to find the command type and parse all subsequent arguments provided by the client.
chatRoom | This class contains all the data relevant for a specific channel as well as any methods necessary for the server to carry out commands.  Related commands include /join, /invite, /knock, /msg.
client | This is the driver for a user to interact with the chat system.  The client is an interactive shell where requests can be made to the server and process server responses.  The client can either be run as a guest or a registerd user on the IRC server which will require an password to be supplied by the user. 
cmds | This file holds global enums related to command types, user levels, handlers, and task types.  This is simply used to make code more readable.
log | This class has all the logic to write output to a given log file.  
tcpServerSocket | Provided by Professor Francisco R. Ortega, handles server socket logic.
tcpUserSocket | Provided by Professor Francisco R. Ortega,  handles user socket logic.


We also have folder for db, conf, scripts, and tests. 

## Progress
****
#### Chat Command Progression
Command  | Current Functionality
------------- | -------------
  /AWAY <message>              |       With arguments, away sets the user to "away" mode and sets an away message, displayed when someone attempts to message you. With no arguments, turns off "away" mode.
    /INFO                       |        Provides information about the server.
    /NICK <nickname>             |       Allows user to change their nickName.
    /OPER <nickname> <password>       | Server validates if that username is a irc operator. User privileges are set upon startup and currently cannot be changed.
    /NOTICE <username> <message>   |     Forwards message the specified user.  
    /TOPIC <channel>                |    Server responds with the specified channels description.
    /JOIN <channel> <password>      |    Adds the user to the specified channel given that the channel name and password are correct. Some channels require no paswords.
    /PRIVMSG <username> <password>  |    Forwards message the specified user.
    /NICK <nickname>                |    Updates the users nickname.
    /PING                            |   Server responds with PONG which validates that the connection is still present.  We will eventually implement a system that calculates the latency between the client and server. 
    /TIME                            |   Server responds with its local time and date.
    /USERIP <username>               |   Server responds with the IPV4 Address of the specified user.
    /LIST                            |   Server responds with the list of registerd channels of the server.
    /MSG <channel> <message>         |   Forwards message the specified channel.
    /SETNAME <username>              |    Updates uesrname of the requested user.
    /INVITE <username> <channel>      |    Server forwards an invitation to the given user for the specified channel.  The recipient of the invitatation will be sent the channels password, if applicable.
    /KICK <channel> <client> <message>     | Channel operators can remove a user from a channel. 
    /KILL <client> <comment>            |  Forcibly removes <client> from the network. This command may only be issued by IRC operators. Defined in RFC.
    /KNOCK <channel> [<message>]     |    Sends a NOTICE to an invitation-only <channel> with an optional <message>, requesting an invite.
    /QUIT | Disconnects the client from the server.
    /DIE                               |    Shuts down each client that is connected to the server and finally terminates the server.

Each command has its respective error/case handling. 
For example if a user attempts to KICK another user from a channel then the following must be true:
* the user must be a channel operator
* the user must be in the channel specified
* the targeted user must exist or be active
* the targeted user must be in the channel
* the channel must exist
#### Client Progress
Currently the client behaves as it should as defined by the rubric.  The client can connect to the server as a guest or registered user.  The client can also send any message to the server without worrying about crashes and receive the expected output as described above in the command table.  The client can handle test files and will execute the list of commands within the file. The client multi-threads the receiver and handles disconnects intitiated eitehr itself or the server. Currently the -L <logfile> argument is completely decoupled from the server.  However, the server has the functionality for logging a chat user's session but we ran out of time to hook the classes implementation with a client request and/or the configuration file. This is on the todo list for the next project iteration.  

#### Server Progress
The Server can currently process all of the commands listed above in the table as well being interactive. For example, within the shell you can type "USERS" and the server will respond with all online users curerntly logged in.  This is helpful when testing system state when clients are connecting and disconnecting. The server can differentiate between guests and registered users as well as a users privilege level as defined in the database.  The server can also handle the most important commands for private messaging users, messaging channels, and disconnecting users.  All of the channel commands are done besides cprivmsg which will be handled by next iteration.  A user can request an invite to a channel (KNOCK), send an invite (INVITE), remove a user from a channel(KICK), join a channel(JOIN), and message a channel (MSG).  The MSG command is not a formal IRC command but will be used behind the scenes by the client when we have implemented the User Interface (UI).  The server threads each client connection as well as the handling for each message that is received. 