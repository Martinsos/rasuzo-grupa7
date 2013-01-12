
#include "SampleClassifier.hpp"

int main(int argc, char const *argv[])
{
    // Create instance of Classifier
    Classifier* sampleCl = new SampleClassifier();
    
    // Count wrong classifications
    int wrongs = sampleCl->test("testConfSample.txt", 3, "../../../siluete/");

    cout << "Number of wrong classifications: " << wrongs << endl;
    return 0;
}
