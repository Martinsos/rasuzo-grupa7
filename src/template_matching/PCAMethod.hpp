#ifndef  PCA_METHOD_HPP
#define	 PCA_METHOD_HPP

#include "../testing/Classifier.hpp"
#include <set>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

class PCAMethod:public Classifier
{
public:
	/**Classify the image
	* @param img image to be classified
	* @param resNum number of resullts to return
	*
	*@return ID's of identified classes, sorted form the the best matching
			 Elements of vector: (classID, assocValue),
			 [(Viktor,0.55),(Matija,0.22)] 
	*/
	
	vector<pair<string, double> > classify(Mat img, int resNum);

	/** Learning of features
	*
	*@param LearningData
	*
	*/
	void learn(map < string, vector<Mat> >& learningData, void* param);

private:
	Mat meanImage;

	Mat matrixRow;

	Mat features;

	Mat pcaMatrix;

	Mat eigevectors;

	Mat adjustByCenter(Mat image,CvPoint center);

	map< int, string > classNames;

	CvPoint findCenter(Mat image);

	void performPCA(int numComponents);

	void calculateEuclidianDistancePCA(set< pair<double, string> >& averages, Mat coeffs);
};

#endif // ! PCA_METODA_HPP
