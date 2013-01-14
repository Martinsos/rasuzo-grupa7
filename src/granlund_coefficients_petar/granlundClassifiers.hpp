#pragma once
#ifndef GRACLASS_HPP
#define GRACLASS_HPP

#include "../testing/Classifier.hpp"
#include "../Classifiers/BayesAdapter.hpp"

using namespace std;
using namespace cv;

// Bayesian classificator
class BayesClassifier: public Classifier
{
    public:
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        virtual void learn(map< string, vector<Mat> >& learningData);
};

// Distance classificator - Square root of sum of quadratic errors
class DistanceClassifier: public Classifier
{
    public:
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        virtual void learn(map< string, vector<Mat> >& learningData);
};

#endif // GRACLASS_HPP

