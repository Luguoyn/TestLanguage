#include "Util.h"

bool FileUtil::readFile(const string &fileName, string &str) {
    ifstream fin(fileName);
    if (fin.bad() || fin.fail()) {
        cout << "打开文件失败" << endl;
        return false;
    }

    str.clear();

    string temp;
    while (getline(fin, temp)) {
        str += temp;
        str += "\n";
    }

    fin.close();
    return true;
}

void FileUtil::write(const string &str) {
    this->fout << str;
}

void FileUtil::write(char c) {
    this->fout << c;
}

bool ScanUtil::charInList(char c, const vector<char> &list) {
    return any_of(list.begin(), list.end(), [c](char item) { return c == item; });
}

bool ScanUtil::strInList(const string &str, const vector<string> &list) {
    return any_of(list.begin(), list.end(), [str](string item) { return str == item; });
}

bool ScanUtil::charIsFirst(char c, const vector<string> &list) {
    return any_of(list.begin(), list.end(), [c](string item) { return item[0] == c; });
}

bool ScanUtil::isDigit(char c) {
    return '0' <= c && '9' >= c;
}

bool ScanUtil::isLetter(char c) {
    return ('a' <= c && 'z' >= c) || ('A' <= c && 'Z' >= c);
}

bool ScanUtil::isDigitOrLetter(char c) {
    return isDigit(c) || isLetter(c);
}

unsigned long long ScanUtil::getLineCount(unsigned long long index, const string &str) {
    unsigned long long count = 1;
    for (int i = 0; i < index; ++i) {
        if (str[i] == '\n') {
            count++;
        }
    }
    return count;
}

void node::delNode(node *node) {
    if (node == nullptr) {
        return;
    }
    if (node->childsNum > 0 && node->childsNum <= 10) {
        for (int i = 0; i < node->childsNum; i++) {
            delNode(node->childs[i]);
        }
    }

    delete node;
}

int node::addChild(node *child) {
    if (childsNum + 1 >= MAX_CHILD) {
        return -1;
    }

    this->childs[childsNum] = child;
    childsNum++;

    return childsNum;
}

void node::draw(node *root, int level, vector<bool> &flags, FileUtil &fout) {
    while (level >= flags.size()) {
        flags.push_back(true);
    }

    if (level == 0) {
        cout << root->name << endl;
        fout.write(root->name + "\n");
    }

    flags[level] = true;

    for (int i = 0; i < root->childsNum; i++) {
        int flag = 0;
        for (int j = 0; j <= level; j++) {
            if (flags[j]) {
                flag = j;
            }
        }
        for (int j = 0; j <= level; j++) {

            if (flags[j]) {
                if (flag <= j) {
                    if (i == root->childsNum - 1) {
                        cout << "  |__";
                        fout.write("  |__");
                        flags[level] = false;
                    } else {
                        cout << "  |--";
                        fout.write("  |--");
                    }
                } else {
                    cout << "  |  ";
                    fout.write("  |  ");
                }
            } else {
                if (flag <= j) {
                    if (i == root->childsNum - 1) {
                        cout << "_____";
                        fout.write("_____");
                        flags[level] = false;
                    } else {
                        cout << "-----";
                        fout.write("-----");
                    }
                } else {
                    cout << "     ";
                    fout.write("     ");
                }
            }
        }

        if (!root->childs[i]->value.empty()) {
            cout << root->childs[i]->name << " -> " << root->childs[i]->value << endl;
            fout.write(root->childs[i]->name + " -> " + root->childs[i]->value + "\n");
        } else {
            cout << root->childs[i]->name << endl;
            fout.write(root->childs[i]->name + "\n");
        }

        if (root->childs[i]->childsNum > 0) {
            draw(root->childs[i], level + 1, flags, fout);
        }

    }
}

void node::draw(node *root, const string &fileName) {
    vector<bool> flags = {true, true, true};
    FileUtil fout(fileName);
    draw(root, 0, flags, fout);
}

void node::draw(node *root, FileUtil &fout) {
    vector<bool> flags = {true, true, true};
    draw(root, 0, flags, fout);
}
