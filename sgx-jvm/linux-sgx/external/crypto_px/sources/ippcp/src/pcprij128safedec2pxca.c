/*
* Copyright (C) 2016 Intel Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   * Neither the name of Intel Corporation nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "owndefs.h"
#include "owncp.h"

#include "pcprij128safe2.h"
#include "pcprijtables.h"


__INLINE Ipp8u getInvSboxValue(Ipp32u x)
{
   Ipp32u t[sizeof(RijDecSbox)/CACHE_LINE_SIZE];
   const Ipp8u* SboxEntry = RijDecSbox +x%CACHE_LINE_SIZE;
   Ipp32u i;
   for(i=0; i<sizeof(RijDecSbox)/CACHE_LINE_SIZE; i+=4, SboxEntry += 4*CACHE_LINE_SIZE) {
      t[i]   = SboxEntry[CACHE_LINE_SIZE*0];
      t[i+1] = SboxEntry[CACHE_LINE_SIZE*1];
      t[i+2] = SboxEntry[CACHE_LINE_SIZE*2];
      t[i+3] = SboxEntry[CACHE_LINE_SIZE*3];
   }
   return (Ipp8u)t[x/CACHE_LINE_SIZE];
}

__INLINE void invSubBytes(Ipp8u state[])
{
   int i;
   for(i=0;i<16;i++)
      state[i] = getInvSboxValue((Ipp32u)state[i]);
}

__INLINE void invShiftRows(Ipp32u* state)
{
   state[1] =  ROR32(state[1], 24);
   state[2] =  ROR32(state[2], 16);
   state[3] =  ROR32(state[3],  8);
}

__INLINE void invMixColumns(Ipp32u* state)
{
   Ipp32u y0 = state[1] ^ state[2] ^ state[3];
   Ipp32u y1 = state[0] ^ state[2] ^ state[3];
   Ipp32u y2 = state[0] ^ state[1] ^ state[3];
   Ipp32u y3 = state[0] ^ state[1] ^ state[2];
   Ipp32u t02, t13, t0123;

   state[0] = xtime4(state[0]);
   state[1] = xtime4(state[1]);
   state[2] = xtime4(state[2]);
   state[3] = xtime4(state[3]);

   y0 ^= state[0] ^ state[1];
   y1 ^= state[1] ^ state[2];
   y2 ^= state[2] ^ state[3];
   y3 ^= state[3] ^ state[0];

   t02 = state[0] ^ state[2];
   t13 = state[1] ^ state[3];
   t02 = xtime4(t02);
   t13 = xtime4(t13);

   t0123 = t02^t13;
   t0123 = xtime4(t0123);

   state[0] = y0 ^t02 ^t0123;
   state[1] = y1 ^t13 ^t0123;
   state[2] = y2 ^t02 ^t0123;
   state[3] = y3 ^t13 ^t0123;
}


void Safe2Decrypt_RIJ128(const Ipp8u* in,
                               Ipp8u* out,
                               int Nr,
                               const Ipp8u* RoundKey,
                               const void* sbox)
{
   Ipp32u state[4];

   int round=0;

   UNREFERENCED_PARAMETER(sbox);

   // copy input to the state array
   TRANSPOSE((Ipp8u*)state, in);

   // add the round key to the state before starting the rounds.
   XorRoundKey((Ipp32u*)state, (Ipp32u*)(RoundKey+Nr*16));

   // there will be Nr rounds
   for(round=Nr-1;round>0;round--) {
      invShiftRows(state);
      invSubBytes((Ipp8u*)state);
      XorRoundKey(state,(Ipp32u*)(RoundKey+round*16));
      invMixColumns(state);
    }

   // last round
   invShiftRows(state);
   invSubBytes((Ipp8u*)state);
   XorRoundKey(state,(Ipp32u*)(RoundKey+0*16));

   // copy from the state to output
   TRANSPOSE(out, (Ipp8u*)state);
}
