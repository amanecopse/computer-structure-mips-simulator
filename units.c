#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "defines.h"

#define PROG_START 0x00400000
#define PROG_END 0x00500000
#define DATA_START 0x10000000
#define DATA_END 0x10100000
#define STAK_START 0x7FF00000
#define STAK_END 0x80000000

const int M_SIZE = 0x100000;
const int ACCESS_READ = 0x0; //입출력 설정 상수
const int ACCESS_WRITE = 0x1;
const int BYTE_SIZE = 0x0; //접근데이터 크기 설정 상수
const int HALF_SIZE = 0x1;
const int WORD_SIZE = 0x2;
unsigned char progMEM[0x100000], dataMEM[0x100000], stakMEM[0x100000]; //메모리
int registers[32];                                                     //범용 레지스터

int ALU_hi; //곱셈 유닛에 있는 hi, lo 레지스터
int ALU_lo;

int ALU(int fct, int v1, int v2)
{
    switch (fct)
    {
    case ADD:
        return v1 + v2;
        break;
    case ADDI:
        return v1 + v2;
        break;
    case SUB:
        return v1 - v2;
        break;
    case AND:
        return v1 & v2;
        break;
    case ANDI:
        return v1 & v2;
        break;
    case OR:
        return v1 | v2;
        break;
    case ORI:
        return v1 | v2;
        break;
    case XOR:
        return v1 ^ v2;
        break;
    case XORI:
        return v1 ^ v2;
        break;
    case NOR:
        return ~(v1 | v2);
        break;
    case SLL:
        return v1 << v2;
        break;
    case SRL:
        return (unsigned int)v1 >> v2;
        break;
    case SRA:
        return v1 >> v2;
        break;
    case MUL:
        ALU_hi = (__int64)((__int64)v1 * (__int64)v2) >> 32;
        ALU_lo = (__int64)((__int64)v1 * (__int64)v2) & 0xFFFFFFFF;
        return 1;
        break;
    case SLT:
        return v1 < v2;
        break;
    case SLTI:
        return v1 < v2;
        break;
    case LUI:
        return v2 << 16;
        break;
    }
}

unsigned int MEM(unsigned int A, int V, unsigned int nRW, unsigned int S)
{

    unsigned int memSelect, offset; // A = memSelect << 20 + offset
    unsigned char *pM;
    //	unsigned char cdata;
    //	unsigned short int sdata;
    //	unsigned int idata;

    if (nRW > 1 || S > 2)
    {
        printf("Error: Invalid range of nRW or S");
        fflush(stdout);
        exit(1);
    }

    memSelect = A >> 20;
    offset = A & 0xFFFFF;
    if (memSelect == 0x004)
        pM = progMEM; // program memory
    else if (memSelect == 0x100)
        pM = dataMEM; // data memory
    else if (memSelect == 0x7FF)
        pM = stakMEM; // stack
    else
    {
        printf("Error: No memory with address: %08x\n", A);
        fflush(stdout);
        exit(1);
    }

    if (S == 0)
    { // byte
        if (nRW == 0)
        { // read
            return pM[offset];
        }
        else
        { // write
            pM[offset] = (unsigned char)V;
            return 1;
        }
    }
    else if (S == 1)
    {                                 // half word
        offset = offset & 0xfffffffe; // for aligned access
        if (nRW == 0)
        { // read
            return (pM[offset] << 8) + pM[offset + 1];
        }
        else
        { // write
            pM[offset] = (unsigned char)((V >> 8) & 0xff);
            pM[offset + 1] = (unsigned char)(V & 0xff);
            return 1;
        }
    }
    else if (S == 2)
    {                                 // word
        offset = offset & 0xfffffffc; // for aligned access
        if (nRW == 0)
        { // read
            return (pM[offset] << 24) + (pM[offset + 1] << 16) + (pM[offset + 2] << 8) + pM[offset + 3];
        }
        else
        { // write
            pM[offset] = (unsigned char)((V >> 24) & 0xff);
            pM[offset + 1] = (unsigned char)((V >> 16) & 0xff);
            pM[offset + 2] = (unsigned char)((V >> 8) & 0xff);
            pM[offset + 3] = (unsigned char)(V & 0xff);
            return 1;
        }
    }
    return 0;
}

unsigned int REG(unsigned int A, int V, unsigned int nRW)
{
    if ((nRW > 1) | (A > 31))
    {
        printf("Error: Invalid range of nRW or register address");
        fflush(stdout);
        exit(1);
    }

    if (nRW == 0)
        return registers[A];
    else if (nRW == 1)
        registers[A] = V;
}