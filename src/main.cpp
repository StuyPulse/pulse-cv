#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
<<<<<<< HEAD
//#include "camera/camera.h"
//#include "camera/camera.cpp"
=======
#include "camera/camera.h"
>>>>>>> Add camera input both for webcam and AXIS cam. not the most efficient, but it works

using namespace std;
using namespace cv;

int main() {
  Camera cam;
  namedWindow("frame");
  bool running = true;
  while (running) {
    Mat frame = cam.getFrame();
    imshow("frame", frame);
  }

  return 0;
}
