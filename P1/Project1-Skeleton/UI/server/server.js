const express = require('express');
const app = express();
const port = process.env.PORT || 57000;

// console.log that your server is up and running
app.listen(port, () => console.log(`Listening on port ${port}`));

// create a GET route
app.get('/express_backend', (req, res) => {
    res.header("Access-Control-Allow-Origin", "*");
  res.send({ express: 'YOUR EXPRESS BACKEND IS CONNECTED TO REACT' });
});



/*
In the node.js intro tutorial (http://nodejs.org/), they show a basic tcp 
server, but for some reason omit a client connecting to it.  I added an 
example at the bottom.
Save the following server in example.js:
*/

var net = require('net');

var server = net.createServer(function(socket) {
	socket.write('Echo server\r\n');
	socket.pipe(socket);
});

server.listen(2001, '127.0.0.1');

/*
And connect with a tcp client from the command line using netcat, the *nix 
utility for reading and writing across tcp/udp network connections.  I've only 
used it for debugging myself.
$ netcat 127.0.0.1 1337
You should see:
> Echo server
*/

/* Or use this example tcp client written in node.js.  (Originated with 
example code from 
http://www.hacksparrow.com/tcp-socket-programming-in-node-js.html.) */

var net = require('net');

var client = new net.Socket();
client.connect(2000, '127.0.0.1', function() {
	console.log('Connected');
    ///client.write('Hello, server! Love, Client.');
    client.write('guest'); // authenticate on irc server as a guest
});

client.on('data', function(data) {
	console.log('Received: ' + data);
	//client.destroy(); // kill client after server's response
});

client.on('close', function() {
	console.log('Connection closed');
});

function startServerConnectoin() {
  var socket = new WebSocket('ws://127.0.0.1:2000');
  socket.onopen = function(event) {
	
    // Send an initial message
    socket.send('I am the client and I\'m listening!');
    
    // Listen for messages
    socket.onmessage = function(event) {
      console.log('Client received a message',event);
    };
    
    // Listen for socket closes
    socket.onclose = function(event) {
      console.log('Client notified socket has closed',event);
    };
    
    // To close the socket....
    //socket.close()
    
  };
}