#include <iostream>
#include <highgui.hpp>

using namespace std;
using namespace cv;

const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

Mat src1, src2, dst;

void on_trackbar(int, void*){
	alpha = (double) alpha_slider/alpha_slider_max ;
	beta = ( 1.0 - alpha );

	addWeighted( src1, alpha, src2, beta, 0.0, dst);

	imshow( "Linear Blend", dst );
}

int main(int argc, char** argv){
	src1 = imread("../data/LinuxLogo.jpg",1);
	src2 = imread("../data/WindowsLogo.jpg", 1);

	if(!src1.data){
		cout << "Error loading src1" << endl;
	}
	if(!src2.data){
		cout << "Error loading src2" << endl;
	}
	alpha_slider = 50;

	namedWindow("Linear Blend", 1);


	on_trackbar(0, 0); // this is a show for the first frame

	createTrackbar("blend ratio", "Linear Blend",
		&alpha_slider, alpha_slider_max,
		on_trackbar, 0);

	waitKey();
	return 0;
}