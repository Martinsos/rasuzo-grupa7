/** Sample classifier
 */

#ifndef SAMPLE_CLASSIFIER
#define SAMPLE_CLASSIFIER

#include "../Classifier.hpp"

class SampleClassifier : public Classifier
{
    public: 

        // Virtualna metoda - obavezno implementirati
        virtual string classify(Mat img);

        // Virtualna metoda - obavezno implementirati
        virtual void learn(map< string, vector<Mat> >& learningData);

        
        // Ovdje ce dalje sad ici metode i podaci specificni za vas klasifikator
        
        // npr:
        // void izluciZnacajke(...);
        // vector< vector<Double> > koeficijenti;

};

#endif /* end of include guard: SAMPLE_CLASSIFIER */

