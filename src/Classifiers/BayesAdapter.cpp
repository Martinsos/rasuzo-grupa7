#include "BayesAdapter.hpp"

#include <algorithm>


BayesAdapter::BayesAdapter(vector< vector<float> >& examples, vector<string>& labels) {
    // Build examples matrix.
    Mat matExamples(examples.size(), examples[0].size(), CV_32FC1);
    for (int r = 0; r < examples.size(); r++)
	for (int c = 0; c < examples[c].size(); c++)
	    matExamples.at<float>(r,c) = examples[r][c];

    // Build labels matrix.
    uniqueLabels = labels;
    sort(uniqueLabels.begin(), uniqueLabels.end());
    uniqueLabels.erase(unique(uniqueLabels.begin(), uniqueLabels.end()), uniqueLabels.end());
    map<string, int> labelIndexes;
    for (int i = 0; i < uniqueLabels.size(); i++)
	labelIndexes[uniqueLabels[i]] = i;
    Mat matLabels(examples.size(), 1, CV_32FC1);
    for (int r = 0; r < labels.size(); r++)
	matLabels.at<float>(r,0) = (float)labelIndexes[labels[r]];
    
    bayes.train(matExamples, matLabels);
}

string BayesAdapter::classify(vector<float>& example) {
    Mat matExample(1, example.size(), CV_32FC1);
    for (int c = 0; c < example.size(); c++)
	matExample.at<float>(0,c) = example[c];

    float prediction = bayes.predict(matExample);

    int labelIndex = round(prediction);
    return uniqueLabels[labelIndex];
}
 
