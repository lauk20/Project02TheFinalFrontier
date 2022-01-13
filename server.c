#include <stdio.h>
#include <stdlib.h>
#include "networking.h"

//creates server socket and listens
//returns listening socket
int server_create(){
  return server_setup();
}

//accepts connections
//returns client socket created by accept
int server_accept(int listening_socket){
  return server_connect(listening_socket);
}

//sends from subserver to client
int send_to_client(int client_socket, char * message){
  write(client_socket, message, BUFFER_SIZE);

  return 0;
}

//handles subserver communcations with main server
int subserver_server(int client_socket, int pipe_read){
  char * message = calloc(BUFFER_SIZE, 1);

  int connected = 1;
  while(connected){
    int bytes_read = read(pipe_read, message, BUFFER_SIZE);

    if (bytes_read != 0){
      printf("[%d] Sub-Server has read from Main Server: %s\n", getpid(), message);
      send_to_client(client_socket, message);
    } else {
      connected = 0;
    }
  }

  exit(0);
}

//send from subserver to main server
int send_to_server(char * message, int pipe_write)){
  write(pipe_write, message, BUFFER_SIZE);

  return 0;
}

//handles subserver communications between client
int subserver_handler(int client_socket, int pipe_read, int pipe_write){
  char * message = calloc(BUFFER_SIZE, 1);

  int f = fork();

  if (f){
    int connected = 1;
    while(connected){
      int bytes_read = read(client_socket, message, BUFFER_SIZE);

      if (bytes_read != 0){
        printf("[%d] Sub-Server has read from client: %s\n", getpid(), message);
        send_to_server(message, pipe_write);
      } else {
        connected = 0;
      }
    }
  } else {
    subserver_server(client_socket, pipe_read);
  }

  exit(0);
}

//main server sets up subservers
int main(){
  int listening_socket = server_create();

  while(1){
    int client_socket = server_accept();

    int fds[2]; //SERVER READ
    pipe(fds);

    int fds2[2]; //SERVER WRITE
    pipe(fds2);

    int f = fork();

    if (f == 0){ //subserver
      close(fds[0]);
      close(fds2[1]);
      subserver_handler(client_socket, fds2[0], fds[1]);
    } else { //main server
      close(fds[1]);
      close(fds2[0]);
      close(client_socket);

      f = fork();

      if (f == 0){
        send_to_all(pipe_read, pipe_write);
      }
    }
  }
}
