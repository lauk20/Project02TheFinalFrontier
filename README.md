# Project02TheFinalFrontier

**Members:** Kenny Lau

**Class Period:** 5

**Project Description:** A chat room/server that can handle multiple clients connecting. Each client should be able to send their own message and receive messages from others.

**How the Project Will Be Used:** All interaction will happen through the terminal. Client sending and receiving messages will all appear on the terminal (though sending and receiving messages may need to be on different terminals). Creating the server and connecting to it will also be done through the terminal by running the C programs. One feature that it should contain is that every Client should be able to enter a name they want to go by when chatting.

**Technical Design:**
1. Topics covered in class that will be used: Networking concepts (sockets), Pipes (sub-server and server communication), Allocating memory, Processes (forking server/sub-server)
2. Breakdown of the project/who is responsible for what: I will be responsible for everything because I am working alone. There are three main parts to the project: the server and the client and how to handle them.
- Client and Main Server connection, then forking off to Sub-Server.
- Sub-Server handling all communications with the Client and Main Server (read from Server, write to Server, read from Client, and write to Client)
- Main Server will need to write to all Sub-Servers, which will need to send to all Clients.
- Client displays message in a way that does not interfere with terminal typing/input.
3. Data structures: structs may be used to be used as the data being sent. The message struct should contain the client's name and message. Maybe an array to store all active connection file descriptors so that the server can write to all clients.
4. Algorithms: I will be using the forking server model to handle client connections.

**Timeline:**
1. 1/14/22: Complete basic Client and Server connection, including forking off a Sub-Server to handle each individual Client.
2. 1/17/22: Complete Client -> Sub-Server -> Main Server communication. Complete Main Server -> All Sub-Servers -> Clients communication.
3. 1/19/22: Complete the two terminal setup for Clients (one to display incoming messages and one to send messages since the typing could be messed up if the incoming messages are printed while a person is typing on the terminal)
4. 1/21/22: Complete the name feature, where every message will display who sent a message.
5. 1/23/22: Complete/make sure everything works correctly/make it so that the server/clients disconnect cleanly.
