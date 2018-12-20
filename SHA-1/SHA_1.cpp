#include "SHA_1.h"

using std::cout;
using std::endl;
using std::memset;

// constent number K
uint32_t SHA1::K(int t) {
    if (0 <= t && t <= 19) {
        return 0x5A827999;
    } else if (20 <= t && t <= 39) {
        return 0x6ED9EBA1;
    } else if (40 <= t && t <= 59) {
        return 0x8F1BBCDC;
    } else if (60 <= t && t <= 79) {
        return 0xCA62C1D6;
    } else {
        cout << "K: t out of range! valid range: [0, 79]" << endl;
        return 0;
    }
}

// Rotate [Word] Left for [n] bit
uint32_t SHA1::ROTL(int n, uint32_t Word) {
    return (Word << n) | (Word >> (32 - n));
}

// f: the f process in the new A calculate
uint32_t SHA1::f(int t, uint32_t B, uint32_t C, uint32_t D) {
    if (0 <= t && t <= 19) {
        return (B & C) | ((~B) & D);
    } else if (20 <= t && t <= 39) {
        return B ^ C ^ D;
    } else if (40 <= t && t <= 59) {
        return (B & C) | (B & D) | (C & D);
    } else if (60 <= t && t <= 79) {
        return B ^ C ^ D;
    } else {
        cout << "f: t out of range! valid range: [0, 79]" << endl;
        return 0;
    }
}

// SHA-1 padding process
void SHA1::SHA1_pad(string str) {
    // cout << "str.size() = " << str.size() << " bytes" << endl;
    // at least (str.size() + 1 + 8) bytes
    buffer_count =
        (str.size() + 1 + 8) / 4 + (((str.size() + 1 + 8) % 4 != 0) ? 1 : 0);
    if (buffer_count % 16 != 0) {
        buffer_count = (buffer_count / 16) * 16 + 16;
    }
    buffer = new uint32_t[buffer_count];  // buffer_count % 16 == 0
    memset(buffer, 0, buffer_count * 4);
    // cout << "buffer_count = " << buffer_count << endl;
    uint64_t l = str.size() * 8;  // bit count of input str x
    int d = (447 - l) % 512;      // bit count of 0 to append
    if (d < 0) {
        d += 512;
    }
    int i = 0;
    for (; i < str.size() / 4; i++) {  // 4 byte to a uint32_t
        buffer[i] = (uint32_t(str[4 * i]) << 24) |
                    (uint32_t(str[4 * i + 1]) << 16) |
                    (uint32_t(str[4 * i + 2]) << 8) | uint32_t(str[4 * i + 3]);
    }
    // for (int i = 0; i < buffer_count; i++) {
    //     cout << std::hex << buffer[i] << " ";
    // }
    // cout << "<-buffer insert str" << endl;
    int j = 0;
    for (; j < str.size() % 4; j++) {
        buffer[i] |= uint32_t(str[4 * i + j]) << ((3 - j) * 8);
    }
    buffer[i] |= (uint32_t(1) << 7) << ((3 - j) * 8);  // bit 1 to insert
    for (i += 1; i < buffer_count - 2; i++) {  // insert 0 for 32 bit each loop
        buffer[i] = 0;
        // cout << "insert 0, i=" << i << endl;
    }
    buffer[buffer_count - 2] = uint32_t(l >> 32);
    buffer[buffer_count - 1] = uint32_t(l & 0xFFFF);
    // for (int i = 0; i < buffer_count; i++) {
    //     cout << std::hex << buffer[i] << " ";
    // }
    // cout << "<- str after padding" << endl;
}

// public method: Hash process
string SHA1::Hash(string str) {
    if (buffer != NULL) {  // delete the old buffer
        delete buffer;
        buffer = NULL;
    }
    SHA1_pad(str);                    // padding
    int n = buffer_count * 32 / 512;  // size of buffer
    for (int i = 0; i < 5; i++) {     // ini of H[0] ~ H[4]
        ans_H[i] = ori_H[i];
    }
    uint32_t Word[80];             // 80 Word
    for (int i = 0; i < n; i++) {  // 512bit per loop, 16 * 32bit per loop
        for (int t = 0; t < 16; t++) {
            Word[t] = buffer[i * 16 + t];
        }
        for (int t = 16; t < 80; t++) {  // 16 to 79
            Word[t] = ROTL(
                1, Word[t - 3] ^ Word[t - 8] ^ Word[t - 14] ^ Word[t - 16]);
        }
        uint32_t A = ans_H[0];
        uint32_t B = ans_H[1];
        uint32_t C = ans_H[2];
        uint32_t D = ans_H[3];
        uint32_t E = ans_H[4];
        for (int t = 0; t < 80; t++) {  // update A B C D E
            uint32_t temp = ROTL(5, A) + f(t, B, C, D) + E + Word[t] + K(t);
            E = D;
            D = C;
            C = ROTL(30, B);
            B = A;
            A = temp;
        }
        ans_H[0] += A;
        ans_H[1] += B;
        ans_H[2] += C;
        ans_H[3] += D;
        ans_H[4] += E;
    }
    // for (int i = 0; i < 5; i++) {
    //     cout << std::hex << std::setfill('0') << std::setw(8) <<
    //          << " ";
    // }
    char arr_char[40];
    for (int i = 0; i < 5; i++) {  // transfer to hex number
        sprintf(arr_char + 8 * i * sizeof(char), "%08x", ans_H[i]);
    }
    string out_str(arr_char);  // transfer to a string
    return out_str;
}