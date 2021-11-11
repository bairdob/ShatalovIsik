#include <iostream>
#include <fstream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

const string UL_LAT = "CORNER_UL_LAT_PRODUCT";
const string UL_LON = "CORNER_UL_LON_PRODUCT";
const string UR_LAT = "CORNER_UR_LAT_PRODUCT";
const string UR_LON = "CORNER_UR_LON_PRODUCT";
const string LL_LAT = "CORNER_LL_LAT_PRODUCT";
const string LL_LON = "CORNER_LL_LON_PRODUCT";
const string LR_LAT = "CORNER_LR_LAT_PRODUCT";
const string LR_LON = "CORNER_LR_LON_PRODUCT";

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
			if (getCornerFromLine(line, UL_LAT) != -1){
				angles[UL_LAT] = getCornerFromLine(line, UL_LAT);
			}
			if (getCornerFromLine(line, UL_LON) != -1){
				angles[UL_LON] = getCornerFromLine(line, UL_LON);
			}


			if (getCornerFromLine(line, UR_LAT) != -1){
				angles[UR_LAT] = getCornerFromLine(line, UR_LAT);
			}
			if (getCornerFromLine(line, UR_LON) != -1){
				angles[UR_LON] = getCornerFromLine(line, UR_LON);
			}

			if (getCornerFromLine(line, LL_LAT) != -1){
				angles[LL_LAT] = getCornerFromLine(line, LL_LAT);
			}
			if (getCornerFromLine(line, LL_LON) != -1){
				angles[LL_LON] = getCornerFromLine(line, LL_LON);
			}			

			if (getCornerFromLine(line, LR_LAT) != -1){
				angles[LR_LAT] = getCornerFromLine(line, LR_LAT);
			}
			if (getCornerFromLine(line, LR_LON) != -1){
				angles[LR_LON] = getCornerFromLine(line, LR_LON);
			}				
	    }   
	};

	double getCornerFromLine(string line, string target){
	    size_t pos = line.find(target);
	    double num = -1;

	    if (pos != string::npos){
            	num = stod(line.substr(pos+24)); //stod = string to double
	    }

	    return num;
	}	

	void setDelta(){
		delta["DELTA_LAT"] = abs(angles[UR_LAT] - angles[LL_LAT]);
		delta["DELTA_LON"] = abs(angles[UR_LON] - angles[LL_LON]);
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
			std::cout << key << " = " << value << endl;
		}
		cout << "\n";		
	}
};


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

	// cv::Point2f pt(2000 , 3180);
	// circle(copy, pt, 50, Scalar(0, 0, 255), FILLED );

	cout << corners[2].x << " " << corners[2].y << endl;
	cv::Point2f pt(6191 , 7549);
	circle(copy, pt, 50, Scalar(0, 0, 255), FILLED );
    
	cv::Rect rect(1750, 2950, 500, 500);
	rectangle(copy, rect, cv::Scalar(0, 255, 255), 5,LINE_8);

    // copy = imgOriginal(rect).clone();
	cv::imshow("Output", copy);
	imwrite("test.jpg", copy);
	cv::waitKey(0);

	return 0;
}
