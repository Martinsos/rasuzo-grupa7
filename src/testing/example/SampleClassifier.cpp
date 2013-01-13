/** Sample classifier
 */

#include "SampleClassifier.hpp"

vector< pair<string, double> > SampleClassifier::classify(Mat img, int resNum)
{
    /** Ovo je metoda koja za danu sliku vrati kojoj klasi pripada, tj klasificira je.
     *
     *  Ovdje je tipican scenarij: izlucim znacajke iz ulazne slike, klasificiram pomocu 
     *  struktura podataka koje sam inicijalizirao u learn() metodi.
     *  
     */

    // Vratim vector parova, gdje je broj u svakom paru neka vrijednost po kojoj odredim koliko
    // je izgledna koja klasa (vjerojatnost, poklapanje ili nesto trece).
    // Ako je klasifikator takav da ne vraca nikakvu vrijednost, stavite -1 ili nesto bzvz po zelji.

    vector< pair<string, double> > ret;
    ret.push_back(make_pair("sil2_hrvoje", 0.93));
    ret.push_back(make_pair("sil2_zorana", 0.07));  // Pazite koliki je resNum, toliko rezultata treba vratiti!

    return ret;
}

void SampleClassifier::learn(map< string, vector<Mat> >& learningData, void* param)
{
    /** Ovo je metoda gdje stvorite strukture specificne za svoj konkretni klasifikator.
     *  
     *  Kao ulaz dobijete oznacene slike, mapu formata (imeKlase -> skupSlika).
     *  Vi sad ovdje dalje izlucujete znacajke iz tih slika, provodite ucenje s kojim stvarate
     *  strukture koje cete poslije koristiti u klasifikaciji.
     *
     *  Uglavnom, kad pozovete test() funkciju (koja je vec implementirana), ona ce pozvati ovu
     *  metodu koja ce sve srediti tako da dalje moze zvati classify().
     *  Ova metoda zapravo "priprema tlo" za classify(...) metodu.
     *
     */

    // Do stuff
}
