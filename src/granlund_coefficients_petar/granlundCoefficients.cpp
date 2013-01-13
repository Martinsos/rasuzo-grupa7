#include<complex>
#include<math.h>
#include<algorithm>
#include<iostream>
#include<map>

#include "granlundCoefficients.hpp"
#include "../testing/Classifier.hpp"

using namespace std;
using namespace cv;


float errorFunction(vector <complex <float> > first, vector <complex <float> > second) {
    complex <float> sum = 0;

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

class MyClassifier: public Classifier
{
    public:

        virtual vector< pair<string, double> > classify(Mat img, int resNum) {
            vector <complex <float> > gran_cof_img;
            vector <Point> contour;
            vector <pair <string,double> > error;
            double err;
            int index;
            int first_n = 3;
            vector< pair<string, double> > ret;

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

        virtual void learn(map< string, vector<Mat> >& learningData) {
            Mat image;
            vector <Point> contour;
            vector <complex <float> > gran_cof_img;
            map <string, vector<Mat> >::iterator data;

            for (data = learningData.begin(); data != learningData.end(); data++) {
                for (int i = 0; i < (data->second).size(); i++) {

                    image = (data->second)[i];
                    contour = preparePicture(image);
                    gran_cof_img = granlundCoefficients(contour);

                    gran_cof.push_back(make_pair(data->first,gran_cof_img));
                }
            }
        }
};

vector <complex <float> > granlundCoefficients(vector <Point> contour) {
    float cent_x = 0, cent_y = 0;
    int contour_size = contour.size();

    // Fourier coefficients index
    int cof[] = {-4,-3,-2,-1,0,1,2,3,4,5};
    int cof_size = sizeof(cof) / sizeof(int);

    // Fourier coefficients
    map <int, complex <float> > a;

    // Granlund coefficients
    vector <complex <float> > gran_cof;
    complex <float> temp;


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
        a[cof[k]] = complex <float> (sum_r,sum_i);
    }

    // Granlund coefficients
    gran_cof.push_back((pow(a[2],2) * pow(a[-1],1)) / pow(a[1],3)); // m=1, n=2
    gran_cof.push_back((pow(a[3],2) * pow(a[-1],2)) / pow(a[1],4)); // m=2, n=2
    gran_cof.push_back((pow(a[3],3) * pow(a[-2],2)) / pow(a[1],5)); // m=2, n=3
    gran_cof.push_back((pow(a[3],4) * pow(a[-3],2)) / pow(a[1],6)); // m=2, n=4
    gran_cof.push_back((pow(a[3],5) * pow(a[-4],2)) / pow(a[1],6)); // m=2, n=5
    gran_cof.push_back((pow(a[4],3) * pow(a[-2],3)) / pow(a[1],6)); // m=3, n=3
    gran_cof.push_back((pow(a[5],3) * pow(a[-2],4)) / pow(a[1],7)); // m=4, n=3
    gran_cof.push_back((pow(a[5],4) * pow(a[-3],4)) / pow(a[1],8)); // m=4, n=4

    return gran_cof;
}

int main(int argc, char** argv) {
    Mat image, imgray, thresh;
    vector <vector <Point> > contours;
    vector <complex <float> > gran_cof_img;

    Classifier* myCl = new MyClassifier();

    int wrongs = myCl->test("testConfSample.txt", 3, "../../siluete/","sampleReport.html");
    cout << wrongs << endl;
    return 0;
}

