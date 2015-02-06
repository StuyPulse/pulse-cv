#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int, char**) {
	VideoCapture cap(0);
	if(!cap.isOpened()){
		return -1;
	}

