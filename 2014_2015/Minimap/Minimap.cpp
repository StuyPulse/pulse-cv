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

double camera_angle;
double fieldofview_horizontal , fieldofview_vertical;
int cameraheight; //In inches

int img_height;
int img_base;

int minimapDraw_xcoor;
int minimapDraw_ycoor;

Mat minimap;
Mat yellowToReturn , grayToReturn , greenToReturn;
Mat yellowHSV , grayHSV , greenHSV;

Mat getYellow(Mat original) {
	cvtColor(original , yellowHSV , CV_BGR2HSV);
	inRange(yellowHSV , Scalar(26 , 30 , 32) , Scalar(33 , 255 , 255) , yellowToReturn);
	//GaussianBlur(yellowToReturn , yellowToReturn , Size(3,3) , 1.5 , 1.5);

	/*erode(yellowToReturn , yellowToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(yellowToReturn , yellowToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(yellowToReturn , yellowToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	erode(yellowToReturn , yellowToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );*/

	//threshold(yellowToReturn , yellowToReturn , 127 , 255 , THRESH_BINARY);
	return yellowToReturn;
}

Mat getGray(Mat original) {
	cvtColor(original , grayHSV , CV_BGR2HSV);
	inRange(grayHSV , Scalar(0 , 8 , 0) , Scalar(179 , 40 , 128) , grayToReturn);
	//GaussianBlur(grayToReturn , grayToReturn , Size(3,3) , 1.5 , 1.5);

	/*erode(grayToReturn , grayToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(grayToReturn , grayToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(grayToReturn , grayToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	erode(grayToReturn , grayToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );*/

	//threshold(grayToReturn , grayToReturn , 127 , 255 , THRESH_BINARY);
	return grayToReturn;
}

Mat getGreen(Mat original) {
	cvtColor(original , greenHSV , CV_BGR2HSV);
	inRange(greenHSV , Scalar(42 , 60 , 15) , Scalar(67 , 255 , 90) , greenToReturn);
	//GaussianBlur(greenToReturn , greenToReturn , Size(3,3) , 1.5 , 1.5);

	/*erode(greenToReturn , greenToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(greenToReturn , greenToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	dilate(greenToReturn , greenToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );
	erode(greenToReturn , greenToReturn , getStructuringElement(MORPH_RECT, Size(5 , 5)) );*/

	//threshold(greenToReturn , greenToReturn , 127 , 255 , THRESH_BINARY);
	return greenToReturn;
}

void drawObjOnMap(vector<Point> contours , int color) {
	//Takes a found rectangle in track() and performs trig calculations to figure out distance away from the robot
	//Draws the detected object on the minimap using circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)

	sort(contours.begin() , contours.end() , pointXAxisSort);

	xcoor = contours.at(0).x;
	width = contours.at(contours.size() - 1).x - xcoor;

	sort(contours.begin() , contours.end() , pointYAxisSort);

	ycoor = contours.at(0).y;
	height = contours.at(contours.size() - 1).y - ycoor;
	/*
	cout << "xcoor: " << xcoor << endl;
	cout << "ycoor: " << ycoor << endl;
	cout << "width: " << width << endl;
	cout << "height: " << height << endl;
	 */
	//Formulas:

}

Mat yellowEdges;
Mat greyEdges;
Mat greenEdges;

vector<vector<Point> > yellowContours;
vector<vector<Point> > greyContours;
vector<vector<Point> > greenContours;

Rect yellowBounds = Rect(0,0,0,0);
Rect greyBounds = Rect(0,0,0,0);
Rect greenBounds = Rect(0,0,0,0);

Mat track(Mat yellow , Mat grey , Mat green , Mat original) {

	/* Mat yellowEdges;
	Mat greyEdges;
	Mat greenEdges;
	 */
	Mat dst = original.clone();
	/* vector<vector<Point> > yellowContours;
	vector<vector<Point> > greyContours;
	vector<vector<Point> > greenContours;

	Rect yellowBounds = Rect(0,0,0,0);
	Rect greyBounds = Rect(0,0,0,0);
	Rect greenBounds = Rect(0,0,0,0);
	 */

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
			drawObjOnMap(temp.at(numDraw) , 1);

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
			drawObjOnMap(temp.at(numDraw) , 2);

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
			drawObjOnMap(temp.at(numDraw) , 3);

			numDraw++;
		}
	}

	temp.clear();

	return dst;
}

void readconf() {
	char buffer[32];
	ifstream fin("cv.conf");
	while (fin.good()) {
		fin >> buffer;
		cout << buffer << endl;
		if (strcmp(buffer , "theta") == 0) {
			fin >> camera_angle;
		} else if (strcmp(buffer , "height") == 0) {
			fin >> cameraheight;
		} else if (strcmp(buffer , "vertviewangle") == 0) {
			fin >> fieldofview_vertical;
		} else if (strcmp(buffer , "horizviewangle") == 0) {
			fin >> fieldofview_horizontal;
		} else {
			fin >> buffer;
		}
		cout << buffer << endl;
	}

	/*
	cout << camera_angle << endl;
	cout << cameraheight << endl;
	cout << fieldofview_vertical << endl;
	cout << fieldofview_horizontal << endl;
	 */
	fin.close();
}

int main() {

	readconf();

	VideoCapture cap(0);
	//cvNamedWindow("Window" , CV_WINDOW_NORMAL);

	if (!cap.isOpened()) {
		cout << "Webcam not found" << endl;
		return -1;
	}

	Mat frame;
	Mat yellow , grey , green;
	Mat proxy;

	minimap = Mat::zeros(256 , 256 , CV_8UC3);
	Point roboCenter = Point(128 , 128);

	circle(minimap , roboCenter , 2 , Scalar(0 , 0 , 255) , 2 , 8 , 0);

	cap >> frame;

	//Get size of image:
	img_height = frame.rows;
	img_base = frame.cols;

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

		if(waitKey(15) >= 0) {
			break;
		}

	}

}
