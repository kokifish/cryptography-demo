#include <iostream>
#include "SHA_1.h"

using namespace std;

bool Single_Test(string plain_text, string expected_ans) {
    SHA1 sha_1;
    string ans;
    cout << left << setw(12) << "plain text:" << plain_text << endl;
    ans = sha_1.Hash(plain_text);
    if (ans != expected_ans) {
        cout << setw(12) << "[error]" << ans << "   is wrong!" << endl;
        cout << setw(12) << "expected:" << expected_ans << endl;
        return false;
    } else {
        cout << setw(12) << "[Pass]" << ans << "   is right!" << endl;
        return true;
    }
}

bool Sha1_Test() {
    if (Single_Test("16337087", "55c69f4d95933cefbcf8918e6d665599be02ae92") ==
        false) {
        return false;
    }
    if (Single_Test("SYSU_student_ID=16337087",
                    "607f71ef0006caea6c4a5ab2a23de6110bf52108") == false) {
        return false;
    }
    if (Single_Test("SYSU_abcdefghijklmnABCDEFGzZzZzZ",
                    "5aade4224471bce9a99c886c37b08f9ad90e6430") == false) {
        return false;
    }
    if (Single_Test("abcd", "81fe8bfe87576c3ecb22426f8e57847382917acf") ==
        false) {
        return false;
    }
    if (Single_Test("SDCS is best!",
                    "539d8c0527c616d1d3d1e15c461af0ceba5affae") == false) {
        return false;
    }
    return true;
}

int main() {
    cout << "\n=== Test for Sha-1 Hash Algorithm, stu_id=16337087\n\n";
    cout << (Sha1_Test() == true ? "Test Pass" : "Test Failed") << endl;
    return 0;
}