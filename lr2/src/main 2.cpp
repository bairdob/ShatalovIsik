#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){

	Mat red = imread("../data/RED.jpg",CV_8UC1);
	Mat nir = imread("../data/NIR.jpg",CV_8UC1);

	if(red.empty() || nir.empty()) {
		std::cout << "Error: the image has been incorrectly loaded." << endl;
		return 0; 
	} 

	// Mat red32f  = Mat::zeros(500, 500, CV_32F);;
	// Mat nir32f =  Mat::zeros(500, 500, CV_32F);;
	// red.convertTo(red32f, CV_32FC1);
	// nir.convertTo(nir32f, CV_32FC1);

	Mat nirMinusRed  = Mat::zeros(500, 500, CV_8UC1);;
	Mat nirPlusRed =  Mat::zeros(500, 500, CV_8UC1);;
	Mat ndvi = Mat::zeros(500, 500, CV_8UC1);

   	for (int i = 0; i < red.rows; i++){
		for (int j = 0; j < red.cols; j++){
			nirMinusRed.at<int>(j,i) = nir.at<int>(j,i) - red.at<int>(j,i);
			nirPlusRed.at<int>(j,i) = red.at<int>(j,i) + nir.at<int>(j,i);
			// ndvi.at<int>(j,i) = (nir.at<int>(j,i) - red.at<int>(j,i)) / (red.at<int>(j,i) + nir.at<int>(j,i)+0.001);
		}
    }
    ndvi = nirMinusRed / nirPlusRed;
    	applyColorMap(nirMinusRed, nirMinusRed, COLORMAP_COOL);
	putText(nirMinusRed, "NIR - RED", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(55, 255, 255));
	putText(nirPlusRed, "RED + NIR", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(118, 185, 0));
	putText(ndvi, "NDVI", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255));

	imshow("NIR-RED", nirMinusRed);
	imshow("NIR+RED", nirPlusRed);
	imshow("NDVI", ndvi);

	// imwrite("NIR-RED.jpg", nirMinusRed);
	// imwrite("RED+NIR.jpg", nirPlusRed);
	imwrite("NDVI.jpg", ndvi);
	waitKey(0);

	return 0;
}
