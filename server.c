#include <stdio.h>
#include <stdlib.h>
#include "networking.h"
#include <errno.h>

struct message_struct {
  char name[21];
  char message[BUFFER_SIZE - 21 - sizeof(int)];
  int type;
};

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
  int wrote = write(client_socket, message, BUFFER_SIZE);
  //printf("wr: %d\n", wrote);

  return wrote;
}

//THIS FUNCTION ENDED UP NOT BEING USED
//handles subserver communcations with main server
/*
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
*/

//send from subserver to main server
int send_to_server(struct message_struct * message, int pipe_write){
  int w = write(pipe_write, message, BUFFER_SIZE);
  //printf("wrote to server %d\n", w);
  free(message);

  return 0;
}

//handles subserver communications between client
int subserver_handler(int client_socket, int pipe_read, int pipe_write, int to_close){
  printf("Sub-Server created\n");

  char * name = calloc(21, 1);

  fd_set read_descriptors;

  int max_descriptor;
  if(client_socket > pipe_read){
    max_descriptor = client_socket;
  } else {
    max_descriptor = pipe_read;
  }

  int connected = 1;
  while(connected){
    FD_ZERO(&read_descriptors);
    FD_SET(client_socket, &read_descriptors);
    if (errno){
      printf("%1s\n", strerror(errno));
      exit(-1);
    }
    FD_SET(pipe_read, &read_descriptors);
    if (errno){
      printf("%2s\n", strerror(errno));
      exit(-1);
    }

    int s = select(max_descriptor + 1, &read_descriptors, NULL, NULL, NULL);

    if (FD_ISSET(client_socket, &read_descriptors)){
      char * message = calloc(BUFFER_SIZE, 1);
      int bytes_read = read(client_socket, message, BUFFER_SIZE);

      if (bytes_read != 0){
        if (strlen(name) == 0){
          printf("[%d] Sub-Server has read NAME from Client: %s\n", getpid(), message);
          strcpy(name, message);
          printf("[%d] %s has joined the server\n", getpid(), name);
          struct message_struct * message_data = calloc(sizeof(struct message_struct), 1);
          strcpy(message_data->name, "SERVER");
          strcpy(message_data->message, message);
          //printf("%s work\n", message_data->message);
          strcat(message_data->message, " has joined the chat!");
          send_to_server(message_data, pipe_write);
        } else {
          printf("[%d] Sub-Server has read from Client: %s\n", getpid(), message);
          struct message_struct * message_data = calloc(sizeof(struct message_struct), 1);
          strcpy(message_data->name, name);
          strcpy(message_data->message, message);
          send_to_server(message_data, pipe_write);
        }
      } else {
        printf("Client not connected\n");
        connected = 0;
      }

      free(message);
    } else {
      char * message = calloc(BUFFER_SIZE, 1);
      struct message_struct * message_data = calloc(sizeof(struct message_struct), 1);
      int bytes_read = read(pipe_read, message_data, BUFFER_SIZE);
      if (errno){
        printf("%s\n", strerror(errno));
        exit(-1);
      }

      if (bytes_read != 0){
        printf("[%d] Sub-Server has read from Server: %s\n", getpid(), message_data->message);
        strcat(message, "[");
        strcat(message, message_data->name);
        strcat(message, "] ");
        strcat(message, message_data->message);
        send_to_client(client_socket, message);
      } else {
        //printf("Server not connected\n");
      }

      free(message);
      free(message_data);
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
  struct message_struct * message_data = calloc(sizeof(struct message_struct), 1);
  message_data->type = to_close;
  send_to_server(message_data, pipe_write);
  exit(0);
}

int send_to_subserver(struct message_struct * message, int pipe_write){
  int w = write(pipe_write, message, BUFFER_SIZE);

  return w;
}

int send_to_all(struct message_struct * message, fd_set * write_set, int max){
  int i;
  for (i = 0; i <= max; i++){
    if (FD_ISSET(i, write_set)){
      int wrote = send_to_subserver(message, i);
      if (wrote == 0){
        FD_CLR(i, write_set);
      }
    }
  }
}

//main server sets up subservers
int main(){
  int listening_socket = server_create();
  //int subserver_read_pipes[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  //int subserver_write_pipes[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

  fd_set read_descriptors;
  fd_set read_holder;
  FD_ZERO(&read_holder);
  FD_SET(listening_socket, &read_holder);

  fd_set write_descriptors;
  fd_set write_holder;
  FD_ZERO(&write_holder);

  int max_descriptor = listening_socket;

  while(1){
    FD_ZERO(&read_descriptors);
    FD_ZERO(&write_descriptors);
    read_descriptors = read_holder;
    write_descriptors = write_holder;

    int i = 0;

    int s = select(max_descriptor + 1, &read_descriptors, NULL, NULL, NULL);

    if (FD_ISSET(listening_socket, &read_descriptors)){
      int client_socket = server_accept(listening_socket);

      int fds[2]; //SERVER READ
      pipe(fds);
      FD_SET(fds[0], &read_holder);

      if (fds[0] > max_descriptor){
        max_descriptor = fds[0];
      }

      int fds2[2]; //SERVER WRITE
      pipe(fds2);
      FD_SET(fds2[1], &write_holder);

      if (fds2[1] > max_descriptor){
        max_descriptor = fds2[1];
      }

      int f = fork();

      if (f == 0){ //subserver
        close(fds[0]);
        close(fds2[1]);
        subserver_handler(client_socket, fds2[0], fds[1], fds2[1]);
      } else { //main server
        close(fds[1]);
        close(fds2[0]);
        close(client_socket);
      }
    }

    i = 0;
    for (i = 0; i <= max_descriptor; i++){
      //printf("%d\n", i);
      if (FD_ISSET(i, &read_descriptors) && i != listening_socket){
        //char * message = calloc(BUFFER_SIZE, 1);
        struct message_struct * message_data = calloc(sizeof(struct message_struct), 1);

        if (read(i, message_data, BUFFER_SIZE)){
          printf("[%d] Server read from Sub-Server: %s\n", getpid(), message_data->message);
          if (message_data->type == 0){
            send_to_all(message_data, &write_holder, max_descriptor);
          } else {
            FD_CLR(message_data->type, &write_descriptors);
            FD_CLR(message_data->type, &write_holder);
          }
        } else {
          FD_CLR(i, &read_holder);
          close(i);
          printf("closed\n");
        }

        //free(message_data);
      }
    }

    /*
    i = 0;
    for (i = 0; i < 10; i++){
      //printf("checking %d\n", subserver_read_pipes[i]);
      if (subserver_read_pipes[i] != -1 && FD_ISSET(subserver_read_pipes[i], &read_descriptors)){
        //printf("Read from SubServer!\n");
        char * message = calloc(BUFFER_SIZE, 1);
        read(subserver_read_pipes[i], message, BUFFER_SIZE);
        printf("[%d] Server read from Sub-Server: %s\n", getpid(), message);
        /*int w = write(subserver_write_pipes[i], message, BUFFER_SIZE);
        pritnf("w: %d\n", w);

        send_to_all(message, subserver_write_pipes);

        free(message);
      }
    }
    */

  }

  return 0;
}
