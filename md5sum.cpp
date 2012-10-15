#include"md5.hpp"
#include<cstdlib>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<ctime>
#include<cstdio>
#include<iostream>
#include<fstream>
using namespace std;

const int BLOCK_SIZE = 2048;
char buf[BLOCK_SIZE];

int main(int argc, char**argv){
    MD5Sum md5;
    if(argc < 2) return 1;
    // argv[1] specifies the input filename
    ifstream fin(argv[1]);

    // get the file size
    fin.seekg(0, ios::end);
    uLL fsize = fin.tellg();
    fin.seekg(0, ios::beg);

    // read the file, BLOCK_SIZE bytes per time
    uLL p, readsize;
    for(p = 0; p < fsize; p += BLOCK_SIZE){
        readsize = min(fsize - p, (uLL)BLOCK_SIZE);
        fin.read(buf, readsize);
        md5.update(buf, readsize);
    }
    fin.close();
    cout << md5.hexdigest();
    return 0;
}
