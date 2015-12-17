/*
 * QuadDetect.cpp
 *
 * Solution to Quadrilateral Detection Via Webcam
 *
 * Note: You can easily alter main to take a file input
 *
 *  Created on: Dec 22, 2014
 *      Author: sublimau5
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int polygonDetect(Mat src , int verts) {
    if (src.empty()) {
        cout << "Source image not found";
        return -1; //Kill if no input
    }
    Mat grey;
    int numQuads = 0;
    cvtColor(src , grey , CV_BGR2GRAY);
    Mat edges;
    Canny(grey , edges , 0 , 100 , 5);
    vector<vector<Point> > contours;
    findContours(edges , contours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
    vector<Point> approx;
    Mat dest = src.clone();
    for (int i = 0; i < contours.size(); i++) {
        approxPolyDP(Mat(contours[i]) , approx , arcLength(Mat(contours[i]) , true) * 0.02 , true);
        if (fabs(contourArea(contours[i])) < 10 || !isContourConvex(approx)) {
            continue; //Skip small objects or non concave objects
        }
        if (approx.size() == verts) {
            numQuads++;
        }
    }
    cout << "Number of detected quadrilaterals: " << numQuads << endl;
    return numQuads;
}

int webcamDetect() {
    VideoCapture cap(0); //Default Webcam
    if (!cap.isOpened()) {
        cout << "No webcam found!" << endl;
        return -1; //Webcam isn't opened D:
    }
    Mat frame;
    for(;;) {
        cap >> frame; //Get the frame!
        polygonDetect(frame , 4); //Number of detected quads from webcam
    }
    return 0;
}

int imgDetect() {
    Mat img1 = imread("QUAD1.png");
    Mat img2 = imread("QUAD2.png");
    Mat img3 = imread("QUAD3.png");
    if (img1.empty() || img2.empty() || img3.empty()) {
        return -1;
    }
    polygonDetect(img1 , 4);
    polygonDetect(img2 , 4);
    polygonDetect(img3 , 4);
    return 0;
}

int main() {
    //webcamDetect();
    imgDetect();
    /*
     * What Happens:
     * 	sublimau5@PFRuojia:~/Development/CV2015/pulse-cv/2014_2015/Practice/QuadrilateralDetect/Solution$ g++ QuadDetect.cpp -o QuadDetect.out `pkg-config --libs --cflags opencv`
     * 	sublimau5@PFRuojia:~/Development/CV2015/pulse-cv/2014_2015/Practice/QuadrilateralDetect/Solution$ ./QuadDetect.out
     Number of detected quadrilaterals: 0
     Number of detected quadrilaterals: 2
     Number of detected quadrilaterals: 1
     *
     */
}
