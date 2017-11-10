/*@@@ Первая попытка сделать детектор углов. Алгоритм: сравнение областей каждой точки изображения с её соседями в 8-ми направлениях
и если область точки уникальна по сравнению с её соседями, то её координаты записываются в вектор.
Алгоритм неустойчив к поворотам и шумам и вообще работает крайне ужасно. 
@@@*/

#include "opencv2\opencv.hpp"
#include <iostream>
#include <vector>

struct coords {
	ptrdiff_t x;
	ptrdiff_t y;
};


int main() {
	char image_sample_name[] = "32.jpg";
	cv::Mat image_sample = cv::imread(image_sample_name, 1);
	cv::Mat image_sample_copy;
	image_sample.copyTo(image_sample_copy);
	cv::cvtColor(image_sample, image_sample, cv::COLOR_BGR2GRAY);

	const uchar radius_of_matrix = 3;
	const uchar size_of_range = radius_of_matrix * radius_of_matrix - 1;
	uchar number_of_matches = 0;
	uchar near_point_number_of_matches = 0;
	uchar point_range[size_of_range];
	uchar near_point_range[size_of_range];
	coords range_matrix[size_of_range] = {-1,-1,-1,0,-1,1,0,1,1,1,1,0,1,-1,0,-1};
	///coords range_matrix[size_of_range] = {-1,-1,0,-1,1,-1,1,0,1,1,0,1,-1,1,-1,0};
	///coords range_matrix[size_of_range] = { -2,-2,-2,-1,-2,0,-2,1,-2,2,-1,-2,-1,-1,-1,0,-1,1,-1,2,0,-2,0,-1,0,1,0,2,1,-2,1,-1,1,0,1,1,1,2,2,-2,2,-1,2,0,2,1,2,2 };
	std::vector<coords> coords_list;
	ptrdiff_t coords_list_size;
	ptrdiff_t image_sample_cols = image_sample.cols;
	ptrdiff_t image_sample_rows = image_sample.rows;
	ptrdiff_t x = 0, y = 0,x1 = 0, y1 = 0;

	for (ptrdiff_t i = 0; i < image_sample_rows; ++i) {
		for (ptrdiff_t j = 0; j < image_sample_cols; ++j) {
			if (image_sample.at<uchar>(i, j) != 255) {
				for (ptrdiff_t b = 0; b < size_of_range; ++b) {
					x = i + range_matrix[b].x;
					y = j + range_matrix[b].y;
					point_range[b] = ((x > 0) && (y > 0) && (x<image_sample_rows) && (y<image_sample_cols)) ? image_sample.at<uchar>(x, y) : 255;
				}
				for (ptrdiff_t g = 0; g < size_of_range; ++g) {
					x = i + range_matrix[g].x;
					y = j + range_matrix[g].y;
					for (ptrdiff_t a = 0; a < size_of_range; ++a) {
						x1 = x + range_matrix[a].x;
						y1 = y + range_matrix[a].y;
						near_point_range[a] = ((x1 > 0) && (y1 > 0)&&(x1<image_sample_rows)&&(y1<image_sample_cols)) ? image_sample.at<uchar>(x1, y1) : 255;
					}

					for (ptrdiff_t s = 0; s < size_of_range; ++s) {
						if (point_range[s] != near_point_range[s]) ++near_point_number_of_matches;
					}

					if (near_point_number_of_matches != 0) ++number_of_matches;

					near_point_number_of_matches = 0;
				}


				if (number_of_matches == size_of_range) {
					coords cord;
					cord.x = i;
					cord.y = j;
					coords_list.push_back(cord);
				}
				number_of_matches = 0;
			}
		}
	}

	coords_list_size = coords_list.size();
	std::cout << "size: " << coords_list_size << std::endl;


	for (ptrdiff_t i = 0; i < coords_list_size; ++i) {
		cv::circle(image_sample_copy, cv::Point(coords_list[i].y, coords_list[i].x), 10, cv::Scalar(0, 0, 255));
		std::cout << coords_list[i].y << "  " << coords_list[i].x << std::endl;
	}

	cv::imshow("result", image_sample_copy);
	cv::imwrite("ex2.bmp", image_sample_copy);
	cv::waitKey(0);		

	return 0;
}