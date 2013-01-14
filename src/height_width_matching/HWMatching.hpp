#ifndef HW_MATCHING_CLASSIFIER
#define HW_MATCHING_CLASSIFIER

#include "../testing/Classifier.hpp"

class HWMatching : public Classifier {
  public:
    virtual vector< pair<string, double> > classify(Mat img, int resNum);
    virtual void learn(map< string, vector<Mat> >& learningData, void* param);

};

#endif
