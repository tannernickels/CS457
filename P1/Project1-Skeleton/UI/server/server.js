const express = require('express');
const app = express();
const port = process.env.PORT || 57000;


// console.log that your server is up and running
app.listen(port, () => console.log(`Listening on port ${port}`));

// create a GET route
app.get('/express_backend', (req, res) => {
    res.header("Access-Control-Allow-Origin", "*");
    res.send({ express: 'YOUR EXPRESS BACKEND IS CONNECTED TO REACT' });

    /* tcp client written in node.js.  (Originated with 
        example code from 
        http://www.hacksparrow.com/tcp-socket-programming-in-node-js.html.) */
    var net = require('net');

    var client = new net.Socket();
    
    client.connect(2000, '127.0.0.1', function() {
      console.log('Connected');
        client.write('guest'); // authenticate on irc server as a guest
    });

    client.on('data', function(data) {
      console.log('Received: ' + data);
      //client.destroy(); // kill client after server's response
    });

    client.on('close', function() {
      console.log('Connection closed');
    });
});


