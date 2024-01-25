const express = require('express');
const bodyParser = require('body-parser');
const app = express();
//const mongoose = require('mongoose');
const axios = require('axios');
const WebSocket = require('ws');
const http = require('http');  // Import the http modul

// Create an HTTP server using the app
const server = http.createServer(app);

// Create a WebSocket server using the server instance
const wss = new WebSocket.Server({ server });

let messages = ['helloo'];

const port = 3000;
// Use bodyParser middleware to parse incoming POST requests
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

// Handle POST requests to /api/data endpoint
app.post('/api/data', (req, res) => {

  console.log("Request arrived");
  const letter = req.body.letter;
  console.log('letter = '+letter);
  messages.push(letter);

});

// Handle WebSocket connections from frontend
wss.on('connection', (socket) => {
  console.log('WebSocket connection established');

  // Send existing messages to the new connection
  socket.send(JSON.stringify(messages));

  // Handle incoming WebSocket messages (if needed)
  // socket.on('message', (message) => {
  //   // Handle WebSocket messages from the frontend, if needed
  // });
});

// Start the server
app.listen(port, () => {
  console.log(`Server started on port ${port}`);
});

