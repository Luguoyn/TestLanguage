#include "TestParse.h"

static unsigned long long line = 1;

bool TestParse::parse(const string &str, const string &parseFileName, node *root) {

    FileUtil fout(parseFileName);
    root->name = "<program>";

    string temp = string(str);
    istringstream sout(temp);
    vector<word> wordList;
    while (!sout.eof()) {
        word tempWord;
        if (sout >> tempWord.key) {
            sout >> tempWord.value;

            wordList.push_back(tempWord);
        }
    }

    bool flag = program(wordList, 0, root) > 0;

    node::draw(root, fout);

    if (flag <= 0) {
        cerr << "Parsing failed in line " << line << "." << endl;
        fout.write(string("Parsing failed in line ").append(to_string(line).append(".\n")));
    } else {
        cout << "finish!" << endl;
    }

    return flag > 0;
}

//<program> -> { <declaration_list><statement_list> }
long long program(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "{") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        now = new node("<declaration_list>", "", line);
        index = declarationList(wordList, index, now);
        if (index > 0) {
            father->addChild(now);

            now = new node("<statement_list>", "", line);
            index = statementList(wordList, index, now);
            if (index > 0) {
                father->addChild(now);

                if (wordList[index].key == "}") {
                    now = new node(wordList[index].key, wordList[index].value, line);
                    father->addChild(now);
                    index++;

//                    cout << "<program> -> { <declaration_list><statement_list> }" << endl;
                    return index;
                }
            }

        }
        node::delNode(now);
        return -1;
    }
}

//<declaration_list> -> <declaration_list_2>
long long declarationList(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    now = new node("<declaration_list_2>", "", line);
    index = declarationList2(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<declaration_list> -> <declaration_list_2>" << endl;
        return index;
    }

    node::delNode(now);
    return -1;

}

//<declaration_list_2> -> <declaration_stat><declaration_list> | null
long long declarationList2(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    long long temp = index;
    node *now = nullptr;

    now = new node("<declaration_stat>", "", line);
    index = declarationStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

        now = new node("<declaration_list>", "", line);
        index = declarationList(wordList, index, now);
        if (index > 0) {
            father->addChild(now);

//            cout << "<declaration_list_2> -> <declaration_stat><declaration_list>" << endl;
            return index;
        } else {
            node::delNode(now);
            return -1;
        }


    } else {
        node::delNode(now);

        father->addChild(new node("null", "null", line));
//        cout << "<declaration_list_2> -> null" << endl;
        return temp;
    }
}

//<declaration_stat> -> int ID;
long long declarationStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "int") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        if (wordList[index].key == "ID") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            if (wordList[index].key == ";") {
                now = new node(wordList[index].key, wordList[index].value, line);
                father->addChild(now);
                index++;

//                cout << "<declaration_stat> -> int ID;" << endl;
                return index;
            }
        }
    }

    return -1;
}

//<statement_list> -> <statement_list_2>
long long statementList(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    now = new node("<statement_list_2>", "", line);
    index = statementList2(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement_list> -> <statement_list_2>" << endl;
        return index;
    }

    node::delNode(now);
    return -1;
}

//<statement_list_2> -> <statement><statement_list> | null
long long statementList2(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    long long temp = index;
    node *now = nullptr;

    now = new node("<statement>", "", line);
    index = statement(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

        now = new node("<statement_list>", "", line);
        index = statementList(wordList, index, now);
        if (index > 0) {
            father->addChild(now);

//            cout << "<statement_list_2> -> <statement><statement_list_2>" << endl;
            return index;
        } else {
            node::delNode(now);
            return -1;
        }

    } else {
        node::delNode(now);

        father->addChild(new node("null", "null", line));
//        cout << "<statement_list_2> -> null" << endl;
        return temp;
    }
}

//<statement> -> <if_stat> | <while_stat> | <for_stat> | <read_stat>
// | <write_stat> | <compound_stat> | <expression_stat>
long long statement(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    long long temp = index;
    now = new node("<if_stat>", "", line);
    index = ifStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement> -> <if_stat>" << endl;
        return index;
    }
    node::delNode(now);

    index = temp;
    now = new node("<while_stat>", "", line);
    index = whileStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement> -> <while_stat>" << endl;
        return index;
    }
    node::delNode(now);

    index = temp;
    now = new node("<for_stat>", "", line);
    index = forStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement> -> <for_stat>" << endl;
        return index;
    }
    node::delNode(now);

    index = temp;
    now = new node("<read_stat>", "", line);
    index = readStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement> -> <read_stat>" << endl;
        return index;
    }
    node::delNode(now);

    index = temp;
    now = new node("<write_stat>", "", line);
    index = writeStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement> -> <write_stat>" << endl;
        return index;
    }
    node::delNode(now);

    index = temp;
    now = new node("<compound_stat>", "", line);
    index = compoundStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement> -> <compound_stat>" << endl;
        return index;
    }
    node::delNode(now);

    index = temp;
    now = new node("<expression_stat>", "", line);
    index = expressionStat(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<statement> -> <expression_stat>" << endl;
        return index;
    }
    node::delNode(now);

    return -1;
}

//<if_stat> -> if(<expression>) <statement> [else <statement>]
long long ifStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "if") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        if (wordList[index].key == "(") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            now = new node("<expression>", "", line);
            index = expression(wordList, index, now);
            if (index > 0) {
                father->addChild(now);

                if (wordList[index].key == ")") {
                    now = new node(wordList[index].key, wordList[index].value, line);
                    father->addChild(now);
                    index++;

                    now = new node("<statement>", "", line);
                    index = statement(wordList, index, now);
                    if (index > 0) {
                        father->addChild(now);

                        if (wordList[index].key == "else") {
                            now = new node(wordList[index].key, wordList[index].value, line);
                            father->addChild(now);
                            index++;

                            now = new node("<statement>", "", line);
                            index = statement(wordList, index, now);
                            if (index > 0) {
                                father->addChild(now);

//                                cout << "<if_stat> -> if(<expression>) <statement> else <statement>" << endl;
                                return index;
                            }
                        } else {
//                            cout << "<if_stat> -> if(<expression>) <statement>" << endl;
                            return index;
                        }
                    }
                }
            }
        }
    }

    node::delNode(now);
    return -1;
}

//<while_stat> -> while(<expression>) <statement>
long long whileStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "while") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        if (wordList[index].key == "(") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            now = new node("<expression>", "", line);
            index = expression(wordList, index, now);
            if (index > 0) {
                father->addChild(now);

                if (wordList[index].key == ")") {
                    now = new node(wordList[index].key, wordList[index].value, line);
                    father->addChild(now);
                    index++;

                    now = new node("<statement>", "", line);
                    index = statement(wordList, index, now);
                    if (index > 0) {
                        father->addChild(now);

//                        cout << "<while_stat> -> while(<expression>) <statement>" << endl;
                        return index;
                    }
                }
            }
        }
    }

    node::delNode(now);
    return -1;
}

//<for_stat> -> for(<expression>;<expression>;<expression>) <statement>
long long forStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "for") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        if (wordList[index].key == "(") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            now = new node("<expression>", "", line);
            index = expression(wordList, index, now);
            if (index > 0) {
                father->addChild(now);

                if (wordList[index].key == ";", line) {
                    now = new node(wordList[index].key, wordList[index].value, line);
                    father->addChild(now);
                    index++;

                    now = new node("<expression>", "", line);
                    index = expression(wordList, index, now);
                    if (index > 0) {
                        father->addChild(now);

                        if (wordList[index].key == ";") {
                            now = new node(wordList[index].key, wordList[index].value, line);
                            father->addChild(now);
                            index++;

                            now = new node("<expression>", "", line);
                            index = expression(wordList, index, now);
                            if (index > 0) {
                                father->addChild(now);

                                if (wordList[index].key == ")") {
                                    now = new node(wordList[index].key, wordList[index].value, line);
                                    father->addChild(now);
                                    index++;

                                    now = new node("<statement>", "", line);
                                    index = statement(wordList, index, now);
                                    if (index > 0) {
                                        father->addChild(now);
//                                        cout << "<for_stat> -> for(<expression>;<expression>;<expression>) <statement>" << endl;
                                        return index;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    node::delNode(now);
    return -1;
}

//<write_stat> -> write <expression>;
long long writeStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "write") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        now = new node("<expression>", "", line);
        index = expression(wordList, index, now);
        if (index > 0) {
            father->addChild(now);

            if (wordList[index].key == ";") {
                now = new node(wordList[index].key, wordList[index].value, line);
                father->addChild(now);
                index++;

//                cout << "<write_stat> -> write <expression>;" << endl;
                return index;
            }

        }
    }

    node::delNode(now);
    return -1;
}

//<read_stat> -> read ID;
long long readStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "read") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        if (wordList[index].key == "ID") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            if (wordList[index].key == ";") {
                now = new node(wordList[index].key, wordList[index].value, line);
                father->addChild(now);
                index++;

//                cout << "<read_stat> -> read <expression>;" << endl;
                return index;
            }
        }
    }

    return -1;
}

//<compound_stat> -> {<statement_list>}
long long compoundStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "{") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        now = new node("<statement_list>", "", line);
        index = statementList(wordList, index, now);
        if (index > 0) {
            father->addChild(now);

            if (wordList[index].key == "}") {
                now = new node(wordList[index].key, wordList[index].value, line);
                father->addChild(now);
                index++;

//                cout << "<compound_stat> -> {<statement_list>}" << endl;
                return index;
            }
        }
    }

    node::delNode(now);
    return -1;
}

//<expression_stat> -> <expression>; | ;
long long expressionStat(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == ";") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

//        cout << "<expression_stat> -> ;" << endl;
        return index;
    }

    now = new node("<expression>", "", line);
    index = expression(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

        if (wordList[index].key == ";") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

//            cout << "<expression_stat> -> <expression>;" << endl;
            return index;
        }
    }

    node::delNode(now);
    return -1;
}

//<expression> -> ID=<bool_expr> | <bool_expr>
long long expression(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    long long temp = index;
    node *now = nullptr;

    if (wordList[index].key == "ID" && wordList[index+1].key == "=") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        if (wordList[index].key == "=") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            now = new node("<bool_expr>", "", line);
            index = boolExpr(wordList, index, now);
            if (index > 0) {
                father->addChild(now);

//                cout << "<expression> -> ID=<bool_expr>" << endl;
                return index;
            }
        }
    }

    index = temp;
    now = new node("<bool_expr>", "", line);
    index = boolExpr(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<expression> -> <bool_expr>" << endl;
        return index;
    }

    node::delNode(now);
    return -1;
}

//<bool_expr> -> <additive_expr>
//| <additive_expr>(>|<|>=|<=|==|!=)<additive_expr>
long long boolExpr(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    vector<string> docs = {">", "<", ">=", "<=", "==", "!="};
    node *now = nullptr;

    if (ScanUtil::strInList(wordList[index + 1].key, docs)) {

        now = new node("<additive_expr>", "", line);
        index = additiveExpr(wordList, index, now);
        if (index > 0) {
            father->addChild(now);

            if (ScanUtil::strInList(wordList[index].key, docs)) {
                //string temp = wordList[index].key;
                now = new node(wordList[index].key, wordList[index].value, line);
                father->addChild(now);
                index++;

                now = new node("<additive_expr>", "", line);
                index = additiveExpr(wordList, index, now);
                if (index > 0) {
                    father->addChild(now);

//                    cout << "<additive_expr>" << temp << "<additive_expr>" << endl;
                    return index;
                }
            }
        }
    }

    now = new node("<additive_expr>", "", line);
    index = additiveExpr(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

//        cout << "<bool_expr> -> <additive_expr>" << endl;
        return index;
    }

    node::delNode(now);
    return -1;
}

//<additive_expr> -> <term>{(+|-)<term>}
long long additiveExpr(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    now = new node("<term>", "", line);

    index = term(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

        if (wordList[index].key == "+" || wordList[index].key == "-") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            now = new node("<term>", "", line);
            index = term(wordList, index, now);
            if (index > 0) {
                father->addChild(now);

//                cout << "<additive_expr> -> <term>(+|-)<term>" << endl;
                return index;
            }
        }

//        cout << "<additive_expr> -> <term>" << endl;
        return index;
    }

    node::delNode(now);
    return -1;
}

//<term> -> <factor>{(*|/)<factor>}
long long term(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    now = new node("<factor>", "", line);
    index = factor(wordList, index, now);
    if (index > 0) {
        father->addChild(now);

        if (wordList[index].key == "*" || wordList[index].key == "/") {
            now = new node(wordList[index].key, wordList[index].value, line);
            father->addChild(now);
            index++;

            now = new node("<factor>", "", line);
            index = factor(wordList, index, now);
            if (index > 0) {
                father->addChild(now);

//                cout << "<term> -> <factor>(*|/)<factor>" << endl;
                return index;
            }
        }

//        cout << "<term> -> <factor>" << endl;
        return index;
    }

    node::delNode(now);
    return -1;
}

//<factor> -> (<expression>) | ID | NUM
long long factor(const vector<word> &wordList, long long index, node *father) {
    while (wordList[index].key == "LINE") {
        istringstream(wordList[index].value) >> line;
        index++;
    }

    node *now = nullptr;

    if (wordList[index].key == "(") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

        now = new node("<expression>", "", line);
        index = expression(wordList, index, now);
        if (index > 0) {
            father->addChild(now);

            if (wordList[index].key == ")") {
                now = new node(wordList[index].key, wordList[index].value, line);
                father->addChild(now);
                index++;

//                cout << "<factor> -> (<expression>)" << endl;
                return index;
            }
        }
    }

    if (wordList[index].key == "ID") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

//        cout << "<factor> -> ID" << endl;
        return index;
    }

    if (wordList[index].key == "NUM") {
        now = new node(wordList[index].key, wordList[index].value, line);
        father->addChild(now);
        index++;

//        cout << "<factor> -> NUM" << endl;
        return index;
    }

    node::delNode(now);
    return -1;
}
