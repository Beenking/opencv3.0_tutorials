// run this .cpp after you have got camera.yml by excute calibration.exe which needs imagelist.xml
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
	solvePnP(Mat(boardPoints), Mat(imageCorners), intrinsics, distortion, rvec, tvec, false);
}