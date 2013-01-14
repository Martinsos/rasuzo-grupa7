#include "StdAfx.h"
#include "MinimumDistancePCAClassifier.hpp"
#include<complex>
#include<math.h>

#define PI 3.14159265

vector< pair<string, double> > MinimumDistancePCAClassifier::classify(Mat img, int resNum) 
{
	vector< pair<string, double> > results;
	set< pair<double, string>> averages;
	vector<Point> contour;
	vector<double> coeffs;
	set< pair<double, string>>::iterator iter;
	Mat imgBW;
	string className;
	set< string> usedNames;
	
	cvtColor(img, imgBW, CV_BGR2GRAY);
	contour = findMatContours(imgBW);

	for (int i = 0; i < (sizeof(coeffIndexes) / sizeof(int)); i = i + 2) 
	{
		calculateGranlundCoefficients(contour, coeffIndexes[i], coeffIndexes[i + 1], coeffs);
	}

	Mat_<double> matrix(coeffs, true);
	Mat testFeature = eigenvectors * matrix;
	testFeature = testFeature.t();
	calculateDistancePCA(averages, testFeature);

	// getting best results
	iter = averages.begin();
	while (results.size() < resNum) 
	{
		className = iter->second;
		if (usedNames.find(className) == usedNames.end()) 
		{
			results.push_back(make_pair(className, iter->first));
			usedNames.insert(className);
			
		}
		++iter;
	}
	
	return results;
}

void MinimumDistancePCAClassifier::learn(map< string, vector<Mat> >& learningData, void* param) 
{
	map< string, vector<Mat> >::iterator classId;
	vector<Point> contour;
	Mat img;
	int numComponents = 15;

	// choosing Granlund coefficient parameters
	int index = 0;
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 2; j <= 5; j++) 
		{
			coeffIndexes[index] = i;
			coeffIndexes[index + 1] = j;
			index = index + 2;
		}
	}

	for (classId = learningData.begin(); classId != learningData.end(); ++classId) 
	{
		for (int picture = 0; picture < (int) classId->second.size(); picture++) 
		{
			cvtColor(classId->second.at(picture), img, CV_BGR2GRAY); // turn picture to black and white
			contour = findMatContours(img);

			vector<double> coeffs;
			for (int i = 0; i < (sizeof(coeffIndexes) / sizeof(int)); i = i + 2) 
			{
				calculateGranlundCoefficients(contour, coeffIndexes[i], coeffIndexes[i + 1], coeffs);
			}

			classNames[pcaCoeffs.rows] = classId->first;
			Mat_<double> matrix(coeffs, true);
			matrix = matrix.t();
			pcaCoeffs.push_back(matrix);		
		}
	}

	performPCA(numComponents);
}

vector<Point> MinimumDistancePCAClassifier::findMatContours(Mat& bin) 
{
	vector<vector<Point>> contours;
	Mat frame = bin.clone();
	int max = 0;

	// find contours
	findContours(frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (unsigned int i = 0; i < contours.size(); i++) 
	{
		if (contours[i].size() > contours[max].size()) 
		{
			max = i;
		}
	}

	return contours[max];
}

void MinimumDistancePCAClassifier::calculateGranlundCoefficients(vector<Point> contour, int m, int n, 
	vector<double>& coeffs) 
{
	double realAm = 0.0, imagAm = 0.0, realAn = 0.0, imagAn = 0.0, realAone = 0.0, imagAone = 0.0;
	double realDmn = 0.0, imagDmn = 0.0;
	double x, y;
	double multiplicationConst;
	int N = contour.size();

	for (int i = 0; i < N; i++) 
	{
		x = contour[i].x;
		y = contour[i].y;

		multiplicationConst = 2 * PI * i / N;
		realAm += x * cos(multiplicationConst * (m + 1)) + y * sin(multiplicationConst * (m + 1));
		imagAm += y * cos(multiplicationConst * (m + 1)) - x * sin(multiplicationConst * (m + 1));

		realAn += x * cos(multiplicationConst * (1 - n)) + y * sin(multiplicationConst * (1 - n));
		imagAn += y * cos(multiplicationConst * (1 - n)) - x * sin(multiplicationConst * (1 - n));

		realAone += x * cos(multiplicationConst) + y * sin(multiplicationConst);
		imagAone += y * cos(multiplicationConst) - x * sin(multiplicationConst);
	}

	complex<double> am(realAm, imagAm);
	complex<double> an(realAn, imagAn);
	complex<double> aone(realAone, imagAone);

	realDmn = real((pow(am, n) * pow(an, m)) / pow(aone, m + n));
	imagDmn = imag((pow(am, n) * pow(an, m)) / pow(aone, m + n));

	coeffs.push_back(realDmn);
	coeffs.push_back(imagDmn);
}

void MinimumDistancePCAClassifier::calculateDistancePCA(set< pair<double, string>>& averages, Mat coeffs)
{
	double diff;
	double sum = 0.0;
	int i = 0, j = 0;

	for (i = 0; i < features.rows; i++) 
	{
		for (j = 0; j < features.cols; j++) 
		{
			diff = abs(features.at<double>(i, j) - coeffs.at<double>(0, j));
			sum += diff;
		}

		sum /= j;
		averages.insert(make_pair(sum, classNames[i]));
		sum = 0.0;
	}
}

void MinimumDistancePCAClassifier::performPCA(int numComponents) 
{
	PCA pca(pcaCoeffs, Mat(), CV_PCA_DATA_AS_ROW, numComponents);

	Mat mean = pca.mean.clone();
	Mat eigenvalues = pca.eigenvalues.clone();
	eigenvectors = pca.eigenvectors.clone();

	Mat t_pcaCoeffs = pcaCoeffs.t();
	features = eigenvectors * t_pcaCoeffs;
	features = features.t();
}