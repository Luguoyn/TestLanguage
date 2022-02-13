#ifndef TESTLANGUAGE_TESTPARSE_H
#define TESTLANGUAGE_TESTPARSE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "Util.h"

using namespace std;

class TestParse {
public:
    static bool parse(const string &str, const string &parseFileName, node *root);

private:
    TestParse() { ; }
};

//<program> -> { <declaration_list><statement_list> }
long long program(const vector<word> &wordList, long long index, node *node);

//<declaration_list> -> <declaration_list_2>
long long declarationList(const vector<word> &wordList, long long index, node *node);

//<declaration_list_2> -> <declaration_stat><declaration_list> | null
long long declarationList2(const vector<word> &wordList, long long index, node *node);

//<declaration_stat> -> int ID;
long long declarationStat(const vector<word> &wordList, long long index, node *node);

//<statement_list> -> <statement_list_2>
long long statementList(const vector<word> &wordList, long long index, node *node);

//<statement_list_2> -> <statement><statement_list> | null
long long statementList2(const vector<word> &wordList, long long index, node *node);

//<statement> -> <if_stat> | <while_stat> | <for_stat> | <read_stat>
// | <write_stat> | <compound_stat> | <expression_stat>
long long statement(const vector<word> &wordList, long long index, node *node);

//<if_stat> -> if(<expression>) <statement> [else <statement>]
long long ifStat(const vector<word> &wordList, long long index, node *node);

//<while_stat> -> while(<expression>) <statement>
long long whileStat(const vector<word> &wordList, long long index, node *node);

//<for_stat> -> for(<expression>;<expression>;<expression>) <statement>
long long forStat(const vector<word> &wordList, long long index, node *node);

//<write_stat> -> write <expression>;
long long writeStat(const vector<word> &wordList, long long index, node *node);

//<read_stat> -> read ID;
long long readStat(const vector<word> &wordList, long long index, node *node);

//<compound_stat> -> {<statement_list>}
long long compoundStat(const vector<word> &wordList, long long index, node *node);

//<expression_stat> -> <expression>; | ;
long long expressionStat(const vector<word> &wordList, long long index, node *node);

//<expression> -> ID=<bool_expr> | <bool_expr>
long long expression(const vector<word> &wordList, long long index, node *node);

//<bool_expr> -> <additive_expr>
//| <additive_expr>(>|<|>=|<=|==|!=)<additive_expr>
long long boolExpr(const vector<word> &wordList, long long index, node *node);

//<additive_expr> -> <term>{(+|-)<term>}
long long additiveExpr(const vector<word> &wordList, long long index, node *node);

//<term> -> <factor>{(*|/)<factor>}
long long term(const vector<word> &wordList, long long index, node *node);

//<factor> -> (<expression>) | ID | NUM
long long factor(const vector<word> &wordList, long long index, node *node);

#endif //TESTLANGUAGE_TESTPARSE_H
