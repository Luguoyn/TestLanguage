#include "Data.h"

const vector<string> Data::keyword = {
        "if", "else",
        "for", "while", "do",
        "int",
        "read", "write"
};

const vector<char> Data::singleWord = {
        '+', '-', '*', '/', '=',
        '(', ')', '{', '}',
        ';', ',', ':',
        '>', '<', '!'
};

const vector<string> Data::doubleWord = {
        ">=", "<=", "!=", "=="
};

const string Data::commentFirst = "/*";

const string Data::commentLast = "*/";

const vector<char> Data::emptyChar = {
        ' ', '\n', '\t'
};