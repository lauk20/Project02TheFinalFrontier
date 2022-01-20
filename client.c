#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "networking.h"

int main(int argc, char ** args){
  char * address = calloc(16, 1);
  strcpy(address, "127.0.0.1");
  if (argc > 1){
    strcpy(address, args[1]);
  }
  char * name = calloc(21, 1);
  printf("Name (<= 20 Characters): ");
  fgets(name, 20, stdin);

  mkfifo("display", 0644);
  mkfifo("readDisplay", 0644);

  printf("Please run ./client_display to continue, it will display the chat messages. This program is responsible for sending messages\n");

  int fdwrite = open("display", O_WRONLY, 0644);
  int fdread = open("readDisplay", O_RDONLY, 0644);

  printf("Client Display has been opened\n");

  int socket;

  socket = client_connect(address);

  fd_set read_descriptors;
  fd_set read_holder;

  int max_descriptor;
  if(socket > STDIN_FILENO){
    max_descriptor = socket;
  } else {
    max_descriptor = STDIN_FILENO;
  }

  FD_ZERO(&read_holder);
  FD_SET(socket, &read_holder);
  FD_SET(STDIN_FILENO, &read_holder);
  FD_SET(fdread, &read_holder);

  fflush(stdin);

  int connected = 1;
  *strchr(name, '\n') = 0;
  write(socket, name, strlen(name));
  printf("Type Your Message: \n");
  while(connected){
    FD_ZERO(&read_descriptors);
    read_descriptors = read_holder;

    int s = select(max_descriptor + 1, &read_descriptors, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_descriptors)){
      char * message = calloc(BUFFER_SIZE, 1);
      printf("Type Your Message: \n");
      fgets(message, BUFFER_SIZE, stdin);

      *strchr(message, '\n') = 0;

      write(socket, message, strlen(message));

      free(message);
    } else if (FD_ISSET(socket, &read_holder)){
      char * message = calloc(BUFFER_SIZE, 1);
      int r = read(socket, message, BUFFER_SIZE);

      if (r){
        printf("here2\n");
        int w = write(fdwrite, message, BUFFER_SIZE);
        printf("here\n");
        if (w == 0){
          connected = 0;
        }
      } else {
        connected = 0;
      }

      free(message);
    } else {
      printf("asdf\n");
      char * message = calloc(BUFFER_SIZE, 1);
      int r = read(fdread, message, BUFFER_SIZE);

      printf("Chat Display has been closed, disconnecting...\n");
    }
  }
}
