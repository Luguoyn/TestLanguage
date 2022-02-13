#ifndef TESTLANGUAGE_TESTSEMANTICANALYSIS_H
#define TESTLANGUAGE_TESTSEMANTICANALYSIS_H

#include "Util.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TestSemanticAnalysis {
public:
    static bool semanticAnalysis(node* root, const string& semanticAnalysisFileName);

private:

    static bool analysis(node* n, FileUtil& fout);
};


#endif //TESTLANGUAGE_TESTSEMANTICANALYSIS_H
