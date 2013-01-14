/*
 * Implementation of classifiers
 * defined in granlundClassifiers.hpp
 *
 */

#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/ml/ml.hpp>
#include<vector>
#include<iostream>

#include "granlundCoefficients.hpp"
#include "granlundClassifiers.hpp"
#include "../Classifiers/BayesAdapter.hpp"
#include "../testing/Classifier.hpp"

using namespace std;
using namespace cv;

BayesAdapter* bayes;
vector <vector <float> > training_data;
vector <string> classes;

// Granlund coefficients
vector <pair <string, vector <complex <double> > > > gran_cof;

vector< pair<string, double> > BayesClassifier::classify(Mat img, int resNum) {
            vector <complex <double> > gran_cof_img;
            vector <Point> contour;
            int index;
            vector< pair<string, double> > ret;
            float data[8];
            vector <float> gran_cof_vect;

            contour = preparePicture(img);
            gran_cof_img = granlundCoefficients(contour);

            for (int j = 0; j < gran_cof_img.size(); j++) {
                gran_cof_vect.push_back((float)gran_cof_img[j].real());
                gran_cof_vect.push_back((float)gran_cof_img[j].imag());
            }

            string kl = bayes->classify(gran_cof_vect);
            gran_cof_vect.clear();

            ret.push_back(make_pair(kl,0));
            return ret;
}

void BayesClassifier::learn(map< string, vector<Mat> >& learningData) {
            Mat image;
            vector <Point> contour;
            vector <complex <double> > gran_cof_img;
            map <string, vector<Mat> >::iterator data;
            vector <float> gran_cof_vect;

            int k = 0;
            int count = 0;
            for (data = learningData.begin(); data != learningData.end(); data++) {

                for (int i = 0; i < (data->second).size(); i++) {
                    classes.push_back(data->first);

                    image = (data->second)[i];
                    contour = preparePicture(image);
                    gran_cof_img = granlundCoefficients(contour);

                    for (int j = 0; j < gran_cof_img.size(); j++) {
                        gran_cof_vect.push_back((float)gran_cof_img[j].real());
                        gran_cof_vect.push_back((float)gran_cof_img[j].imag());
                    }

                    training_data.push_back(gran_cof_vect);
                    gran_cof_vect.clear();
                }
            }

            bayes = new BayesAdapter(training_data, classes);
}

vector< pair<string, double> > DistanceClassifier::classify(Mat img, int resNum) {
            vector <complex <double> > gran_cof_img;
            vector <Point> contour;
            vector <pair <string,double> > error;
            vector< pair<string, double> > ret;
            double err;
            int index;

            // First how many classes to return
            int first_n = 3;


            contour = preparePicture(img);
            gran_cof_img = granlundCoefficients(contour);

            // Find all errors
            for (int i = 0; i < gran_cof.size(); i++) {
                err = errorFunction(gran_cof_img,gran_cof[i].second);
                error.push_back(make_pair(gran_cof[i].first,err));
            }

            // Sort vector of errors
            sort (error.begin(), error.end(), sortFunction);

            // Return first n errors
            for (int i = 0; i < first_n; i++) {
                ret.push_back(make_pair(error[i].first, error[i].second));
            }

            return ret;
}

void DistanceClassifier::learn(map< string, vector<Mat> >& learningData) {
            Mat image;
            vector <Point> contour;
            vector <complex <double> > gran_cof_img;
            map <string, vector<Mat> >::iterator data;

            int k = 0;
            int count = 0;
            for (data = learningData.begin(); data != learningData.end(); data++) {

                for (int i = 0; i < (data->second).size(); i++) {

                    image = (data->second)[i];
                    contour = preparePicture(image);
                    gran_cof_img = granlundCoefficients(contour);

                    gran_cof.push_back(make_pair(data->first,gran_cof_img));
                }
            }
}
