//
//  main.cpp
//  ctest
//
//  Created by 李晓辉 on 3/4/15.
//  Copyright (c) 2015 李晓辉 <lixiaohui0812@gmail.com> or <727615480@qq.com>. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

static const string ENCODE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int FILTER = 0b111111;
static const string VERSION = "Version 0.0.1\n";
static const string HELP = "\
This is a base64 tool.                                              \n\
Commands:                                                           \n\
    -h, --help     display this message                             \n\
    -v, --version  display version info                             \n\
    -d, --decode   decodes input                                    \n\
    -f, --file     encode the given file use base64                 \n\
    -s, --string   encode ths string                                \n\
";

string _encode_file(string);
string _encode_string(string);
string _decode(string);
string _encode(string);
string _getFileContent(string);
string getFillString(string);
int _parse(string);

int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc == 1) {
        cout << HELP << endl;
        return 0;
    }
    switch (_parse(argv[1])) {
        case 1:
            cout << HELP;
            return 1;
        case 2:
            cout << VERSION;
            return 2;
        case 3:
            cout << _decode(argv[1]);
            return 3;
        case 4:
            cout << _encode_file(argv[2]) << endl;
            return 4;
        case 5:
            cout << _encode_string(argv[2]) << endl;
            return 5;
        case -1:
            cout << "\n   \033[31merr\033[m commands. Please check it. \n\n";
            cout << HELP << endl;
            return -1;
        default:
            cout << HELP;
            return 0;
    }
    return 0;
}

string _encode_file(string file) {
    string msg = _getFileContent(file);
    return _encode_string(msg);
}

string _encode_string(string str) {
    string res, tmp;
    int block = (int)str.length()/3;
    int rest = str.length()%3;
    for (int i = 0; i<block; i++) {
        tmp.push_back(str[3 * i]);
        tmp.push_back(str[3 * i + 1]);
        tmp.push_back(str[3 * i + 2]);
        str.substr(i * 3, i*3+2);
        res += _encode(tmp);
        tmp.clear();
    }
    if (rest != 0) {
        for (int j = 0; j < rest; j++) {
            tmp.push_back(str[3 * block + j]);
        }
        res += _encode(tmp);
    }
    return res;
}

string _decode(string file) {
    string msg = _getFileContent(file);
    return "";
}

string _encode(string msg) {
    long _filter;
    if (msg.length() == 2) {
        _filter = toascii(msg[0]) << 16 | toascii(msg[1]) << 8;
    }else if (msg.length() == 1) {
        _filter = toascii(msg[0]) << 16;
    }else{
        _filter = toascii(msg[0]) << 16 | toascii(msg[1]) << 8 | toascii(msg[2]);
    }

    int _first = (int)(_filter >> 18 & FILTER);
    int _second = (int)(_filter >> 12 & FILTER);
    int _third = (int)(_filter >> 6 & FILTER);
    int _fourth = (int)(_filter & FILTER);
    string _res;
    if (msg.length()==3) {
        _res.push_back(ENCODE[_first--]);
        _res.push_back(ENCODE[_second--]);
        _res.push_back(ENCODE[_third--]);
        _res.push_back(ENCODE[_fourth--]);
    }else if (msg.length()==2) {
        _res.push_back(ENCODE[_first--]);
        _res.push_back(ENCODE[_second--]);
        _res.push_back(ENCODE[_third--]);
        _res.push_back('=');
    }else {
        _res.push_back(ENCODE[_first--]);
        _res.push_back(ENCODE[_second--]);
        _res.push_back('=');
        _res.push_back('=');
    }
    return _res;
}

int _parse(string tag) {
    if ((tag == "-h"||tag == "--help")) {
        return 1;
    }else if ((tag == "-v")||(tag == "--version")) {
        return 2;
    }else if ((tag == "-d")||(tag == "--decode")) {
        return 3;
    }else if ((tag == "-f")||(tag == "--file")) {
        return 4;
    }else if ((tag == "-s")||(tag == "--string")) {
        return 5;
    }else{
        return -1;
    }
}

string _getFileContent(string file){
    streampos size;
    char * content;
    fstream _encode_file(file, ios::in | ios::binary | ios::ate);
    if (_encode_file.is_open()) {
        size = _encode_file.tellg();
        content = new char [size];
        _encode_file.seekg(0, ios::beg);
        _encode_file.read(content, size);
        _encode_file.close();
    }
    return content;
}
