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
    int circles = 0;
    cvtColor(maty , gray , CV_BGR2GRAY);
    Mat edge;
    Canny(gray , edge , 0 , 100 , 5);
    vector<vector<Point> > contours;
    findContours(edge , contours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
    vector<Point> vector;
    Mat desperate = maty.clone();
    for(int i = 0 ; i < contours.size() ; i++) {
        approxPolyDP (Mat(contours[i]) , vector , 500000 , true);
        if (fabs(contourArea(contours[i])) < 10 || !isContourConvex(vector)) {
            continue;
        }
        if (vector.size() == pts) {
            circles++;
        }
    }
    cout << "Circles detected: " << circles << endl;
    return circles;
}

int imgDetect() {
    Mat img1 = imread("CIRCLE.png");
    if (img1.empty()) {
        return -1;
    }
    polyDetect(img1 , 500000);
    return 0;
}

int main() {
    imgDetect();
}
