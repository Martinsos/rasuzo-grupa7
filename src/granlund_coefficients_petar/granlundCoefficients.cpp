#include<complex>
#include<math.h>
#include<algorithm>
#include<iostream>
#include<map>

#include "granlundCoefficients.hpp"
#include "granlundClassifiers.hpp"
#include "../testing/Classifier.hpp"
#include "../Classifiers/BayesAdapter.hpp"

using namespace std;
using namespace cv;


double errorFunction(vector <complex <double> > first, vector <complex <double> > second) {
    complex <double> sum = 0;

    for (int i = 0; i < first.size(); i++) {
        sum += pow(first[i] - second[i], 2);
    }
    return sqrt(abs(sum));
}

bool sortFunction (pair <string, double> i, pair <string, double> j) {
    return (i.second < j.second);
}

vector <Point> preparePicture(Mat img) {
    Mat imgray, thresh;
    vector <vector <Point> > contours;

    cvtColor(img, imgray, COLOR_BGR2GRAY);
    threshold(imgray,thresh,127,255,0);
    findContours(thresh,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);
    return contours[0];
}

vector <complex <double> > granlundCoefficients(vector <Point> contour) {
    double cent_x = 0, cent_y = 0;
    int contour_size = contour.size();

    // Fourier coefficients index
    int cof[] = {-4,-3,-2,-1,0,1,2,3,4,5};
    int cof_size = sizeof(cof) / sizeof(int);

    // Fourier coefficients
    map <int, complex <double> > a;

    // Granlund coefficients
    vector <complex <double> > gran_cof_internal;
    complex <double> temp;


    // Find contour center
    for (int i = 0; i < contour_size; i++) {
        cent_x += contour[i].x;
        cent_y += contour[i].y;
    }
    cent_x /= contour_size;
    cent_y /= contour_size;

    // Find difference between contour and contour center
    for (int i = 0; i < contour_size; i++) {
        contour[i].x = contour[i].x - cent_x;
        contour[i].y = contour[i].y - cent_y;
    }

    for (int k = 0; k < cof_size; k++) {
        int sum_r = 0, sum_i = 0;

        for (int n = 0; n < contour_size; n++) {
            sum_r = sum_r +
                    contour[n].x * cos( 2 * M_PI * cof[k] * n / contour_size) +
                    contour[n].y * sin( 2 * M_PI * cof[k] * n / contour_size);

            sum_i = sum_i +
                    contour[n].y * cos( 2 * M_PI * cof[k] * n / contour_size) -
                    contour[n].x * sin( 2 * M_PI * cof[k] * n / contour_size);

        }
        a[cof[k]] = complex <double> (sum_r,sum_i);
    }

    // Granlund coefficients - uncomment to apply
    gran_cof_internal.push_back((pow(a[2],2) * pow(a[-1],1)) / pow(a[1],3)); // m=1, n=2
//    gran_cof_internal.push_back((pow(a[3],2) * pow(a[-1],2)) / pow(a[1],4)); // m=2, n=2
//    gran_cof_internal.push_back((pow(a[3],3) * pow(a[-2],2)) / pow(a[1],5)); // m=2, n=3
//    gran_cof_internal.push_back((pow(a[3],4) * pow(a[-3],2)) / pow(a[1],6)); // m=2, n=4
//    gran_cof_internal.push_back((pow(a[3],5) * pow(a[-4],2)) / pow(a[1],6)); // m=2, n=5
//    gran_cof_internal.push_back((pow(a[4],3) * pow(a[-2],3)) / pow(a[1],6)); // m=3, n=3
//    gran_cof_internal.push_back((pow(a[5],3) * pow(a[-2],4)) / pow(a[1],7)); // m=4, n=3
//    gran_cof_internal.push_back((pow(a[5],4) * pow(a[-3],4)) / pow(a[1],8)); // m=4, n=4

    return gran_cof_internal;
}

int main(int argc, char** argv) {
    int wrongs;

    Classifier* bay = new BayesClassifier();
    Classifier* dis = new DistanceClassifier();
    Classifier* svm  = new SVMClassifier();

    wrongs = bay ->test("testConfSample.txt", 3, "../../siluete/","sampleReportBayes.html");
    cout << wrongs << endl;

    wrongs = dis ->test("testConfSample.txt", 3, "../../siluete/","sampleReportDistance.html");
    cout << wrongs << endl;

    wrongs = svm ->test("testConfSample.txt", 3, "../../siluete/","sampleReportSVM.html");
    cout << wrongs << endl;

    return 0;
}

