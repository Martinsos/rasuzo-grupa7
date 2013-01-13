#ifndef SVMADAPTER_HPP
#define SVMADAPTER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace cv;


class SVMAdapter {
  private:
    CvSVM svm;
    vector<string> uniqueLabels;

  public:
    /*
     * Trains SVM.
     * @param examples Vector containing examples, each example is vector of floats.
     * @param labels Vector containing labels, label at position i corresponds to example i.
     */
   SVMAdapter(vector< vector<float> >& examples, vector<string>& labels);

    /*
     * Classifies given example.
     * @param example
     * @return Predicted label(class).
     */
    string classify(vector<float>& example);
};

#endif // SVMADAPTER_HPP
