/*
* Copyright (c) 2010 Centre for Electronics Design and Technology (CEDT),
*  Indian Institute of Science (IISc) and Laboratory for Cryptologic
*  Algorithms (LACAL), Ecole Polytechnique Federale de Lausanne (EPFL).
*
* Author: Sylvain Pelissier <sylvain.pelissier@gmail.com>
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* - Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* - Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the
*   distribution.
* - Neither the name of INSERT_AFFILIATION_NAME_HERE nor the names of
*   its contributors may be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL STANFORD
* UNIVERSITY OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "AES.h"
#include "printfZ1.h"

module TestAESC{
	uses interface Boot;
	uses interface AES;
    uses interface Timer<TMilli> as TimerBlink;
    uses interface Leds;
}

implementation{

    /*
        Secret key.
    */
	uint8_t K[32] =  {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t i;

    /*
        Array to store the expanded key
    */
	uint8_t exp[240];

	/*
        First plaintext block
    */
    uint8_t in[16] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    /*
        Second plaintext block
    */
    uint8_t in2[16] = {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    /*
        Ciphertext blocks.
    */
    uint8_t out[16],out2[16];
    uint8_t dec[16],dec2[16];

	event void Boot.booted()
	{
        printfz1_init();
        printfz1("Starting the program\n");

        /*
            Key expansion.
        */
        call AES.keyExpansion(exp,K);

        call Leds.led2Toggle();
        call TimerBlink.startPeriodic(1000);
	}

    event void TimerBlink.fired()
    {
        /* Print plain text */
        printfz1("Plain: %d\n", in[0]);
        printfz1("Plain: %d\n", in2[0]);

         call Leds.led0Toggle();

        /*
            First block encryption
        */
        call AES.encrypt(in,exp,out);
        dbg("Boot", "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",out[0],out[1],out[2],out[3],out[4],out[5],out[6],out[7],out[8],out[9],out[10],out[11],out[12],out[13],out[14],out[15]);
        printfz1("Cipher1: %d\n", out[0]);
        /*
            Second block encryption
        */
        call AES.encrypt(in2,exp,out2);
        dbg("Boot", "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",out2[0],out2[1],out2[2],out2[3],out2[4],out2[5],out2[6],out2[7],out2[8],out2[9],out2[10],out2[11],out2[12],out2[13],out2[14],out2[15]);
        printfz1("Cipher2: %d\n", out2[0]);

        call Leds.led1Toggle();

        /*
            First block decryption
        */
        call AES.decrypt(out,exp,dec);
        dbg("Boot", "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",in[0],in[1],in[2],in[3],in[4],in[5],in[6],in[7],in[8],in[9],in[10],in[11],in[12],in[13],in[14],in[15]);
        printfz1("Decipher1: %d\n", dec[0]);

        /*
            Second block decryption
        */
        call AES.decrypt(out2,exp,dec2);
        dbg("Boot", "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",in2[0],in2[1],in2[2],in2[3],in2[4],in2[5],in2[6],in2[7],in2[8],in2[9],in2[10],in2[11],in2[12],in2[13],in2[14],in2[15]);
        printfz1("Decipher2: %d\n", dec2[0]);

        call Leds.led2Toggle();

    }

}
