#ifndef TEMPLATE_MATCHING_HPP
#define TEMPLATE_MATCHING_HPP

#include "../testing/Classifier.hpp"
#include <set>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

class TemplateMatching : public Classifier
{
public:
	vector< pair<string, double> > classify(Mat img, int resNum);

	void learn(map < string, vector<Mat> >& learningData, void* param);

private:
	vector<Mat> templateImages;

	vector<string> classNames;

	CvPoint findCenter(Mat image);

	Mat adjustByCenter(Mat image,CvPoint center);

	double matching(Mat image, Mat testImage);
};

#endif
