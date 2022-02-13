#include <iostream>
#include "TestScan.h"
#include "Util.h"
#include "TestParse.h"
#include "TestSemanticAnalysis.h"

using namespace std;

int main() {
    string readFileName = "../AAA.T";
    string scanFileName = "../BBB.T";
    string parseFileName = "../CCC.T";
    string semanticAnalysisFileName = "../DDD.T";

    string inputFileName;
    string fileName = "";

    cout << "Please enter the file name:" << endl;
    cin >> inputFileName;

    if (FILE *file = fopen(inputFileName.data(), "r")) {
        fclose(file);
        string temp = inputFileName.substr(inputFileName.length() - 2, inputFileName.length());
        if (temp != ".t" or temp != ".T") {
            cerr << "Bad file suffix, this program can only open files with Suffix \".T\"." << endl;
        }

    } else {
        cerr << "Cannot open the file " << inputFileName << endl;
        return 1;
    }

    fileName = inputFileName.substr(0, inputFileName.length() - 2);

    readFileName = fileName + "" +".T";
    scanFileName = fileName + "_scan" +".tsc";
    parseFileName = fileName + "_parse" +".tpa";
    semanticAnalysisFileName = fileName +".tat";

    string str;
    node *root = new node;

    //词法分析
    FileUtil::readFile(readFileName, str);
    if (!TestScan::scan(str, scanFileName)) {
        return 1;
    }

    //语法分析
    FileUtil::readFile(scanFileName, str);
    if (!TestParse::parse(str, parseFileName, root)) {
        node::delNode(root);
        return 1;
    }

    //语义分析&中间代码生成
    if (!TestSemanticAnalysis::semanticAnalysis(root, semanticAnalysisFileName)) {
        node::delNode(root);
        return 1;
    }

    node::delNode(root);
    return 0;
}
