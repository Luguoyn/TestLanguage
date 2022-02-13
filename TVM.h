#ifndef TESTVM_TVM_H
#define TESTVM_TVM_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

class TVM {
private:
    vector<int> stack;
    vector<int> data;
    vector<string> code;
    map<string, int> labelMap;

    void put(int i) {
        stack.push_back(i);
    }

    void load(int d) {
        put(data[d]);
    }

    void save(int d) {
        if (data.size() <= d) {
            data.resize(d + 1);
        }
        data[d] = stack[stack.size() - 1];
    }

    int pop() {
        int i = stack[stack.size() - 1];
        stack.pop_back();
        return i;
    }

    int run(int i);

public:
    int run(const string &fileName);
};


#endif //TESTVM_TVM_H
