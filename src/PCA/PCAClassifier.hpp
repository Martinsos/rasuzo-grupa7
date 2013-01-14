#ifndef PCA_CLASSIFIER
#define PCA_CLASSIFIER

#include "../testing/Classifier.hpp"

class PCAClassifier :
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
	
	int PCA_metoda(vector<Mat> slike, Mat temp, int num_components);

	int templateMatching_metoda(vector<Mat> slike, Mat temp);

	Mat prilagodba_slike(Mat slika, int max_redak, int max_stupac);

	Mat roi_retci(Mat src,int* prvi_redak, int* zadnji_redak);

	Mat roi_stupci(Mat src,int* prvi_stupac, int* zadnji_stupac);

	void prilagodba(Mat* image_adj,Mat* temp_adj, Mat image, Mat temp,int prvi_img,int zadnji_img,int prvi_tmp,int zadnji_tmp,int vrsta);

	double podudaranje(Mat image, Mat temp);

	double cross_correlation( IplImage* img1, IplImage* img2 );

	CvPoint nadi_centroid(Mat slika);

	vector<Mat> podesavanje_po_centroidu(vector<Mat> slike);

};

#endif
