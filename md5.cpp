#include"md5.hpp"

const int MD5Sum::SHIFT[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
};
const MD5Sum::uint MD5Sum::T[64] = {
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


bool MD5Sum::has_chunk(){
    // each chunk has 64 * 8 = 512 bits
    return (chunk_i < (_len1 >> 6));
}

void print_chunk(MD5Sum::uint* chunk){
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
void MD5Sum::next_chunk(uint *chunk){
    // each chunk has 512 bits = 16 * 32 bits = 16 words = 64 bytes
    // for each word, we place the each msg byte to in from Hbyte to Lbyte
    LL i, j;
    int k;
    if(chunk_i < (_len >> 6)){
        // i: the i(th) word of the chunk
        // j: the j(th) byte of the message
        for(i = 0, j = chunk_i << 6; i < 16; i++, j+=4){
            chunk[i] = 0;
            For(k, 4)
                chunk[i] |= ((unsigned char)_msg[j + k] << ((k) << 3));
        }
    }else{
        for(i = 0, j = chunk_i << 6; i < 16; i++, j+=4){
            chunk[i] = 0;
            For(k, 4){
                if(j + k < _len)
                    chunk[i] |= ((unsigned char)_msg[j + k] << ((k) << 3));
                else if(j + k == _len)
                    chunk[i] |= (0x80 << ((k) << 3));
            }
        }
        for(k = 7; k >= 0; k--){
            int shift = ((7 - k)<<3);
            chunk[15 - k/4] |= ((_len << 3 >> shift) & 0xff) << ((3 - k % 4) << 3);
        }
    }
#ifdef DEBUG
    print_chunk(chunk);
#endif
    chunk_i ++;
}
void MD5Sum::encode(char *msg, LL len, char* output){
    uint h0, h1, h2, h3, a, b, c, d, f, g;
    int i;
    uint chunk[16]; // each chunk is 512 bit, 64 byte
    chunk_i = 0;
    _len = len;
    _len1 = (((_len + 9 - 1) >> 6) + 1) << 6;
    _msg = msg;
    h0 = 0x67452301;   //A
    h1 = 0xefcdab89;   //B
    h2 = 0x98badcfe;   //C
    h3 = 0x10325476;   //D
    while(has_chunk()){
        next_chunk(chunk);
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
    // generate output, little_endian
#define PUT(i, hk) \
    output[i + 0] = (hk >> 0) & 0xff; \
    output[i + 1] = (hk >> 8)  & 0xff; \
    output[i + 2] = (hk >> 16) & 0xff; \
    output[i + 3] = (hk >> 24) & 0xff; 
    PUT(0, h0);
    PUT(4, h1);
    PUT(8, h2);
    PUT(12, h3);
}
