/* eslint react/no-multi-comp: 0, react/prop-types: 0 */
/* ReactStrap example */
import React from 'react';
import { Button, Modal, ModalHeader, ModalBody, ModalFooter, Form, FormGroup, Label, Input } from 'reactstrap';
import './App.css';

class LoginModal extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      modal: true
    };

    this.toggle = this.toggle.bind(this);
    this.authenticateUser = this.authenticateUser.bind(this);
    this.registerNewGuest = this.registerNewGuest.bind(this);
  }

  toggle() {
      if(this.props.buttonLabel == 'disconnect'){
          this.props.disconnect();
      }
    this.setState({
      modal: !this.state.modal
    });
  }

  registerNewGuest(){
    this.props.guest_sign_in();
    this.toggle();
  }

  authenticateUser(){
    this.props.user_sign_in(document.getElementById('username').value, document.getElementById('password').value);
    this.toggle();
  }


  render() {
      
    return (
      <div id="modal_popup" className ="modal_popup" >
        <Button id="modal_popup" color="primary" onClick={this.toggle}>{this.props.buttonLabel}</Button>
        <Modal isOpen={this.state.modal} modalTransition={{ timeout: 700 }} backdropTransition={{ timeout: 1300 }}
          toggle={this.toggle} className={this.props.className}>
          <ModalHeader toggle={this.toggle}>Join IRC Server</ModalHeader>
          <ModalBody>
            <Form inline>
                <FormGroup>
                    <Label for="exampleEmail" hidden>Username</Label>
                    <Input type="email"  name="email" id="username" placeholder="Username" />
                </FormGroup>
                {' '}
                <FormGroup>
                    <Label for="examplePassword" hidden>Password</Label>
                    <Input type="password" name="password" id="password" placeholder="Password" />
                </FormGroup>
            </Form>
            <Button block color="primary" onClick={this.authenticateUser}>Submit</Button>
          </ModalBody>
          
          <ModalFooter>
            
            <Button color="secondary" onClick={this.registerNewGuest}>Join As Guest</Button>{' '}
            <Button color="secondary" onClick={this.toggle}>Cancel</Button>
          </ModalFooter>
        </Modal>
      </div>
    );
  }
}

export default LoginModal;