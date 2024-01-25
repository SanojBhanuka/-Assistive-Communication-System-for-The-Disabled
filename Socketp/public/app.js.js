// server.js

const express = require('express');
const WebSocket = require('ws');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');

var message;
var received = false;


const app = express();
app.use(express.static('public'));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

const server = app.listen(3000, () => {
  console.log('Listening on port 3000');
});

// Handle POST requests to /api/data endpoint
app.post('/api/data', (req, res) => {

  console.log("Request arrived");
  const letter = req.body.letter;
  console.log('letter = '+letter);
  message = letter;
  received = true;

});


  // Create WebSocket server
  const wss = new WebSocket.Server({ server });


  wss.on('connection', (ws) => {
    console.log('WebSocket connected');

    console.log("Doneeeee");
    
    setInterval(() =>{
      console.log("received = "+received);
      if(received){
        console.log('inside if, letter = '+ message);
        ws.send(message);
        received = false;
       // message = "";
      }
      
    }, 3000)
  
  });


