/*************************************************************
Random Number Generator (randgen.cpp)

Copyright (c) 2003, Earz Music Software Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Earz Music Software nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL EARZ MUSIC SOFTWARE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

RGD 02/22/03 -- Initial Coding
RGD 03/26/03 -- Implemented the Mersenne Twister algorithm for random number generation.

**************************************************************/

#include <cassert>
#include <algorithm>
#include "randgen.h"







unsigned long Util::RandGen::ptgfsr[N];
unsigned long Util::RandGen::mag01[2] = {0x0, MATRIX_A};
unsigned Util::RandGen::INDEX = 0;
Util::RandGen RandGen::s_gen;




unsigned Util::RandGen::operator () (unsigned lim)
{

    unsigned long y;

    if(INDEX == N)
    { /* generate N words at one time */
        unsigned long kk;
        for (kk = 0; kk < N - M; kk++) 
        {
            y = (ptgfsr[kk] & UPPER_MASK)|(ptgfsr[kk + 1] & LOWER_MASK);
            ptgfsr[kk] = ptgfsr[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (;kk < N - 1; kk++) 
        {
            y = (ptgfsr[kk] & UPPER_MASK) | (ptgfsr[kk + 1 ]& LOWER_MASK);
            ptgfsr[kk] = ptgfsr[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (ptgfsr[N - 1] & UPPER_MASK) | (ptgfsr[0] & LOWER_MASK);
        ptgfsr[N - 1] = ptgfsr[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];
        INDEX = 0;
    }

    y = ptgfsr[INDEX++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y &= 0xffffffff; /* you may delete this line if word size = 32 */
    y ^= TEMPERING_SHIFT_L(y);
    
    return y % lim;

}

/*=============================================================================================
skew() takes an unsigned representing the size of a list in which one element is to 
be chosen randomly and a int that represents the skewing applied to the the 
randomness. Lower skew values approaching 0 favour returns closer to index 0.  
Higher skew values approaching 100 favour returns closer to (nTableSize - 1). 

ARGLIST:
    -  nSizeOfTable:  The size of the list from which elements will be chosen.
    -  nSkewValue: The percentage that the frequency of the first element compared
       to the average frequency.
RETURN:
    -  an unsigned >= 0 and < nSizeOfTable that is the index of the randomly chosen element.

===============================================================================================*/



unsigned Util::RandGen::skew(unsigned nTableSize, int nSkew)
{
    assert(nTableSize > 0);
    if(nTableSize == 1)
        return 0;   

    //adjust skew to (0, 100)
    nSkew = std::min(std::max(nSkew, 0), 100);
    
    nSkew = MarkToSkew(nSkew);
    unsigned nCounter = 0;
    double fDecrement, fFirstValue, fRandomValue;    
    //Calculate the average frequency
    fFirstValue = 1 / static_cast<double>(nTableSize);

    //Calculate the average of the first list element.
    fFirstValue *= (1.0 + (static_cast<double>(nSkew)) / 100.0);

    //Find the size of the excess if fFirstValue is extrapolated across the field.
    fDecrement = fFirstValue * static_cast<double>(nTableSize) - 1.0;

    //Calculate the size of the decrement by dividing the excess by the number
    //of decrements needed.

    fDecrement /= static_cast<double>(nTableSize * (nTableSize - 1) / 2);
    fRandomValue = getNormalized();

    
    //Subtract the frequency of the first value plus the adjustment depending on which iteration
    //If the Random value fall to 0.0 or below, that index is chosen.
    do
    {      
        fRandomValue -= (fFirstValue - (static_cast<double>(nCounter) * fDecrement));
        if(fRandomValue <= 0.0)
		{
            return nCounter;
		}
        nCounter++;
    }while(true);
}

double Util::RandGen::getNormalized(void)
{
    double v = operator()(0xffffffff);
    return v / static_cast<double>(0xffffffff);
}



