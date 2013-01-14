#include "HWMatching.hpp"

int main(int argc, char const *argv[] )
{
    // Create instance of Classifier
    Classifier* sampleCl = new HWMatching();

    string method = "knn";

    // Count wrong classifications
    int wrongs = sampleCl->test("testConfFull.txt", 1, "../../siluete/", "sampleReport.html", &method);

    // VAZNO!!
    // Vazno je da path do silueta na kraju ima ovaj slash (ili backslash, ovisno o OS), inace vam nece raditi.

    cout << "Number of wrong classifications: " << wrongs << endl;
    return 0;
}

