#include "KNNAdapter.hpp"

#include <algorithm>

KNNAdapter::KNNAdapter(int k_/*=1*/) {
    k = k_;
}

void KNNAdapter::train(vector< vector<float> >& examples, vector<string>& labels) {
    Mat matExamples, matLabels;
    prepareTrainingData(examples, labels, matExamples, matLabels);
    knn.train(matExamples, matLabels);
}

string KNNAdapter::classify(vector<float>& example) {
    Mat matExample;
    prepareExample(example, matExample);
    float prediction = knn.find_nearest(matExample, k);
    return getLabelStringFromOpenCV(prediction);
}
 
