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
int Alatch, Blatch, Amux;
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

unsigned int RAM[4096];        // This is our RAM.

// Part Nine: Initial values are PC=0 and MPC = 0;
//PC =0;
int opcode, stackop, address,constant;
int i = 0;
// Part Eight: The assembly instructions.  Write the code for each of these functions.
void LODD (int address){AC = RAM[address];}
void LOCO (int address){AC = address;}
void LODL (int address){AC = RAM[SP+address];}

void STOD (int address){RAM[address] = AC;}
void SUBD (int address){AC -= RAM[address];}
void STOL (int address){RAM[SP+address] = AC;}
void SUBL (int address){AC -= RAM[SP+address];}
void SWAP (int address){int temp = AC; AC = SP; SP = temp;}
void STOP (){}

void ADDD (int address){AC += RAM[address];}
void ADDL (int address){AC += RAM[SP+address];}

void JPOS (int address){if(AC >= 0)PC = address;}
void JZER (int address){if(AC == 0)PC = address;}
void JUMP (int address){PC = address;}
void JNEG (int address){if(AC < 0)PC = address;}
void JNZE (int address){if(AC != 0)PC = address;}

void PSHI (int address){SP--; RAM[SP] = RAM[AC];}
void POPI (int address){RAM[AC] = RAM[SP]; SP++;}
void PUSH (int address){SP--; RAM[SP] = AC;}
void POPP (int address){AC = RAM[SP]; SP++;}

void CALL (int address){SP--;RAM[SP] = PC; PC = address;}
void RETN (int address){PC = RAM[SP];SP++;}
void INSP (int address){if(address >= 0 && address <= 255)SP++;}
void DESP (int address){if(address >= 0 && address <= 255)SP--;}

void fetchInstruction()
{
    MAR = PC;
    MBR = RAM[MAR];
    IR  = MBR;
    PC += 1;
}
void decodeInstruction()
{
    opcode   = (IR >> 12);    // Rotate IR 12 times (bits) to the right
    stackop  = (IR >> 0x8) & 0x0007; // Rotate & mask; See machine design for this.
}
void calculateAddress()
{
    address  = (IR & 0x0FFF); // Mask with 0000111111111111
    constant = (IR & 0X00FF); // Mask with 0000000011111111
}
void executeInstruction()
{
    switch (opcode) {
        case 0:
            LODD(address);
            break;
        case 1:
            STOD(address);
            break;
        case 2:
            ADDD(address);
            break;
        case 3:
            SUBD(address);
            break;
        case 4:
            JPOS(address);
            break;
        case 5:
            JZER(address);
            break;
        case 6:
            JUMP(address);
            break;
        case 7:
            LOCO(address);
            break;
        case 8:
            LODL(address);
            break;
        case 9:
            STOL(address);
            break;
        case 10:
            ADDL(address);
            break;
        case 11:
            SUBL(address);
            break;
        case 12:
            JNEG(address);
            break;
        case 13:
            JNZE(address);
            break;
        case 14:
            CALL(address);
            break;
        case 15:
            switch(opcode){
                case 0:
                    PSHI(address);
                    break;
                case 1:
                    POPI(address);
                    break;
                case 2:
                    PUSH(address);
                    break;
                case 3:
                    POPP(address);
                    break;
                case 4:
                    RETN(address);
                    break;
                case 5:
                    SWAP(address);
                    break;
                case 6:
                    INSP(address);
                    break;
                case 7:
                    DESP(address);
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}



const int startAdd = 1024;
const int startData = 2049;


int main()
{
    int data = startData, inst= startAdd, tmpData, tmpInst, count = 0, N = 2048, tempN, result = 3071;

    ifstream inFile;

    char dataOrInst;

    inFile.open("program.txt");

    inFile >> dataOrInst;

    while (dataOrInst != 'E') //While the file is not at the end
    {
        
        if(dataOrInst == 'N'){
            inFile >> tempN;
            RAM[N] = tempN;
        }
        
        if (dataOrInst == 'D') //If dataOrInst is equal to 'D', this means that the variables after it are RAM address and data
        {
            inFile >> tmpData; // Import RAM address and data
            RAM[data] = tmpData; // Assign the values
            data++; // Increment data to allow inserting the next data
        }
        
        else if (dataOrInst == 'I') //if dataOrInst is equal to 'I', this means that the variable after it are RAM address and instructions
        {
            inFile >> hex >> tmpInst; //Import instructions
            RAM[inst] = tmpInst; // Assign the values
            inst++; // Increment inst to allow inserting the next instructions
            count++;
        }
        
        inFile >> dataOrInst; //Input the next line
    }

    inFile.close();

    PC = startAdd;

    while (i != count)
    {
        fetchInstruction();
        decodeInstruction();
        calculateAddress();
        executeInstruction();
        i++;
    }

    int Avg = RAM[3071]/RAM[N];

    cout << "AC: " << AC << endl << "SUM: " << RAM[result] << endl << "Average: " << Avg << endl;

    return 0;
    }

