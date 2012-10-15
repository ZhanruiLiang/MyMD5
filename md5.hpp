#pragma once
#include<iostream>
#include<string>
#include<cstdio>
#include<cmath>
#include<cstring>

typedef unsigned int uint;
typedef unsigned long long uLL;
typedef unsigned char uchar;
using std::string;

class MD5Sum{
public:
    // generate ouput
    string hexdigest();
    // update the state using the message
    void update(const char* msg, uLL len);
    // reset
    void clear();
    MD5Sum(){
        clear();
    }
private:
    const static int SHIFT[64];
    const static uint T[64];
    const static uint BUF_SIZE = 1024;
    const static uint CHUNK_SIZE = 512 / 8;
    uchar msgbuf[BUF_SIZE + CHUNK_SIZE];
    uint chunk[16];

    uint buf_size;
    uLL used_len;
    uint h0, h1, h2, h3;

    inline uint left_rotate(uint x, int c){
        return (x << c) | (x >> (32 - c));
    }
    // update the states using chunk
    void transform();
    // write to chunk using bytes from msgbuf starts at p
    void make_chunk(uint p);
    string make_output();
};
