
#include "SampleClassifier.hpp"

int main(int argc, char const *argv[])
{
    // Create instance of Classifier
    Classifier* sampleCl = new SampleClassifier();
    
    // Count wrong classifications
    int wrongs = sampleCl->test("testConfSample.txt", 3, "../../../siluete/", "sampleReport.html");

    // VAZNO!!
    // Vazno je da path do silueta na kraju ima ovaj slash (ili backslash, ovisno o OS), inace vam nece raditi.

    cout << "Number of wrong classifications: " << wrongs << endl;
    return 0;
}
