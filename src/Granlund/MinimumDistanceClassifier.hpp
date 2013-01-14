#ifndef MINIMUM_DISTANCE_CLASSIFIER_HPP
#define MINIMUM_DISTANCE_CLASSIFIER_HPP

#include "../testing/Classifier.hpp"
#include <set>
#include <math.h>

class MinimumDistanceClassifier :
	public Classifier
{
public:

	/** Classifies given example
         *
         *  @param  imageName   Name of image to classify
         *  @param  resNum      Number of results to return
         *
         *  @returns            Ids of identified classses, sorted starting from the best.
         *                      Elements of vector are: (classId, assocValue).
         *                      E.g [("Viktor", 0.78), ("Tena", 0.22)]
         */
	vector< pair<string, double> > classify(Mat img, int resNum);

	/** Learns classifier
	 *
	 *  @param  learningData    
	 */

	void learn(map< string, vector<Mat> >& learningData, void* param);

private:
	/**
	 * Granlund coefficients of images in training set
	 */
	map< string, vector< vector<double>> > GranlundCoefficients;

	/** Finds contours from binary silhuette image
	 *
	 * @param	bin		Binary image
	 * 
	 * @returns			The largest found contour
	 */
	vector<Point> findMatContours(Mat& bin);

	/** Calculates Granlund coefficient for contour
	 *
	 * @param	contour
	 * @param	m		First parameter of Granlund coefficient
	 * @param	n		Second parameter of Granlund coefficient (e.g. d_12)
	 * @param	coeffs	Vector which stores calculated coefficients
	 */
	void calculateGranlundCoefficients(vector<Point> contour, int m, int n, vector<double>& coeffs);

	/** Calculates distances between Granlund coefficients of training set and test image
	 *
	 * @param	averages	Average distance for every pair (training image, test image)
	 * @param	coeffs		Test image Granlund coefficients
	 */
	void calculateDistance(set< pair<double, string>>& averages, vector<double> coeffs);

	/** Removes silx_ from image name
	 *
	 * @param	id		Name before modification

	 * @returns			Modified name
	 */
	string makeClassId(string id);
};

#endif // MINIMUM_DISTANCE_CLASSIFIER_HPP

