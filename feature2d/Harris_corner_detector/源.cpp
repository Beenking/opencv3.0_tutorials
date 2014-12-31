#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Global variables
Mat src, src_gray;
int thresh =190;
int max_thresh = 255;

char* source_window = "Source image";
char* corners_window = "Corners detected";

// Function header
void cornerHarris_demo(int, void*);

// function main
int main(int argc, char** argv){
	// Load source image and convert it to gray
	src = imread(argv[1], 1);
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	// Create a window and a trackbar
	namedWindow( source_window, cv::WINDOW_AUTOSIZE );
	createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
	imshow( source_window, src);

	cornerHarris_demo( 0 ,0 );

	waitKey();
	return 0;
}

//** @function cornerHarris_demo */
void cornerHarris_demo( int, void* ){
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros( src.size(), CV_32FC1 );

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

	/// Nromalizing 
	normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs( dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners
	for( int j=0; j<dst_norm.rows; j++){
		for(int i=0; i<dst_norm.cols; i++){
			if( (int)dst_norm.at<float>(j,i) > thresh)
				circle( dst_norm_scaled, Point(i,j),5, Scalar(0), 2, 8, 0);
		}
	}
	/// showing the result
	namedWindow(corners_window, cv::WINDOW_AUTOSIZE);
	imshow( corners_window, dst_norm_scaled);
}