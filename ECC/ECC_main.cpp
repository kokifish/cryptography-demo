#include <iomanip>
#include <string>
#include <vector>
#include "ECC.h"

using namespace std;

void TestShift() {
    GF gf_a = GF(131);
    vector<size_t> vec_pos({0, 1, 6, 13, 63, 64, 65, 127, 128});
    gf_a.setPoly(vec_pos);
    GF gf_b = GF(131);
    vector<size_t> vec_pos2({0, 1, 2, 13, 14, 128, 130});
    gf_b.setPoly(vec_pos2);
    size_t num1 = 1;
    size_t num2 = 65;
    cout << "===TestShift===start==========" << endl << left;
    cout << setw(10) << "a:" << gf_a.print() << "  Rshift:" << num1
         << " LShift:" << num2 << endl;
    cout << setw(10) << "a:" << gf_a.printBit() << endl;
    cout << setw(10) << "RShift:" << gf_a.RShift(num1) << endl;
    cout << setw(10) << "LShift:" << gf_a.LShift(num2) << endl;
    cout << setw(10) << "b:" << gf_b.print() << "  Rshift:" << num2
         << " LShift:" << num1 << endl;
    cout << setw(10) << "b:" << gf_b.printBit() << endl;
    cout << setw(10) << "RShift:" << gf_b.RShift(num2) << endl;
    cout << setw(10) << "LShift:" << gf_b.LShift(num1) << endl;
    cout << "===test_shift===end==========" << endl << endl;
}
void TestAddMultiplyMod() {
    GF gf_a = GF(131);
    vector<size_t> vec_pos({0, 1, 6, 13, 127});  //{0, 1, 63, 64, 127, 130}
    gf_a.setPoly(vec_pos);
    GF gf_b = GF(131);
    vector<size_t> vec_pos2({0, 1, 2, 13, 14, 130});
    gf_b.setPoly(vec_pos2);
    cout << left << endl << "===TestAddMultiplyMod===start==========" << endl;
    cout << setw(10) << "a:" << gf_a.print() << endl;
    cout << setw(10) << "a:" << gf_a.printBit() << endl;
    cout << setw(10) << "b:" << gf_b.print() << endl;
    cout << setw(10) << "b:" << gf_b.printBit() << endl;
    GF ans = gf_a.Add(gf_b);
    cout << setw(10) << "a+b=" << ans.print() << endl;
    cout << setw(10) << "a+b=" << ans << endl;
    ans = gf_a.Multiply(gf_b);
    cout << setw(10) << "a*b=" << ans.print() << endl;
    cout << setw(10) << "a*b=" << ans << endl;
    cout << setw(10) << "a*b mod=" << ans.Mod().Reduce().print() << endl;
    cout << setw(10) << "a*b mod=" << ans.Mod().Reduce() << endl;
    cout << "===TestAddMultiplyMod===end==========" << endl << endl;
}

void TestModInverse() {
    GF gf_a = GF(131);
    vector<size_t> vec_pos({0, 1, 2, 13, 14, 25});
    gf_a.setPoly(vec_pos);
    cout << left << endl << "===TestModInverse===start==========" << endl;
    cout << setw(10) << "a:" << gf_a.print() << " =Inverse Test=" << endl;
    cout << setw(10) << "a:" << gf_a.printBit() << endl;
    cout << setw(10) << "a.Inverse" << gf_a.Inverse() << endl;
    cout << setw(10) << "a*a.I" << gf_a.Inverse().Multiply(gf_a).Mod() << endl;
    GF gf_b = GF(131);
    gf_b.setPoly(16337087);
    cout << setw(10) << "StuId:" << gf_b.print() << " StuId = 16337087" << endl;
    cout << setw(10) << "StuId:" << gf_b.printBit() << endl;
    cout << setw(10) << "Inverse:" << gf_b.Inverse().print() << endl;
    cout << setw(10) << "Inverse:" << gf_b.Inverse() << endl;
    cout << setw(10) << "s*sI mod:" << gf_b.Inverse().Multiply(gf_b).Mod()
         << endl;
    cout << "===TestModInverse===end==========" << endl << endl;
}

// f(x) = 1 +x +x^2 +x^13 +x^131 //irreducible polynomial for GF(2^131)
// f(x) = x^131 + r(x); r(x) = 1 +x +x^2 +x^13
int main() {
    cout << left
         << "=== Tips: Bits print from low to high; Word(size_t) size is "
            "machine related, typically 64 or 32; StuId: 16337087\n\n";
    TestShift();           // shift test
    TestAddMultiplyMod();  // AddMultiplyMo test
    TestModInverse();      // ModInverse test
    GF gf_a = GF(131);
    vector<size_t> vec_pos({0, 1, 6, 13, 127});  //{0, 1, 63, 64, 127, 130}
    gf_a.setPoly(vec_pos);
    GF gf_b = GF(131);
    vector<size_t> vec_pos2(
        {0, 1, 2, 13, 14, 131});  //{2,3,127,128}2, 3, 127, 128
    gf_b.setPoly(vec_pos2);

    // cout << setw(10) << "a:" << gf_a.print() << endl;
    // cout << setw(10) << "a:" << gf_a.printBit() << endl;
    // cout << setw(10) << "b:" << gf_b.print() << endl;
    // cout << setw(10) << "b:" << gf_b.printBit() << endl;

    // cout << setw(10) << "Mod:" << gf_b.Mod() << endl;
    // gf_b = gf_b.Mod();
    // cout << setw(10) << "Inverse:" << gf_b.Inverse().print() << endl;
    // cout << setw(10) << "Inverse:" << (gf_b.Inverse()).Multiply(gf_b).print()
    //      << endl;
    // cout << setw(10) << "Inverse:" << ((gf_b.Inverse()).Multiply(gf_b)).Mod()
    //      << endl;
}