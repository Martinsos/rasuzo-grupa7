#pragma once
#ifndef GRANCOEF_HPP
#define GRANCOEF_HPP

#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/ml/ml.hpp>
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
double errorFunction(vector <complex <double> > first, vector <complex <double> > second);

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
vector <complex <double> > granlundCoefficients(vector <Point> contour);

#endif // GRANCOEF_HPP
