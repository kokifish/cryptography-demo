#include "ECC.h"
// true: num is a prime, vice versa
bool isPrime(size_t num) {
    if (num < 2)
        return false;
    for (size_t i = 2; i < (size_t)sqrt((double)num); i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

// get the irreducible polynomial
GF GF::getFx(size_t m) const {
    if (m == 0) {
        m = this->m;
    }
    if (m == 131) {
        GF out(131);
        std::vector<size_t> vec_pos({0, 1, 2, 13, 131});
        out.setPoly(vec_pos);
        return out;
    } else {  // error
        return GF(0);
    }
}
// get the Rx of a Fx
GF GF::toRx(GF Fx) const {
    GF out = Fx;
    size_t count = Fx.m / group_size;
    out.vec_poly[count] &= ~(size_t(1) << (Fx.m % group_size));
    return out;
}

// class GF: group_size, bit count of a size_t var, typically 64, 32
size_t GF::group_size = (sizeof(size_t) * 8);

// class GF: constructor
GF::GF(size_t m) : m(m) {
    if (isPrime(m) == false) {
        throw "m is not prime, 2^m cannot be a Galois Fields";
    } else {
        clear();
    }
}

// return m of this GF
size_t GF::getM() const {
    return m;
}

// set to empty poly, set the poly to 0(empty)
// vec_poly size will be just enough
void GF::clear() {
    vec_poly.clear();
    size_t count = m / group_size + 1 - (size_t)(m % group_size == 0);
    for (size_t i = 0; i < count; i++) {
        vec_poly.push_back(0);
    }
}

// set vec_poly accroding to the big_num
bool GF::setPoly(size_t big_num) {
    std::vector<size_t> vec_pos;
    for (size_t i = 0; i < group_size; i++) {
        if ((big_num & (size_t(1) << i)) > 0) {
            vec_pos.push_back(i);
        }
    }
    setPoly(vec_pos);
    return true;
}

// set vec_poly accroding to the pos
bool GF::setPoly(const std::vector<size_t> vec_pos) {
    // set to empty poly, set the poly to 0
    clear();
    // set poly according to pos //pos=0: 1; pos=1: x; pos=130: x^130
    for (std::vector<size_t>::const_iterator itr = vec_pos.cbegin();
         itr != vec_pos.cend(); itr++) {
        size_t count = *itr / group_size;  // vec_poly[count]
        while (vec_poly.size() <= count) {
            vec_poly.push_back(0);
        }
        vec_poly[count] |= (size_t(1) << (*itr - group_size * count));
        // cout << "num:" << vec_poly[count] << endl;
    }
    return true;
}

// return poly as a string//e.g. 1 +x +x^130
std::string GF::print() const {
    std::string out;
    for (size_t i = 0; i < vec_poly.size(); i++) {
        for (size_t j = 0; j < group_size; j++) {
            size_t exp = i * group_size + j;
            if (vec_poly[i] & (size_t(1) << j)) {
                if (exp == 0) {
                    out.append(" +1");
                } else if (exp == 1) {
                    out.append(" +x");
                } else {
                    out.append(" +x^");
                    out.append(std::to_string(exp));
                }
            }
        }
    }
    if (out.size() > 2) {
        out.erase(0, 2);
    }
    return out;
}

// return poly as a {0, 1} string
std::string GF::printBit() const {
    std::string out;
    for (size_t i = 0; i < vec_poly.size(); i++) {  // i: [0, vec size)
        for (size_t j = 0; j < group_size; j++) {
            size_t exp = i * group_size + j;
            if (vec_poly[i] & (size_t(1) << j)) {
                out.append("1");
            } else {
                out.append("0");
            }
        }
        out.append(" ");
    }
    return out;
}

//重载输出<<
std::ostream& operator<<(std::ostream& out, const GF& gf) {
    out << gf.printBit();
    return out;
}

// return highest degree
size_t GF::getDegree() const {
    size_t max = 0;                                 // record highest degree
    for (size_t i = 0; i < vec_poly.size(); i++) {  // i: [0, vec size)
        for (size_t j = 0; j < group_size; j++) {
            if (vec_poly[i] & (size_t(1) << j)) {
                max = i * group_size + j;
            }
        }
    }
    return max;
}

// Reduce the size of vec_poly to (m/group_size + 1), remove the useless 0
GF GF::Reduce() {
    GF out = *this;
    while (out.vec_poly.back() == 0 &&
           out.vec_poly.size() > (m / group_size + 1)) {
        out.vec_poly.pop_back();
    }
    return out;
}

//右移，相当于乘以x^num
GF GF::RShift(size_t num) const {
    if (num == 0) {
        return *this;
    }
    GF out(m);
    // Note that sometimes the highest degree is greater than m
    size_t count = (getDegree() + num) / group_size + 1;
    while (out.vec_poly.size() < count) {
        out.vec_poly.push_back(size_t(0));
    }
    size_t shift_big = num / group_size;
    for (size_t i = 0; i < vec_poly.size(); i++) {
        out.vec_poly[i + shift_big] = vec_poly[i];
    }
    size_t shift_small = num - (num / group_size) * group_size;
    if (shift_small == 0)
        return out;
    // cout << "count:" << count << "; " << shift_big
    //      << "<-shift_big, shift_small->" << shift_small << endl;
    // cout << "    " << out << endl;
    out.vec_poly[shift_big] <<= shift_small;
    for (size_t i = 1 + shift_big; i < out.vec_poly.size(); i++) {
        out.vec_poly[i] <<= shift_small;
        if (i - 1 - shift_big < vec_poly.size()) {
            out.vec_poly[i] |=
                (vec_poly[i - 1 - shift_big] >> (group_size - shift_small));
        }
    }
    out = out.Reduce();
    return out;
}

//左移
GF GF::LShift(size_t num) const {
    GF out(m);
    size_t shift_big = num / group_size;  // big shift
    for (size_t i = shift_big; i < vec_poly.size(); i++) {
        out.vec_poly[i - shift_big] = vec_poly[i];
    }
    size_t shift_small = num - num / group_size * group_size;  // small shift
    if (shift_small == 0)                                      // no small shift
        return out;
    // cout << shift_big << "<-shift_big, shift_small->" << shift_small << endl;
    for (size_t i = 0; i < out.vec_poly.size(); i++) {
        out.vec_poly[i] >>= shift_small;
        if (i + shift_big + 1 < vec_poly.size()) {
            out.vec_poly[i] |=
                (vec_poly[i + shift_big + 1] << (group_size - shift_small));
        }
    }
    out = out.Reduce();
    return out;
}

// return GF after adding
GF GF::Add(GF rhs) const {
    size_t max_size = vec_poly.size() > rhs.vec_poly.size()
                          ? vec_poly.size()
                          : rhs.vec_poly.size();  // cal the greater one
    GF out(m);
    out.vec_poly.clear();
    for (size_t i = 0; i < max_size; i++) {
        if (i < vec_poly.size() && i < rhs.vec_poly.size()) {
            out.vec_poly.push_back(vec_poly[i] ^ rhs.vec_poly[i]);
        } else if (i < vec_poly.size() && i >= rhs.vec_poly.size()) {
            out.vec_poly.push_back(vec_poly[i]);
        } else if (i >= vec_poly.size() && i < rhs.vec_poly.size()) {
            out.vec_poly.push_back(rhs.vec_poly[i]);
        }
    }
    out = out.Reduce();
    return out;
}

// return GF after Multiply without mod
GF GF::Multiply(GF rhs) const {
    GF out(m);
    size_t count = (2 * m) / group_size + 1 - (size_t)(m % group_size == 0);
    while (out.vec_poly.size() < count) {
        out.vec_poly.push_back(0);
    }
    // cout << "Mul: out.vec_poly.size()" << out.vec_poly.size() << endl;
    for (size_t k = 0; k < group_size; k++) {
        for (size_t j = 0; j < vec_poly.size(); j++) {
            if ((vec_poly[j] & (size_t(1) << k)) > 0) {
                // cout << "j:" << j << ", k:" << k << endl;
                out = out.Add(rhs.RShift(j * group_size));
                // cout << std::setw(10) << "Added:" << out << endl;
            }
        }
        rhs = rhs.RShift(1);
    }
    out = out.Reduce();
    return out;
}

// return GF after Mod
GF GF::Mod() const {
    GF out = *this;
    if (out.getDegree() < m) {
        return out;
    }
    while (out.vec_poly.size() >
           (m / group_size + 1 - (size_t)(m % group_size == 0))) {
        out.vec_poly.pop_back();
    }
    GF Fx = getFx();
    GF Rx = toRx(Fx);
    // cout << "Rx: " << Rx.print() << endl;
    // cout << "*this: " << this->print() << endl;
    std::vector<GF> vec_u;
    for (size_t i = 0; i < group_size; i++) {
        vec_u.push_back(Rx.RShift(i));
        // cout << Rx.RShift(i) << endl;
    }
    // cout << "Mod out:" << out << endl;
    // i: index of the bit(exp)
    for (size_t i = vec_poly.size() * group_size - 1; i >= m; i--) {
        if ((vec_poly[i / group_size] & (size_t(1) << (i % group_size))) > 0 &&
            i <= 2 * m - 2) {
            size_t j = (i - m) / group_size;
            size_t k = (i - m) - group_size * j;
            out = out.Add(vec_u[k].RShift(j * group_size));
            // cout << "Mod add:" << vec_u[k].RShift(j * group_size)
            //      << " j*group_size:" << j * group_size << " i=" << i
            //      << " j,k:" << j << "," << k << endl;
            // cout << "Mod out:" << out << endl;
        }
    }
    for (size_t i = m % group_size; i < group_size; i++) {  // high bit mask
        out.vec_poly[m / group_size] &= ~(size_t(1) << i);
    }
    out = out.Reduce();
    return out;
}

// return GF after inverse
GF GF::Inverse() const {
    GF b(131);
    b.setPoly(1);
    GF c(131);
    GF u = *this;
    GF v = getFx(131);
    while (u.getDegree() > 0) {
        int j = u.getDegree() - v.getDegree();
        // cout << "u.getDegree():" << u.getDegree() << " j: " << j << endl;
        if (j < 0) {
            GF temp = u;
            u = v;
            v = temp;
            temp = b;
            b = c;
            c = temp;
            j = -j;
        }
        // cout << "v:  " << v << " size_t(j)=" << size_t(j) << endl;
        // cout << "v Rs" << v.RShift(size_t(j)) << endl;
        // cout << "u be" << u << endl;
        u = u.Add(v.RShift(size_t(j))).Reduce();
        // cout << "u:  " << u << endl;
        b = b.Add(c.RShift(size_t(j))).Reduce();
    }
    b = b.Reduce();
    return b;
}
