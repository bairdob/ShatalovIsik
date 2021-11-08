#include <iostream>
#include <fstream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){

	Mat RED = imread("../data/RED.jpg",CV_8UC1);
	Mat NIR = imread("../data/NIR.jpg",CV_8UC1);
	Mat NDVI = Mat::zeros(500, 500, CV_32FC1);

	if(RED.empty() || NIR.empty()) {
		std::cout << "Error: the image has been incorrectly loaded." << endl;
		return 0; 
	} 

	Mat numerator  = Mat::zeros(500, 500, 0);;
	Mat denominator =  Mat::zeros(500, 500, 0);;
	Mat floatMat = Mat::zeros(500, 500, CV_32FC1);

   	for (int i = 0; i < RED.rows; i++){
		for (int j = 0; j < RED.cols; j++){
			numerator.at<int>(j,i) = (RED.at<int>(j,i) - NIR.at<int>(j,i)) ;
			denominator.at<int>(j,i) = (RED.at<int>(j,i) + NIR.at<int>(j,i));
			floatMat.at<float>(j,i) = numerator.at<int>(j,i) / (denominator.at<int>(j,i) + 0.0001) ; // *255 
		}
    }

    // cout << numerator;
    // cout << denominator;

	// cv::imshow("RED", RED);
	// cv::imshow("NIR", NIR);
	
	putText(numerator, "RED - NIR", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(118, 185, 0));
	putText(denominator, "RED + NIR", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(118, 185, 0));
	putText(floatMat, "NDVI", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255));
	//cvtColor(floatMat, NDVI, COLOR_GRAY2BGR);
	imshow("numerator", numerator);
	imshow("denominator", denominator);
	imshow("ndvi", floatMat);

	// imwrite("RED-NIR.jpg", numerator);
	// imwrite("RED+NIR.jpg", denominator);
	// imwrite("NDVI.jpg", floatMat);
	waitKey(0);

	return 0;
}
