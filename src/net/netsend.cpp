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
  printf("socket id: %d\n", socket_id);
   
  // IPv4, yo
  server.sin_family = AF_INET;
  // Listen to everything coming in.
  server.sin_addr.s_addr = INADDR_ANY;
  // Select the coolest port
  server.sin_port = htons(PORT);

  // Bind the socket
  int res = bind(socket_id, (struct sockaddr *)&server, sizeof(server));
  if (res < 0) {printf("Binding failed.") ; }
  //Check if "linger" code is necessary

  // Listen to the socket
  res = listen(socket_id, 0);
  if (res < 0) {printf("Listening failed.") ; }

  printf("Waiting for connection\n");
  socklen_t l = sizeof(server);
  //Do we need a different variable than server?
  res = accept(socket_id, (struct sockaddr *)&server, &l);
  if (res < 0) {printf("Accepting failed.") ; }
  printf("Connected!\n");
  while (1) {

    send_value = 1;
    printf("Sending data: %d\n", send_value);
    write(socket_id, &send_value, sizeof(send_value));
  }
  pthread_exit(NULL);
}
