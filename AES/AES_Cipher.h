#ifndef AES_CIPHER_H
#define AES_CIPHER_H

#include <bitset>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

typedef std::bitset<8> byte;
typedef std::bitset<32> word;

extern byte S_Box[16][16];
extern word RCon[10];
extern byte IV[16];
void print(byte state[4 * 4]);
// state: byte[4*4] index:col1:0123; col2:4567; col3:891011; col4:12 13 14 15
/* state, key: index:
0 4 8 12
1 5 9 13
2 6 10 14
3 7 11 15
*/
// AES for 128 bit
class AES {
   private:
    word ext_key[44];  // call KeyExpansion to set this
    // key store according to columns
    byte key[16];  // call setKey to set this // 16 Bytes

   public:
    AES(string s_key = "sysu");
    // set key//original key
    void setKey(string s_key = "sysu");
    // 4 bytes to 1 word
    word toWord(byte b1, byte b2, byte b3, byte b4);
    // RotWord(b0,b1,b2,b3) = (b1,b2,b3,b0)
    word RotWord(word w);
    // input a word, for each byte, substitute by S box
    word SubWord(word w);
    // expand the key
    void KeyExpansion();
    // FieldMult: return multiply result under GF(2^8), mod: x^8 +x^4 +x^3 +x +1
    byte FieldMult(byte lhs, byte rhs);
    // SubBytes //loop step 1
    void SubBytes(byte state[4 * 4]);
    // ShiftRows //loop step 2
    void ShiftRows(byte state[4 * 4]);
    // MixColumns //loop step 2
    void MixColumns(byte state[4 * 4]);
    // AddRoundKey//No. Nr_k round key
    void AddRoundKey(byte state[4 * 4], size_t Nr_k);
    // encrypt
    void Encrypt(byte state[4 * 4]);
    // input a string to Encrypt
    vector<byte> EncryptCBC(string txt);
};
#endif