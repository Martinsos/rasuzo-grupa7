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

       /**  Main testing function
         *  Loops through all examples and classifies them
         *
         *  @param  testConf    Path to file holding testing configuration 
         *  @param  resNum      Number of best results to look at
         *  @param  pathToSil   Path to folder with silhouettes
         *  @param  reportPath  Path where is HTML report stored
         *  @param  param       Used for giving parameters to learn() method
         *
         *  @returns            Number of correctly classified examples
         */
        int test(string testConf, int resNum, string pathToSil, string reportPath, void* param = NULL);

        /** Classifies given example
         *
         *  @param  img         Image to classify
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
         *  @param  param           Parameters for learning process
         */
        virtual void learn(map< string, vector<Mat> >& learningData, void* param) = 0;
    
    protected:
        map< string, vector<Mat> > learningData;

    private:

        map< string, vector<Mat> > testData;

        /** Loads testing configuration from file
         *
         *  @param  testConf    Path to file holding testing configuration 
         *  @param  pathToSil   Path to folder with silhouettes
         */
        int loadData(string testConf, string pathToSil);

        // -------------------------- Report generation -------------------------- //

        /** Generates HTML table for given confusion matrix
         *
         *  @param  confusionMat    Confusion matrix
         *  @returns                String with HTML record
         */
        typedef map < string, map<string, int> > ConfusionMatrix;
        string confusionMatrixToHTML(ConfusionMatrix& confusionMat);


        /** Generates HTML table for given classification results
         *
         *  @param  clRes   Classification results 
         *  @param  resNum  Number of results returned by classifier for each test
         *  @returns        String with HTML record
         */
        typedef vector< pair < string, vector< pair<string, double> > > > ClassifResults;
        string classifResultsToHTML(ClassifResults& clRes, int resNum);


        /** Generates report to given path.
         *  So far, report consists of confusion matrix.
         *
         *  @param  confusionMatrix Confusion matrix 
         *  @param  repPath         Path of report file 
         *  @returns                status - 0 if everyhting ok
         */
        int generateReport(ConfusionMatrix& confusionMat, int wrong, int total, string repPath,
                           int resNum, ClassifResults& clRes, int wrongStrict);

        // ----------------------------------------------------------------------- //

        /**
         * Returns the files from given folder.
         * Implementation is OS independant
         *
         * @param name of the folder
         * @return vector with filenames
         */
        static vector<string> getFilesFromFolder(string folderName);

        /** Gets all class IDs.
         *  Actually reads keys from learningSet
         */
        vector<string> getClassIDs();


};

#endif /* end of include guard: CLASSIFIER_HPP */

