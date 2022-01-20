#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "networking.h"

int main(){
  int fd = open("display", O_RDONLY, 0644);

  if (fd > -1){
    remove("display");
    int connected = 1;
    while (connected){
      char * message = calloc(BUFFER_SIZE, 1);
      if (read(fd, message, BUFFER_SIZE)){
        printf("%s\n", message);
      } else {
        connected = 0;
      }
    }
  } else {
    printf("Please run ./client before running this\n");
  }

  return 0;
}
