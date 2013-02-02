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
 * Binary logarithm
 * 
 * @param   x  The value of which to calculate the binary logarithm
 * @retunr     The binary logarithm
 */
inline long lb(long x)
{
    long rc;
    rc  = ((x & 0xFFFF0000L) != 0) << 4;
    rc += ((x & 0xFF00FF00L) != 0) << 3;
    rc += ((x & 0xF0F0F0F0L) != 0) << 2;
    rc += ((x & 0xCCCCCCCCL) != 0) << 1;
    rc += ((x & 0xAAAAAAAAL) != 0);
    return rc;
}


/**
 * Compute the Keccak[b−r,c,d] sponge function on a message
 * 
 * @param  b  Bitsize
 * @param  r  Bitrate
 * @param  n  Output size
 */
void keccak(long b, long r, long n) /* 1600, 576, 1024 */
{
    long c = b - r; /* capacity */
    long w = b / 25;
    llong wmod = (1LL < w) - 1LL;
    long l = lb(w);
    long nr = 12 + (l << 1);
    llong s[5][5];
    
}

