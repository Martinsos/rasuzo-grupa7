/** Demo
 *  In this main is presented the functionality of our program.
 */

#include "../height_width_matching/HWMatching.hpp"
#include "../Granlund/MinimumDistanceClassifier.hpp"

#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <method_name>\n\n", argv[0]);
        
        fprintf(stderr, "You can choose between multiple methods:\n"
                         "1. granlund: Granlund coefficients\n"
                         "2. bp-knn  : Body proportions matching with knn\n"
                         "3. bp-qe   : BP matching with quadratic error\n"
                         "4. bp-bayes: BP matching with bayes classificator\n"
                         "5. bp-swm  : BP matching with SVM classificator\n"
                         "6. bp-rf   : BP matching with Random Forest\n"
                         "HTML report will be generated.\n");

        cout << endl;
        return -1;
    }

    string method       = string(argv[1]);
    string testConfPath = "testConfDemo.txt";
    string silPath      = "../../siluete/";

    // -------------------- HW matching --------------------- //
    
    if( method.size() >= 2 ) {
        if (method.substr( 0, 2 ) == "bp")
        {
            // Instantiate classifier
            Classifier* hwCl = new HWMatching();
            
            // divide features with silhoutte height
            bool hr = false;

            // Set params
            pair<string, bool> param;

            if( method == "bp-qe" ) 
                   param = make_pair("", hr);
            else if( method == "bp-knn" ) 
                   param = make_pair("knn", hr);
            else if( method == "bp-bayes" )
                   param = make_pair("bayes", hr);
            else if( method == "bp-svm" ) 
                   param = make_pair("svm", hr);
            else if( method == "bp-rf" ) 
                   param = make_pair("random_forest", hr);
            else {
                  cout << "Wrong method typed in" << endl;
                  return 1;
            }
 
            string bpReportName = method + "Report.html";

            // Generate report
            hwCl->test(testConfPath, 3, silPath, bpReportName, &param);

            cout << "Generated " << bpReportName << endl;
        }
    }
    
    // -------------------- Granlund coefficients --------------------- //
    
    if (method == "granlund")
    {
        // Instantiate classifier
        Classifier* granCl = new MinimumDistanceClassifier();

        // Generate report
        granCl->test(testConfPath, 3, silPath, "granlundReport.html");

        cout << "Generated granlundReport.html" << endl;
    }

    return 0;
}
