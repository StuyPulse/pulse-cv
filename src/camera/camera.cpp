#include <sys/types.h>
#include "camera.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "jpgd.h"
#include <err.h>

#define PORTNUM 80
Camera::Camera() {
#if USE_WEBCAM
  cap = VideoCapture(0);
#endif
}

Camera::~Camera() {
#if USE_WEBCAM
#else
  printf("We are here!\n");
  //Remove our temp file
  //remove("img.jpg");
#endif
}
Mat Camera::getFrame() {
#if USE_WEBCAM
  cap = VideoCapture(0);
  Mat frame;
  cap >> frame;
  return frame;
#else
  FILE* pFile = fopen("img.jpg", "w");//tmpfile();
  //printf("%s\n", strerror(errno));

  CURL* easyhandle = curl_easy_init();
  curl_easy_setopt(easyhandle, CURLOPT_URL, IMG_URL);
  curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, pFile);
  int success = curl_easy_perform(easyhandle);
  curl_easy_cleanup(easyhandle);

  if (success == 0) {
    fclose(pFile);

    Mat out = imread("img.jpg");
    //remove("img.jpg");
    return out;
  } else {
    return Mat();
  }
#endif
}
