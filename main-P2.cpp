//
//  main.cpp
//  CPU Simulator
//
//  Created by Abdulahi Osoble on 3/8/16.
//  Copyright © 2016 Abdulahi Osoble. All rights reserved.
//

#include <iostream>
#include <ctype.h>
#include <fstream>
using namespace std;

// Machine CPU Architecture is described below
// Part one: The System Registers.

int PC, AC, SP, IR, TIR, MAR, MBR;
const int Zero = 0;
const int PlusOne = 1;
const int MinusOne = -1;
const int AMASK = 0773;
const int BMASK = 066;

// Part Two: The User's General Purpose Registers.
int A, B, C, D, E, F;

// Part Three: Hardware Components.
int ALU, Shifter;

// Part Four: The System Latches.
int Alatch, Blatch, AMUX;
bool z0, z1;
// Part Five: The Buses.
int Abus, Bbus, Cbus, AddressOut, DataIn, DataOut;
int RAddress;
int N,Z;
// Part Six: The Hardware for ROM.
int Mmux;
int MPC = 0;
int MIR;
int Increment;
int ENC;
// Part Seven: The Size of RAM and ROM.
unsigned int ROM[80] =
{                                0x00C00000,
                                 0x00506000,
                                 0xB013001C,
                                 0x24143313,
                                 0x3414040B,
                                 0x30000409,
                                 0x00C03000,
                                 0x00000000,
                                 0xF0110000,
                                 0x11A03100,
                                 0x60200000,
                                 0x3000040F,
                                 0x00C03000,
                                 0x00000000,
                                 0xE0111000,
                                 0x11003000,
                                 0x00116100,
                                 0x985A0000,
                                 0x60111A00,
                                 0x34140419,
                                 0x30000417,
                                 0x30000100,
                                 0x68103800,
                                 0x50000116,
                                 0x70000000,
                                 0x3000041B,
                                 0x68113800,
                                 0x68113800,
                                 0x24043328,
                                 0x34040423,
                                 0x30004021,
                                 0x000A2320,
                                 0x70C0A007,
                                 0x000A2322,
                                 0x71B0A10A,
                                 0x3000400D,
                                 0x000A2325,
                                 0x70C0A00D,
                                 0x000A3200,
                                 0x60C0A010,
                                 0x3404042E,
                                 0x5000042C,
                                 0x30000116,
                                 0x60000000,
                                 0x50000100,
                                 0x68003800,
                                 0x34040432,
                                 0x00022700,
                                 0x01B02000,
                                 0x68203800,
                                 0x34040441,
                                 0x3404043B,
                                 0x30000438,
                                 0x10400000,
                                 0x00522700,
                                 0x70A0000A,
                                 0X00C22600,
                                 0X00400000,
                                 0x70B0100A,
                                 0x3000043E,
                                 0x00022700,
                                 0x71B0210A,
                                 0x00C22600,
                                 0x00400000,
                                 0xF1110000,
                                 0x34040449,
                                 0x30000446,
                                 0x00C22600,
                                 0x00400000,
                                 0xF1000000,
                                 0x100A0100,
                                 0x10010200,
                                 0x70120A00,
                                 0x3000044C,
                                 0x081A9300,
                                 0x6012A200,
                                 0x081A9300,
                                 0x180A0A00,
                                 0x600AA64B
}; // This is our ROM.

unsigned int RAM[4096];        // This is our RAM.

// Part Nine: Initial values are PC=0 and MPC = 0;
//PC =0;
int opcode, stackop, address,constant;
int i = 0;
int COND,SH,RD,WR;
// Part Eight: The assembly instructions.  Write the code for each of these functions.

void subcycle1()
{
        MIR = ROM[MPC];
        MPC++;

        AMUX  = MIR>>31;
        COND  = (MIR>>29) & 0x00000003;
        ALU   = (MIR>>27) & 0x00000003;
        SH    = (MIR>>25) & 0x00000003;
        MBR   = (MIR>>24) & 0x00000001;
        MAR   = (MIR>>23) & 0x00000001;
        RD    = (MIR>>22) & 0x00000001;
        WR    = (MIR>>21) & 0x00000001;
        ENC   = (MIR>>20) & 0x00000001;
        Cbus  = (MIR>>16) & 0x0000000F;
        Bbus  = (MIR>>12) & 0x0000000F;
        Abus  = (MIR>>8)  & 0x0000000F;
        RAddress =  MIR   & 0x000000FF;

}



void subcycle2()
{
    fAbus();
    fBbus();

    fAlatch();
    fBlatch();
}


void subcycle3()
{
    fMAR(MAR);
    fRD(RD);
}

void subcycle4()
{
    fAMUX(AMUX);
    fALU(ALU);
    RAddress = MIR & 0x000000FF;
    fNZ();
    fCOND(COND);
    fSH(SH);
    fMBR(MBR);
    fWR(WR);
    fENC(ENC);
    fCbus(Cbus);


}


//  ####################################### Function Bodies ##############################################


void fAMUX(int n)
{
    if(n==0)
        Amux = Alatch;
    else if(n==1)
        Amux = MBR;
}

void fCOND(int n)
{
    switch(n)
    {
        case 0: Mmux++; break;
        case 1: if(N==1) Mmux = RAddress; break;
        case 2: if(Z==1) Mmux = RAddress; break;
        case 3: Mmux = RAddress; break;

    }
}

void fALU(int n)
{
    switch(n)
    {
        case 0: ALU = Amux + Blatch; break;
        case 1: ALU = Amux && Blatch; break;
        case 2: ALU = Amux; break;
        case 3: ALU = !Amux; break;
    }
}

void fSH(int n)
{
    switch(n)
    {
        case 0: Shifter = ALU; break;
        case 1: Shifter = Rshift1(ALU); break;
        case 2: Shifter = Lshift1(ALU); break;
    }
}


void fMBR(int n)
{
    if(n==1)
    MBR = Shifter;

}



void fMAR(int n)
{
    if(n==1)
    MAR = Blatch;

}


void fRD(int n)
{
    if(n==1)
        MBR = RAM[MAR];

}


void fWR(int n)
{
    if(n==1)
        RAM[MAR]=MBR;

}


void fENC(int n){
    ENC = n;
}


void fCbus()
{
    if(ENC == 1)
        {
          switch(Cbus)
          {
            case 0: PC = Cbus;
                break;
            case 1: AC = Cbus;
                break;
            case 2: SP = Cbus;
                break;
            case 3: IR = Cbus;
                break;
            case 4: TIR = Cbus;
                break;
            case 10: A = Cbus;
                break;
            case 11: B = Cbus;
                break;
            case 12: C = Cbus;
                break;
            case 13: D = Cbus;
                break;
            case 14: E = Cbus;
                break;
            case 15: F = Cbus;
                break;
          }
       }
}




void fBbus(){
    switch(Bbus){
        case 0: Bbus = PC;
            break;
        case 1: Bbus = AC;
            break;
        case 2: Bbus = SP;
            break;
        case 3: Bbus = IR;
            break;
        case 4: Bbus = TIR;
            break;
        case 5: Bbus = Zero;
            break;
        case 6: Bbus = PlusOne;
            break;
        case 7: Bbus = MinusOne;
            break;
        case 8: Bbus = AMASK;
            break;
        case 9: Bbus = BMASK;
            break;
        case 10: Bbus = A;
            break;
        case 11: Bbus = B;
            break;
        case 12: Bbus = C;
            break;
        case 13: Bbus = D;
            break;
        case 14: Bbus = E;
            break;
        case 15: Bbus = F;
            break;
    }
}



void fAbus(){
    switch(Abus){
        case 0: Abus = PC;
            break;
        case 1: Abus = AC;
            break;
        case 2: Abus = SP;
            break;
        case 3: Abus = IR;
            break;
        case 4: Abus = TIR;
            break;
        case 5: Abus = Zero;
            break;
        case 6: Abus = PlusOne;
            break;
        case 7: Abus = MinusOne;
            break;
        case 8: Abus = AMASK;
            break;
        case 9: Abus = BMASK;
            break;
        case 10: Abus = A;
            break;
        case 11: Abus = B;
            break;
        case 12: Abus = C;
            break;
        case 13: Abus = D;
            break;
        case 14: Abus = E;
            break;
        case 15: Abus = F;
            break;
    }
}




void fAlatch()
{
        Alatch = Abus;
}
void fBlatch()
{

        Blatch = Bbus;
}


void fNZ()
{
    if(ALU==0) Z = 1;
    else N = 1;
}


int Lshift2(int o1, int o2)
{
    return ((o1+o2) << 1);
}

int Lshift1(int o)
{
    return o << 1;
}

int Rshift1(int o)
{
    return o >> 1;
}




int main()
{

    MPC = 0;

        for(int i = 0; i <; i++)
    {
        subcycle1();
        subcycle2();
        subcycle3();
        subcycle4();

    }



    return 0;
    }

