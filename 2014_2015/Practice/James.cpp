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
	
	Mat A; 
	cvNamedWindow("Window" , CV_WINDOW_NORMAL);

	for(;;) {
		cap >> A;
		GaussianBlur(A , A , Size(5,5) , 1.5 , 1.5);
		inRange(A, Scalar(0, 0, 0), Scalar(0, 255, 0), A);
		cvtColor(A , A , CV_BGR2HSV);
		erode(A , A , getStructuringElement(MORPH_RECT , Size(5,5));
		dilate(A , A , getStructuringElement(MORPH_RECT , Size(5,5));
		dilate(A , A , getStructuringElement(MORPH_RECT , Size(5,5));
		erode(A , A , getStructuringElement(MORPH_RECT , Size(5,5));
		imshow("Window" , A );
 
	}
		

	return 0;
}
