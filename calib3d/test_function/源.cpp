#include <iostream>
#include <vector>
#include <opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
	Mat img = imread(argv[1], cv::IMREAD_GRAYSCALE);

	std::vector<cv::Point2f> imageCorners;
	bool isfound = findChessboardCorners(img, Size(9,6), imageCorners);

	FileStorage fs(argv[2], FileStorage::READ);
	Mat intrinsics, distortion;
	fs["camera_matrix"] >> intrinsics;
	fs["distortion_coefficients"] >> distortion;

	std::vector<Point3f> boardPoints;
	for(int i=0; i< 9; i++){
		for(int j=0; j< 6; j++){
			boardPoints.push_back(Point3f(i,j,0.0f));
		}
	}

	Mat rvec, tvec;
	solvePnP(Mat(boardPoints), Mat(imageCorners), intrinsics, distortion, rvec, tvec, false);
	cout << rvec << endl << endl << tvec << endl <<endl;
}
