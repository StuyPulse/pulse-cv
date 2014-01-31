#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "camera/camera.h"

using namespace std;
using namespace cv;

#define MAX_SLOPE_ERR 3.0f
#define MIN_HORIZ_ERR 40
#define N_CLOSE 2

void printVec(Vec4i vec) {
  printf("%d, %d to %d, %d\n", vec[0], vec[1], vec[2], vec[3]);
}
int lowerFirstX(Vec4i first, Vec4i second) {
  return first[0] > second[0];
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

size_t num_in_range(vector<Vec4i> pts, Vec4i mid, int range) {
  size_t num = 0;
  for (int i = 0; i < pts.size(); i++) {
    if (abs(pts[i][0] - mid[0]) >= range) {
      ++num;
    }
  }  

  return num;
}

int main() {
  Camera cam;
  namedWindow("frame");
  bool running = true;
  char buffer[64];
  while (running) {
    Mat frame = cam.getFrame();
    //cvtColor(frame, frame, CV_BGR2HSV);
    inRange(frame, Scalar(40, 55, 55), Scalar(220, 175, 200), frame);
    GaussianBlur(frame, frame, Size(9,9), 9, 9);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(20, 20));
    morphologyEx(frame, frame, MORPH_CLOSE, kernel);
    bitwise_not(frame, frame);
    vector<Vec4i> lines;
    vector<Vec4i> horizontals = vector<Vec4i>();
    HoughLinesP(frame, lines, 1, CV_PI/180, 300, 30, 10);
    cvtColor(frame, frame, CV_GRAY2BGR);
    for (int i = 0; i < lines.size(); i++) {
      double yd = (lines[i][3] - lines[i][2]);
      double xd = (lines[i][1] - lines[i][0]);
      double slope = yd / xd; 
      if (abs(slope) < MAX_SLOPE_ERR) {
        horizontals.push_back(lines[i]);
        line(frame, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]),
            Scalar(0,200,200), 1, 1);
      } else {
        line(frame, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]),
            Scalar(0,0,255), 1, 1);
      }
      circle(frame, Point(lines[i][0], lines[i][1]), 5, Scalar(255, 0, 0));
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
      for (int i = 0; i < horizontals.size(); i++) {
        circle(frame, Point(horizontals[i][0], horizontals[i][1]), 5, Scalar(255, 0, 255));
      }
      printf("Median: ");
      Vec4i median = calc_median(horizontals);
      printVec(median);
      vector<Vec4i> newSet = exterminate_in_range(horizontals, median, MIN_HORIZ_ERR);
      line(frame, Point(median[0], 0), Point(median[0], frame.size().height), Scalar(0,255,0), 3, 8);
      if (newSet.size() > 0) {
        for (int i = 0; i < newSet.size(); i++) {
          circle(frame, Point(newSet[i][0], newSet[i][1]), 5, Scalar(0, 255, 0));
        }
        median = calc_median(newSet);
        line(frame, Point(median[0], 0), Point(median[0], frame.size().height), Scalar(255,0,0), 3, 8);
        printVec(median);
        int num = num_in_range(newSet, median, MIN_HORIZ_ERR);
        if ((num * N_CLOSE) > newSet.size()) {
          printf("I THINK WE FOUND A HOT ONE! (%d of %lu)\n", num, newSet.size());
        }
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
