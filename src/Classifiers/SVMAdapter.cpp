#include "SVMAdapter.hpp"

#include <algorithm>


void SVMAdapter::train(vector< vector<float> >& examples, vector<string>& labels) {
    Mat matExamples, matLabels;
    prepareTrainingData(examples, labels, matExamples, matLabels);
    // Uses RBF kernel
    svm.train(matExamples, matLabels);
}

string SVMAdapter::classify(vector<float>& example) {
    Mat matExample;
    prepareExample(example, matExample);
    float prediction = svm.predict(matExample);
    return getLabelStringFromOpenCV(prediction);
}
 
