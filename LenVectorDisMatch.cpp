#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>


void Draw_corners(cv::Mat* src, std::vector<cv::Point2f>* corners, char source_window[]);

int main(){

	std::clock_t start_time = std::clock();
	cv::Mat sample, ground, sample_gray, ground_gray;
	size_t number_of_sample_corners = 2000;
	size_t number_of_ground_corners = 2000;
	size_t number_of_sample_vectors;
	size_t number_of_ground_vectors;
	size_t number_of_good_matches = 0;
	char sample_window[] = "Image sample";
	char ground_window[] = "Image ground";
	std::vector<cv::Point2f> sample_corners;
	std::vector<cv::Point2f> ground_corners;
	std::vector<size_t> len_vectors_sample;
	size_t len_vector;
	std::vector<cv::Point2f> suitable_point;
	std::vector<cv::Point2f> good_matches;

	uint vector_len_eps = 0;
	uchar color_point_eps = 0;
	double qualityLevel = 0.01; 
	double minDistance = 10; 
	int blockSize = 3; 
	bool useHarrisDetector = true ; 
	double k = 0.04;


	sample = cv::imread("sample/redbull.jpg", 1);
	ground = cv::imread("ground/find_redbull.jpg", 1);
	cv::cvtColor(sample, sample_gray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(ground, ground_gray, cv::COLOR_BGR2GRAY);

	cv::namedWindow(sample_window, CV_WINDOW_AUTOSIZE);
	cv::namedWindow(ground_window, CV_WINDOW_AUTOSIZE);

	cv::goodFeaturesToTrack(sample_gray,sample_corners,number_of_sample_corners,qualityLevel,minDistance,cv::Mat(),blockSize,useHarrisDetector,k);
	cv::goodFeaturesToTrack(ground_gray,ground_corners,number_of_ground_corners,qualityLevel,minDistance,cv::Mat(),blockSize,useHarrisDetector,k);

	number_of_sample_corners = sample_corners.size();
	number_of_sample_vectors = number_of_sample_corners - 1;
	number_of_ground_corners = ground_corners.size();
	number_of_ground_vectors = number_of_ground_corners - 1;

	Draw_corners(&sample, &sample_corners, sample_window);
	Draw_corners(&ground, &ground_corners, ground_window);

	for (size_t i = 0; i < number_of_sample_vectors; ++i) {
		len_vectors_sample.push_back(pow(sample_corners[i + 1].x - sample_corners[0].x, 2) + pow(sample_corners[i + 1].y - sample_corners[0].y, 2));
	}


	for (size_t i = 0; i < number_of_ground_vectors; ++i) {
		for (size_t j = i + 1; j < number_of_ground_vectors; ++j) {
			len_vector = pow(ground_corners[j].x - ground_corners[i].x, 2) + pow(ground_corners[j].y - ground_corners[i].y, 2);
			if ((len_vector >= (len_vectors_sample[0] - vector_len_eps)) && (len_vector <= (len_vectors_sample[0] + vector_len_eps))) {
				uchar sample_point  = sample_gray.at<uchar>(sample_corners[0]);
				uchar ground_point_i = ground_gray.at<uchar>(ground_corners[i]);
				uchar ground_point_j = ground_gray.at<uchar>(ground_corners[j]);
				if ((ground_point_i >= sample_point - color_point_eps) && (ground_point_i <= sample_point + color_point_eps)) { suitable_point.push_back(ground_corners[i]); }
				if ((ground_point_j >= sample_point - color_point_eps) && (ground_point_j <= sample_point + color_point_eps)) { suitable_point.push_back(ground_corners[j]); }
			}
		}
	}

	for (size_t i = 0; i < suitable_point.size(); ++i) {
		for (size_t j = 0; j < number_of_sample_vectors; ++j) {
			for (size_t k = 0; k < number_of_ground_vectors; ++k) {
				len_vector = pow(ground_corners[k].x - suitable_point[i].x, 2) + pow(ground_corners[k].y - suitable_point[i].y, 2);
				if ((len_vector >= len_vectors_sample[j] - vector_len_eps) && (len_vector <= len_vectors_sample[j] + vector_len_eps)) {
					++number_of_good_matches;
					good_matches.push_back(ground_corners[k]);
				}
			}
		}
	}

	std::cout << "number_of_good_matches: " <<number_of_good_matches << std::endl;

	for (int i = 0; i < ground_corners.size(); i++)
	{
		cv::circle(ground, ground_corners[i], 4, cv::Scalar(0, 255, 0), -1, 8, 0);
	}

	for (int i = 0; i < suitable_point.size(); i++)
	{
		cv::circle(ground, suitable_point[i], 15, cv::Scalar(0, 0, 0), 1, 8, 0);
		cv::circle(ground, suitable_point[i], 4, cv::Scalar(0, 0, 255), -1, 8, 0);
	}

	for (int i = 0; i < good_matches.size(); i++)
	{
		cv::circle(ground, good_matches[i], 4, cv::Scalar(0, 0, 255), -1, 8, 0);
	}

	cv::imshow(ground_window, ground);

	std::clock_t end_time = std::clock(); 
	std::clock_t search_time = end_time - start_time; 
	std::cout << "Time: " << double(search_time) / CLOCKS_PER_SEC;

	cv::waitKey(0);
	return(0);
}


void Draw_corners(cv::Mat* src, std::vector<cv::Point2f>* corners, char source_window[]){
	cv::Mat copy;
	copy = src->clone();
	size_t size = corners->size();

	std::cout << "Number of corners detected: " << corners->size() << std::endl;
	int r = 4;
	for (int i = 0; i < size; ++i)
	{
		cv::circle(copy, (*corners)[i], r, cv::Scalar(255, 0, 255), -1, 8, 0);
	}

	if (size > 2) { size = 2; }

	for (int i = 0; i <size; ++i)
	{
		cv::circle(copy, (*corners)[i], r * 4, cv::Scalar(0, 0, 0), 1, 8, 0);
	}

	cv::namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	cv::imshow(source_window, copy);
}