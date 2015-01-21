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
	inRange(HSV , Scalar(26 , 40 , 64) , Scalar(33 , 255 , 255) , toReturn);
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

	Mat yellowEdge;
	Mat greyEdge;
	Mat greenEdge;
	Mat dst;
	vector<vector<Point> > yellowContours;
        vector<vector<Point> > greyContours;
        vector<vector<Point> > greenContours;

	Rect yellowBounds = Rect(0,0,0,0);
	Rect greyBounds = Rect(0,0,0,0);
	Rect greenBounds = Rect(0,0,0,0);

	Canny(yellow , yellowEdges , 0 , 100 , 5);
	Canny(grey , greyEdge , 0 , 100 , 5);
	Canny(green , greenEdges , 0 , 100 , 5);

	findContours(yellowEdges , yellowContours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	findContours(greyEdges , greyContours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
	findContours(greenEdges , greenContours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);

	if (yellowContours.size() > 0) {
		vector<vector<Point> > temp;
		temp.push_back(yellowContours.at(yellowContours.size() - 1));
		yellowBounds = boundingRect(temp.at(0));
	}
	if (greyContours.size() > 0) {
		vector<vector<Point> > temp;
		temp.push_back(greyContours.at(greyContours.size() - 1));
		greyBounds = boundingRect(temp.at(0));
	}
	if (greenContours.size() > 0) {
		vector<vector<Point> > temp;
		temp.push_back(greenContours.at(greenContours.size() - 1));
		greenBounds = boundingRect(temp.at(0));
	}

	dst = original.clone();

	//COLOR_HERE is just a place holder. Put real stuff in later

	rectangle(dst , yellowBounds , COLOR_HERE , 1 , 8 , 0);
	rectangle(dst , greyBounds , COLOR_HERE , 1 , 8 , 0);
	rectangle(dst , greenBounds , COLOR_HERE , 1 , 8 , 0);

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

	for (;;) {
		cap >> frame;
		yellow = getYellow(frame);
		grey = getGray(frame);
		green = getGreen(frame);

		imshow("Yellow" , yellow);
		imshow("Grey" , grey);
		imshow("Green" , green);

		if(waitKey(30) >= 0) {
			break;
		}

	}



}
