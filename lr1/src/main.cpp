#include <iostream>
#include <fstream>
#include <map>



using namespace std;

const string UL = "CORNER_UL_LAT_PRODUCT";
const string UR = "CORNER_UR_LAT_PRODUCT";
const string LL = "CORNER_LL_LAT_PRODUCT";
const string LR = "CORNER_LR_LAT_PRODUCT";


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

	double getAngleFromLine(string line, string str){
	    size_t pos = line.find(str);
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
	    for (const auto& [key, value] : angles) {
	        std::cout << key << " = " << value << endl;
	    }
	    std::cout << "\n";
	}


	void printDelta(){
		cout.precision(10);
	    for (const auto& [key, value] : delta) {
	        std::cout << "delta " << key << " = " << value << endl;
	    }
	    std::cout << "\n";		
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
	return 0;
}