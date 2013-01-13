#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<vector>
#include<iostream>

using namespace std;
using namespace cv;


/* Computes error
 *
 * @param first     First vector of granlund coefficients
 * @param second    Second vector of granlund coefficients
 *
 * @returns         Computed error between vectors
 */
float errorFunction(vector <complex <float> > first, vector <complex <float> > second);

/* Function that assists in sorting structure
 *
 * @param i         First element
 * @param j         Second element
 *
 * @returns         True if i < j or false otherwise
 */
bool sortFunction (pair <string, double> i, pair <string, double> j);

/* Function prepares picture and get contour on picture
 *
 * @param img       OpenCV Mat object representing picture
 *
 * @returns         Returns contour points
 */
vector <Point> preparePicture(Mat img);

/* Computes fourier coefficients and returns
 * granlund coefficients
 *
 * @param contour   Points of contour
 *
 * @returns         Returns vector of grandlund computed granlund coefficients
 */
vector <complex <float> > granlundCoefficients(vector <Point> contour);

// Granlund coefficients
vector <pair <string, vector <complex <float> > > > gran_cof;
