// run this .cpp after you have got camera.yml by excute calibration.exe which needs imagelist.xml
#include <opencv.hpp>

using namespace std;
using namespace cv;

class CameraCalibrator{
private:
	// points in the real world
	std::vector<std::vector<Point3f>> objectPonits;
	// points int the picture
	std::vector<vector<Point2f>> imagePoints;

	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	double avg_reprojection_error;
	vector<Mat> rvecs, tvecs;

	int flag;
	int success;
	cv::Mat map1,map2;
	bool mustInitUndistort;

private:
	void addPoints(const vector<Point2f>& imageCorners, const vector<Point3f>& objectCorners);
	
public:
	CameraCalibrator():flag(0),mustInitUndistort(true),success(0){}
	int addChessboardPoints(const std::vector<string>& filelist, Size& boardSize);
	void calibrate(cv::Size& imageSize);
	Mat remap(const cv::Mat& image);
	void print();
};
