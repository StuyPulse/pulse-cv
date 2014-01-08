#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
  VideoCapture cap(0); //Open default camera
  if (!cap.isOpened())
    return -1;

  Mat edges;
  const string WINNAME = string("edges");
  namedWindow(WINNAME, CV_WINDOW_AUTOSIZE);
  for(;;)
  {
    Mat frame, dst;
    cap >> frame; // get a new frame from camera
    cvtColor(frame, edges, CV_BGR2GRAY);

    for (int i = 0; i < 10; i++)
      GaussianBlur(edges, edges, Size(9,9), 1.5, 1.5);

    Canny(edges, dst, 0, 30, 3);

    vector<Vec4i> lines;
    HoughLinesP( dst, lines, 1, CV_PI/180, 80, 30, 10 );

    cvtColor(dst, dst, CV_GRAY2BGR);

    for( size_t i = 0; i < lines.size(); i++ )
    {
        line( dst, Point(lines[i][0], lines[i][1]),
            Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
    }

    // draw the image
    imshow(WINNAME, dst);

    // press ESC to break
    if(waitKey(30) >= 0) break;
  }

  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
