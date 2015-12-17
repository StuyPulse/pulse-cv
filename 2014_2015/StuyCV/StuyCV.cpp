#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

/**
 * parameter src - Source matrix to be color-detected
 * parameter lowerBound - Lower scalar for colors for the range
 * parameter upperBound - Upper scalar for colors for the range
 * return - Process matrix (in this case it's the source)
 */
Mat colorDetect(Mat src, Scalar lowerBound, Scalar upperBound) {
    // Use lower and upper bounds as declared above
    cvtColor(src, src, CV_BGR2HSV);
    inRange(src, lowerBound, upperBound, src);

    GaussianBlur(src, src, Size(3,3), 1.5, 1.5);

    // Sequence of erode and dilate
    erode(src, src, getStructuringElement(MORPH_RECT, Size(3,3)));
    dilate(src, src, getStructuringElement(MORPH_RECT, Size(3,3)));
    dilate(src, src, getStructuringElement(MORPH_RECT, Size(3,3)));
    erode(src, src, getStructuringElement(MORPH_RECT, Size(3,3)));

    //threshold to reduce noise
    threshold(src, src, 127, 255, THRESH_BINARY);

    return src;
}

/**
 * parameter cam - Camera that will be used
 * parameter src  - Source matrix that will be shown
 */
void checkFeed(VideoCapture cam, Mat src){
    if(!cam.isOpened()){
        cout << "Camera not detected.";
    }
    if (cam.grab()){
        if (!cam.retrieve(src)){
            cout << "Could not retrieve.";
        }
    }
}

/**
 * parameter path - Absolute path to desired picture
 */
void showPicture(String path){
    Mat img = imread(path);
    imshow("Picture", img);

}

// Function takes Mat src for processing
vector<vector<Point> > edgeDetect(Mat src){
    //Destination matrix for processed image
    Mat edges;
    Canny(src, edges, 0, 100);

    //contour - Returned vector<vector<Point> >
    vector<vector<Point> > contour;
    findContours(edges, contour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    return contour;
}

int polygonDetect(Mat src, int sides, int epsilon) {

    vector<Point> vector;
    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY);
    vector<vector<Point> > ;

    /*for (int i = 0; i < contour.size(); i++) {

      contour.get(i).convertTo(MOP2f, CvType.CV_32FC1);
      Imgproc.approxPolyDP(MOP2f, MOP2f, epsilon, true);
      MOP2f.convertTo(contourMOP, CvType.CV_32SC2);
      List<Point> contourList = contourMOP.toList();

      if (contourList.size() == sides) {
      polygonsFound++;
      }
      }
      */
    return polygonsFound;
}
