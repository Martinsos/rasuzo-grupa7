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
void extractSilhouette2(Mat&, Mat&, Mat&, int, int, int);
unsigned char medianBW(Mat&);
void fillHoles(Mat&, Mat&);

int blurf1 = 5;
int blurf2 = 5;
int thresh = 10;
void thresh_callback(int, void*);

Mat a, b, bin;

int main() {
    a = imread("/home/martin/rasuzo-grupa7/RASUZOslikeSmanjene/PC200030.jpg", 1);
    b = imread("/home/martin/rasuzo-grupa7/RASUZOslikeSmanjene/PC200015.jpg", 1);

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

    //imwrite("silhouette.jpg", bin);

    waitKey(0);
    return 0;
}

void thresh_callback(int, void* ) {
    extractSilhouette2(a, b, bin, blurf1, blurf2, thresh);
    namedWindow("Binary", CV_WINDOW_AUTOSIZE);
    imshow("Binary", bin);   
}

void extractSilhouette(Mat& img, Mat& bkg, Mat& binary, int blurf1, int blurf2, int thresh) {
    // convert image and background to BW and blur them(to reduce noise)
    Mat imgBW, bkgBW;
    Mat imgBL, bkgBL;
    cvtColor(img, imgBW, CV_BGR2GRAY);
    cvtColor(bkg, bkgBW, CV_BGR2GRAY);
    blur(imgBW, imgBL, Size(blurf1, blurf1));
    blur(bkgBW, bkgBL, Size(blurf1, blurf1));
    
    // diff = background - image. blur diff.
    Mat diff, diffBL;
    absdiff(bkgBL, imgBL, diff);
    blur(diff, diffBL, Size(blurf2, blurf2));
    
    // find median of diffBL
    vector<unsigned short> vals;
    for (int i = 0; i < diffBL.rows; i++)
	for (int j = 0; j < diffBL.cols; j++)
	    vals.push_back(diffBL.at<uchar>(i, j));
    sort(vals.begin(), vals.end());
    unsigned char median = vals[vals.size()/2];

    // binarize image -> 255 is silhouette, 0 is background. Use median as threshold.
    Mat bin;
    threshold(diffBL, bin, median+thresh, 255, CV_THRESH_BINARY);

    // fill holes
    Mat binFull;
    fillHoles(bin, binFull);
    
    binary = binFull;
}

void extractSilhouette2(Mat& img, Mat& bkg, Mat& binary, int blurf1, int blurf2, int thresh) {
    // blur input images
    Mat img_, bkg_;
    blur(img, img_, Size(blurf1, blurf1));
    blur(bkg, bkg_, Size(blurf1, blurf1));
    
    // separate input images into channels (rgb)
    vector<Mat> chsImg;
    split(img_, chsImg);
    vector<Mat> chsBkg;
    split(bkg_, chsBkg);
    
    // find difference for each channel
    vector<Mat> diffs(3);
    for (int i = 0; i < 3; i++)
	absdiff(chsBkg[i], chsImg[i], diffs[i]);

    // blur differences
    vector<Mat> diffsBl(3);
    for (int i = 0; i < 3; i++)
	blur(diffs[i], diffsBl[i], Size(blurf2, blurf2));
    
    /*
    // find medians of diffBl
    vector<usigned char> medians(3);
    for (int i = 0; i < 3; i++)
	medians[i] = medianBW(diffsBl[i]);
    */
    
    // take maximum difference for each pixel.
    Mat diff_, diff;
    max(diffsBl[0], diffsBl[1], diff_);
    max(diffsBl[2], diff_, diff);
    //diff = (diffsBl[0]+diffsBl[1]+diffsBl[2])/3;

    // find median
    unsigned char median = medianBW(diff);

    // binarize image -> 255 is silhouette, 0 is background. Use median as threshold.
    Mat bin;
    threshold(diff, bin, median+thresh, 255, CV_THRESH_BINARY);

    // fill holes
    Mat binFull;
    fillHoles(bin, binFull);
    
    binary = binFull;
}

unsigned char medianBW(Mat& img) {
    vector<unsigned char> vals;
    for (int i = 0; i < img.rows; i++)
	for (int j = 0; j < img.cols; j++)
	    vals.push_back(img.at<uchar>(i, j));
    sort(vals.begin(), vals.end());
    unsigned char median = vals[vals.size()/2];
    return median;
}

/*
 * 
 */
void fillHoles(Mat& src, Mat& dst) {
    vector<vector<Point> > contours;
    Mat src_ = Mat(src);
    findContours(src_, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    dst = Mat::zeros(src_.size(), src_.type());
    drawContours(dst, contours, -1, Scalar::all(255), CV_FILLED);
}

