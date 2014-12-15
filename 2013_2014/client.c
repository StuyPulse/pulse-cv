#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define PORT 12345
#define IP "127.0.0.1"
int main() {
  char buffer[256];
  struct sockaddr_in sock;
  int socket_id;

  socket_id = socket(AF_INET, SOCK_STREAM, 0);

  // IPv4, yo
  sock.sin_family = AF_INET;
  
  // Convert the host's ip to something usable
  inet_aton(IP, &(sock.sin_addr));

  sock.sin_port = htons(PORT);

  // Establish connection
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("c: %d\n", c);

  printf("Writing\n");
  write(socket_id, "asdf", sizeof("asdf"));

  printf("Waiting for response...\n");
  // Read their response into buffer
  while(1) {
    read(socket_id, buffer, sizeof(buffer));

    printf("response: %c\n", *(char*)&buffer);
    
  }
  

  close(socket_id);
  return 0;
}
