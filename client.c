#include <stdio.h>
#include <stdlib.h>
#include "networking.h"

int main(){
  int socket;

  socket = client_connect();

  while(1){
    char * message = calloc(BUFFER_SIZE, 1);
    printf("Input: \n");
    fgets(message, BUFFER_SIZE, stdin);

    write(socket, message, strlen(message));

    free(message);
  }
}
