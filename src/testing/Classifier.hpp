/** Abstract class for classifier testing
 *
 *  Usage: Concrete class extends this interface
 */

#ifndef CLASSIFIER_HPP
#define CLASSIFIER_HPP

#include <string>
#include <map>

#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Classifier
{
    public:

        /** Main testing function
         *  Loops through all examples and classifies them
         *
         *  @param  testConf    Path to file holding testing configuration 
         *  @param  pathToSil   Path to folder with silhouettes
         *  @returns            Number of correctly classified examples
         */
        int test(string testConf, string pathToSil);

        /** Classifies given example
         *
         *  @param  imageName   name of image to be classified
         *  @returns            Id of identified class        
         */
        virtual string classify(Mat img) = 0;

        /** Learns classifier
         *
         *  @param  learningData    
         */
        virtual void learn(map< string, vector<Mat> >& learningData) = 0;
    

    private:

        map< string, vector<Mat> > testData;
        map< string, vector<Mat> > learningData;

        /** Loads testing configuration from file
         *
         *  @param  testConf    Path to file holding testing configuration 
         *  @param  pathToSil   Path to folder with silhouettes
         */
        void loadData(string testConf, string pathToSil);


        /** Counts wrong classified examples
         *
         *  @return 
         */
        int countWrongs();
};

#endif /* end of include guard: CLASSIFIER_HPP */

