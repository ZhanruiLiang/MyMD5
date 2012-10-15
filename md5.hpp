#pragma once
#include<iostream>
#include<string>
#include<cstdio>
#include<cmath>
#include<cstring>
#define For(i, n) for(i = 0; i < n; i++)
class MD5Sum{
public:
    typedef unsigned long long LL;
    typedef unsigned int uint;
    void encode(char *msg, LL len, char* output);
private:
    const static int SHIFT[64];
    const static uint T[64];
    LL chunk_i;
    LL _len, _len1;
    char * _msg;

    void next_chunk(uint * chunk);
    bool has_chunk();
    inline uint left_rotate(uint x, int c){
        return (x << c) | (x >> (32 - c));
    }
};
