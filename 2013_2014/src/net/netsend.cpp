#include "netsend.h"


NetSend::NetSend() {
}

int NetSend::start_server() {
  pthread_attr_t attr;
  pthread_attr_init(&attr); // Initialize attr
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // make the attribute have JOINABLE option

  thread_id = pthread_create(&thread, &attr, &init_server, (void*)&send_value); // thread out

  return 0;
}

int NetSend::stop_server() {
  pthread_cancel(thread);
  return 0;
}

void* NetSend::init_server(void* threadarg) {
  while (1) {
    int socket_id, socket_client;
    char buffer[256];

    struct sockaddr_in server, client;
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
    if (res < 0) {printf("Binding failed.\n") ; }
    //Check if "linger" code is necessary

    // Listen to the socket
    res = listen(socket_id, 0);
    if (res < 0) {printf("Listening failed.\b") ; }

    printf("Waiting for connection\n");
    socklen_t l = sizeof(client);
    //Do we need a different variable than server?
    res = accept(socket_id, (struct sockaddr *)&client, &l);
    if (res < 0) {printf("Accepting failed.\n") ; }
    printf("Connected!\n");
    bool running = true;
    while (running) {

      //printf("Sending data: %d\n", send_value);
      int wrval = write(res, threadarg, sizeof(int));
      if (wrval == -1) {
        printf("Failed write: %s %d\n", strerror(errno), errno);
        running = false;
        close(socket_id);
      } else {
        read(res, buffer, sizeof(buffer));
      }
    }
  }
  pthread_exit(NULL);
}
