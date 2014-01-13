#include <sys/types.h>
#include "camera.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PORTNUM 80
Camera::Camera() {
}

Mat Camera::getFrame() {
	int sockID;
	struct sockaddr_in dest; 
	sockID = socket(AF_INET , SOCK_STREAM , 0);
	memset(&dest, 0, sizeof(dest));                /* zero the struct */
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1"); /* set destination IP number */ 
	dest.sin_port = htons(PORTNUM);                /* set destination port number */  
	//destination = "10.6.94.12/axis-cgi/jpg/image.cgi"
  	connect(sockID, (struct sockaddr *)&dest, sizeof(struct sockaddr));
  	return Mat();
}
