#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<queue>
#include<vector>
#include<algorithm>

using namespace std;
using namespace cv;

void extractSilhouette(Mat&, Mat&, Mat&, int, int, int);
void contours(Mat&, Mat&);

int blurf1 = 3;
int blurf2 = 5;
int thresh = 30;
void thresh_callback(int, void*);

Mat a, b, bin;

int main() {
    a = imread("/home/martin/rasuzo-grupa7/RASUZOslikeSmanjene/PC200170.jpg", 1);
    b = imread("/home/martin/rasuzo-grupa7/RASUZOslikeSmanjene/PC200013.jpg", 1);

    //extractSilhouette(a, b, bin, blurf1, blurf2);

    namedWindow("Slika1", CV_WINDOW_AUTOSIZE);
    imshow("Slika1", a);
    //namedWindow("Slika2", CV_WINDOW_AUTOSIZE);
    //imshow("Slika2", b);
    //namedWindow("Binary", CV_WINDOW_AUTOSIZE);
    //imshow("Binary", bin);   

    createTrackbar( "blur1", "Slika1", &blurf1, 10, thresh_callback);
    createTrackbar( "blur2", "Slika1", &blurf2, 10, thresh_callback);
    createTrackbar( "thresh", "Slika1", &thresh, 255, thresh_callback);
    thresh_callback( 0, 0 );

    waitKey(0);
    return 0;
}

void thresh_callback(int, void* ) {
    extractSilhouette(a, b, bin, blurf1, blurf2, thresh);
    namedWindow("Binary", CV_WINDOW_AUTOSIZE);
    imshow("Binary", bin);   
}

void extractSilhouette(Mat& img, Mat& bkg, Mat& binary, int blurf1, int blurf2, int thresh) {
    Mat imgBW, bkgBW;
    Mat imgBL, bkgBL;
    cvtColor(img, imgBW, CV_BGR2GRAY);
    cvtColor(bkg, bkgBW, CV_BGR2GRAY);
    blur(imgBW, imgBL, Size(blurf1, blurf1));
    blur(bkgBW, bkgBL, Size(blurf1, blurf1));
    
    Mat diff;
    blur(bkgBL - imgBL, diff, Size(blurf2, blurf2));
    
    vector<unsigned short> vals;
    for (int i = 0; i < diff.rows; i++)
	for (int j = 0; j < diff.cols; j++)
	    vals.push_back(diff.at<ushort>(i, j));
    sort(vals.begin(), vals.end());
    unsigned short th = vals[vals.size()/2];

    Mat bin;
    threshold(diff, bin, th+thresh, 255, CV_THRESH_BINARY);
    binary = bin;
}

void contours(Mat& img, Mat& conts) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// Find contours
    findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    Mat drawing = Mat::zeros( img.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ ) {
	Scalar color = Scalar(255,255,255);
	drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    cvtColor(drawing, conts, CV_BGR2GRAY);
}

