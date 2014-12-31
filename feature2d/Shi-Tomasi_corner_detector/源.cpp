#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Global variables
Mat src, src_gray;

int maxCorners = 55;
int maxTrackbar = 200;

RNG rng(12345);
char* source_window = "Image";

/// Function header
void goodFeaturesToTrack_Demo(int, void*);

/** @function main **/
int main(int argc, char** argv){
	/// Load source image and convert it to gray
	src = imread(argv[1], 1);
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	/// Create Window
	namedWindow(source_window, cv::WINDOW_AUTOSIZE);

	/// Create Trackerbar to set the number of corners
	createTrackbar("Max corners:", source_window, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo);

	//imshow(source_window, src);
	goodFeaturesToTrack_Demo(0, 0);

	waitKey();
	return 0;
}

/**
* @function goodFeaturesToTrack_Demo.cpp
* @brief Apply Shi-Tomasi corner detector
*/
void goodFeaturesToTrack_Demo(int, void*){
	if(maxCorners < 1){maxCorners = 1;}

	/// Parameters for Shi-Tomasi algorithm
	std::vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = true;
	double k = 0.04;

	/// Copy the source image
	Mat copy = src.clone();

	/// Apply corner detction
	goodFeaturesToTrack(src_gray,
						corners,
						maxCorners,
						qualityLevel,
						minDistance,
						Mat(),
						blockSize,
						useHarrisDetector,
						k);

	/// Draw corners detected
	cout << "** Number of corners detected:" << corners.size() << endl;
	int r = 4;
	for(int i = 0; i < corners.size(); i++){
		circle(copy, corners[i], r, Scalar(rng.uniform(0,255),
				rng.uniform(0,255),rng.uniform(0,255)), -1 , 8, 0);
	}

	/// Show what you got 
	namedWindow( source_window, cv::WINDOW_AUTOSIZE);
	imshow (source_window, copy);
}