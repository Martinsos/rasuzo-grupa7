#ifdef WIN32
    #include "StdAfx.h"
#endif

#include "MinimumDistanceClassifier.hpp"
#include <complex>
#include <sstream>
#include <math.h>

#define PI 3.14159265

int coeffIndexes[] = {1, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, 6, 2, 2, 3, 2}; // Granlund coefficient parameters

string longToStr(long long a)
{
    stringstream ss;
    ss << a;

    return ss.str();
}

vector< pair<string, double> > MinimumDistanceClassifier::classify(Mat img, int resNum) 
{
	vector< pair<string, double> > results;
	set< pair<double, string> > averages;
	vector<Point> contour;
	vector<double> coeffs;
	set< pair<double, string> >::iterator iter;
	Mat imgBW;
	string className;
	set< string> usedNames;
	
	cvtColor(img, imgBW, CV_BGR2GRAY);
	contour = findMatContours(imgBW);

	for (int i = 0; i < (sizeof(coeffIndexes) / sizeof(int)); i = i + 2) 
	{
		calculateGranlundCoefficients(contour, coeffIndexes[i], coeffIndexes[i + 1], coeffs);
	}

	calculateDistance(averages, coeffs);

	// getting best results
	iter = averages.begin();
	while (results.size() < resNum) 
	{
		className = makeClassId(iter->second);		
		if (usedNames.find(className) == usedNames.end()) 
		{
			results.push_back(make_pair(className, iter->first));
			usedNames.insert(className);
		}
		++iter;
	}
	
	return results;
}

void MinimumDistanceClassifier::learn(map< string, vector<Mat> >& learningData, void* param) 
{
	map< string, vector<Mat> >::iterator classId;
	vector<Point> contour;
	Mat img;

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

			GranlundCoefficients[classId->first].push_back(coeffs);
		}
	}
}

vector<Point> MinimumDistanceClassifier::findMatContours(Mat& bin) 
{
	vector<vector<Point> > contours;
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

void MinimumDistanceClassifier::calculateGranlundCoefficients(vector<Point> contour, int m, int n, 
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

void MinimumDistanceClassifier::calculateDistance(set< pair<double, string> >& averages, vector<double> coeffs)
{
	map< string, vector< vector<double> > >::iterator iter;
	double sum = 0.0;
	double diff;
	int i = 0, j = 0;

	for (iter = GranlundCoefficients.begin(); iter != GranlundCoefficients.end(); ++iter) 
	{ // for every person
		for (i = 0; i < (int) iter->second.size(); i++) 
		{ // for every image of that person
			for (j = 0; j < (int) iter->second[i].size(); j++) 
			{ // for every element of Granlund coefficients
				diff = abs(iter->second.at(i).at(j) - coeffs.at(j));
				sum += diff;
			}

			string name = iter->first + longToStr((long long) i);
			sum /= j;
			averages.insert(make_pair(sum, name));
			sum = 0.0;
		}
	}
}

string MinimumDistanceClassifier::makeClassId(string id) 
{
	string name;
	int index;

	name = id;
	index = name.size();
	name.erase(index - 1, 1);

	return name;
}
