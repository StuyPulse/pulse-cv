#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

Mat getYellow(Mat original) {
	Mat toReturn;
	Mat HSV;
	cvtColor(original , HSV , CV_BGR2HSV);
	inRange(HSV , Scalar(26 , 30 , 32) , Scalar(33 , 255 , 255) , toReturn);
	GaussianBlur(toReturn , toReturn , Size(3,3) , 1.5 , 1.5);
/*
	erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
	dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
	dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
	erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
*/
	threshold(toReturn , toReturn , 127 , 255 , THRESH_BINARY);
	return toReturn;
}

Mat getGray(Mat original) {
        Mat toReturn;
        Mat HSV;
        cvtColor(original , HSV , CV_BGR2HSV);
        inRange(HSV , Scalar(0 , 8 , 0) , Scalar(179 , 40 , 128) , toReturn);
	GaussianBlur(toReturn , toReturn , Size(3,3) , 1.5 , 1.5);
/*
        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
*/
	threshold(toReturn , toReturn , 127 , 255 , THRESH_BINARY);
        return toReturn;
}

Mat getGreen(Mat original) {
        Mat toReturn;
        Mat HSV;
        cvtColor(original , HSV , CV_BGR2HSV);
        inRange(HSV , Scalar(43 , 128 , 51) , Scalar(75 , 255 , 166) , toReturn);
	GaussianBlur(toReturn , toReturn , Size(3,3) , 1.5 , 1.5);
/*
        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
        dilate(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
        erode(toReturn , toReturn , getStructuringElement(MORPH_RECT, Size(3 , 3)) );
*/
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

	for (int i = 0; i < yellowContours.size(); i++) {
		if (fabs(contourArea(yellowContours[i])) > 100) {
			vector<vector<Point> > temp;
			temp.push_back(yellowContours.at(i));
			yellowBounds = boundingRect(temp.at(0));
			rectangle(dst , yellowBounds , Scalar(29 , 222 , 253) , 1 , 8 , 0);
		}
	}

	for (int i = 0; i < greyContours.size(); i++) {
		if (fabs(contourArea(greyContours[i])) > 100) {
			vector<vector<Point> > temp;
			temp.push_back(greyContours.at(i));
			greyBounds = boundingRect(temp.at(0));
			rectangle(dst , greyBounds , Scalar(90 , 5 , 102) , 1 , 8 , 0);
		}
	}

	for(int i = 0; i < greenContours.size(); i++) {
		if (fabs(contourArea(greenContours[i])) > 200) {
			vector<vector<Point> > temp;
			temp.push_back(greenContours.at(i));
			greenBounds = boundingRect(temp.at(0));
			rectangle(dst , greenBounds , Scalar(58 , 254 , 254) , 1 , 8 , 0);
		}
	}

	return dst;

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

	for (;;) {
		cap >> frame;
		yellow = getYellow(frame);
		grey = getGray(frame);
		green = getGreen(frame);


//		imshow("Yellow" , yellow);
//		imshow("Grey" , grey);
//		imshow("Green" , green);


		proxy = track(yellow , grey , green , frame);
		imshow("Minimap" , proxy);

		if(waitKey(30) >= 0) {
			break;
		}

	}



}
