#ifndef SVMADAPTER_HPP
#define SVMADAPTER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <map>

#include "AbstractAdapter.hpp"

using namespace std;
using namespace cv;


class SVMAdapter : AbstractAdapter {
private:
    CvSVM svm;

public:
    virtual void train (vector< vector<float> >& examples, vector<string>& labels);

    virtual string classify(vector<float>& example);
};

#endif // SVMADAPTER_HPP
