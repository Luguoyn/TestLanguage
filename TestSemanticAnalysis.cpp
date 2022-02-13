#include "TestSemanticAnalysis.h"

static vector<testID> ID_list;
static int idCount = 0;
static int labelCount = 0;

//查询地址
int getAddress(const string &name) {
    for (const auto &item : ID_list) {
        if (item.name == name) {
            return item.address;
        }
    }
    return -1;
}


//往符号表添加
bool addID(const string &name) {
    if (getAddress(name) != -1) {
        return false;
    }

    testID id;
    id.name = name;
    id.address = idCount++;
    ID_list.push_back(id);
    return true;
}

//获得一个标签
string getLabel() {
    return string("LABEL").append(to_string(labelCount++));
}

bool TestSemanticAnalysis::semanticAnalysis(node *root, const string &semanticAnalysisFileName) {
    FileUtil fout(semanticAnalysisFileName);
    bool flag = analysis(root, fout);
    if (flag) {
        cout << "finish!" << endl;
    }
    return flag;
}

//语义分析
bool TestSemanticAnalysis::analysis(node *n, FileUtil &fout) {


    //如果是终结符节点
    if (n->value != "") {
        return true;
    }


    //如果是program节点
    if (n->name == "<program>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        return true;
    }


    //如果是declaration_list节点
    if (n->name == "<declaration_list>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }

        return true;
    }


    //如果是declaration_list_2节点
    if (n->name == "<declaration_list_2>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        return true;
    }


    //如果是declaration_stat节点
    if (n->name == "<declaration_stat>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }

        if (addID(n->childs[1]->value)) {
            return true;
        } else {
            cout << "Semantic analysis failed in line "
                 << n->childs[1]->line
                 << " : duplicate variable name" << endl;
            fout.write(string("Semantic analysis failed in line ")
                               .append(to_string(n->childs[1]->line))
                               .append(" : duplicate variable name\n"));
            return false;
        }
    }


    //如果是statement_list节点
    if (n->name == "<statement_list>") {

        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        return true;
    }


    //如果是statement_list_2节点
    if (n->name == "<statement_list_2>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        return true;
    }


    //如果是statement节点
    if (n->name == "<statement>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        return true;
    }


    //如果是if_stat节点
    //<if_stat> -> if ( <expression> ) {BRF(label1);}
    // <statement> {BR(label2); SETLabel(label1);}
    // [else<statement>] {SETLabel(label2);}
    if (n->name == "<if_stat>") {
        string label1 = getLabel();
        string label2 = getLabel();

        //如果<expression>节点不通过，则报错
        if (!analysis(n->childs[2], fout)) {
            return false;
        }
        fout.write(string("BRF ")
                           .append(label1)
                           .append("\n"));

        //如果<statement>节点不通过，则报错
        if (!analysis(n->childs[4], fout)) {
            return false;
        }
        fout.write(string("BR ")
                           .append(label2)
                           .append("\n"));
        fout.write(string("")
                           .append(label1)
                           .append(":\n"));

        if (n->childsNum == 7) {
            //如果<statement>节点不通过，则报错
            if (!analysis(n->childs[6], fout)) {
                return false;
            }
            fout.write(string("")
                               .append(label2)
                               .append(":\n"));
        }

        return true;

    }


    //如果是while_stat节点
    //<while_stat> -> while{SETLabel(label1);}
    // (<expression>) {BRF(label2);}
    //<statement> {BR(label1); SETLabel(label2);}
    if (n->name == "<while_stat>") {

        string label1 = getLabel();
        string label2 = getLabel();

        fout.write(string("")
                           .append(label1)
                           .append(":\n"));

        //如果<expression>分析出错，则报错。
        if (!analysis(n->childs[4], fout)) {
            return false;
        }

        fout.write(string("BRF ")
                           .append(label2)
                           .append("\n"));

        //如果<statement>分析出错，则报错。
        if (!analysis(n->childs[2], fout)) {
            return false;
        }


        fout.write(string("BRF ")
                           .append(label2)
                           .append("\n"));

        fout.write(string("")
                           .append(label2)
                           .append(":\n"));

        return true;
    }


    //如果是for_stat节点
    //<for_stat>->for(<expression>{POP;} ; {SETLabel(label1);}
    // <expression> {BRF(label2);BR(label3);} ; {SETLabel(label4);}
    // <expression> {POP; BR(label1);} ) {SETLabel(label3);}
    // <statement> {BR(label4); SETLabel(label2);}
    if (n->name == "<for_stat>") {
        string label1 = getLabel();
        string label2 = getLabel();
        string label3 = getLabel();
        string label4 = getLabel();


        //如果<expression>分析失败，则报错。
        if (!analysis(n->childs[2], fout)) {
            return false;
        }
        //<for_stat>->for(<expression>{POP;} ; {SETLabel(label1);}
        fout.write("POP\n");
        fout.write(string("")
                           .append(label1)
                           .append(":\n"));


        //如果<expression>分析失败，则报错。
        if (!analysis(n->childs[4], fout)) {
            return false;
        }
        // <expression> {BRF(label2);BR(label3);} ; {SETLabel(label4);}
        fout.write(string("BRF ")
                           .append(label2)
                           .append("\n"));
        fout.write(string("BR ")
                           .append(label3)
                           .append("\n"));
        fout.write(string("")
                           .append(label4)
                           .append(":\n"));


        //如果<expression>分析失败，则报错。
        if (!analysis(n->childs[6], fout)) {
            return false;
        }
        // <expression> {POP; BR(label1);} ) {SETLabel(label3);}
        fout.write("POP\n");
        fout.write(string("BR ")
                           .append(label1)
                           .append("\n"));
        fout.write(string("")
                           .append(label3)
                           .append(":\n"));


        //如果<statement>分析失败，则报错。
        if (!analysis(n->childs[8], fout)) {
            return false;
        }
        // <statement> {BR(label4); SETLabel(label2);}
        fout.write(string("BR ")
                           .append(label4)
                           .append("\n"));
        fout.write(string("")
                           .append(label2)
                           .append(":\n"));

        return true;
    }


    //如果是write_stat节点
    //<write_stat> -> write <expression> {OUT;}
    if (n->name == "<write_stat>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        fout.write("OUT\n");
        return true;
    }


    //如果是read_stat节点
    //<read_stat> -> read ID {LOOKUP(n,d); IN; STO(d); POP}
    if (n->name == "<read_stat>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }

        //取出变量地址
        int address = getAddress(n->childs[1]->value);

        //如果地址不存在，则报错
        if (address < 0) {
            cout << "Semantic analysis failed in line "
                 << n->childs[1]->line
                 << " : variable "
                 << n->childs[1]->value
                 << " is undefined."
                 << endl;
            fout.write(string("Semantic analysis failed in line ")
                               .append(to_string(n->childs[1]->line))
                               .append(" : variable ")
                               .append(n->childs[1]->value)
                               .append(" is undefined.\n"));
            return false;
        }

        fout.write("IN\n");

        fout.write(string("STO ")
                           .append(to_string(address))
                           .append("\n"));

        fout.write("POP\n");

        return true;
    }


    //如果是compound_stat节点
    if (n->name == "<compound_stat>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        return true;
    }


    //如果是expression_stat节点
    if (n->name == "<expression_stat>") {
        //如果是<expression_stat> -> <expression>;
        //则<expression_stat> -> <expression> {POP;}
        if (n->childs[0]->name == "<expression>") {
            if (!analysis(n->childs[0], fout)) {
                return false;
            }
            fout.write("POP\n");
            return true;
        }

        //否则
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }
        return true;
    }


    //如果是expression节点
    if (n->name == "<expression>") {

        //如果是<expression> -> <bool_expr>
        if (n->childsNum == 1) {
            if (!analysis(n->childs[0], fout)) {
                return false;
            }
            return true;
        }

        //如果是<expression> -> ID=<bool_expr>
        //则<expression> -> ID {LOOKUP(n,d);} = <bool_expr> {STO d}
        if (n->childsNum == 3) {
            //取出变量地址
            int address = getAddress(n->childs[0]->value);

            //如果地址不存在，则报错
            if (address < 0) {
                cout << "Semantic analysis failed in line "
                     << n->childs[0]->line
                     << " : variable "
                     << n->childs[0]->value
                     << " is undefined."
                     << endl;

                fout.write(string("Semantic analysis failed in line ")
                                   .append(to_string(n->childs[0]->line))
                                   .append(" : variable ")
                                   .append(n->childs[0]->value)
                                   .append(" is undefined.\n"));
                return false;
            }

            if (!analysis(n->childs[2], fout)) {
                return false;
            }

            fout.write(string("STO ")
                               .append(to_string(address))
                               .append("\n"));
            return true;
        }

        cerr << "Unknown Error in line "
             << n->line
             << "."
             << endl;
        fout.write(string("Unknown Error in line ")
                           .append(to_string(n->line))
                           .append(".\n"));
        return false;
    }


    //如果是bool_expr节点
    if (n->name == "<bool_expr>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }

        //如果是<bool_expr> -> <additive_expr>(>|<|>=|<=|==|!=)<additive_expr>
//        <bool_expr> -> <additive_expr>
//        | <additive_expr> > <additive_expr> {GT}
//        | <additive_expr> < <additive_expr> {LES}
//        | <additive_expr> >= <additive_expr> {GE}
//        | <additive_expr> <= <additive_expr> {LE}
//        | <additive_expr> == <additive_expr> {EQ}
//        | <additive_expr> != <additive_expr> {NOTEQ}
        if (n->childsNum > 1) {
            string op = n->childs[1]->name;
            if (op == ">") {
                fout.write("GT\n");
            } else if (op == "<") {
                fout.write("LES\n");
            } else if (op == ">=") {
                fout.write("GE\n");
            } else if (op == "<=") {
                fout.write("LE\n");
            } else if (op == "==") {
                fout.write("EQ\n");
            } else if (op == "!=") {
                fout.write("NOTEQ\n");
            } else {
                cerr << "Unknown Error in line "
                     << n->childs[1]->line
                     << "."
                     << endl;
                fout.write(string("Unknown Error in line ")
                                   .append(to_string(n->childs[1]->line))
                                   .append(".\n"));
                return false;
            }
        }

        return true;
    }


    //如果是additive_expr节点
    if (n->name == "<additive_expr>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }

        if (n->childsNum > 1) {
            string op = n->childs[1]->name;
            if (op == "+") {
                fout.write("ADD\n");
            } else if (op == "-") {
                fout.write("SUB\n");
            } else {
                cerr << "Unknown Error in line "
                     << n->childs[1]->line
                     << "."
                     << endl;
                fout.write(string("Unknown Error in line ")
                                   .append(to_string(n->childs[1]->line))
                                   .append(".\n"));
                return false;
            }
        }
        return true;
    }


    //如果是term节点
    if (n->name == "<term>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }

        if (n->childsNum > 1) {
            string op = n->childs[1]->name;
            if (op == "*") {
                fout.write("MULT\n");
            } else if (op == "/") {
                fout.write("DIV\n");
            } else {
                cerr << "Unknown Error in line "
                     << n->childs[1]->line
                     << "."
                     << endl;
                fout.write(string("Unknown Error in line ")
                                   .append(to_string(n->childs[1]->line))
                                   .append(".\n"));
                return false;
            }
        }
        return true;
    }


    //如果是factor节点
    if (n->name == "<factor>") {
        for (int i = 0; i < n->childsNum; i++) {
            if (!analysis(n->childs[i], fout)) {
                return false;
            }
        }

        //<factor> -> (<expression >)
        // | ID {LOOKUP(n,d); LOAD d;}
        // | NUM {LOADI I;}

        string key = n->childs[0]->name;

        if (key == "ID") {
            //取出变量地址
            int address = getAddress(n->childs[0]->value);

            //如果地址不存在，则报错
            if (address < 0) {
                cout << "Semantic analysis failed in line "
                     << n->childs[0]->line
                     << " : variable "
                     << n->childs[0]->value
                     << " is undefined."
                     << endl;
                fout.write(string("Semantic analysis failed in line ")
                                   .append(to_string(n->childs[0]->line))
                                   .append(" : variable ")
                                   .append(n->childs[0]->value)
                                   .append(" is undefined.\n"));
                return false;
            }

            fout.write(string("LOAD ")
                               .append(to_string(address))
                               .append("\n"));
        } else if (key == "NUM") {
            fout.write(string("LOADI ")
                               .append(n->childs[0]->value)
                               .append("\n"));
        } else {
            cout << "Unknown Error in line "
                 << n->childs[1]->line
                 << "."
                 << endl;
            fout.write(string("Unknown Error in line ")
                               .append(to_string(n->childs[1]->line))
                               .append(".\n"));
            return false;
        }

        return true;
    }

    cerr << "Unknown Error in line "
         << n->childs[1]->line
         << "."
         << endl;
    fout.write(string("Unknown Error in line ")
                       .append(to_string(n->childs[1]->line))
                       .append(".\n"));
    return false;
}
