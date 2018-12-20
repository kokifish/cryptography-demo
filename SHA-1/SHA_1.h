#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
using std::string;

class SHA1 {
   private:
    const uint32_t ori_H[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476,
                               0xC3D2E1F0};

    size_t buffer_count = 0;  // size of buffer
    uint32_t* buffer = NULL;  // text after padding
    // SHA-1: padding
    void SHA1_pad(string str);
    // constant number K
    uint32_t K(int t);
    // rotate the word (Word) left for n bits
    uint32_t ROTL(int n, uint32_t Word);
    // t:round num;
    uint32_t f(int t, uint32_t B, uint32_t C, uint32_t D);

   public:
    uint32_t ans_H[5] = {0, 0, 0, 0, 0};
    // hash str using SHA-1
    string Hash(string str);
};
