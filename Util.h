#ifndef TESTLANGUAGE_UTIL_H
#define TESTLANGUAGE_UTIL_H

#include "fstream"
#include "string"
#include "iostream"
#include "vector"
#include "algorithm"

#define MAX_CHILD 10

using namespace std;


struct testID {
    string name;
    int address;
};

class FileUtil {
public:
    explicit FileUtil(const string &outputFileName) {
        this->fileName = outputFileName;
        this->fout = ofstream(this->fileName);
        if (fout.bad() || fout.fail()) {
            cerr << "文件存储时出错" << endl;
        }
    }

    ~FileUtil() {
        if (fout.good()) {
            fout.close();
        }
    }

    static bool readFile(const string &fileName, string &str);

    void write(const string &str);

    void write(char c);

private:
    string fileName;
    ofstream fout;
};

class ScanUtil {
public:
    static bool charInList(char c, const vector<char> &list);

    static bool strInList(const string &str, const vector<string> &list);

    static bool charIsFirst(char c, const vector<string> &list);

    static bool isDigit(char c);

    static bool isLetter(char c);

    static bool isDigitOrLetter(char c);

    static unsigned long long getLineCount(unsigned long long index, const string &str);
};

struct word {
    string key;
    string value;
};


class node {
public:
    int childsNum = 0;
    string name = "";
    string value = "";
    unsigned long long line = 0;
    node* childs[MAX_CHILD];

    node() { ; }

    node(const string &name, const string &value, const unsigned long long line) {
        node();
        this->name = name;
        this->value = value;
        this->line = line;
    }

    void del() {
        delNode(this);
    }

    static void delNode(node *node);

    int addChild(node *child);

    static void draw(node *root, const string &fileName);

    static void draw(node *root, FileUtil &fout);

    static void draw(node *root, int level, vector<bool> &flags, FileUtil &fout);
};

#endif //TESTLANGUAGE_UTIL_H
