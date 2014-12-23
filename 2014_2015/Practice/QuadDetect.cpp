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
	Canny(grey , edges , 100 , 300 , 5);
	vector<vector<Point> > contours;
	findContours(edges.clone() , contours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	vector<Point> approx;
	Mat dest = src.clone();
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]) , approx , arcLength(Mat(contours[i]) , true) * 0.02 , true);
		if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx)) {
			continue; //Skip small objects or non concave objects
		}
		if (approx.size() == 4) { //We have a 4 vertex shape! Quadrilateral!
			numQuads++;
		}
	}
	cout << "Number of detected quadrilaterals: " << numQuads << endl;
	return numQuads;
}

int main() {
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
