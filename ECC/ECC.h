#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
bool isPrime(size_t num);

// GF(2^m)
// if m==0, means cannot use or unknown GF
class GF {
   private:
    // m: mod number
    size_t m;
    // 1100 0001 means 1+x+x^7
    std::vector<size_t> vec_poly;

   public:
    // group size: bits of size_t
    static size_t group_size;
    // constructor GF(x^m)
    GF(size_t m = 131);
    // get the irreducible polynomial
    GF getFx(size_t m = 0) const;
    // transform a Fx to a Rx
    GF toRx(GF Fx) const;
    // get m; m: GF(x^m)
    size_t getM() const;
    // set the poly to 0
    void clear();
    // set vec_poly accroding to the pos
    bool setPoly(std::vector<size_t> vec_pos);
    // set vec_poly accroding to the big_num
    bool setPoly(size_t big_num);
    // print as poly//e.g. 1 +x +x^3 +x^130
    std::string print() const;
    // print as bit //e.g. 10010001 //one space after group_size bits
    std::string printBit() const;
    // print as bit
    friend std::ostream& operator<<(std::ostream& out, const GF& gf);
    // return highest degree
    size_t getDegree() const;
    // Reduce the size of vec_poly to (m/group_size+1), if the extra size_t is 0
    GF Reduce();
    // Right shift
    GF RShift(size_t num) const;
    // Light shift
    GF LShift(size_t num) const;
    // GF add
    GF Add(GF rhs) const;
    // GF Multiply
    GF Multiply(GF rhs) const;
    // Mod irreducible polynomial for GF(2^m)
    GF Mod() const;
    // GF Inverse
    GF Inverse() const;
};
