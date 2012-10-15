#include"md5.hpp"
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<cstdio>
#include<iostream>
#include<fstream>
using namespace std;

int main(int argc, char**argv){
    MD5Sum md5;
    char output[16];
    string s;
    char c;
    if(argc < 2) return 1;
    ifstream fin(argv[1]);

    fin.seekg(0, ios::end);
    int fsize = fin.tellg();
    fin.seekg(0, ios::beg);
    s.resize(fsize);
    fin.read(&s[0], fsize);
    // empty string output should be: d41d8cd98f00b204e9800998ecf8427e
    md5.encode((char*)s.c_str(), s.size(), output);
    int i;
    For(i, 16) printf("%02x", int(((int)output[i] + 256) % 256));
    return 0;
}
