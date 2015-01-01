#include "CameraCalibrator.h"

void CameraCalibrator::addPoints(const vector<Point2f>& imageCorners, const vector<Point3f>& objectCorners){
	imagePoints.push_back(imageCorners);
	objectPonits.push_back(objectCorners);
}


int CameraCalibrator::addChessboardPoints(const std::vector<string>& filelist, Size& boardSize){
	// there are two type cordi 
	vector<Point2f> imageCorners;
	vector<Point3f> objectCorners;

	// 3D points
	for(int i=0; i<boardSize.height; i++){
		for(int j=0; j<boardSize.width; j++){
			objectCorners.push_back(Point3f(i,j,0.0f));
		}
	}
	// 2D points
	Mat image;
	int suceess = 0;
	//for all viewPoints
	for (int i=0; i<filelist.size(); i++){
		// open image
		image = cv::imread(filelist[i], 0);
		bool found = cv::findChessboardCorners(image, boardSize, imageCorners);

		// obtain subPixel precision
		cv::cornerSubPix(image,
			imageCorners,
			Size(5,5),
			Size(-1,-1),
			TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS,30, 0.1));

		if(imageCorners.size() == boardSize.area()){
			addPoints(imageCorners, objectCorners);
			success++;
		}
	}
	return success;
}


void CameraCalibrator::calibrate(cv::Size& imageSize){
	mustInitUndistort = true;

	avg_reprojection_error = calibrateCamera(objectPonits,
							imagePoints,
							imageSize,
							cameraMatrix,
							distCoeffs,
							rvecs,
							tvecs,
							flag);
}


Mat CameraCalibrator::remap(const cv::Mat& image){
	cv::Mat undistorted;
	if(mustInitUndistort){
		cv::initUndistortRectifyMap(cameraMatrix,
									distCoeffs,
									Mat(),
									Mat(),
									image.size(),
									CV_32FC1,
									map1,
									map2);
		mustInitUndistort = false;
	}

	cv::remap(image, undistorted, map1, map2, cv::INTER_LINEAR);
	return undistorted;
}


void CameraCalibrator::print(){
	cout<< "avg_reprojection_error:" << endl << avg_reprojection_error << endl <<endl;
	cout <<"camera Matrix: " << endl << cameraMatrix << endl << endl;
	cout <<"distCoeffs: " << endl << distCoeffs << endl <<endl;
	cout <<"rvec[0]: " << endl << rvecs[0] << endl <<endl;
	cout <<"tvec[0]: " << endl << tvecs[0] << endl <<endl;
}