#ifndef BAYESADAPTER_HPP
#define BAYESADAPTER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <map>

#include "AbstractAdapter.hpp"

using namespace std;
using namespace cv;


class BayesAdapter : public AbstractAdapter {
private:
    CvNormalBayesClassifier bayes;

public:
    virtual void train (vector< vector<float> >& examples, vector<string>& labels);

    virtual string classify(vector<float>& example);
};

#endif // BAYESADAPTER_HPP
