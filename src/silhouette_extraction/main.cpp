#include <iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

#include "silhouetteExtraction.hpp"

using namespace std;
using namespace cv;

int blurf1 = 3;
int blurf2 = 7;
int thresh = 20;
void thresh_callback(int, void*);

Mat a, b, bin;


void printUsage() {
    cout << "Usage: ./progName <path_to_image> <path_to_background_image> [<path_to_save_silhouette>]" << endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
	printUsage();
	return 1;
    }

    a = imread(argv[1], 1); // fg
    b = imread(argv[2], 1); // bkg

    // calibrate image a regarding to image b
    Mat aCal;
    calibrateImage(a, b, aCal);
    a = aCal;

    if (argc == 4) {// if third argument is given, write silhouette to file.
	extractSilhouetteRGBSum(a, b, bin, blurf1, blurf2, thresh, false);
	imwrite(argv[3], bin);
    } else {// if third argument is not given, display GUI.
	namedWindow("Slika", CV_WINDOW_AUTOSIZE);
	imshow("Slika", a);  
	createTrackbar( "blur1", "Slika", &blurf1, 20, thresh_callback);
	createTrackbar( "blur2", "Slika", &blurf2, 20, thresh_callback);
	createTrackbar( "thresh", "Slika", &thresh, 255, thresh_callback);
	thresh_callback( 0, 0 );
	waitKey(0);
    }

    return 0;
}

void thresh_callback(int, void* ) {
    extractSilhouetteRGBSum(a, b, bin, blurf1, blurf2, thresh, false);
    namedWindow("SiluetaSum", CV_WINDOW_AUTOSIZE);
    imshow("SiluetaSum", bin);    
}
