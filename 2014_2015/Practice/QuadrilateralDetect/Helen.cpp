#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int polyDetect(Mat maty, int pts) {
	if (maty.empty()) {
		cout << "Image not found";
		return -1;
	}
	Mat gray;
	int quads = 0;
	cvtColor(maty , gray , CV_BGR2GRAY);
	Mat edge;
	Canny(gray , edge , 0 , 100 , 5);
	vector<vector<Point> > contours;
	findContours(edge , contours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	vector<Point> vector;
	Mat desperate = maty.clone();
	for(int i = 0 ; i < contours.size() ; i++) {
		approxPolyDP (Mat(contours[i]) , vector , arcLength(Mat(contours[i]), true) * 0.02 , true);
		if (fabs(contourArea(contours[i])) < 10 || !isContourConvex(vector)) {
			continue;
		}
		if (vector.size() == pts) {
			quads++;
		}
	}
	cout << "Quadrilaterals detected: " << quads << endl;
	return quads;
}

int imgDetect() {
	Mat img1 = imread("QUAD1.png");
	Mat img2 = imread("QUAD2.png");
	Mat img3 = imread("QUAD3.png");
	if (img1.empty() || img2.empty() || img3.empty()) {
		return -1;
	}
	polyDetect(img1 , 4);
	polyDetect(img2 , 4);
	polyDetect(img3 , 4);
	return 0;
}

int main() {
	imgDetect();
}


