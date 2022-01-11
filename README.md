# Project02TheFinalFrontier

**Members:** Kenny Lau

**Class Period:** 5

**Project Description:** A chat room/server that can handle multiple clients connecting. Each client should be able to send their own message and receive messages from others.

**How the Project Will Be Used:** All interaction will happen through the terminal. Client sending and receiving messages will all appear on the terminal (though sending and receiving messages may need to be on different terminals). Creating the server and connecting to it will also be done through the terminal by running the C programs.

**Technical Design:**
1. Topics covered in class that will be used: Networking concepts (sockets), Pipes (sub-server and server communication), Allocating memory, Processes (forking server/sub-server)
2. Breakdown of the project/who is responsible for what: I will be responsible for everything because I am working alone. There are three main parts to the project: the server and the client and how to handle them.
- Client and Main Server connection, then forking off to Sub-Server.
- Sub-Server handling all communications with the Client and Main Server (read from Server, write to Server, read from Client, and write to Client)
- Main Server will need to write to all Sub-Servers, which will need to send to all Clients.
- Client displays message in a way that does not interfere with terminal typing/input.
3. Data structures: SOCKETS to handle client and server/sub-server connection/communications. PIPES to handle sub-server and main server communication. MEMORY ALLOCATION to handle anything involving memory (such as strings/structs). FORK (sub-processes) to handle the sub-servers.
4. Algorithms: I will be using the forking server model to handle client connections.

**Timeline:**
1. ??/??/??: Complete ?
