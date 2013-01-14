#pragma once
#ifndef GRACLASS_HPP
#define GRACLASS_HPP

#include "../testing/Classifier.hpp"
#include "../Classifiers/BayesAdapter.hpp"
#include "../Classifiers/SVMAdapter.hpp"

using namespace std;
using namespace cv;

// Bayesian classificator
class BayesClassifier: public Classifier
{
    public:
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        virtual void learn(map< string, vector<Mat> >& learningData, void* param);
};

// SVM classificator
class SVMClassifier: public Classifier
{
    public:
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        virtual void learn(map< string, vector<Mat> >& learningData, void* param);
};

// Distance classificator - Square root of sum of quadratic errors
class DistanceClassifier: public Classifier
{
    public:
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        virtual void learn(map< string, vector<Mat> >& learningData, void* param);
};

#endif // GRACLASS_HPP

