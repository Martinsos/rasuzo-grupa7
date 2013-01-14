#include "RandomForestAdapter.hpp"

#include <algorithm>
#include <cmath>

void RandomForestAdapter::train(vector< vector<float> >& examples, vector<string>& labels) {
    Mat matExamples, matLabels;
    prepareTrainingData(examples, labels, matExamples, matLabels);
    int minSamples = std::min(1, (int)round(0.01*examples.size()));
    CvRTParams params = CvRTParams(5, minSamples, 0, false, 10, 0, false, 0, 50, 0.1, CV_TERMCRIT_ITER | CV_TERMCRIT_EPS);
    Mat dummy = Mat();
    rf.train(matExamples, CV_ROW_SAMPLE, matLabels, dummy,  dummy, dummy, dummy, params);
}

string RandomForestAdapter::classify(vector<float>& example) {
    Mat matExample;
    prepareExample(example, matExample);
    float prediction = rf.predict(matExample);
    return getLabelStringFromOpenCV(prediction);
}
 
