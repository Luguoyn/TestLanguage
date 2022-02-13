#ifndef TESTLANGUAGE_TESTSCAN_H
#define TESTLANGUAGE_TESTSCAN_H

#include "Data.h"
#include "iostream"
#include "string"
#include "vector"
#include "Util.h"

using namespace std;

class TestScan {
public:
    static bool scan(const string& str, const string& scanFileName);
private:
    TestScan(){;}
};


#endif //TESTLANGUAGE_TESTSCAN_H
