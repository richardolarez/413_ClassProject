# 413_ClassProject
Class Project for 413
The team had to create an application that is running on an aws server with a mongoDB storing the data from an IoT device that is used to record heart heart for the users. This IoT is based off a particle argon and the sensor on a breadboard. The app will keep track of their heart rates and generate a weekly and detailed view report.

Group 9:
  Mario Weiler
  Antony Mangala
  Richardo Larez

Running the project:
  First start a local instance of MongoDB
  first go to the directory /413_Project
  Then you can run run npm start
  The app will then be on http://localhost:3002

The main API is used for the webhook to send data from the device to the server that then saves it to the database.

This api is http:localhost:3002/api/particle_data

Login Credentials
  mario@email.com
  Password123

Link to Server
  
  http://ec2-18-116-36-90.us-east-2.compute.amazonaws.com:3000/

Pitch Video
https://youtu.be/UlINpHNgYcA

Demonstration Video

UX Video
https://youtu.be/R7MJeDkFOvg


  
