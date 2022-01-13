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
int send_to_server(char * message, int pipe_write){
  write(pipe_write, message, BUFFER_SIZE);
  //printf("wrote to server\n");

  return 0;
}

//handles subserver communications between client
int subserver_handler(int client_socket, int pipe_read, int pipe_write){
  printf("Sub-Server created\n");

  fd_set read_descriptors;

  int max_descriptor;
  if(client_socket > pipe_read){
    max_descriptor = client_socket;
  } else {
    max_descriptor = pipe_read;
  }

  while(1){
    FD_ZERO(&read_descriptors);
    FD_SET(client_socket, &read_descriptors);
    FD_SET(pipe_read, &read_descriptors);

    int s = select(max_descriptor + 1, &read_descriptors, NULL, NULL, NULL);

    if (FD_ISSET(client_socket, &read_descriptors)){
      char * message = calloc(BUFFER_SIZE, 1);
      int bytes_read = read(client_socket, message, BUFFER_SIZE);

      if (bytes_read != 0){
        printf("[%d] Sub-Server has read from Client: %s\n", getpid(), message);
        send_to_server(message, pipe_write);
      } else {
        printf("Client not connected\n");
      }

      free(message);
    } else {
      char * message = calloc(BUFFER_SIZE, 1);
      int bytes_read = read(pipe_read, message, BUFFER_SIZE);

      if (bytes_read != 0){
        printf("[%d] Sub-Server has read from Server: %s\n", getpid(), message);
        send_to_client(client_socket, message);
      } else {
        printf("Server not connected\n");
      }

      free(message);
    }
  }

  /*
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
    //subserver_server(client_socket, pipe_read);
  }
  */

  exit(0);
}

int send_to_subserver(char * message, int pipe_write){
  write(pipe_write, message, BUFFER_SIZE);

  return 0;
}

int send_to_all(char * message, int pipes[10]){
  int i;
  for (i = 0; i < 10; i++){
    printf("%d\n", pipes[i]);
    if (pipes[i] > -1){
      send_to_subserver(message, pipes[i]);
    }
  }
}

//main server sets up subservers
int main(){
  int listening_socket = server_create();
  int subserver_read_pipes[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  int subserver_write_pipes[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

  fd_set read_descriptors;

  while(1){
    FD_ZERO(&read_descriptors);
    FD_SET(listening_socket, &read_descriptors);

    int max_descriptor = listening_socket;
    int i = 0;
    for (i = 0; i < 10; i++){
      FD_SET(subserver_read_pipes[i], &read_descriptors);
      //printf("set %d\n", subserver_read_pipes[i]);
      if (subserver_read_pipes[i] > max_descriptor){
        max_descriptor = subserver_read_pipes[i];
      }
    }

    int s = select(max_descriptor + 1, &read_descriptors, NULL, NULL, NULL);

    if (FD_ISSET(listening_socket, &read_descriptors)){
      int client_socket = server_accept(listening_socket);

      int fds[2]; //SERVER READ
      pipe(fds);

      int v = 0;
      for (v = 0; v < 10; v++){
        if (subserver_read_pipes[v] == -1){
          subserver_read_pipes[v] = fds[0];
          //printf("added %d\n", subserver_read_pipes[v]);
          break;
        }
      }

      int fds2[2]; //SERVER WRITE
      pipe(fds2);

      v = 0;
      for (v = 0; v < 10; v++){
        if (subserver_write_pipes[v] == -1){
          subserver_write_pipes[v] = fds2[0];
          //printf("added %d\n", subserver_read_pipes[v]);
          break;
        }
      }

      int f = fork();

      if (f == 0){ //subserver
        close(fds[0]);
        close(fds2[1]);
        subserver_handler(client_socket, fds2[0], fds[1]);
      } else { //main server
        close(fds[1]);
        close(fds2[0]);
        close(client_socket);
      }
    }

    i = 0;
    for (i = 0; i < 10; i++){
      //printf("checking %d\n", subserver_read_pipes[i]);
      if (subserver_read_pipes[i] != -1 && FD_ISSET(subserver_read_pipes[i], &read_descriptors)){
        //printf("Read from SubServer!\n");
        char * message = calloc(BUFFER_SIZE, 1);
        read(subserver_read_pipes[i], message, BUFFER_SIZE);
        printf("[%d] Server read from Sub-Server: %s\n", getpid(), message);
        send_to_all(message, subserver_write_pipes);

        free(message);
      }
    }

  }
}
