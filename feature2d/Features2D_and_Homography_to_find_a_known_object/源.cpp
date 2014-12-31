#include <iostream>
#include <opencv.hpp>
#include <xfeatures2d.hpp>

using namespace cv;

void readme();

/* @function main */
int main(int argc, char** argv){
	if(argc !=3){
		readme();
		return -1;
	}
	Mat img_object = imread(argv[1], IMREAD_GRAYSCALE);
	Mat img_scene = imread(argv[2], IMREAD_GRAYSCALE);

	if( !img_object.data || !img_scene.data){
		std::cout << " --(!) Error reading image " << std::endl;
		return -1;
	}
	//-- Step 1: Detect the keyPoints and calculate descriptors using SURF Detector
	int minHessian = 400;
	Ptr<xfeatures2d::SURF> detector = xfeatures2d::SURF::create(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_secne;
	Mat descriptors_object, descriptors_secne;

	detector ->detectAndCompute(img_object, Mat(), keypoints_object, descriptors_object);
	detector ->detectAndCompute(img_scene, Mat(), keypoints_secne, descriptors_secne);

	//-- Step 2: Matching dscriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptors_object, descriptors_secne, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distance between keypoints
	for (int i=0; i<descriptors_object.rows; i++){
		double dist = matches[i].distance;
		if(dist <min_dist) min_dist = dist;
		if(dist >max_dist) max_dist = dist;
	}
	std::cout << "-- Max dist : " << max_dist << std::endl;
	std::cout << "-- Min dist : " << min_dist << std::endl;

	//-- Draw only "good" matches (i.e. whoes distance is less than 3*min_dist)
	std::vector<DMatch> good_matches;

	for(int i=0; i<descriptors_object.rows; i++){
		if(matches[i].distance < 3*min_dist)
			good_matches.push_back(matches[i]);
	}
	Mat img_matches;
	drawMatches(img_object, 
				keypoints_object, 
				img_scene, 
				keypoints_secne,
				good_matches,
				img_matches);

	//--  Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for(int i=0; i<good_matches.size(); i++){
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_secne[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, cv::RANSAC);

	//-- Get the corners from the image_1 (the object to be "detected")
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0,0);
	obj_corners[1] = Point(img_object.cols, 0);
	obj_corners[2] = Point(img_object.cols, img_object.rows);
	obj_corners[3] = Point(0, img_object.rows);

	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object int the scene - image2)
  line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
  line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

   //-- Show detected matches
  namedWindow("Good Matches & Object detection",cv::WINDOW_KEEPRATIO);
  imshow( "Good Matches & Object detection", img_matches );

  waitKey(0);
  return 0;
}

 /* @function readme */
  void readme()
  { std::cout << " Usage: ./SURF_descriptor <img1> <img2>" << std::endl; }