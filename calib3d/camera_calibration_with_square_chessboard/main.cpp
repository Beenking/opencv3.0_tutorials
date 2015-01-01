
#include "CameraCalibrator.h"

int main(){
	vector<string> filelist;
	filelist.push_back("../data/chessboard/left01.jpg");
	filelist.push_back("../data/chessboard/left02.jpg");
	filelist.push_back("../data/chessboard/left03.jpg");
	filelist.push_back("../data/chessboard/left04.jpg");
	filelist.push_back("../data/chessboard/left05.jpg");
	filelist.push_back("../data/chessboard/left06.jpg");
	filelist.push_back("../data/chessboard/left07.jpg");
	filelist.push_back("../data/chessboard/left08.jpg");
	filelist.push_back("../data/chessboard/left09.jpg");
	filelist.push_back("../data/chessboard/left11.jpg");
	filelist.push_back("../data/chessboard/left12.jpg");
	filelist.push_back("../data/chessboard/left13.jpg");
	filelist.push_back("../data/chessboard/left14.jpg");

	filelist.push_back("../data/chessboard/right01.jpg");
	filelist.push_back("../data/chessboard/right02.jpg");
	filelist.push_back("../data/chessboard/right03.jpg");
	filelist.push_back("../data/chessboard/right04.jpg");
	filelist.push_back("../data/chessboard/right05.jpg");
	filelist.push_back("../data/chessboard/right06.jpg");
	filelist.push_back("../data/chessboard/right07.jpg");
	filelist.push_back("../data/chessboard/right08.jpg");
	filelist.push_back("../data/chessboard/right09.jpg");
	filelist.push_back("../data/chessboard/right11.jpg");
	filelist.push_back("../data/chessboard/right12.jpg");
	filelist.push_back("../data/chessboard/right13.jpg");
	filelist.push_back("../data/chessboard/right14.jpg");

	cv::Size boardSize(9,6);
	CameraCalibrator ccalibrator;
	ccalibrator.addChessboardPoints(filelist, boardSize);

	cv::Size imageSize(640, 480);
	ccalibrator.calibrate(imageSize);

	ccalibrator.print();

	Mat distorted = imread(filelist[0], cv::IMREAD_GRAYSCALE);
	imshow("distorted image", distorted);
	imshow("undistorted image", ccalibrator.remap(distorted));

	waitKey();
	return 0;
}