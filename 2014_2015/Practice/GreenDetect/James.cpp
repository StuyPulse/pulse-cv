#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <memory>
#include <vector>
#include <string>
#include <iostream>

//Compile with g++ main.cpp -o main `pkg-config --libs --cflags opencv`

using namespace cv;
using namespace std;

int main(int, char**) {
	VideoCapture cap(0);
	if(!cap.isOpened()){
		return -1;
	}

	//Code here!
	
	Mat HSV;
	Mat Green;
	
	cvNamedWindow("Window" , CV_WINDOW_NORMAL);

	for(;;) {
		Mat Original;
		cap >> Original;
		cvtColor(Original , HSV , CV_BGR2HSV);
		inRange(HSV, Scalar(42, 0, 0), Scalar(73, 255, 255), Green);
		GaussianBlur(Green , Green , Size(5,5) , 1.5 , 1.5);
		erode(Green , Green , getStructuringElement(MORPH_RECT , Size(5,5)));
		dilate(Green , Green , getStructuringElement(MORPH_RECT , Size(5,5)));
		dilate(Green , Green , getStructuringElement(MORPH_RECT , Size(5,5)));
		erode(Green , Green , getStructuringElement(MORPH_RECT , Size(5,5)));
		imshow("Window", Green );
 		if(waitKey(30) >= 0) {
			break;
		}
	}
		

	return 0;
}
