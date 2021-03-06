/** Sample classifier
 */

#ifndef SAMPLE_CLASSIFIER
#define SAMPLE_CLASSIFIER

#include "../Classifier.hpp"

class SampleClassifier : public Classifier
{
    public: 

        // Virtualna metoda - obavezno implementirati
        virtual vector< pair<string, double> > classify(Mat img, int resNum);

        // Virtualna metoda - obavezno implementirati
        virtual void learn(map< string, vector<Mat> >& learningData, void* param);

        
        // Ovdje ce dalje sad ici metode i podaci specificni za vas klasifikator
        
        // npr:
        // void izluciZnacajke(...);
        // vector< vector<Double> > koeficijenti;

};

#endif /* end of include guard: SAMPLE_CLASSIFIER */

