#include "AES_Cipher.h"

// S盒
byte S_Box[16][16] = {{0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30,
                       0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
                      {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD,
                       0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
                      {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34,
                       0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
                      {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07,
                       0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
                      {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52,
                       0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
                      {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A,
                       0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
                      {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45,
                       0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
                      {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC,
                       0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
                      {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4,
                       0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
                      {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46,
                       0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
                      {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2,
                       0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
                      {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C,
                       0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
                      {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8,
                       0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
                      {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61,
                       0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
                      {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B,
                       0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
                      {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41,
                       0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}};
//
word RCon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
//第一个明文块与一个叫初始化向量IV的数据块异或
byte IV[16] = {0x31, 0x32, 0x33, 0x0, 0x0, 0x0, 0x0, 0x0,
               0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0};

void print(byte state[4 * 4]) {
    for (int i = 0; i < 4; i++) {
        cout << state[i] << " " << state[i + 4] << " " << state[i + 8] << " "
             << state[i + 12] << endl;
    }
    cout << endl;
}
void print(word w) {
    cout << byte((w >> 24).to_ulong()) << " ";
    cout << byte(((w << 8) >> 24).to_ulong()) << " ";
    cout << byte(((w << 16) >> 24).to_ulong()) << " ";
    cout << byte(((w << 24) >> 24).to_ulong()) << " ";
    cout << endl;
}

AES::AES(string s_key) {
    setKey(s_key);
}
/* state, key: index:
0 4 8 12
1 5 9 13
2 6 10 14
3 7 11 15
*/
// key 一列一列的存
void AES::setKey(string s_key) {
    for (int i = 0; i < 16 && i < s_key.size(); i++) {  // clear
        key[i] = byte(int(s_key[i]));
    }
    // cout << "key:" << endl;
    // print(key);
}

word AES::toWord(byte b0, byte b1, byte b2, byte b3) {
    word out(0);
    out |= b3.to_ulong();
    out |= b2.to_ulong() << 8;
    out |= b1.to_ulong() << 16;
    out |= b0.to_ulong() << 24;
    // cout << "toWord:" << out << endl;
    return out;
}
// RotWord(b0,b1,b2,b3) = (b1,b2,b3,b0)
word AES::RotWord(word w) {
    return (w << 8) | (w >> 24);
}

word AES::SubWord(word w) {
    word out(0);
    for (int i = 0; i < 32; i += 8) {  // 4 bytes
        int row = w[i + 7] * 8 + w[i + 6] * 4 + w[i + 5] * 2 + w[i + 4];
        int col = w[i + 3] * 8 + w[i + 2] * 4 + w[i + 1] * 2 + w[i];
        byte val = S_Box[row][col];
        for (int j = 0; j < 8; j++) {
            out[i + j] = val[j];
        }
    }
    return out;
}
//扩展密钥
void AES::KeyExpansion() {
    for (int i = 0; i < 3; i++) {
        ext_key[i] =
            toWord(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    }
    for (int i = 4; i < 44; i++) {
        word temp = ext_key[i - 1];
        // cout << "temp ori" << endl;
        // print(temp);
        if (i % 4 == 0) {
            temp = SubWord(RotWord(temp)) ^ RCon[i / 4 - 1];
        }
        ext_key[i] = ext_key[i - 4] ^ temp;
        // cout << "KeyExpansion: i=" << i << endl;
        // print(ext_key[i]);
    }
    // for (int i = 0; i < 44; i++) {
    //     cout << "KeyExpansion: i=" << i << endl;
    //     print(ext_key[i]);
    // }
}

byte AES::FieldMult(byte lhs, byte rhs) {
    byte out(0);
    for (int i = 0; i < 8; i++) {
        if ((lhs & (byte(1) << i)) != 0) {
            out ^= rhs;
        }
        if ((rhs & (byte(1) << 7)) != 0) {  // meet the highest bit
            rhs <<= 1;
            rhs ^= byte(0x1b);  // mod x^8 +x^4 +x^3 +x +1
        } else {
            rhs <<= 1;
        }
    }
    return out;
}
//根据S盒进行置换
void AES::SubBytes(byte state[4 * 4]) {
    for (int i = 0; i < 16; i++) {
        int row =
            state[i][7] * 8 + state[i][6] * 4 + state[i][5] * 2 + state[i][4];
        int col =
            state[i][3] * 8 + state[i][2] * 4 + state[i][1] * 2 + state[i][0];
        state[i] = S_Box[row][col];
        // cout << row << "<-row,col->" << col << " " << state[i] << "i:" << i
        //      << endl;
    }
}

void AES::ShiftRows(byte state[4 * 4]) {
    // cout << "shift:start" << endl;
    // print(state);
    byte exchange = state[1];
    for (int i = 0; i < 3; i++) {  // row 2
        state[4 * i + 1] = state[4 * i + 5];
    }
    state[13] = exchange;          // row 2 shift end
    for (int i = 0; i < 2; i++) {  // row 3
        exchange = state[4 * i + 2];
        state[4 * i + 2] = state[4 * i + 10];
        state[4 * i + 10] = exchange;
    }  // row 3 shift end
    // row4 left shift 3, or right shift 1
    exchange = state[15];
    for (int i = 3; i > 0; i--) {
        state[4 * i + 3] = state[4 * i - 1];
    }
    state[3] = exchange;
    // cout << "shift:end" << endl;
    // print(state);
}

void AES::MixColumns(byte state[4 * 4]) {
    byte t[4];
    for (int i = 0; i < 4; i++) {  // i col
        for (int j = 0; j < 4; j++) {
            t[j] = state[4 * i + j];
        }
        state[4 * i] =
            FieldMult(0x2, t[0]) ^ FieldMult(0x3, t[1]) ^ t[2] ^ t[3];
        state[4 * i + 1] =
            FieldMult(0x2, t[1]) ^ FieldMult(0x3, t[2]) ^ t[3] ^ t[0];
        state[4 * i + 2] =
            FieldMult(0x2, t[2]) ^ FieldMult(0x3, t[3]) ^ t[0] ^ t[1];
        state[4 * i + 3] =
            FieldMult(0x2, t[3]) ^ FieldMult(0x3, t[0]) ^ t[1] ^ t[2];
    }
}

void AES::AddRoundKey(byte state[4 * 4], size_t Nr_k) {
    // cout << "AddRoundKey: start" << endl;
    // print(state);
    for (int i = 0; i < 4; i++) {  // i col
        unsigned long n0 = (ext_key[Nr_k * 4 + i] >> 24).to_ulong();
        state[4 * i] ^= byte(n0);
        unsigned long n1 = ((ext_key[Nr_k * 4 + i] << 8) >> 24).to_ulong();
        state[4 * i + 1] ^= byte(n1);
        unsigned long n2 = ((ext_key[Nr_k * 4 + i] << 16) >> 24).to_ulong();
        state[4 * i + 2] ^= byte(n2);
        unsigned long n3 = ((ext_key[Nr_k * 4 + i] << 24) >> 24).to_ulong();
        state[4 * i + 3] ^= byte(n3);
    }
    // cout << "AddRoundKey: end" << endl;
    // print(state);
}
void AES::Encrypt(byte state[4 * 4]) {
    size_t Nr_k = 0;
    KeyExpansion();
    AddRoundKey(state, 0);
    // print(state);
    for (int Nr_k = 1; Nr_k < 10; Nr_k++) {
        // cout << "Nr_k:" << Nr_k << endl;
        // print(state);
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, Nr_k);
    }
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, 10);
}

vector<byte> AES::EncryptCBC(string txt) {
    vector<byte> out;
    // string s;
    int pad = 16 - txt.size() % 16;
    for (int i = 0; i < pad; i++) {
        txt += char(pad);
    }
    // cout << txt.size() << "<-size, pad->" << pad << endl;
    byte to_xor[4 * 4];
    memcpy(to_xor, IV, sizeof(IV));
    // for (int i = 0; i < 16; i++) {
    //     to_xor[i] = IV[i];
    //     cout << i << "<-i" << to_xor[i] << endl;
    // }
    for (int i = 0; i < txt.size(); i += 16) {
        byte state[4 * 4];
        for (int j = 0; j < 16; j++) {
            state[j] = byte(int(txt.at(i + j)));  // Xor:input 16 bytes
            state[j] ^= to_xor[j];
        }
        Encrypt(state);
        memcpy(to_xor, state, sizeof(state));
        for (int j = 0; j < 16; j++) {
            out.push_back(state[j].to_ulong());
        }
        // for (int j = 0; j < 16; j++) {
        //     s.push_back(char(state[j].to_ulong()));
        // }
        // for (int i = 0; i < out.size(); i++) {
        //     cout << std::hex << int(s.at(i)) << " ";
        // }
    }
    return out;
}