const express = require('express');
const path = require('path');

const app = express();

// // Serve static files from the public directory
// app.use(express.static(path.join(__dirname, 'public')));

// Route for the login page
app.get('/login', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'login.html'));
});

// Start the server
const port = 3000;
app.listen(port, () => {
    console.log(`Server is running on port ${port}`);
});
