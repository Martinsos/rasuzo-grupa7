#ifndef MINIMUM_DISTANCE_CLASSIFIER_HPP
#define MINIMUM_DISTANCE_CLASSIFIER_HPP

#include "Classifier.hpp"
#include <set>
#include <math.h>

class MinimumDistancePCAClassifier :
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
	/** Granlund coefficient parameters */
	int coeffIndexes[40];

	/** Matrix of Granlund coefficients for every example, used for PCA */
	Mat pcaCoeffs;

	/** Matrix of all examples after PCA */
	Mat features;

	/** Eigenvectors created by PCA */
	Mat eigenvectors;

	/** Image index + image name */
	map< int, string> classNames;

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
	void calculateDistancePCA(set< pair<double, string>>& averages, Mat coeffs);

	/** Performs PCA
	 *
	 * @param	numComponents	number of features after PCA
	 */
	void performPCA(int numComponents);
};

#endif // MINIMUM_DISTANCE_CLASSIFIER_HPP

