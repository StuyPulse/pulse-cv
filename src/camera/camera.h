#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <curl/curl.h>

#define USE_WEBCAM true
//#define IMG_URL "http://10.6.94.12/axis-cgi/jpg/camera.cgi"
#define IMG_URL "http://localhost:8000//Felis_silvestris_silvestris.jpg"

using namespace cv;
using namespace std;
class Camera {
public:
  Camera();       // Creates the camera, detecting whether or not
                  // there is a camera attached that is valid for VideoCapture
                  // Otherwise, it will use an AXIS camera at the IP Address
                  // defined by AXIS_IP
  ~Camera();
  Mat getFrame(); // Returns the current camera frame
private:
#if USE_WEBCAM
  VideoCapture cap;
#endif
  
};
