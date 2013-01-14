#include <iostream>
#include <vector>
#include <string>

#include "SVMAdapter.hpp"
#include "BayesAdapter.hpp"
#include "KNNAdapter.hpp"

using namespace std;

int main() {
    vector< vector<float> > examples;
    vector<string> labels;
    
    vector<float> example(2);
    
    example[0] = -1; example[1] = -1;
    examples.push_back(example); labels.push_back("x");
    example[0] = -2; example[1] = -2;
    examples.push_back(example); labels.push_back("x");
    example[0] = -1; example[1] = -3;
    examples.push_back(example); labels.push_back("x");

    example[0] = 1; example[1] = 1;
    examples.push_back(example); labels.push_back("o");
    example[0] = 2; example[1] = -1;
    examples.push_back(example); labels.push_back("o");
    example[0] = 8; example[1] = 3;
    examples.push_back(example); labels.push_back("o");

    example[0] = -1; example[1] = 5;
    examples.push_back(example); labels.push_back("*");
    example[0] = 0; example[1] = 4;
    examples.push_back(example); labels.push_back("*");    

    SVMAdapter* svm = new SVMAdapter();
    svm->train(examples, labels);
    BayesAdapter* bayes = new BayesAdapter(); 
    bayes->train(examples, labels);
    KNNAdapter* knn = new KNNAdapter(); 
    knn->train(examples, labels);

    vector<float> e(2);
    e[0] = 4; e[1] = 0;
    cout << "Should be o: SVM=>" << svm->classify(e) << " Bayes=>" << bayes->classify(e) << " KNN=>" << knn->classify(e) << endl;
    e[0] = -1; e[1] = 5;
    cout << "Should be *: SVM=>" << svm->classify(e) << " Bayes=>" << bayes->classify(e) << " KNN=>" << knn->classify(e) << endl;
    e[0] = -2; e[1] = -2;
    cout << "Should be x: SVM=>" << svm->classify(e) << " Bayes=>" << bayes->classify(e) << " KNN=>" << knn->classify(e) << endl;
    e[0] = 6; e[1] = 1;
    cout << "Should be o: SVM=>" << svm->classify(e) << " Bayes=>" << bayes->classify(e) << " KNN=>" << knn->classify(e) << endl;
    e[0] = 0; e[1] = 4;
    cout << "Should be *: SVM=>" << svm->classify(e) << " Bayes=>" << bayes->classify(e) << " KNN=>" << knn->classify(e) << endl;
    
    delete svm;
    delete bayes;
    delete knn;
    return 0;
}
