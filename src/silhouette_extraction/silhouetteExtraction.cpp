#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<queue>
#include<vector>
#include<algorithm>
#include<queue>
#include<utility>

using namespace std;
using namespace cv;

void extractSilhouette(Mat&, Mat&, Mat&, int, int, int);
void extractSilhouetteRGB(Mat&, Mat&, Mat&, int, int, int);
unsigned char medianBW(Mat&);
void fillHoles(Mat&, Mat&);
int floodFill(Mat&, Point2i, unsigned char);
void extractLargestBlob(Mat&);
pair<int, int> findBestTranslation(Mat&, Mat&, int, int, int, int);
void calibrateImage(Mat&, Mat&, Mat&);

int blurf1 = 5;
int blurf2 = 5;
int thresh = 10;
void thresh_callback(int, void*);

Mat a, aCal, b, bin;


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

    // calibrate image a regarding to image b and store it to aCal
    calibrateImage(a, b, aCal);

    if (argc == 4) {// if third argument is given, write silhouette to file.
	extractSilhouetteRGB(aCal, b, bin, blurf1, blurf2, thresh);
	imwrite(argv[3], bin);
    } else {// if third argument is not given, display GUI.
	/* DIFF */
	Mat differ;
	absdiff(a, b, differ);
	imshow("Prije kalibracije", differ);
	absdiff(aCal, b, differ);
	imshow("Poslije kalibracije", differ);
	/* */

	namedWindow("Slika (kalibrirana)", CV_WINDOW_AUTOSIZE);
	imshow("Slika (kalibrirana)", aCal);  
	createTrackbar( "blur1", "Slika (kalibrirana)", &blurf1, 10, thresh_callback);
	createTrackbar( "blur2", "Slika (kalibrirana)", &blurf2, 10, thresh_callback);
	createTrackbar( "thresh", "Slika (kalibrirana)", &thresh, 255, thresh_callback);
	thresh_callback( 0, 0 );
	waitKey(0);
    }

    return 0;
}

void thresh_callback(int, void* ) {
    extractSilhouetteRGB(aCal, b, bin, blurf1, blurf2, thresh);
    namedWindow("Silueta", CV_WINDOW_AUTOSIZE);
    imshow("Silueta", bin);   
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

    // extract largest blob
    extractLargestBlob(binFull);
    
    binary = binFull;
}

void extractSilhouetteRGB(Mat& img, Mat& bkg, Mat& binary, int blurf1, int blurf2, int thresh) {
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
    
    // take maximum difference for each pixel.
    Mat diff_, diff;
    max(diffsBl[0], diffsBl[1], diff_);
    max(diffsBl[2], diff_, diff);
    //diff = (diffsBl[0]/3+diffsBl[1]/3+diffsBl[2]/3);

    // find median
    unsigned char median = medianBW(diff);

    // binarize image -> 255 is silhouette, 0 is background. Use median as threshold.
    Mat bin;
    threshold(diff, bin, median+thresh, 255, CV_THRESH_BINARY);

    // fill holes
    Mat binFull;
    fillHoles(bin, binFull);

    // extract largest blob
    extractLargestBlob(binFull);
    
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
 * Fills holes in binary image.
 */
void fillHoles(Mat& src, Mat& dst) {
    vector<vector<Point> > contours;
    Mat src_ = Mat(src);
    findContours(src_, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    dst = Mat::zeros(src_.size(), src_.type());
    drawContours(dst, contours, -1, Scalar::all(255), CV_FILLED);
}

/*
  Starting from given start field, all neighbouring fields with same value as start field are flooded.
  All flooded fields are labeled with given label. Flooded fields become start fields. Fields that have already been labeled are not flooded again.
  @param I Matrix upon which floodFill will be executed. Matrix is changed.
  @param start Coordinates of field in matrix I where floodFill will start.
  @param label Value that is used to label flooded fields.
  @return Number of flooded fields.
 */
int floodFill(Mat& I, Point2i start, unsigned char label) {
    int floodSize = 0;
    queue<Point2i> front;
    
    int value = I.at<uchar>(start.x, start.y);// value of starting field -> we will flood only fields that have this value.
    front.push(start);
    I.at<uchar>(start.x, start.y) = label;

    int dlength = 8;
    int dr[] = {-1, -1, -1, 0, 1, 1,  1,  0};
    int dc[] = {-1,  0,  1, 1, 1, 0, -1, -1};
    while (front.size() > 0) {
	int r = front.front().x;
	int c = front.front().y;
	front.pop();
	floodSize++;

	for (int i = 0; i < dlength; i++) {// for each neighbour
	    int rr = r + dr[i];
	    int cc = c + dc[i];

	    if (rr < 0 || rr >= I.rows || cc < 0 || cc >= I.cols)
		continue;
	    if (I.at<uchar>(rr,cc) != value || I.at<uchar>(rr,cc) == label)
		continue;

	    front.push(Point2i(rr,cc));
	    I.at<uchar>(rr,cc) = label;
	}
    }

    return floodSize;
}

void extractLargestBlob(Mat& I) {
    unsigned char label = 42;
    unsigned char blobValue = 255;// value of fields that make blobs(that are to be flooded).
    Point2i largestBlobStart;
    int largestBlobSize = 0;

    // find the largest blob.
    for (int r = 0; r < I.rows; r++)
	for (int c = 0; c < I.cols; c++) {
	    if (I.at<uchar>(r,c) != label && I.at<uchar>(r,c) == blobValue) {
		int blobSize = floodFill(I, Point2i(r, c), label);
		if (blobSize > largestBlobSize) {
		    largestBlobStart = Point2i(r, c);
		    largestBlobSize = blobSize;
		}
	    }
	}

    // set the fields of largest blob to blobValue.
    floodFill(I, largestBlobStart, blobValue);

    // set everything else to 0.
    for (int r = 0; r < I.rows; r++)
	for (int c = 0; c < I.cols; c++)
	    if (I.at<uchar>(r,c) != blobValue)
		I.at<uchar>(r,c) = 0;
}

/* a and b must be grayscale */
pair<int, int> findBestTranslation(Mat& a, Mat& b, int R=10, int step=2, int drStart = 0, int dcStart = 0) {
    pair<int, int> bestTranslation = make_pair(0,0);
    double smallestDiff = -1;

    // try each translation
    for (int dr = -R; dr < R; dr += step)
	for (int dc = -R; dc < R; dc += step) {
	    int numPixels = 0;
	    long long diffSum = 0;
	    // calculate difference for each pixel
	    for (int r = 0; r < b.rows; r++)
		for (int c = 0; c < b.cols; c++) {
		    int rr = r-(dr+drStart);
		    int cc = c-(dc+dcStart);
		    // check if translated point is inside boundaries
		    if (rr > 0 && rr <= a.rows && cc > 0 && cc <= a.cols) {
			diffSum += abs(b.at<uchar>(r,c) - a.at<uchar>(rr,cc));
			numPixels++;
		    } else {		     
			//diffSum += 100;
			//numPixels++;
		    }
		}

	    // if this is best translation, remember it.
	    double normDiff = diffSum / (double)numPixels;
	    if (smallestDiff == -1 || normDiff < smallestDiff) {
		smallestDiff = normDiff;
		bestTranslation = make_pair(dr+drStart, dc+dcStart);
	    }
	}

    if (step > 1)
	return findBestTranslation(a, b, step-1, 1, bestTranslation.first, bestTranslation.second);
    else return bestTranslation;
}

/* takes image a, translates it so it matches image b as best as it can, and fills created empty space with pixels from b. Result is stored in res. */
void calibrateImage(Mat& a, Mat& b, Mat& res) {
    // find translation that matches a to b.
    Mat aa, bb;
    cvtColor(a, aa, CV_BGR2GRAY);
    cvtColor(b, bb, CV_BGR2GRAY);
    pair<int, int> tr = findBestTranslation(aa, bb);
    cout << "calibration: " << tr.first << " " << tr.second << endl;

    res.create(b.rows, b.cols, b.type());
    for (int r = 0; r < res.rows; r++)
	for (int c = 0; c < res.cols; c++) {
	    int rr = r - tr.first;
	    int cc = c - tr.second;
	    if (rr > 0 && rr <= a.rows && cc > 0 && cc <= a.cols)
		res.at<Vec3b>(r,c) = a.at<Vec3b>(rr,cc);
	    else res.at<Vec3b>(r,c) = b.at<Vec3b>(r,c);
	}
}
