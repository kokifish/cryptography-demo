#include <iostream>
#include <string>
#include "AES_Cipher.h"
using std::cout;
using std::endl;
using std::string;
int main() {
    AES aes;
    byte b1 = 0x1;
    byte b2 = 0x2;
    byte b3 = 0x4;
    byte b4 = 0x8;
    // aes.toWord(b1, b2, b3, b4);
    // b4 <<= 3;
    // cout << aes.FieldMult(b3, b4);
    string key("sysu");
    string txt("School of data science and computer, Sun Yat-senUniversity.");
    cout << "plain text: " << txt << endl;
    cout << "key:        " << key << endl;

    aes.setKey(key);
    vector<byte> out = aes.EncryptCBC(txt);

    cout << "after Encrypt: " << endl;
    for (int i = 0; i < out.size(); i++) {
        if (out.at(i).to_ulong() < 16) {
            cout << 0;
        }
        cout << std::hex << out.at(i).to_ulong();
    }
    return 0;
}