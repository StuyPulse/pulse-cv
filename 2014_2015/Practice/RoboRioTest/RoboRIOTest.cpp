#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int, char**) {
    Mat img;
    img = imread("TestImage.png");
    Mat grey;
    cvtColor(img , grey , CV_BGR2GRAY);
    GaussianBlur(grey , grey , Size(5,5) , 1.5 , 1.5);
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    try {
        imwrite("OUTPUT.png" , grey , compression_params);
    } catch (int error) {
        cout << "Errorrr~~~";
        return -1;
    }

    return 0;
}
