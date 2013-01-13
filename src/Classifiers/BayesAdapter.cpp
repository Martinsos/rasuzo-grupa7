#include "BayesAdapter.hpp"

#include <algorithm>


void BayesAdapter::train(vector< vector<float> >& examples, vector<string>& labels) {
    Mat matExamples, matLabels;
    prepareTrainingData(examples, labels, matExamples, matLabels);
    bayes.train(matExamples, matLabels);
}

string BayesAdapter::classify(vector<float>& example) {
    Mat matExample;
    prepareExample(example, matExample);
    float prediction = bayes.predict(matExample);
    return getLabelStringFromOpenCV(prediction);
}
 
