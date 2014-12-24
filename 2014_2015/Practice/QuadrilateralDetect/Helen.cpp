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

	Mat gray;
	Mat filter;
	Mat quad;

	cvNamedWindow("Window" , CV_WINDOW_NORMAL);

	for (;;) {
		Mat original;
		cap >> original;
		cvtColor(original , gray , CV_BGR2GRAY);
		GaussianBlur(filter , filter , Size(5,5) , 1.5 , 1.5);
		approxPolyDP (filter, quad,  ) 
		imshow("Window", filter );
 		if(waitKey(30) >= 0) {
			break;
		}
	}
}

