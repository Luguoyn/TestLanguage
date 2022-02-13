#ifndef TESTLANGUAGE_DATA_H
#define TESTLANGUAGE_DATA_H

#include "string"
#include "vector"

using namespace std;

class Data {
public:
    static const vector<string> keyword;
    static const vector<char> singleWord;
    static const vector<string> doubleWord;
    static const string commentFirst;
    static const string commentLast;
    static const vector<char> emptyChar;
private:
    Data() { ; }
};


#endif //TESTLANGUAGE_DATA_H
