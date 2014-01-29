#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "camera/camera.h"

using namespace std;
using namespace cv;

#define MAX_SLOPE_ERR 2
#define MIN_HORIZ_ERR 100

void printVec(Vec4i vec) {
  printf("%d, %d to %d, %d\n", vec[0], vec[1], vec[2], vec[3]);
}
int lowerFirstX(Vec4i first, Vec4i second) {
  return first[0] < second[0];
}

Vec4i calc_median(vector<Vec4i> pts) {
  sort(pts.begin(), pts.end(), lowerFirstX); //Sort our points
  return pts[pts.size()/2];
}

vector<Vec4i> exterminate_in_range(vector<Vec4i> pts, Vec4i mid, int range) {
  vector<Vec4i> results = vector<Vec4i>();
  for (int i = 0; i < pts.size(); i++) {
    if (abs(pts[i][0] - mid[0]) >= range) {
      results.push_back(pts[i]);
    }
  }  

  return results;

}

int main() {
  Camera cam;
  namedWindow("frame");
  bool running = true;
  char buffer[64];
  while (running) {
    Mat frame = cam.getFrame();
    //cvtColor(frame, frame, CV_BGR2HSV);
    inRange(frame, Scalar(0, 55, 55), Scalar(200, 150, 200), frame);
    GaussianBlur(frame, frame, Size(9,9), 9, 9);
    bitwise_not(frame, frame);
    vector<Vec4i> lines;
    vector<Vec4i> horizontals = vector<Vec4i>();
    HoughLinesP(frame, lines, 1, CV_PI/180, 80, 30, 10);
    cvtColor(frame, frame, CV_GRAY2BGR);
    for (int i = 0; i < lines.size(); i++) {
      line(frame, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]),
          Scalar(0,0,255), 3, 8);
      double yd = (lines[i][3] - lines[i][2]);
      double xd = (lines[i][1] - lines[i][0]);
      double slope = yd / xd; 
      if (abs(slope) < MAX_SLOPE_ERR) {
        horizontals.push_back(lines[i]);
      }
    }
    printf("lines: %lu, horiz: %lu\n", lines.size(), horizontals.size());
    // Algorithm:
    // Find Median of leftmost X values
    // Eliminate lines within range of median
    // Find new median, check if enough points within error range (not done)
    //  if so, goal is hot
    //  else, goal is cold
    //
    if (horizontals.size() > 0) {
      printf("Median: ");
      Vec4i median = calc_median(horizontals);
      printVec(median);
      vector<Vec4i> newSet = exterminate_in_range(horizontals, median, MIN_HORIZ_ERR);
      line(frame, Point(median[0], median[1]), Point(median[2], median[3]), Scalar(0,255,0), 3, 8);
      if (newSet.size() > 0) {
        median = calc_median(newSet);
        line(frame, Point(median[0], median[1]), Point(median[2], median[3]), Scalar(255,0,0), 3, 8);
        printVec(median);
        printf("\n");
      } // Else we are derp
    }
    //Canny(frame, frame, 0, 30);
    //GaussianBlur(frame, frame, Size(9,9), 9, 9);
    //cvtColor(frame, frame, CV_HSV2BGR);
    imshow("frame", frame);
    waitKey(30);
  }

  return 0;
}
