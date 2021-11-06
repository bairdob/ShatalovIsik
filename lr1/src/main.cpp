#include <iostream>
#include <fstream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

const string UL = "CORNER_UL_LAT_PRODUCT";
const string UR = "CORNER_UR_LAT_PRODUCT";
const string LL = "CORNER_LL_LAT_PRODUCT";
const string LR = "CORNER_LR_LAT_PRODUCT";

const float city_latitude = 51.8333;
const float city_lantitude = 107.6167;


class Picture {
private:
	string txtFilePath;
	map<string, double> angles;
	map<string, double> delta;

public:
	Picture(string str){
		txtFilePath = str;
	};

	void setData(){
		setAngles();
		setDelta();
	}

	void setAngles(){
		std::ifstream infile(txtFilePath);
		if (!infile){
			std::cout << "Error in opening file !" << std::endl;            
	    	}

		string line; 
		while (std::getline(infile, line)){
			if (getAngleFromLine(line, UL) != -1){
				angles["UpperLeft"] = getAngleFromLine(line, UL);
			}

			if (getAngleFromLine(line, UR) != -1){
				angles["UpperRight"] = getAngleFromLine(line, UR);
			}

			if (getAngleFromLine(line, LL) != -1){
				angles["LowerLeft"] = getAngleFromLine(line, LL);
			}

			if (getAngleFromLine(line, LR) != -1){
				angles["LowerRight"] = getAngleFromLine(line, LR);
			}
	    	}   
	};

	double getAngleFromLine(string line, string target){
	    size_t pos = line.find(target);
	    double num = -1;

	    if (pos != string::npos){
            	num = stod(line.substr(pos+24)); //stod = string to double
	    }

	    return num;
	}	

	void setDelta(){
		delta["x"] = angles["UpperLeft"] - angles["LowerLeft"];
		delta["y"] = angles["UpperRight"] - angles["LowerRight"];
	}

	void printAngles(){
		cout.precision(10);
		for (const auto& [key, value] : angles){
			std::cout << key << " = " << value << endl;
		}
		cout << "\n";
	}


	void printDelta(){
		cout.precision(10);
		for (const auto& [key, value] : delta){
			std::cout << "delta " << key << " = " << value << endl;
		}
		cout << "\n";		
	}
};

 //    CORNER_UL_LAT_PRODUCT = 52.66260
 //    CORNER_UR_LAT_PRODUCT = 52.74104
 //    CORNER_LL_LAT_PRODUCT = 50.61285
 //    CORNER_LR_LAT_PRODUCT = 50.68576



int main(){
	Picture p("../data/LE07_L2SP_131024_20021005_20200916_02_T1_MTL.txt");
	p.setData();
	p.printAngles();
	p.printDelta();

	Mat imgOriginal = imread("../data/LE07_L2SP_131024_20021005_20200916_02_T1_SR_B1.TIF", IMREAD_COLOR);

	if(imgOriginal.empty()) {
		cout << "Error: the image has been incorrectly loaded." << endl;
		return 0; 
	} 
    
   	
	Mat thresh;
	cvtColor(imgOriginal, thresh, COLOR_BGR2GRAY);
	threshold(thresh, thresh, 0, 255, THRESH_BINARY);
	medianBlur(thresh, thresh, 5);

	vector<Point2f> corners;
	Mat copy = imgOriginal.clone();
	goodFeaturesToTrack(thresh, corners, 10, 0.5, 3000, Mat(), 20, 3, false, 0.1 );
  
	cout << "** Number of corners detected: " << corners.size() << endl;
	for( size_t i = 0; i < corners.size(); i++ ){
		circle(copy, corners[i], 50, Scalar(0, 255, 0), FILLED );
	}
	cout << corners << endl;

	Mat crop = imgOriginal(Range(1700,2000),Range(3000,3100));

	cv::Point2f pt(2000 , 3180);
	circle(copy, pt, 50, Scalar(0, 0, 255), FILLED );
    
	cv::Rect rect(1750, 2950, 500, 500);
	copy = imgOriginal(rect).clone();
	//rectangle(copy, rect, cv::Scalar(0, 255, 255), 5,LINE_8);
    
	cv::imshow("Output", copy);
	imwrite("test.jpg", copy);
	cv::waitKey(0);

	return 0;
}
