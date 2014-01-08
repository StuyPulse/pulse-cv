#pragma once
#include <opencv2/core/core.hpp>

#define AXIS_IP "10.6.94.12"

using namespace cv;
class Camera {
public:
  Camera();       // Creates the camera, detecting whether or not
                  // there is a camera attached that is valid for VideoCapture
                  // Otherwise, it will use an AXIS camera at the IP Address
                  // defined by AXIS_IP
  Mat getFrame(); // Returns the current camera frame
private:
  
};
