#include "netsend.h"


NetSend::NetSend() {
}

int NetSend::start_server() {
  pthread_attr_t attr;
  pthread_attr_init(&attr); // Initialize attr
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // make the attribute have JOINABLE option

  thread_id = pthread_create(&thread, &attr, &init_server, (void*)NULL); // thread out

  return 0;
}

int NetSend::stop_server() {
  pthread_cancel(thread);
  return 0;
}

void* NetSend::init_server(void* threadarg) {
  int socket_id, socket_client;
  char buffer[256];

  struct sockaddr_in server;
  socklen_t socket_length;

  socket_id = socket(AF_INET, SOCK_STREAM, 0);
   
  // IPv4, yo
  server.sin_family = AF_INET;
  // Listen to everything coming in.
  server.sin_addr.s_addr = INADDR_ANY;
  // Select the coolest port
  server.sin_port = htons(PORT);

  // Bind the socket
  bind(socket_id, (struct sockaddr *)&server, sizeof(server));
  while (1) {
    printf("Running!\n");
  }
  pthread_exit(NULL);
}
