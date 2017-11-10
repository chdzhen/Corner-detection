/*@@@Детектор особых точек ORB, детектирует особые точки с видео потока.@@@*/

//#include "opencv2/core.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/features2d.hpp"
#include "opencv2\opencv.hpp"
#include <iostream>
#include <vector>

void print_on_change(cv::Mat img, cv::Ptr<cv::ORB> detector) {
	std::vector<cv::KeyPoint> kp;
	cv::Mat out;
	(*detector).detect(img, kp);
	std::cout << "Found " << kp.size() << " Keypoints " << std::endl;
	cv::drawKeypoints(img, kp, out, cv::Scalar::all(255));
	cv::imshow("Kpts", out);
}

int main(int argc, char **argv) {

	// parameters of ORB
	int nfeatures = 500;// default (500);
	float scaleFactor = 1.2f;// default (1.2f);
	int nlevels = 8;// default (8);
	int edgeThreshold = 31; // default (31);
	int firstLevel = 0;// default (0);
	int WTA_K = 2;// default (2);
	int scoreType = cv::ORB::HARRIS_SCORE;// default (cv::ORB::HARRIS_SCORE);
	int patchSize = 31;// default (31);
	int fastThreshold = 20;// default (20);

	cv::Ptr<cv::ORB> detector = cv::ORB::create(
		nfeatures,
		scaleFactor,
		nlevels,
		edgeThreshold,
		firstLevel,
		WTA_K,
		scoreType,
		patchSize,
		fastThreshold);

	cv::namedWindow("Kpts");
	cv::VideoCapture cap(0);
	cv::Mat img;
	for (;;) {
		cap >> img;
		print_on_change(img, detector);
		cv::waitKey(1);
	}


	return 0;
}