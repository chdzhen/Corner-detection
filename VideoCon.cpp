#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;

int main(int, char) {

	VideoCapture cap(0); 

	if (!cap.isOpened())

		return -1;

	Mat edges;

	namedWindow("edges", 1);

	while(1) {

		Mat frame;

		cap >> frame; // get a new frame from camera

		cvtColor(frame, edges, CV_BGR2GRAY);

		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);

		Canny(edges, edges, 0, 30, 3);

		imshow("edges", edges);

		if (waitKey(1) != 255) break;

		// the camera will be deinitialized automatically in VideoCapture destructor
	}

	return 0;
}