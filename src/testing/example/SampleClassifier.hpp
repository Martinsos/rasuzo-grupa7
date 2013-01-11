/** Sample classifier
 */

#ifndef SAMPLE_CLASSIFIER
#define SAMPLE_CLASSIFIER

#include "../Classifier.hpp"

class SampleClassifier : public Classifier
{
    public: 
        virtual string classify(Mat img);

        virtual void learn(map< string, vector<Mat> >& learningData);
};

#endif /* end of include guard: SAMPLE_CLASSIFIER */

