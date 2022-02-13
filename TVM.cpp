#include "TVM.h"

int TVM::run(const string &fileName) {
    ifstream fin(fileName);
    if (fin.fail() || fin.bad()) {
        cerr << "FILE NOT FOUND!" << endl;
        exit(0);
    }
    while (!fin.eof()) {
        string str;
        getline(fin, str);
        if (!str.empty()) {
            code.push_back(str);
            if (str.find("LABEL") == 0) {
                labelMap.insert(pair<string, int>(str.substr(0, str.find(':')), code.size() - 1));
            }
        }
    }

    for (int i = 0; i < code.size(); i++) {
        i = run(i);
        if (i == -1) {
            cerr << "Run time Error, Enter any key to exit." << endl;
            cin.get();
            exit(1);
        }
    }

    cout << "End of program operation, enter any key to exit." << endl;
    cin.get();
    return 0;
}

int TVM::run(int i) {
    string code = this->code[i];
    istringstream sin(code);

    string command;
    sin >> command;


    if (command == "LOAD") {
        int d;
        sin >> d;
        load(d);
        return i;
    }


    if (command == "LOADI") {
        int d;
        sin >> d;
        put(d);
        return i;
    }


    if (command == "STO") {
        int d;
        sin >> d;
        save(d);
        return i;
    }


    if (command == "POP") {
        pop();
        return i;
    }


    if (command == "ADD") {
        int b = pop();
        int a = pop();
        put(a + b);
        return i;
    }


    if (command == "SUB") {
        int b = pop();
        int a = pop();
        put(a - b);
        return i;
    }


    if (command == "MULT") {
        int b = pop();
        int a = pop();
        put(a * b);
        return i;
    }


    if (command == "DIV") {
        int b = pop();
        int a = pop();
        put(a + b);
        return i;
    }


    if (command == "BR") {
        string label;
        sin >> label;
        i = labelMap[label];
        return i;
    }


    if (command == "BRF") {
        int flag = pop();

        if (!flag) {
            string label;
            sin >> label;
            i = labelMap[label];
        }
        return i;
    }


    if (command == "EQ") {
        int b = pop();
        int a = pop();
        int flag = (a == b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "NOTEQ") {
        int b = pop();
        int a = pop();
        int flag = (a != b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "GT") {
        int b = pop();
        int a = pop();
        int flag = (a > b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "LES") {
        int b = pop();
        int a = pop();
        int flag = (a < b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "GE") {
        int b = pop();
        int a = pop();
        int flag = (a >= b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "LE") {
        int b = pop();
        int a = pop();
        int flag = (a <= b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "AND") {
        int b = pop();
        int a = pop();
        int flag = (a && b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "OR") {
        int b = pop();
        int a = pop();
        int flag = (a || b) ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "NOT") {
        int a = pop();
        int flag = !a ? 1 : 0;
        put(flag);
        return i;
    }


    if (command == "IN") {
        cout << "INPUT:";
        int a;
        cin >> a;
        put(a);
        return i;
    }


    if (command == "OUT") {
        cout << "OUTPUT:" << pop() << endl;
        return i;
    }


    if (command == "STOP") {
        return -1;
    }


    return i;
}