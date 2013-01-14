#ifndef RANDOMFORESTADAPTER_HPP
#define RANDOMFORESTADAPTER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <map>

#include "AbstractAdapter.hpp"

using namespace std;
using namespace cv;


class RandomForestAdapter : AbstractAdapter {
private:
    CvRTrees rf;

public:
    virtual void train (vector< vector<float> >& examples, vector<string>& labels);

    virtual string classify(vector<float>& example);
};

#endif // RANDOMFORESTADAPTER_HPP
