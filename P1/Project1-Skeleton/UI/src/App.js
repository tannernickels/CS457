import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import {  Button, ButtonGroup, InputGroupAddon, InputGroup, Input, 
          TabContent, TabPane, Nav, NavItem, NavLink, Card, CardTitle, CardText,
          ButtonDropdown, DropdownToggle, DropdownMenu, DropdownItem , Row, Col, Table } from 'reactstrap';
import classnames from 'classnames';
import Messages from "./Messages";
import MessageTable from './HashTable';


function randomName() {
  const adjectives = ["autumn", "hidden", "bitter", "misty", "silent", "empty", "dry", "dark", "summer", "icy", "delicate", "quiet", "white", "cool", "spring", "winter", "patient", "twilight", "dawn", "crimson", "wispy", "weathered", "blue", "billowing", "broken", "cold", "damp", "falling", "frosty", "green", "long", "late", "lingering", "bold", "little", "morning", "muddy", "old", "red", "rough", "still", "small", "sparkling", "throbbing", "shy", "wandering", "withered", "wild", "black", "young", "holy", "solitary", "fragrant", "aged", "snowy", "proud", "floral", "restless", "divine", "polished", "ancient", "purple", "lively", "nameless"];
  const nouns = ["waterfall", "river", "breeze", "moon", "rain", "wind", "sea", "morning", "snow", "lake", "sunset", "pine", "shadow", "leaf", "dawn", "glitter", "forest", "hill", "cloud", "meadow", "sun", "glade", "bird", "brook", "butterfly", "bush", "dew", "dust", "field", "fire", "flower", "firefly", "feather", "grass", "haze", "mountain", "night", "pond", "darkness", "snowflake", "silence", "sound", "sky", "shape", "surf", "thunder", "violet", "water", "wildflower", "wave", "water", "resonance", "sun", "wood", "dream", "cherry", "tree", "fog", "frost", "voice", "paper", "frog", "smoke", "star"];
  const adjective = adjectives[Math.floor(Math.random() * adjectives.length)];
  const noun = nouns[Math.floor(Math.random() * nouns.length)];
  return adjective + noun;
}

function randomColor() {
  return '#' + Math.floor(Math.random() * 0xFFFFFF).toString(16);
}




class App extends Component {
  constructor(props){
    super(props);
    this.state = {
      activeTable: 'server',
      client_id: '',
      data: null,
      tabs: {'tab-1': '1'},
      tabsList: ['server'],
      activeTab: '-1',
      messagesTable: new MessageTable(10),
      messages: [],
      user: {
        id: 'user',
        username: randomName(),
        color: randomColor()
      },
      server: {
        id: 'server',
        username: 'server',
        color: randomColor()
      },
      receivers: new MessageTable(10) //bad practice but this will hold bubbles in the format <id: {id: 'server', username, color}> 
    };
    this.sendMessage = this.sendMessage.bind(this);
    this.getMail = this.getMail.bind(this);
    this.toggle = this.toggle.bind(this);
    this.renderTabs = this.renderTabs.bind(this);
    this.clearMessages = this.clearMessages.bind(this);
    this.getReceiverID = this.getReceiverID.bind(this);
    this.processMail = this.processMail.bind(this);
  }
  componentWillMount(){
    var table = this.state.messagesTable;
    table.insert('server', []);
    this.setState({messagesTable: table});
  }
  componentDidMount() {
    this.setState({client_id: document.location.host});
    // Call our fetch function below once the component mounts
    this.callBackendAPI()
      .then(res => this.setState({ data: res.express }))
      .catch(err => console.log(err));
    // initiate continuous calls to clients mail box
    this.getMail();
  }

  toggle(tab) {
    if (this.state.activeTab !== tab) {
      this.setState({
        activeTab: tab,
        activeTable: tab
      });
    }
  }

  clearMessages(){
    //this.setState({messages: []});
    var table = this.state.messagesTable;
    table.insert(this.state.activeTable, []);
    this.setState({messagesTable: table});
  }


  async sendMessage(){
    var table = this.state.messagesTable;
    var messages = table.get(this.state.activeTable);

    messages.push({
      text: document.getElementById('msg-input').value,
      member: this.state.user
    });
    table.insert(this.state.activeTable, messages);
    this.setState({messagesTable: table});

    console.log(this.state.client_id);
    const body_data = {id: this.state.client_id}
    const response = await fetch('http://localhost:57000/message', {
        method: "POST", // *GET, POST, PUT, DELETE, etc.
        mode: "cors", // no-cors, cors, *same-origin
        headers: {
            "Content-Type": "text/plain"
            // "Content-Type": "application/x-www-form-urlencoded",
        },
        body: this.state.client_id + '~' + document.getElementById('msg-input').value // body data type must match "Content-Type" header
    });
    const res = await response.json();
    //alert(await res.express);
    if(res.express != "null"){
      var table = this.state.messagesTable;
      var message_list = table.get(this.state.activeTable);
      
        for(var i=0; i < res.express.split('~').length; i++){ 
          message_list.push({
            text: res.express.split('~')[i],
            member: this.state.server
          });
          
        }
        //this.setState({messages: message_list});
        var updateTable = this.state.messagesTable;
        updateTable.insert(this.state.activeTable, message_list);
      
        this.setState({messagesTable: updateTable})
        
      }
    }
  

  // Fetches our POST route from the Express server. (Note the route we are fetching matches the POST route from server.js
  callBackendAPI = async () => {
    const response = await fetch('http://localhost:57000/express_backend', {
      method: "POST", // *GET, POST, PUT, DELETE, etc.
        mode: "cors", // no-cors, cors, *same-origin
        headers: {
            "Content-Type": "text/plain"
        },
        body: document.location.host, // body data type must match "Content-Type" header
    });
    const body = await response.json();

    if (response.status !== 200) {
      throw Error(body.message) 
    }
    return body;
  };

  getReceiverID(message){
    var id = message.substring(message.indexOf("<") + 1);
    id = id.substring(0, id.indexOf(">"));
    return id;
  }

  // handles UI processing for new mail
  processMail(res){
    //parse out id and msg... <id> msg
    for(var m=0; m < res.messages.split('~').length; m++){
      var message = res.messages.split('~')[m];
      if(message === 'null'){continue;}
      var id = this.getReceiverID(message);
      var chan_user = 'null';
      var isChannelMsg = false;
      if(id.charAt(0)==='#'){
        id = id.substring(0, id.indexOf(":"))
        chan_user = message.substring(message.indexOf(":") + 1);
        chan_user = chan_user.substring(0, chan_user.indexOf(">"));
        isChannelMsg = true;
      }
    
      var msg = message.substring(message.indexOf(">") + 1);

      var user_table = this.state.receivers;

      var bubble = user_table.get(id); 
      if(isChannelMsg){
        bubble = user_table.get(chan_user);
      }
      if (bubble == null){ // create bubble for user if not found
        if(isChannelMsg){
          user_table.insert(chan_user, {
            id: chan_user,
            username: chan_user,
            color: randomColor()
          });
          this.setState({receivers: user_table});
          user_table = this.state.receivers;
          bubble = user_table.get(chan_user);
        }
        else{
          user_table.insert(id, {
            id: id,
            username: id,
            color: randomColor()
          });
          this.setState({receivers: user_table});
          user_table = this.state.receivers;
          bubble = user_table.get(id);
        }
      }

      var table = this.state.messagesTable;
      var messages = table.get(id);

      if(messages == null){ // received message from a new channel or user.. create tab and add data to the respective messages array
        messages = [];
        
        messages.push({
              text: msg,
              member: bubble
        });
      
        table.insert(id, messages);
        this.setState({messagesTable: table}); // add message
        var tabList = this.state.tabsList;
       
        if(!tabList.includes(id)){
          tabList.push(id);
          this.setState({tabsList: tabList}); // add tab to list if needed
        }
      }
      else{
        
        messages.push({
            text: msg,
            member: bubble
        });
        
        table.insert(id, messages);
        this.setState({messagesTable: table}); // add message
      }
      this.setState({activeTable: id}); // change tab
    }
  }
  // Fetches all unread mailing waiting for this client on the Express server. Gets called every 5 seconds
  getMail = async() => {
    const body_data = {id: this.state.client_id}
    const response = await fetch('http://localhost:57000/mail', {
        method: "POST",
        mode: "cors", 
        headers: {"Content-Type": "text/plain"},
        body: this.state.client_id
    });
    const res = await response.json();
    //alert(JSON.stringify(res));
    if(res.messages != "null"){
      this.processMail(res);
    }
    setTimeout(this.getMail, 5000)
  }
  
  renderTabs(){
    const tabs = this.state.tabsList.map(item => <NavItem><NavLink className={classnames({ active: this.state.activeTab === item })}
    onClick={() => { this.toggle(item); }}>{item}</NavLink></NavItem> );
    return (
      <Nav tabs>
       {tabs}
      </Nav>
    );
                    
  }
  
  render() {
    return (
      
      <div className="App">
        
        <section className="chat-ui">
          <div className="chat-header">
          <h1>IRC Chat App</h1>
          {this.renderTabs()}
          </div>
          <div className="chat-messages">
            <Messages messages={this.state.messagesTable.get(this.state.activeTable)} currentMember={this.state.user}/>
          </div>
          
        </section>
        <div className="chat-input">
            <InputGroup>
              <InputGroupAddon addonType="prepend">
                <Button type="submit" color="primary" onClick={() => this.sendMessage()} >Send</Button>
              </InputGroupAddon>
              <Input id="msg-input"/>
              <InputGroupAddon addonType="prepend">
                <Button color="primary" onClick={() => this.clearMessages()} >Clear Chat</Button>
              </InputGroupAddon>
            </InputGroup>
        </div>
        
      </div>
      
    );
  }
}

export default App;
