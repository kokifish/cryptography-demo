import time
import random # random.randint(a, b)    [a, b]

class RSA:
    def __init__(self, bits):#bits: prime number bits count
        self.bits = bits
        self.n = None# n=p*q
        self.p = None# prime number with [bits] bits
        self.q = None# another prime number with [bits] bits
        self.e = None# encrypt key
        self.d = None# decrypt key
        self.generate_npq(bits)


    #return b^(-1) mode a   # if return 0: b has no inverse modulo a
    def MultiplicativeInverse(self,a,b):
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

    # generate a random int that has [bits] bits
    # make sure start with bit 1, end with bit 1
    def generate_random_int(self, bits):
        num = '1'
        for _ in range(bits-2):
            num += random.choice(['0','1'])
        num += '1'
        # print(num, len(num))
        num = int(num, 2)
        # print(num, len(str(bin(num))), type(num), bin(num))
        return num

    # determine a number n is a prime or not using Miller_Rabin algorithm
    def is_prime(self, n:int):
        for i in range(50):
            if(self.Miller_Rabin(n)==False):#composite number
                return False
        return True

    # True: is prime (maybe a composite)
    # False: is composite, then must be a composite
    def Miller_Rabin(self, n:int):
        if(n <= 0):
            return False
        m = n-1
        k = 0
        while(m % 2 == 0): # m = n-1
            k += 1
            m = m >> 1
        a = random.randint(1, n-1)
        # print('m:', m, '\n a:', a)
        b = pow(a,m,n)
        # print('b:', b)
        if(b == 1):
            return True
        for i in range(k):
            if(b == n-1):
                return True
            else:
                b = pow(b,2,n)
        return False # composite


    def generate_prime(self, bits):
        num = 0
        while(True):
            num = self.generate_random_int(bits)
            if(self.is_prime(num) == True):
                break
        return num



    def generate_npq(self, bits):
        self.p = self.generate_prime(bits)
        self.q = self.generate_prime(bits)
        while(self.q == self.p):
            self.q = self.generate_prime(bits)
        self.n = self.p*self.q
        print('p:', self.p, ' len:', len(str(bin(self.p))[2:]))
        print('q:', self.q, ' len:', len(str(bin(self.q))[2:]))
        print('n:', self.n, ' len:', len(str(bin(self.n))[2:]))
        return self.p, self.q


    def str_to_int(self, s):
        out = 0
        for i in range(len(s)):
            out = (out<<8) + ord(s[i])
        return out

    def int_to_str(self, big_int):
        s = str(bin(big_int))[2:]
        while(len(s)%8 != 0):
            s = '0' + s
        out_str = str()
        for i in range(len(s)//8):
            out_str += chr(int(s[i*8:(i+1)*8], 2))
        return out_str

    #n_bytes >= 8, if not: return 0#default: n_bytes=8
    def generate_PS(self, n_bytes=8):
        PS = 0
        if(n_bytes<8):
            return PS
        for i in range(n_bytes):
            PS = (PS<<8) + random.randint(1, (1<<8) - 1) #8bits
        return PS

    #parameter: public key:e; for mode: n
    #using PKCS#1 v1.5 padding
    def encrypt_PKCS1_padding(self, message:str, e=None, n=None, bits=None):
        ''' octets: 八位字节
        RSAES-PKCS1-V1_5-ENCRYPT ((n, e), M)
       Input:
       (n, e)   recipient's RSA public key (k denotes the length in octets
                of the modulus n)
       M        message to be encrypted, an octet string of length mLen,
                where mLen <= k - 11
       Output:
       C        ciphertext, an octet string of length k

       Error: "message too long"

       Steps:

       1. Length checking: If mLen > k - 11, output "message too long" and
          stop.

       2. EME-PKCS1-v1_5 encoding:

          a. Generate an octet string PS of length k - mLen - 3 consisting
             of pseudo-randomly generated nonzero octets.  The length of PS
             will be at least eight octets.

          b. Concatenate PS, the message M, and other padding to form an
             encoded message EM of length k octets as
                    8bits     8bits    64bits                8bits
                EM = 0x00 || 0x02 || PS(at least 8 octets) || 0x00 || M. (M at most byte_of(n) - 11)
        '''
        if(e==None):
            e=self.e
        if(n==None):
            n=self.n
        if(bits==None):
            bits=self.bits
        if(e<=0 or n<=0 or bits<=0):
            return False
        out_l = list()
        print('message: ',message, len(message))
        byte_total = bits*2 // 8 #total bytes of one rsa encrypt block
        max_block_len = byte_total-11
        print('byte_total:', byte_total)

        for i in range(len(message)//max_block_len - 1):
            PS = self.generate_PS() #64bits, 8bytes
            m = ((0x02<<64) + PS) <<8 #the str before M (part of message)
            print('i=', i, bin(m), len(str(bin(m))))
            m = (m<<(byte_total-11)*8) + self.str_to_int(message[i*max_block_len: (i+1)*max_block_len])
            print(bin(m), len(str(bin(m))))
            out_l.append(pow(m, e, n))
        #last 117 bytes or less than 117 bytes
        last_PS_len = (byte_total-3) - (len(message) % max_block_len)
        PS = self.generate_PS(last_PS_len)#last_PS_len count by bytes
        m = ((0x02<<(last_PS_len*8)) + PS) << 8
        m = (m<<(len(message)%max_block_len)*8) + \
                self.str_to_int(message[len(message)//max_block_len * max_block_len:])
        print(bin(m), len(str(bin(m))))
        out_l.append(pow(m, e, n))
        return out_l

    def encrypt_without_padding(self, message:str, e=None, n=None, bits=None):
        if(e==None):
            e=self.e
        if(n==None):
            n=self.n
        if(bits==None):
            bits=self.bits
        if(e<=0 or n<=0 or bits<=0):
            return False
        if(len(message)*8>=2*bits):
            return 0
        m = self.str_to_int(message)
        return pow(m, e, n)

    def decrypt_without_padding(self, c:int, d=None, n=None, bits=None):
        if(d==None):
            d=self.d
        if(n==None):
            n=self.n
        if(bits==None):
            bits=self.bits
        return pow(c, d, n)

if __name__ == '__main__':
    message = 'SUN YAT-SEN UNIVERSITY'
    #message2 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
    rsa = RSA(512)
    print('message to encrypt:', message)
    rsa.e = 16337087
    rsa.d = rsa.MultiplicativeInverse((rsa.p-1)*(rsa.q-1), rsa.e)
    # out_l = rsa.encrypt_PKCS1_padding(message)
    # if(out_l is False):
    #     print('encrypt Error!')
    # for i in range(len(out_l)):
    #     print(i, 'th: ', out_l[i], str(bin(out_l[i])))
    c = rsa.encrypt_without_padding(message)
    print('cipher int c:', c)
    cipher_str = rsa.int_to_str(c)
    print('cipher_str:', cipher_str)

    m = rsa.decrypt_without_padding(c)
    print('m (int):', m)
    print('message after decrypt:', rsa.int_to_str(m))
    print('e:', rsa.e, ' d:', rsa.d, ' p:', rsa.p, ' q:', rsa.q, ' n:', rsa.n)
