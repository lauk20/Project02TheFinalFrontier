#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "networking.h"

int rfd = 0;

static void signal_handler(int signal){
  if (signal == SIGINT){
    if (rfd){
      printf("here123 %d\n", rfd);
      write(rfd, "asdf", 1);
      exit(0);
    }
  }
}

int main(){
  signal(SIGINT, signal_handler);

  int fd = open("display", O_RDONLY, 0644);
  int fdwrite = open("readDisplay", O_WRONLY, 0644);
  rfd = fdwrite;

  if (fd > -1){
    remove("display");
    remove("readDisplay");
    int connected = 1;
    while (connected){
      char * message = calloc(BUFFER_SIZE, 1);
      int r = read(fd, message, BUFFER_SIZE);
      if (r){
        printf("%s\n", message);
      } else {
        printf("%d\n", r);
        connected = 0;
      }
    }
  } else {
    printf("Please run ./client before running this\n");
  }

  return 0;
}
