/** Sample classifier
 */

#include "SampleClassifier.hpp"

string SampleClassifier::classify(Mat img)
{
    /** Ovo je metoda koja za danu sliku vrati kojoj klasi pripada, tj klasificira je.
     *
     *  Ovdje je tipican scenarij: izlucim znacajke iz ulazne slike, klasificiram pomocu 
     *  struktura podataka koje sam inicijalizirao u learn() metodi.
     *  
     */

    return "classId";
}

void SampleClassifier::learn(map< string, vector<Mat> >& learningData)
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
