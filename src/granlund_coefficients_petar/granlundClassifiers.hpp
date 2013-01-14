#pragma once
#ifndef GRACLASS_HPP
#define GRACLASS_HPP

#include "../testing/Classifier.hpp"
#include "../Classifiers/BayesAdapter.hpp"
#include "../Classifiers/SVMAdapter.hpp"
#include "../Classifiers/KNNAdapter.hpp"
#include "../Classifiers/RandomForestAdapter.hpp"

using namespace std;
using namespace cv;

// Distance classificator - Square root of sum of quadratic errors
class DistanceClassifier: public Classifier
{
    public:
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        virtual void learn(map< string, vector<Mat> >& learningData, void* param);
};

// Class which implements generic learning of classificator and
// generic classifications through parameter "void* param"
class SpecificClassifier: public Classifier
{

    public:
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        virtual void learn(map< string, vector<Mat> >& learningData, void* param);

};


#endif // GRACLASS_HPP

