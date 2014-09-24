/*************************************************************
      Random Number Generator (randgen.h)

Copyright (c) 2003, Earz Music Software Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Earz Music Software nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Earz Music Software BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

RGD 02/22/03 -- Initial Coding
RGD 03/26/03 -- Implemented the Mersenne Twister algorithm for random number generation.


**************************************************************/


#ifndef RANDGEN_H__
#define RANDGEN_H__

#include <cstddef>
#include <ctime>
#include <vector>
#include <cassert>



//-----------------
// class definition
//-----------------


/* Period parameters */  

const unsigned N = 624;
const unsigned M  = 397;


#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */
/* for tempering */   
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

class RandGen;


namespace Util
{

    class RandGen 
    {
        // get a psuedo-random number from 0 to (lim - 1)
        unsigned operator () (unsigned lim);
//        unsigned operator () (void);
        //get a normalized pseudo random number
        unsigned skew(unsigned nTableSize, int nSkew); 
        // constructor
        RandGen(unsigned long initSeed = static_cast<unsigned long>(time(0)));   
        int MarkToSkew(int Mark);
        static unsigned long ptgfsr[N]; /* set initial seeds: N = 624 words */
        static unsigned INDEX;
        static unsigned long mag01[2];
        double getNormalized(void);
        friend class ::RandGen;
    };

    //----------------------------
    // inline function definitions
    //----------------------------

    inline RandGen::RandGen(unsigned long seed)
    {
        unsigned long k;
          
        /* setting initial seeds to ptgfsr[N] using     */
        /* the generator Line 25 of Table 1 in          */
        /* [KNUTH 1981, The Art of Computer Programming */
        /*    Vol. 2 (2nd Ed.), pp102]                  */
        ptgfsr[0]= seed & 0xffffffff;
        for (k=1; k<N; k++)
        {
            ptgfsr[k] = (69069 * ptgfsr[k-1]) & 0xffffffff;
        }

    }



    inline int RandGen::MarkToSkew(int Mark)
    {
        int ret = (Mark * -2) + 100;
        return ret;
    }
}


class RandGen
{
public:

    // get a psuedo-random number from 0 to (lim - 1)
    unsigned operator () (unsigned lim = 0xffffffff) 
    {
        unsigned u = s_gen.operator()(lim);
        return u;
    }

    unsigned skew(unsigned nTableSize, int nSkew)
    {
        assert(nTableSize != 0);
        unsigned u = s_gen.skew(nTableSize, nSkew);
        return u;
    }

    //get a normalized pseudo random number
    double getNormalized()
    {
        double d = s_gen.getNormalized();
        return d;
    }

    bool coinFlip() 
    {
        return (operator()(2) == 0);
    }


private:
    static Util::RandGen s_gen;

};





#endif
