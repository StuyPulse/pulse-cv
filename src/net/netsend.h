#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <err.h>
#include <errno.h>
#include <string.h>

#define PORT 12345

class NetSend {
public:
  NetSend();
  NetSend* getInstance();
  int start_server();
  int stop_server();
  char send_value;
private:
  static NetSend* instance;
  static void* init_server(void* threadarg);
  int thread_id;
  pthread_t thread;
};
