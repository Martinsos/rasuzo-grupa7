/** Demo
 *  In this main is presented the functionality of our program.
 */

#include "../height_width_matching/HWMatching.hpp"

#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s", argv[0]);
        return -1;
    }

    string testConfPath = "../testing/testConfFull.txt";
    string silPath      = "../../siluete/";

    // -------------------- HW matching --------------------- //
    
    // Instantiate classifier
    Classifier* hwCl = new HWMatching();

    // Set params
    string method = "knn";

    // Generate report
    hwCl->test(testConfPath, 1, silPath, "hwReport.html", &method);

    
    return 0;
}
