#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "camera/camera.h"
#include "net/netsend.h"
#include <chrono>

using namespace std;
using namespace cv;

#define MAX_SLOPE_ERR 3.0f
#define MIN_HORIZ_ERR 20
#define N_CLOSE 10

#define I_DONT_KNOW 0
#define FRAME_SEEN 1
#define FRAME_UNSEEN 2

//#define printf(...) [>...<]

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


vector<Vec4i> detect_lines(Mat* image) {
  const double err = 0.000000001f;
  vector<Vec4i> result = vector<Vec4i>();

  for (int i = 0; i < image->rows - 2; i++) {
    bool tracking = false;
    const double* row = image->ptr<double>(i);
    Vec4i curr = Vec4i();
    bool should_print = false;

    for (int j = 0; j < image->cols; j++) {
      double pix = image->at<double>(i,j);
      if (pix != 0) should_print = true;
      if (pix != 0) {//>= err) {
        // We've hit the start of the line
        if (!tracking) {
          tracking = true;
          // Save our starting point
          curr[0] = j;
          curr[1] = i;
        }
      } else if (tracking) { // We've hit the end of the line.
        tracking = false;
        // Save our ending point, push onto our list of lines
        curr[2] = j;
        curr[3] = i;
        result.push_back(curr);
        printf("Reached end of line naturally\n");
        printVec(curr);
        curr = Vec4i();
        break;
      }
    }
    // We've hit the end, finish our line if we've started one
    //printf("Before eol\n");
    if (tracking) {
        tracking = false;
        // Save our ending point, push onto our list of lines
        curr[2] = image->cols;
        curr[3] = i;
        printf("Reached end of line because image edge\n");
        result.push_back(curr);
        curr = Vec4i();
    }
    if (should_print) {
      for (int j = 0; j < image->cols; j++) {
        printf("%d", (row[j]==0?0:1));
      }
      printf("\n");
    }
    //printf("after eol %d of %d\n", i, image.rows);
  }

  printf("End of detect_lines\n");
  return result;

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

int final_out_to_bot(int* last_frames, int num_frames) {
  int num_unknown = 0;
  int num_seen = 0;
  int num_unseen = 0;
  for(int i=0;i<num_frames;i++){
    switch(last_frames[i]) {
      case I_DONT_KNOW:
        num_unknown++;
        break;
      case FRAME_SEEN:
        num_seen++;
        break;
      default:
        num_unseen++;
    }
  }
  if(num_seen>0)
    return FRAME_SEEN;
  if(num_unknown>0)
    return I_DONT_KNOW;
  return FRAME_UNSEEN;
}

int main() {
  const int num_frames = 10;
  int curr_frame = 0;
  int last_frames[num_frames]; // Values of last handful of frames
  for (int i = 0; i < num_frames; i++) {
    last_frames[i] = I_DONT_KNOW;
  }

  NetSend n = NetSend();
  send_value = I_DONT_KNOW;

  n.start_server();
  sleep(1);
  n.stop_server();

  std::chrono::steady_clock::time_point start, end;
  double totalTime = 0;
  int iterations = 0;

  Camera cam;
  namedWindow("frame");
  bool running = true;
  char buffer[64];
  while (running) {

    Mat frame = cam.getFrame();
    //cvtColor(frame, frame, CV_BGR2HSV);
    inRange(frame, Scalar(220, 175, 200), Scalar(255,255,255), frame);
    GaussianBlur(frame, frame, Size(9,9), 9, 9);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(30, 30));
    morphologyEx(frame, frame, MORPH_CLOSE, kernel);
    vector<Vec4i> lines;
    vector<Vec4i> horizontals = vector<Vec4i>();
    start = std::chrono::steady_clock::now();
    HoughLinesP(frame, lines, 3, CV_PI/5, 50, 30, 10);
    //lines = detect_lines(&frame);
    end = std::chrono::steady_clock::now();
    iterations++;
    using namespace std::chrono;
    totalTime += duration<int, std::milli>(duration_cast<duration<int, std::milli> >(end-start)).count();
    printf("Avg time: %f f/s\n", totalTime/iterations);
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
          last_frames[curr_frame] = FRAME_SEEN; // We've seen the target
          printf("I THINK WE FOUND A HOT ONE! (%d of %lu)\n", num, newSet.size());
        } else {
          last_frames[curr_frame] = FRAME_UNSEEN; // No target seen
        }
        printf("\n");
      } else {
        last_frames[curr_frame] = FRAME_UNSEEN; // No target seen
      }
    }
    curr_frame = ++curr_frame % num_frames;

    send_value = final_out_to_bot(last_frames, num_frames);
    imshow("frame", frame);
    waitKey(30);
  }

  return 0;
}
