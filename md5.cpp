#include"md5.hpp"
#define For(i, n) for(i = 0; i < n; i++)

const int MD5Sum::SHIFT[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
};
const uint MD5Sum::T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 ,
};

void MD5Sum::clear(){
    h0 = 0x67452301;   //A
    h1 = 0xefcdab89;   //B
    h2 = 0x98badcfe;   //C
    h3 = 0x10325476;   //D
    used_len = 0;
    buf_size = 0;
}

void print_chunk(uint* chunk){
    int i, j, k;
    For(i, 16){
        For(j, 32){
            if(j > 0 and j % 8 == 0) 
                std::cerr << " ";
            std::cerr << ((chunk[i] >> (31-j))&1);
        }
        std::cerr << "\n";
    }
}
void MD5Sum::make_chunk(uint p){
    uint i, j, k;
    // i: the i(th) word of the chunk
    // j: the j(th) byte of the message
    for(i = 0, j = p; i < 16; j+=4, i++){
        chunk[i] = 0;
        For(k, 4)
            chunk[i] |= ((uchar)msgbuf[j + k] << ((k) << 3));
    }
#ifdef DEBUG
    // print_chunk(chunk);
#endif
}

void MD5Sum::update(const char* msg, uLL len){
    uint i, p;
    For(i, len){
        if(buf_size == BUF_SIZE){
            for(p = 0; p < buf_size; p += 64){
                make_chunk(p);
                transform();
            }
            buf_size = 0;
        }
        msgbuf[buf_size++] = msg[i];
    }
    used_len += len;
}

void MD5Sum::transform(){
    uint i, a, b, c, d, f, g;
    a = h0; b = h1; c = h2; d = h3;
    For(i, 64){
        if(i < 16){
            f = (b & c) | ((~b) & d);
            g = i;
        }else if(i < 32){
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        }else if(i < 48){
            f = (b ^ c ^ d);
            g = (3 * i + 5) % 16;
        }else{
            f = c ^ (b | (~d));
            g = 7 * i % 16;
        }
        uint tmp = d;
        d = c; 
        c = b;
        b += left_rotate(a + f + T[i] + chunk[g], SHIFT[i]);
        a = tmp;
    }
    h0 += a; h1 += b; h2 += c; h3 += d;
}

string MD5Sum::hexdigest(){
    uLL len, len1;
    uint i;
    // len: origin message length
    len = used_len;
    // len1: length after padding
    len1 = (((len + 8) >> 6) + 1) << 6;
    // pad
    //  append 1 bit
    msgbuf[buf_size] = 0x80;
    //  append  '0' bits until 8 bytes left
    //  actually we can fill the buffer with 0 until the end
    uint end_pos = len % BUF_SIZE + len1 - len;
    memset(msgbuf + buf_size + 1, 0, 
            (end_pos - buf_size - 1));
    //  append length(in bit) to the last 2 bytes
    for(int i = 7; i >= 0; i--){
        int shift = ((7 - i)<<3);
        msgbuf[end_pos - 1 - i] = ((len << 3 >> shift) & 0xff);
    }
    int p;
    // p increase 64 each time since each chunk has 64 bytes
    for(p = 0; p < end_pos; p += 64){
        make_chunk(p);
        transform();
    }
    return make_output();
}
string MD5Sum::make_output(){
    uint hs[4] = {h0, h1, h2, h3};
    char s[30];
    string output;
    for(int i = 0; i < 16; i++){
        sprintf(s, "%02x", (uint)((hs[i/4] >> (i%4*8)) & 0xff));
        output.push_back(s[0]);
        output.push_back(s[1]);
    }
    return output;
}
