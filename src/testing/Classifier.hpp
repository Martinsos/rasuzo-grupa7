/** Abstract class for classifier testing
 *
 *  Usage: Concrete class extends this interface
 */

#ifndef CLASSIFIER_HPP
#define CLASSIFIER_HPP

#include <string>
#include <map>
#include <utility>

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
         *  @param  resNum      Number of best resulst to look at
         *  @param  pathToSil   Path to folder with silhouettes
         *  @returns            Number of correctly classified examples
         */
        int test(string testConf, int resNum, string pathToSil);

        /** Classifies given example
         *
         *  @param  imageName   Name of image to classify
         *  @param  resNum      Number of results to return
         *
         *  @returns            Ids of identified classses, sorted starting from the best.
         *                      Elements of vector are: (classId, assocValue).
         *                      E.g. [("Viktor", 0.78), ("Tena", 0.22)]
         */
        virtual vector< pair<string, double> > classify(Mat img, int resNum) = 0;

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
        int loadData(string testConf, string pathToSil);


        /** Counts wrong classified examples
         *
         *  @param resNum   Number of results to look at
         *  @return 
         */
        int countWrongs(int resNum);

        /**
         * Returns the files from given folder.
         * Implementation is OS independant
         *
         * @param name of the folder
         * @return vector with filenames
         */
        static vector<string> getFilesFromFolder(string folderName);
};

#endif /* end of include guard: CLASSIFIER_HPP */

