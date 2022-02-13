#include "TestScan.h"

bool TestScan::scan(const string &str, const string &scanFileName) {
    FileUtil file(scanFileName);
    const unsigned long long length = str.length();
    unsigned long long now = 0;

    unsigned long long line = 1;

    char tempChar;
    string tempStr;

    file.write(string("LINE\t").append(to_string(line)).append("\n"));

    while (now <= length) {
        tempChar = str[now];

        //读出空值
        if (ScanUtil::charInList(tempChar, Data::emptyChar)) {
            now++;
            if(tempChar == '\n'){
                line++;
                file.write(string("LINE\t").append(to_string(line)).append("\n"));
            }
            continue;
        }

        //检测是否为注释
        if (tempChar == Data::commentFirst[0]) {
            //如果是注释头，则继续读取
            tempStr = str.substr(now, Data::commentFirst.length());

            if (tempStr == Data::commentFirst) {
                now += Data::commentFirst.length();

                //读注释循环
                while (now <= length) {

                    //如果遇到注释尾，则跳出循环
                    if (str[now] == Data::commentLast[0]) {

                        unsigned long long temp = now + Data::commentLast.length();

                        if (temp > length) {
                            now = temp;
                            break;
                        }

                        tempStr = str.substr(now, Data::commentLast.length());
                        if (tempStr == Data::commentLast) {
                            now += Data::commentLast.length();
                            break;
                        }
                    }

                    now++;
                }

                //如果循环结束还没有遇到注释尾，则报错
                if (now > length) {
                    cerr << "Lexical analysis error, possibly illegal annotation。" << endl;
                    file.write("Lexical analysis error, possibly illegal annotation。\n");
                    return false;
                }

                continue;
            }
        }

        //检测是否为双字符
        if (ScanUtil::charIsFirst(tempChar, Data::doubleWord)) {
            if (now + 2 <= length) {
                tempStr = str.substr(now, 2);
                if (ScanUtil::strInList(tempStr, Data::doubleWord)) {

                    cout << tempStr << "\t" << tempStr << endl;
                    file.write(tempStr.append("\t").append(tempStr).append("\n"));

                    now += 2;
                    continue;
                }
            }
        }

        //检测是否为单字符
        if (ScanUtil::charInList(tempChar, Data::singleWord)) {

            cout << tempChar << "\t" << tempChar << endl;
            file.write(string(1, tempChar).append("\t").append(string(1, tempChar)).append("\n"));

            now++;
            continue;
        }

        //检测是否为数字
        if (ScanUtil::isDigit(tempChar)) {
            tempStr = "";
            while (now <= length) {
                if (ScanUtil::isDigit(str[now])) {
                    tempStr += str[now];
                    now++;
                } else {
                    //如果下一个字符是字母，则报错
                    if (ScanUtil::isLetter(str[now])) {
                        cerr << "Lexical analysis error, illegal identifier in line "
                             << ScanUtil::getLineCount(now, str) << ": " << tempStr << endl;
                        file.write("Lexical analysis error, illegal identifier in line " +
                                   to_string(ScanUtil::getLineCount(now, str)) + ": " + tempStr + "\n");
                        return false;
                    }

                    //否则跳出循环
                    break;
                }
            }
            cout << "NUM" << "\t" << tempStr << endl;
            file.write(string("NUM").append("\t").append(tempStr).append("\n"));
            continue;
        }

        //检测是否为字母
        if (ScanUtil::isLetter(tempChar)) {
            //读取标识符
            tempStr = "";
            while (now <= length) {
                if (ScanUtil::isDigitOrLetter(str[now])) {
                    tempStr += str[now];
                    now++;
                } else {
                    break;
                }
            }

            //判断标识符是否是保留字
            if (ScanUtil::strInList(tempStr, Data::keyword)) {
                cout << tempStr << "\t" << tempStr << endl;
                file.write(tempStr.append("\t").append(tempStr).append("\n"));
            } else {
                cout << "ID" << "\t" << tempStr << endl;
                file.write(string("ID").append("\t").append(tempStr).append("\n"));
            }

            continue;
        }

        //报错
        if (tempChar != '\0') {
            cerr << "Lexical analysis failed, illegal symbol in line " << ScanUtil::getLineCount(now, str) << ": "
                 << tempChar << endl;
            file.write(
                    "Lexical analysis failed, illegal symbol in line " + to_string(ScanUtil::getLineCount(now, str)) +
                    ": " + string(1, tempChar) + "\n");
            return false;
        }
        now++;
    }

    cout << "finish!" << endl;

    return true;
}
