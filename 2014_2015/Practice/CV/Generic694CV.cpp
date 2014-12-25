/*
 * Generic694CV.cpp
 *
 *  Created on: Dec 23, 2014
 *      Author: Ethan Cheng
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

int polygonDetect(Mat src , int verts , bool isGray) {//From QuadDetect.cpp
	if (src.empty()) {
		cout << "Source image not found";
		return -1; //Kill if no input
	}
	int numDetected = 0;
	Mat grey;
	if (!isGray) {
		cvtColor(src , grey , CV_BGR2GRAY);
	} else {
		grey = src;
	}
	Mat edges;
	Canny(grey , edges , 0 , 100 , 5);
	vector<vector<Point> > contours;
	findContours(edges , contours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	vector<Point> approx;
	Mat dest = src.clone();
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]) , approx , arcLength(Mat(contours[i]) , true) * 0.02 , true);
		if (fabs(contourArea(contours[i])) < 5 || !isContourConvex(approx)) {
			continue; //Skip small objects or non concave objects
		}
		if (approx.size() == verts) {
			numDetected++;
		}
	}
	return numDetected;
}

bool foundTwoOrangeRects(Mat frame) {
	/*
	 * Parameter Mat:Frame MUST BE OF TYPE BGR
	 */
	Mat HSV;
	Mat orange;
	cvtColor(frame , HSV , CV_BGR2HSV);
	inRange(HSV , Scalar(11 , 0 , 0) , Scalar(24 , 100 , 100) , orange); //Values subject to change
	//Clean up speckles
	erode(orange , orange , getStructuringElement(MORPH_ELLIPSE, Size(3 , 3)) );
	dilate(orange , orange , getStructuringElement(MORPH_ELLIPSE, Size(3 , 3)) );
	dilate(orange , orange , getStructuringElement(MORPH_ELLIPSE, Size(3 , 3)) );
	erode(orange , orange , getStructuringElement(MORPH_ELLIPSE, Size(3 , 3)) );
	return (polygonDetect(orange , 4 , true) == 2);
}

int main() {
	VideoCapture cap(0); //Will be changed to get image from the Axis camera
	if (!cap.isOpened()) {
		cout << "Webcam not found" << endl;
		return -1;
	}
 	Mat currImg;
	for(;;) {
		cap >> currImg;
		if (foundTwoOrangeRects(currImg)) {
			cout << "HOT" << endl;
		} else {
			cout << "COLD" << endl;
		}
	}
	return 0;
}
