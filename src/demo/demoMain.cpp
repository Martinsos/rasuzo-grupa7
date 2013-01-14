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
                         "2. hw      : Height-width matching\n"
                         "HTML report will be generated.\n");

        cout << endl;
        return -1;
    }

    string method       = string(argv[1]);
    string testConfPath = "testConfDemo.txt";
    string silPath      = "../../siluete/";

    // -------------------- HW matching --------------------- //
    
    if (method == "hw")
    {
        // Instantiate classifier
        Classifier* hwCl = new HWMatching();

        // Set params
        pair<string, bool> param = make_pair("", false);

        // Generate report
        hwCl->test(testConfPath, 3, silPath, "hwReport.html", &param);

        cout << "Generated hwReport.html" << endl;
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
