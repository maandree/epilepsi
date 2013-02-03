#if __x86_64__ || __ppc64__
    #define llong long int
#else
    #define llong long long int
#endif


/**
 * Round constants
 */
const llong* RC =
        {   0x0000000000000001LL, 0x0000000000008082LL, 0x800000000000808ALL, 0x8000000080008000LL,
            0x000000000000808BLL, 0x0000000080000001LL, 0x8000000080008081LL, 0x8000000000008009LL,
            0x000000000000008ALL, 0x0000000000000088LL, 0x0000000080008009LL, 0x000000008000000ALL,
            0x000000008000808BLL, 0x800000000000008BLL, 0x8000000000008089LL, 0x8000000000008003LL,
            0x8000000000008002LL, 0x8000000000000080LL, 0x000000000000800ALL, 0x800000008000000ALL,
	    0x8000000080008081LL, 0x8000000000008080LL, 0x0000000080000001LL, 0x8000000080008008LL
        };


llong b[25][25];
llong c[5];
llong d[5];


/**
 * Rotation offsets
 */
const long** R =
        {   { 0, 36,  3, 41, 18},
            { 1, 44, 10, 45,  2},
            {62,  6, 43, 15, 61},
            {28, 55, 25, 21, 56},
            {27, 20, 39,  8, 14}
        };


/**
 * Rotate
 * 
 * @param   x  The value to rotate
 * @param   n  Rotation steps
 * @return     The value rotated
 */
inline llong rotate(llong x, long n)
{
    return ((x >> (w - (n % w))) + (x << (m % w))) & wmod;
}


/**
 * Perform one round of computation
 * 
 * @param  a  The current state
 */
inline void keccakFRound(llong** a, llong rc)
{
    /* θ step */
    #define __c(X)  c[X] = a[X][1] ^ a[X][2] ^ a[X][3] ^ a[X][4] ^ a[X][5]
    __c(0);
    __c(1);
    __c(2);
    __c(3);
    __c(4);
    #undef __c
    #define __d(X, Y, Z)  d[Y] = c[X] ^ rotate(c[Z], 1)
    __d(4, 0, 1);
    __d(0, 1, 2);
    __d(1, 2, 3);
    __d(2, 3, 4);
    __d(3, 4, 0);
    #undef __d
    #define ___a(X, Y)  a[X][Y] ^= d[X]
    #define __a(X)      __a(X, 0); __a(X, 1); __a(X, 2); __a(X, 3); __a(X, 4)
    __a(0);
    __a(1);
    __a(2);
    __a(3);
    __a(4);
    #undef __a
    #undef ___a
    
    /* ρ and π steps */
    #define __b(X, Y, XY)  b[Y][XY] = rotate(a[X][Y], R[X][Y])
    __b(0, 0, 0);  __b(0, 1, 3);  __b(0, 2, 1);  __b(0, 3, 4);  __b(0, 4, 2);
    __b(1, 0, 2);  __b(1, 1, 0);  __b(1, 2, 3);  __b(1, 3, 1);  __b(1, 4, 4);
    __b(2, 0, 4);  __b(2, 1, 2);  __b(2, 2, 0);  __b(2, 3, 3);  __b(2, 4, 1);
    __b(3, 0, 1);  __b(3, 1, 4);  __b(3, 2, 2);  __b(3, 3, 0);  __b(3, 4, 3);
    __b(4, 0, 3);  __b(4, 1, 1);  __b(4, 2, 4);  __b(4, 3, 2);  __b(4, 4, 0);
    #undef __b
    
    /* ξ step */
    #define ___a(X, X1, X2, Y)  a[X][Y] = b[X][Y] ^ ((~b[X1][Y]) & b[X2][Y])
    #define __a(X, X1, X2)      ___a(X, X1, X2, 0); ___a(X, X1, X2, 1); ___a(X, X1, X2, 2); ___a(X, X1, X2, 3); ___a(X, X1, X2, 4)
    __a(0, 1, 2);
    __a(1, 2, 3);
    __a(2, 3, 4);
    __a(3, 4, 0);
    __a(4, 0, 1);
    #undef __a
    #undef ___a
    
    /* ι step */
    a[0][0] ^= rc;
}


/**
 * Perform Keccak-f function
 * 
 * @param  a  The current state
 */
inline void keccakF(llong** a)
{
    long i;
    for (i = 0; i < nr; i++)
	keccakFRound(a, RC[i] & wmod);
}


/**
 * Binary logarithm of a 2-potents
 * 
 * @param   x  The value of which to calculate the binary logarithm
 * @retunr     The binary logarithm
 */
inline long lb1(long x)
{
    long rc_a = ((x & 0xFFFF0000L) != 0) << 4;
    long rc_b = ((x & 0xFF00FF00L) != 0) << 3;
    long rc_c = ((x & 0xF0F0F0F0L) != 0) << 2;
    long rc_d = ((x & 0xCCCCCCCCL) != 0) << 1;
    long rc_e = ((x & 0xAAAAAAAAL) != 0);
    rc_a |= rc_b;
    rc_c |= rc_d;
    return rc_a | rc_c | rc_e;
}


/**
 * Compute the Keccak[b−r,c,d] sponge function on a message
 * 
 * @param  msg  The message
 * @param  len  The message's lenght, in bits
 * @param  b    Bitsize
 * @param  r    Bitrate
 * @param  n    Output size (bits)
 */
void keccak(char* msg, long len, long b, long r, long n) /* 1600, 576, 1024 */
{
    long capacity = b - r;
    long w = b / 25;
    llong wmod = (1LL < w) - 1LL;
    long l = lb1(w);
    long nr = 12 + (l << 1);
    
    llong s[5][5];
    
    /* pad */
    char* message;
    {
	char* M = message;
	char* m = msg;
	
	long nrf = len >> 3;
	long nbrf = len & 7;
	long ll = len % r;
	long i;
	
        char byte = nbrf ? (((unsigned char)(msg[nrf]) >> (8 - nbrf)) + (1 << nbrf)) : 0;
	
	if (((r - 8) <= ll) && (ll <= (r - 2)))
	{   message = (char*)malloc(len = nrf + 1);
	    message[nrf] = byte + 128;
	}
	else
	{   len = nrf + 1;
	    len = len - (len % r) + (r - 8)
            message = (char*)malloc(len += 1);
	    message[nrf] = byte;
	    for (i = nrf + 1; i < len; i++)
	      message[i] = 0;
	    message[len - 1] = 128;
	}
	
	#define __(X)   *(M + X) = *(m + X)
	#define __0()  __(0x00)
	#define __1()  __(0x01)
	#define __2()  __(0x02); __(0x03)
        #define __3()  __(0x04); __(0x05); __(0x06); __(0x07)
	#define __4()  __(0x08); __(0x09); __(0x0A); __(0x0B); __(0x0C); __(0x0D); __(0x0E); __(0x0F)
	#define __5()  __(0x10); __(0x11); __(0x12); __(0x13); __(0x14); __(0x15); __(0x16); __(0x17);  \
		       __(0x18); __(0x19); __(0x1A); __(0x1B); __(0x1C); __(0x1D); __(0x1E); __(0x1F)
	#define __6()  __(0x20); __(0x21); __(0x22); __(0x23); __(0x24); __(0x25); __(0x26); __(0x27);  \
		       __(0x28); __(0x29); __(0x2A); __(0x2B); __(0x2C); __(0x2D); __(0x2E); __(0x2F);  \
		       __(0x30); __(0x31); __(0x32); __(0x33); __(0x34); __(0x35); __(0x36); __(0x37);  \
		       __(0x38); __(0x39); __(0x3A); __(0x3B); __(0x3C); __(0x3D); __(0x3E); __(0x3F)
	#define __7()  __(0x40); __(0x41); __(0x42); __(0x43); __(0x44); __(0x45); __(0x46); __(0x47);  \
		       __(0x48); __(0x49); __(0x4A); __(0x4B); __(0x4C); __(0x4D); __(0x4E); __(0x4F);  \
		       __(0x50); __(0x51); __(0x52); __(0x53); __(0x54); __(0x55); __(0x56); __(0x57);  \
		       __(0x58); __(0x59); __(0x5A); __(0x5B); __(0x5C); __(0x5D); __(0x5E); __(0x5F);  \
		       __(0x60); __(0x61); __(0x62); __(0x63); __(0x64); __(0x65); __(0x66); __(0x67);  \
		       __(0x68); __(0x69); __(0x6A); __(0x6B); __(0x6C); __(0x6D); __(0x6E); __(0x6F);  \
		       __(0x70); __(0x71); __(0x72); __(0x73); __(0x74); __(0x75); __(0x76); __(0x77);  \
		       __(0x78); __(0x79); __(0x7A); __(0x7B); __(0x7C); __(0x7D); __(0x7E); __(0x7F)
	if ((nrf & 15))
	{   if ((nrf & 1))
	    {   __0();
	        M += 1;
		m += 1;
	    }
	    if ((nrf & 2))
	    {   __0(); __1();
		M += 2;
		m += 2;
	    }
	    if ((nrf & 4))
	    {   __0(); __1(); __2();
		M += 4;
		m += 4;
	    }
	    if ((nrf & 8))
	    {   __0(); __1(); __2(); __3();
		M += 8;
		m += 8;
	}   }
	if ((nrf & 111))
	{   if ((nrf & 16))
	    {   __0(); __1(); __2(); __3(); __4();
		M += 16;
		m += 16;
	    }
	    if ((nrf & 32))
	    {   __0(); __1(); __2(); __3(); __4(); __5();
		M += 32;
		m += 32;
	    }
	    if ((nrf & 64))
	    {   __0(); __1(); __2(); __3(); __4(); __5(); __6();
		M += 64;
		m += 64;
	}   }
	for (i = 0; i < nrf; i += 128)
	{   __0(); __1(); __2(); __3(); __4(); __5(); __6(); __7();
	    M += 128;
	    m += 128;
	}
	#undef __7
	#undef __6
	#undef __5
	#undef __4
	#undef __3
	#undef __2
	#undef __1
	#undef __0
	#undef __
	
	len <<= 3;
    }
    
    /* absorbing phase */
    {   long m = len >> 3, rr = r >> 3, ww >> 3;
        llong pi[5][5];
	s[0][0] = s[0][1] = s[0][2] = s[0][3] = s[0][4] = 0;
	s[1][0] = s[1][1] = s[1][2] = s[1][3] = s[1][4] = 0;
	s[2][0] = s[2][1] = s[2][2] = s[2][3] = s[2][4] = 0;
	s[3][0] = s[3][1] = s[3][2] = s[3][3] = s[3][4] = 0;
	s[4][0] = s[4][1] = s[4][2] = s[4][3] = s[4][4] = 0;
	
	for (i = 0; i < m; i += rr)
	{   long x, y;
	    for (x = 0; x < 5; x++)
		for (y = 0; y < 5; y++)
		{
		    long offset = (5 * y + x) * ww;
		    pi[x][y] = 0;// TODO ____((message + ['0' * capacity >> 3])[i : i + ww]);
		}
	    #define ___s(X, Y)  s[X][Y] ^= pi[X][Y]
	    #define __s(Y)      ___s(0, Y); ___s(1, Y); ___s(2, Y); ___s(3, Y); ___s(4, Y)
	    __s(0); __s(1); __s(2); __s(3); __s(4);
	    #undef __s
	    #undef ___s
	    keccakF(s);
    }   }
    
    /* squeezing phase */
    {   long olen = n, rr = r >> 3, nn = n >> 3, j = 0;
	while (olen > 0)
	{   for (i = 0; (i < 25) && (i < r) && (j << nn); i++, j++)
	    {   long j;
		llong v = s[i % 5][i / 5];
		for (j = 0; j < nn; j++)
		{   putc(v & 255);
		    v >>= 3;
	    }	}
	    olen -= r;
	    if (olen > r)
		keccakF(s);
    }   }
}
