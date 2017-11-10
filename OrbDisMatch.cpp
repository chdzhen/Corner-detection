/*@@@Поиск объекта с фото-экземпляра на видео-сцене. Существует несколько вариантов "матчей"-алгоритмов для сравнения результатов
поиска особых точек. Детектор - ORB. @@@*/

#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>

int main() {

	cv::Mat img_object = cv::imread("5.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	cv::VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;

	std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
	cv::Mat descriptors_object, descriptors_scene;

	cv::Ptr<cv::ORB> orb = cv::ORB::create();
	///orb->setMaxFeatures(5000);
	///orb->setEdgeThreshold(10);
	///orb->setPatchSize(10);

	//-- object
	orb->detect(img_object, keypoints_object);
	///cv::drawKeypoints(img_object, keypoints_object, img_object, cv::Scalar(0,255,255));
	///cv::imshow("template", img_object);
	orb->compute(img_object, keypoints_object, descriptors_object);
	///if(descriptors_object.type() != CV_32F) descriptors_object.convertTo(descriptors_object, CV_32F);// for Flann

	cv::Mat img_matches;

	for (;;) {
		cv::Mat img_scene; ///= cv::imread("8.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cap >> img_scene;
		cvtColor(img_scene, img_scene, CV_BGR2GRAY);


		orb->detect(img_scene, keypoints_scene);
		orb->compute(img_scene, keypoints_scene, descriptors_scene);
		///if (descriptors_scene.type() != CV_32F) descriptors_scene.convertTo(descriptors_scene, CV_32F);//for Flann

		cv::BFMatcher matcher; ///(cv::NORM_HAMMING);
		std::vector<cv::DMatch> matches;
			if (!descriptors_object.empty() && !descriptors_scene.empty()) {
				matcher.match(descriptors_object, descriptors_scene, matches);


				double max_dist = 0; double min_dist = 100;

				//-- Quick calculation of max and min idstance between keypoints
				for (int i = 0; i < descriptors_object.rows; ++i)
				{
					double dist = matches[i].distance;
					if (dist < min_dist) min_dist = dist;
					if (dist > max_dist) max_dist = dist;
				}

				std::vector< cv::DMatch >good_matches;

				for (int i = 0; i < descriptors_object.rows; i++)
				{
					if (matches[i].distance <= cv::max(2 * min_dist, 0.02))
					{
						good_matches.push_back(matches[i]);
					}
				}

				cv::drawMatches(img_object, keypoints_object, img_scene, keypoints_scene, \
					good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
					std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

				cv::namedWindow("Good Matches", CV_WINDOW_FREERATIO);
				cv::imshow("Good Matches", img_matches);
				for (int i = 0; i < (int)good_matches.size(); i++)
				{
					printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
				}

				if (cv::waitKey(33) != 255) break;
			}
		}

	return 0;
}