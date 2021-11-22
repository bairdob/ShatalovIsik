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

const float cityLatitude = 51.8333;
const float cityLontitude = 107.6167;


class Picture {
public:
	string txtFilePath;
	string tifFilePath;
	map<string, double> angles;
	vector<Point2f>  corners; // LL, UR, UL, LR
	map<string, double> deltaAngles;
	map<string, double> deltaCorners;

public:
	Picture(string txtFile, string tifFile){
		txtFilePath = txtFile;
		tifFilePath = tifFile;
	};

	void setData(){
		readAngles();
		setDeltaAngles();
		setCorners();
		setDeltaCorners();
	}

	void readAngles(){
		std::ifstream infile(txtFilePath);

		if (!infile){
			std::cout << "Error: in opening *.txt file" << std::endl;            
	    }

		string line; 
		while (std::getline(infile, line)){
			if (getAngleFromLine(line, UL_LAT) != -1){
				angles[UL_LAT] = getAngleFromLine(line, UL_LAT);
			}
			if (getAngleFromLine(line, UL_LON) != -1){
				angles[UL_LON] = getAngleFromLine(line, UL_LON);
			}


			if (getAngleFromLine(line, UR_LAT) != -1){
				angles[UR_LAT] = getAngleFromLine(line, UR_LAT);
			}
			if (getAngleFromLine(line, UR_LON) != -1){
				angles[UR_LON] = getAngleFromLine(line, UR_LON);
			}

			if (getAngleFromLine(line, LL_LAT) != -1){
				angles[LL_LAT] = getAngleFromLine(line, LL_LAT);
			}
			if (getAngleFromLine(line, LL_LON) != -1){
				angles[LL_LON] = getAngleFromLine(line, LL_LON);
			}			

			if (getAngleFromLine(line, LR_LAT) != -1){
				angles[LR_LAT] = getAngleFromLine(line, LR_LAT);
			}
			if (getAngleFromLine(line, LR_LON) != -1){
				angles[LR_LON] = getAngleFromLine(line, LR_LON);
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

	void printMap(map<string, double> temp){
		cout.precision(10);
		for (const auto& [key, value] : temp){
			std::cout << key << " = " << value << endl;
		}
		cout << "\n";
	}

	void setDeltaAngles(){
		deltaAngles["DELTA_ANGLE_LAT"] = abs(angles[UL_LAT] - angles[UR_LAT]);
		deltaAngles["DELTA_ANGLE_LON"] = abs(angles[UL_LON] - angles[LL_LON]);
	}

	void setCorners(){
		Mat imgOriginal = imread(tifFilePath, IMREAD_COLOR);

		if(imgOriginal.empty()) {
			cout << "Error: the image has been incorrectly loaded." << endl;
		} 
	    
		Mat thresh;
		cvtColor(imgOriginal, thresh, COLOR_BGR2GRAY);
		threshold(thresh, thresh, 0, 255, THRESH_BINARY);
		medianBlur(thresh, thresh, 5);
		goodFeaturesToTrack(thresh, corners, 10, 0.5, 3000, Mat(), 20, 3, false, 0.1);
	}

	void setDeltaCorners(){
		deltaCorners["DELTA_CORNER_X"] = abs(corners[1].x - corners[2].x);
		deltaCorners["DELTA_CORNER_Y"] = abs(corners[2].y - corners[0].y);
	}

	void printCorners(){
		cout << "** Number of corners detected: " << corners.size() << endl;
		cout << corners << endl;
	}

	void cityCoordinates(){
		cout << "City pixel coordinates:" << endl;
		double k1 = deltaCorners["DELTA_CORNER_X"] / deltaAngles["DELTA_ANGLE_LAT"] ;
		double k2 = deltaCorners["DELTA_CORNER_Y"] / deltaAngles["DELTA_ANGLE_LON"];

		cout << abs(angles[UL_LAT] - cityLatitude) * deltaCorners["DELTA_CORNER_X"] / 2 << endl;
		cout << abs(angles[UL_LON] - cityLontitude) * deltaCorners["DELTA_CORNER_Y"] / 2 << endl;
	}
};


int main(){
	Picture p("../data/LE07_L2SP_131024_20021005_20200916_02_T1_MTL.txt", 
			"../data/LE07_L2SP_131024_20021005_20200916_02_T1_SR_B1.TIF");
	p.setData();
	p.printMap(p.angles);
	p.printMap(p.deltaAngles);
	p.printCorners();
	p.printMap(p.deltaCorners);
	p.cityCoordinates();

	Mat imgOriginal = imread("../data/LE07_L2SP_131024_20021005_20200916_02_T1_SR_B1.TIF", IMREAD_COLOR);
	Mat imgCopy = imgOriginal.clone();

	for(size_t i = 0; i < p.corners.size(); i++){
		circle(imgCopy, p.corners[i], 50, Scalar(0, 255, 0), FILLED );
	}

	Point2f ptCity(2000 , 3180); 
	circle(imgCopy, ptCity, 50, Scalar(0, 0, 255), FILLED);

	Point2f ptCityCalculated(2472 , 2924); 
	circle(imgCopy, ptCityCalculated, 50, Scalar(255, 0, 255), FILLED);
    
	Rect rect(1750, 2950, 500, 500);
	rectangle(imgCopy, rect, Scalar(0, 255, 255), 5,LINE_8);

	Mat crop = imgOriginal(Range(1700,2000),Range(3000,3100));
    crop = imgOriginal(rect).clone();

	imshow("Output", imgCopy);
	imshow("crop", crop);
	imwrite("original.jpg", imgCopy);
	waitKey(0);

	return 0;
}
