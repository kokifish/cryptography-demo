# author: 黄海宇 student id: 16337087; email: hyhuang1024@outlook.com ; 2018.12
import numpy as np
import math
import time
import random

def gcd(a, b):#gcd(a,0) = a
    while(b):
        a, b = b, a%b
    return a


#return b^(-1) mod a   # if return 0: b has no inverse modulo a
def multi_inverse(a,b):
    if(b==0):
        return 0# b has no inverse modulo a
    a0 = a
    b0 = b
    t0 = 0
    t = 1
    q = a0 // b0
    r = a0 % b0
    while(r>0):
        temp = (t0-q*t) % a
        t0, t, a0, b0 = t, temp, b0, r
        q = a0 // b0
        r = a0 % b0
    if(b0!=1):
        return 0# b has no inverse modulo a
    else:
        return t

# cal: log_{a} b mod p
def Shanks_DLPsolver(p, alpha, beta, n=0):
    if(n==0):
        n = p-1
    m = int(math.ceil(pow(n, 0.5)))
    am = pow(alpha, m, p)
    L1 = [(j, pow(am, j, p)) for j in range(m)]
    a_inverse = multi_inverse(p, alpha)
    L2 = [(i, beta * pow(a_inverse, i, p) % p) for i in range(m)]
    # print('L1:', L1)
    # print('L2:', L2)
    L1 = sorted(L1, key=lambda x: x[1])
    L2 = sorted(L2, key=lambda x: x[1])

    i, j = 0, 0
    while(i < m and j < m):
        if(L1[j][1] == L2[i][1]):
            return (m * L1[j][0] + L2[i][0]) % n
        elif(L1[j][1] > L2[i][1]):
            i += 1
        else:
            j += 1
    return None


#貌似随机的函数，根据x属于三个划分中的哪一个，决定使用的变换
def f_Pollard_rho(n, p, alpha, beta, x, a, b):
    if(x%3 == 1):
        return beta*x%p, a, (b+1)%n
    elif(x%3 == 0):
        return x**2%p, 2*a%n, 2*b%n
    else:
        return alpha*x%p, (a+1)%n, b

# 同余式的求解: x*a = b mod n, 注意返回值是一个两个元素的tuple
# return: (x, step), x+= k*step is also valid #(-1,-1):no answer;
def congruence_eq_solver(a, b, n):
    d = gcd(a, n)
    if(d == 1):
        return b * multi_inverse(n, a), 0
    if(b % d != 0):
        return -1, -1
    a, b, n_new = a//d, b//d, n//d
    x = (b * multi_inverse(n_new, a))%n_new
    # print('d:',d,'a:',a,'b:',b,'x:',x,'n:',n,'n_new:',n_new)
    return x, n_new

# Pollard rho Algorithm for DLP # main function of Pollard rho
#cal: log_{alpha} beta mod p;
def Pollard_rho_DLPsolver(p, alpha, beta):
    n = p-1#cal_element_order(p, alpha)
    x1, a1, b1 = f_Pollard_rho(n, p, alpha, beta, 1, 0, 0)
    x2, a2, b2 = f_Pollard_rho(n, p, alpha, beta, x1, a1, b1)
    i = 0
    x = -1
    step = -1
    while(x1 != x2 or x == -1):
        i+=1
        x1, a1, b1 = f_Pollard_rho(n, p, alpha, beta, x1, a1, b1)
        x2, a2, b2 = f_Pollard_rho(n, p, alpha, beta, x2, a2, b2)
        x2, a2, b2 = f_Pollard_rho(n, p, alpha, beta, x2, a2, b2)
        if(x1 == x2):
            x, step = congruence_eq_solver((b2-b1)%n, (a1-a2)%n, n)
    # print('x1==x2:', x1==x2, ' iteration:',i)
    # print('x:', x, 'step:', step)
    d = n//step # gcd(b' - b, n) = d
    for i in range(d):
        if(pow(alpha, x, p) == beta):
            return x
        else:
            x =  (x+step)%p
    return -1 #((a1-a2)*multi_inverse(n, b2-b1)) % n


# 使用中国剩余定理求解同余方程组，其中
# equation: (ai, mi); x = ai mod mi # M = m0*m1*...*mk for all mi in equation
def Chinese_Remainder_Theorem(equation, M):
    x = 0
    for ai, mi in equation:
        Mi = M//mi
        x += ai*Mi*(multi_inverse(mi, Mi)) % M
    return x % M

# Input: A cyclic group G of order n = q^c with generator g and an element h in G
# Output. The unique integer x\in {0,...,n-1} such that g^{x}=h # from wiki
def Pohlig_Hellman_prime_power(p, g, h, q, c):
    xk = 0
    n = q**c # order of g in group G
    gamma = pow(g, pow(q, c-1), p)
    for k in range(c):
        hk = pow(multi_inverse(p, pow(g, xk)) * h, pow(q, c-1-k, p), p)
        # print(p, gamma, hk, q)
        dk = Shanks_DLPsolver(p, gamma, hk)#, q
        # print('dk', dk)
        xk = (xk + pow(q, k, p)*dk )%n
    return xk % n

# Pohlig-Hellman Algorithm for DLP # main function of Pollard rho
# factor_l is factor list of (p-1), should be computed by integer factorization algorithm
def Pohlig_Hellman_DLPsolver(p, n, alpha, beta, factor_l):
    q_l = sorted(list(set(factor_l)))
    c_l = [factor_l.count(i) for i in q_l]
    # n = p-1
    # print('q_l:', q_l, 'c_l:', c_l)
    congruence_eq = list()
    for idx in range(len(q_l)):
        q, c = q_l[idx], c_l[idx]
        a = Pohlig_Hellman_prime_power(p, alpha, beta, q, c)
        congruence_eq.append([a%(q**c), q**c])
    print('congruence_eq:', congruence_eq)
    return Chinese_Remainder_Theorem(congruence_eq, p-1) % p


# Pollard_rho 素数分解
def Pollard_rho_Factoring(n:int, x1:int=1):
    def f(x, n): # x's operation  mod n
        return (pow(x, 2, n) + 1 )%n

    x = x1
    x_s = f(x, n)
    p = gcd(x-x_s, n)
    i = 0
    while(p==1):
        x = f(x, n)
        x_s = f(x_s, n)
        x_s = f(x_s, n)
        p = gcd(n, (x-x_s)%n)
        i+=1
        if(i%1e6==0):
            print(i)
    if(p==n):
        return 0
    else:
        return p

def test(p, g, ya, yb):
    print(Pohlig_Hellman_DLPsolver(29, 28,2, 18, [7,2,2]))
    print('[Shanks_DLPsolver]', Shanks_DLPsolver(809, 3, 525))

    print(p, 'bits of p:',len(str(bin(int(p)))))
    print('gcd(g, p-1)',gcd(g, p-1), multi_inverse(7, 6))
    factor_l = [2, 2154937333, 481006989528686211893, 602384082763980697559, 1037605481820435550019, 1206102098588167138789, 1206102098588167138789, 1524982628045073955591, 1976916723258661210051]
    # print(Pohlig_Hellman_DLPsolver(p, p-1, g, ya, factor_l))

    # print('Pollard_rho_Factoring',Pollard_rho_Factoring((p-1)//2154937333//2, g))
    # loop_time = int(((p-1)//2//2154937333)**0.5)# 1e74
    # print('loop_time for Factoring',loop_time, len(str(loop_time)))
    # print( len(str(bin((p-1)//2//2154937333))), (p-1)//2//2154937333)
    print('g** 2154937333*2 %p :', pow(g, 2154937333*2, p))



# goal: compute: xa = log_{g} ya; xb = log_{g} yb; shared secret: g^{xa xb} mod p
if __name__ == "__main__":
    t = time.process_time()

    p = 5682549022748424631339131913370125786212509227588493537874673173634936008725904358935442101466555561124455782847468955028529037660533553941399408331331403379 #523 bits
    g = 2410497055970432881345493397846112198995088771364307195189734031205605186951241875096796459061741781667380437076874705300974836586165283741668656930807264789 #522 bits
    ya = 973768284341326272301553751114322685324340805902069613339667142187801529585352406975030927008752571917079716318221077758236884342662829402529734009607531649 # 521 bits
    yb = 4149822765985031146554298777122732051870868431387323913747785791685310508836836283702926446817000114868007555717546362425841865173929670156568682745060708314 # 523 bits

    # test(p, g, ya, yb)

    xa = Pollard_rho_DLPsolver(p, g, ya)#p, g, ya
    xb = Pollard_rho_DLPsolver(p, g, yb)#p, g, ya
    print('xa:',xa, ' xb:', xb)
    print('[Answer Test] pow(g, xa, p)==ya and pow(g, xb, p) == yb', pow(g, xa, p) == ya, pow(g, xb, p) == yb)

    shared_secret = pow(ya, xb, p)
    print('Shared Secret:', shared_secret, shared_secret==pow(yb, xa, p), shared_secret==pow(g, xa*xb, p))
    # # print("factor_l:", sorted(factor_l))
    print('[Program total] time used:', time.process_time()-t)