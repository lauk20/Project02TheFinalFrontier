# Project02TheFinalFrontier
## DOCUMENTATION ##

**Project Name:** C-hat

**Team Members and Class Period:** Kenny Lau, Period 5

**Broad Description of Project:** A chat server that can handle multiple clients at the same time.

**List of Any Required Libraries:** NONE

**Instructions to Run:**
1. Run ```make``` to compile the code.
2. Run ```./server``` to start the Server. The Server will start on the machine that you are running this command on. The Server process will print information about what each Process/Sub-process (sub-server) has read from the Client or Main Server.
3. Running the Clients:
- IF you wish to run a Client on the same machine as the Server *which would give it special permissions to kick or mute other Clients (see 5)*. In a *NEW* terminal, please run ```./client```, which will connect to the default address: 127.0.0.1. Next, you should be prompted for a username, that is <= 20 Characters long. After that, you will be asked to run ```./client_display```; also do this in a *NEW* terminal.
- IF you are connecting to the Server from a different machine, run ```./client [SERVER ADDRESS]```. Example: if the Server were started on cslab1-19, you would run ```./client 149.XX.XXX.119``` (REPLACE the X's with the ACTUAL NUMBERS, removed them here in case it shouldn't be on a public Github repo). Now that the Client program has started, you will be asked for a username <= 20 Characters, enter that and it will ask you to run ```./client_display```. Once all that is done, you can now send messages to the Server.
4. Seeing the Messages: Every Client should have 2 terminal windows: one for ```./client``` and one for ```./client_display```. This is because the ```./client``` one is used to send messages while the ```./client_display``` one is used to receive and print messages.
5. Clients with Special Permissions:
List of Commands:
```
/kick username
/mute username
```
If you are connecting a Client to a Server using a machine that is running a Server, you will have special Permissions to **kick** or **mute** a user from the chat. The commands must be typed exactly like this in the ```./client``` terminal: ```/kick username``` (no extra spaces and must be the target's exact username). (Type it in the same place you would type a normal message to send to the Server). Same applies for mute: ```/mute username```

**Concepts Used**
1. Allocating Memory (Allocated memory for structs/strings)
2. Processes (Main Server forking off Sub-Servers to handle individual Clients).
3. Pipes (Both Named and Unnamed: Named Pipe for ./client to send messages for ./client_display to display to the user. Unnamed Pipes that connect each Sub-Server to the Main Server).
4. Sockets (Sockets used for the Client and Sub-Server and Server to communicate on different machines).


## PROPOSAL ##

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
