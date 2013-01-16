
#include "TemplateMatching.hpp"

int main(int argc, char const *argv[])
{
    // Create instance of Classifier
    Classifier* tmCl = new TemplateMatching();
    
    // Count wrong classifications
    int wrongs = tmCl->test("../testing/testConfFull.txt", 2, "../../siluete/", "PCAReport.html");

    // VAZNO!!
    // Vazno je da path do silueta na kraju ima ovaj slash (ili backslash, ovisno o OS), inace vam nece raditi.

    cout << "Number of wrong classifications: " << wrongs << endl;
    return 0;
}
