//	Compile with:
//	clang++ Minimap.cpp `pkg-config --libs --cflags opencv` -O2

//	MicroSoft HD 3000 Webcam Angle of View:
//	Horizontal: 51 Deg
//	Vertical: 30 Deg

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

int xcoor , ycoor , width , height;

bool pointXAxisSort(const Point &a, const Point &b) {
    return a.x < b.x;
}
bool pointYAxisSort(const Point &a, const Point &b) {
    return a.y < b.y;
}

Mat getYellow(Mat original) {
	Mat toReturn;
	Mat HSV;
	cvtColor(original , HSV , CV_BGR2HSV);
	inRange(HSV , Scalar(26 , 30 , 32) , Scalar(33 , 255 , 255) , toReturn);
	GaussianBlur(toReturn , toReturn , Size(3,3) , 1.5 , 1.5);

	erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );

	threshold(toReturn , toReturn , 127 , 255 , THRESH_BINARY);
	return toReturn;
}

Mat getGray(Mat original) {
        Mat toReturn;
        Mat HSV;
        cvtColor(original , HSV , CV_BGR2HSV);
        inRange(HSV , Scalar(0 , 8 , 0) , Scalar(179 , 40 , 128) , toReturn);
	GaussianBlur(toReturn , toReturn , Size(3,3) , 1.5 , 1.5);

        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );

	threshold(toReturn , toReturn , 127 , 255 , THRESH_BINARY);
        return toReturn;
}

Mat getGreen(Mat original) {
        Mat toReturn;
        Mat HSV;
        cvtColor(original , HSV , CV_BGR2HSV);
        inRange(HSV , Scalar(43 , 16 , 32) , Scalar(75 , 128 , 166) , toReturn);
	GaussianBlur(toReturn , toReturn , Size(3,3) , 1.5 , 1.5);

        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );

	threshold(toReturn , toReturn , 127 , 255 , THRESH_BINARY);
        return toReturn;
}

Mat track(Mat yellow , Mat grey , Mat green , Mat original) {

	Mat yellowEdges;
	Mat greyEdges;
	Mat greenEdges;
	Mat dst = original.clone();
	vector<vector<Point> > yellowContours;
        vector<vector<Point> > greyContours;
        vector<vector<Point> > greenContours;

	Rect yellowBounds = Rect(0,0,0,0);
	Rect greyBounds = Rect(0,0,0,0);
	Rect greenBounds = Rect(0,0,0,0);

	Canny(yellow , yellowEdges , 0 , 100 , 5);
	Canny(grey , greyEdges , 0 , 100 , 5);
	Canny(green , greenEdges , 0 , 100 , 5);

	findContours(yellowEdges , yellowContours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	findContours(greyEdges , greyContours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	findContours(greenEdges , greenContours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);

	vector<vector<Point> > temp;
	int numDraw = 0;

	for (int i = 0; i < yellowContours.size(); i++) {
		if (fabs(contourArea(yellowContours[i])) > 500) {
			temp.push_back(yellowContours.at(i));
			yellowBounds = boundingRect(temp.at(numDraw));
			rectangle(dst , yellowBounds , Scalar(0 , 255 , 255) , 3 , 8 , 0);

			numDraw++;
		}
	}

	temp.clear();
	numDraw = 0;

	for (int i = 0; i < greyContours.size(); i++) {
		if (fabs(contourArea(greyContours[i])) > 500) {
			temp.push_back(greyContours.at(i));
			greyBounds = boundingRect(temp.at(numDraw));
			rectangle(dst , greyBounds , Scalar(255 , 128 , 128) , 3 , 8 , 0);

			numDraw++;
		}
	}

	temp.clear();
	numDraw = 0;

	for(int i = 0; i < greenContours.size(); i++) {
		if (fabs(contourArea(greenContours[i])) > 1000) {
			temp.push_back(greenContours.at(i));
			greenBounds = boundingRect(temp.at(numDraw));
			rectangle(dst , greenBounds , Scalar(0 , 255 , 128) , 3 , 8 , 0);

			numDraw++;
		}
	}

	temp.clear();

	return dst;

}

void drawObjOnMap(Mat minimap , vector<Point> contours , int color) {
		//Takes a found rectangle in track() and performs trig calculations to figure out distance away from the robot
		//Draws the detected object on the minimap using circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)

		sort(contours.begin() , contours.end() , pointXAxisSort);

		xcoor = contours.at(0).x;
		width = contours.at(contours.size() - 1).x - xcoor;

		sort(contours.begin() , contours.end() , pointYAxisSort);

		ycoor = contours.at(0).y;
		height = contours.at(contours.size() - 1).y - ycoor;

}

int main() {

	VideoCapture cap(0);
	cvNamedWindow("Window" , CV_WINDOW_NORMAL);

	if (!cap.isOpened()) {
		cout << "Webcam not found" << endl;
		return -1;
	}

	Mat frame;
	Mat yellow , grey , green;
	Mat proxy;

	Mat minimap = Mat::zeros(256 , 256 , CV_8UC3);
	Point roboCenter = Point(128 , 128);

	circle(minimap , roboCenter , 2 , Scalar(0 , 0 , 255) , 2 , 8 , 0);

	for (;;) {
		cap >> frame;
		yellow = getYellow(frame);
		grey = getGray(frame);
		green = getGreen(frame);


//		imshow("Yellow" , yellow);
//		imshow("Grey" , grey);
//		imshow("Green" , green);


		proxy = track(yellow , grey , green , frame);
		imshow("Detection" , proxy);

		imshow("Minimap - CV" , minimap);

		if(waitKey(30) >= 0) {
			break;
		}

	}



}
