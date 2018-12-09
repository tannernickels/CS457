const express = require('express');
const bodyParser = require('body-parser')
const app = express();
const port = process.env.PORT || 57000;
// Table to hole <k,v> wher <"client id" : "client connection">
const Table = require('./HashTable.js'); // Functions: hash, get, getAll, insert

// middleware -- allows plain/text content types to be posted in req.body
app.use(bodyParser.text({type: '*/*'}));

// console.log that server is up and running
app.listen(port, () => console.log(`Listening on port ${port}`));

const table = new Table(100); // table holds host:client key-value pairs for tcp sockets connections to IRC server
const inbox = new Table(100); // inbox holds server echo responses: app.post(/message)
const mail = new Table(100);  // mail holds privmsg and channel msgs: app.get(/mail)

// create a POST route for initiating client connections
app.post('/express_backend', (req, res) => {

    console.log("key: " + req.body);
    res.header("Access-Control-Allow-Origin", "*");
    res.send({ express: 'EXPRESS BACKEND IS CONNECTED' });

    /* tcp client written in node.js -- reference: http://www.hacksparrow.com/tcp-socket-programming-in-node-js.html.) */
    var net = require('net');

    var client = new net.Socket();
    /*  to client-table */
    table.insert(req.body, client);

    client.connect(2000, '127.0.0.1', function() {
      console.log('Connected');
      client.write('guest'); // authenticate as a guest on irc server
    });
    
    client.on('data', function(data) {
      console.log('Received: ' + data);
      if(String(data).charAt(0) == '<'){
        // received a privmsg or channel msg
        var currentMessages = mail.get(req.body);
        var appendedMessages = currentMessages + '~' + data;
        mail.insert(req.body, appendedMessages);
        
      }
      else if(inbox.get(req.body) == null){
        // inbox is empty -- add server echo response for client
        inbox.insert(req.body, data);
      }
      else{
        // if a message in the inbox already exists then append the new message. messages are delimeted by ~
        var currentMessage = inbox.get(req.body);
        var appendedMessage = currentMessage + '~' + data;
        inbox.insert(req.body, appendedMessage)
      }
      //client.destroy(); // kill client after server's response
    });
    
    client.on('close', function() {
      console.log('Connection closed');
    });
});


const sleep = (milliseconds) => {
  return new Promise(resolve => setTimeout(resolve, milliseconds))
}

// Used to relay messages to the IRC server
app.post('/message', (req, res) => {

  var data = req.body.split('~');
  var client_id = data[0];
  var msg = data[1]; 

  console.log("client[" + client_id + "] is sending " + msg);
 
  var client = table.get(client_id);
  client.write(msg);

  res.header("Access-Control-Allow-Origin", "*");
  sleep(500).then(() => {
    var ircResponse = inbox.get(client_id);
    inbox.insert(client_id, null); // clear inbox
    return res.send({ express: String(ircResponse)});
  })

});

// Used to pull unread mail for UI (privmsg or channel msg)
app.post('/mail', (req, res) => {
  var client_id = req.body;
  var unread_mail = mail.get(client_id);
  console.log("client[" + client_id + "] is pulling mail");
  res.header("Access-Control-Allow-Origin", "*");
  mail.insert(client_id, null); // clear inbox
  return res.send({ messages: String(unread_mail) });
});