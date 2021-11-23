#include <iostream>
#include <fstream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){

	Mat red = imread("../data/RED.jpg");
	Mat nir = imread("../data/NIR.jpg");

	if(red.empty() || nir.empty()) {
		std::cout << "Error: the image has been incorrectly loaded." << endl;
		return 0; 
	} 

	//no proper way to load 32f by default
	Mat red32f, nir32f, ndvi;
	red.convertTo(red32f, CV_32F, 1.0/255.0);
	nir.convertTo(nir32f, CV_32F, 1.0/255.0);
	
	ndvi = (nir32f-red32f) / (nir32f+red32f);
	Mat nirMinusRed = nir32f-red32f;
	Mat nirPlusRed = nir32f+red32f;

	putText(nirMinusRed, "NIR - RED", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255));
	putText(nirPlusRed, "RED + NIR", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255));
	putText(ndvi, "NDVI", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255));

	nirMinusRed.convertTo(nirMinusRed, CV_8U, 255.0);
	nirPlusRed.convertTo(nirPlusRed, CV_8U, 255.0);
	ndvi.convertTo(ndvi, CV_8U, 255.0);

	Mat colorNdvi = ndvi.clone();
	normalize(colorNdvi, colorNdvi, 0, 255, NORM_MINMAX, CV_8UC1);
	applyColorMap(colorNdvi, colorNdvi, COLORMAP_HSV);
	putText(colorNdvi, "NDVI color", Point(10, 50), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(0, 0, 0));

	imshow("nir-red", nirMinusRed);
	imshow("nir+red", nirPlusRed);
	imshow("ndvi", ndvi);
	imshow("color ndvi", colorNdvi);

	imwrite("NIR-RED.jpg", nirMinusRed);
	imwrite("RED+NIR.jpg", nirPlusRed);
	imwrite("NDVI.jpg", ndvi);
	imwrite("NDVI_COLOR.jpg", colorNdvi);


	waitKey(0);
	return 0;
}
