#include "networking.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


//returns socket descriptor for server
int server_setup() {
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, "25932", hints, &results);  //Server sets node to NULL

  //create socket
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  int yes = 1;
  if ( setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 ) {
    printf("sockopt  error\n");
    exit(-1);
  }

  bind(sd, results->ai_addr, results->ai_addrlen);

  listen(sd, 10);

  struct sockaddr_in * addr_in = (struct sockaddr_in *)(results->ai_addr);
  printf("Server has been started on: %s\n", inet_ntoa(addr_in->sin_addr));

  free(hints);
  freeaddrinfo(results);

  return sd;
}

//returns client_socket created by accept()
int server_connect(int sd, struct sockaddr_storage * client_address) {
  int client_socket;
  socklen_t sock_size;
  //struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  client_socket = accept(sd, (struct sockaddr *) client_address, &sock_size);

  return client_socket;
}

//returns socket descriptor for client
int client_connect(char * address) {

  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  getaddrinfo(address, "25932", hints, &results);

  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  connect(sd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);

  return sd;
}
