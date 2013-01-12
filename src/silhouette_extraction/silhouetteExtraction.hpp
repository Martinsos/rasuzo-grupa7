#ifndef SILHOUETTE_EXTRACTION_HPP
#define SILHOUETTE_EXTRACTION_HPP

#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<vector>
#include<utility>

using namespace std;
using namespace cv;

/*
 * Given background image and image with object on it (and same background) it extracts silhouette of object.
 * @param img Image with object.
 * @param bkg Background image.
 * @param binary Result of silhouette extraction. Silhouette is white, background is black.
 * @param blurf1 Factor of blur that will be applyed on img and bkg.
 * @param blurf2 Factor of blur that will be applyed on difference image.
 * @param thresh Number that will be added to median to get a threshold for binarizing image.
 * @param calibrate If true, attempt to calibrate img and bkg before extracting silhouette (translate img so it fits bkg as best as possible).
 */
void extractSilhouette(Mat& img, Mat& bkg, Mat& binary, int blurf1, int blurf2, int thresh, bool calibrate);
// Takes maximum difference of RGB for each pixel
void extractSilhouetteRGBMax(Mat& img, Mat& bkg, Mat& binary, int blurf1, int blurf2, int thresh, bool calibrate);
// Takes sum of RGB differences for each pixel
void extractSilhouetteRGBSum(Mat& img, Mat& bkg, Mat& binary, int blurf1, int blurf2, int thresh, bool calibrate);


//---------------- UTILITY -------------------//
unsigned char medianBW(Mat&);
void fillHoles(Mat&, Mat&);
int floodFill(Mat&, Point2i, unsigned char);
void extractLargestBlob(Mat&);
pair<int, int> findBestTranslation(Mat&, Mat&, int, int, int, int);
void calibrateImage(Mat&, Mat&, Mat&);
//--------------------------------------------//

#endif // SILHOUETTE_EXTRACTION_HPP
