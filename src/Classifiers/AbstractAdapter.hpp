#ifndef ABSTRACTADAPTER_HPP
#define ABSTRACTADAPTER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace cv;


class AbstractAdapter {
private:
    vector<string> uniqueLabels;

public:
    /*
     * Trains classifier.
     * @param examples Vector containing examples, each example is vector of floats.
     * @param labels Vector containing labels, label at position i corresponds to example i.
     */
    virtual void train (vector< vector<float> >& examples, vector<string>& labels) = 0;

    /*
     * Classifies given example.
     * @param example
     * @return Predicted label(class).
     */
    virtual string classify(vector<float>& example) = 0;

protected:
    /*
     * Takes examples and labels and constructs matExamples and matLabels that opencv clasiffiers use.
     * Also transforms labels from strings into float (opencv needs floats) and remembers the mapping so it can return them to string later.
     */
    void prepareTrainingData(vector< vector<float> >& examples, vector<string>& labels, Mat& matExamples, Mat& matLabels);
    
    /*
     * Takes example and constructs matExample that opencv classifiers use.
     */
    void prepareExample(vector<float>& example, Mat& matExample);

    /*
     * Opencv classifiers return float as label. This function transforms that float into string.
     */
    string getLabelStringFromOpenCV(float opencvLabel);
};

#endif // ABSTRACTADAPTER_HPP
