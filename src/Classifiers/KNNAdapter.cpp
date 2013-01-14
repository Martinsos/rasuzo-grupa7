#include "KNNAdapter.hpp"

#include <algorithm>


void KNNAdapter::train(vector< vector<float> >& examples, vector<string>& labels) {
    Mat matExamples, matLabels;
    prepareTrainingData(examples, labels, matExamples, matLabels);
    knn.train(matExamples, matLabels);
}

string KNNAdapter::classify(vector<float>& example) {
    Mat matExample;
    prepareExample(example, matExample);
    float prediction = knn.find_nearest(matExample, 1);
    return getLabelStringFromOpenCV(prediction);
}
 
