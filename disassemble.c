#include "stdio.h"
#include "defines.h"
#include "units.c"

extern unsigned int PC;

const char *instName[] = {"nop", "sll", "srl", "sra", "slt", "mfhi", "mflo", "mul",
                          "add", "sub", "jr", "syscall", "and", "or", "xor", "nor",
                          "lb", "sb", "lw", "sw", "slti", "bltz", "j", "jal",
                          "addi", "lbu", "beq", "bne", "andi", "ori", "xori", "lui"};

void execShift(unsigned int op, unsigned int rd, unsigned int rt, unsigned int sht)
{
    printf("%s  $%d, $%d, %d\n", instName[op], rd, rt, sht);
    int v1 = REG(rt, 0, ACCESS_READ);
    int result = ALU(op, v1, sht);
    REG(rt, result, ACCESS_WRITE);
}

void execJr(unsigned int op, unsigned int rs)
{
    printf("%s  $%d\n", instName[op], rs);
    PC = REG(rs, 0, ACCESS_READ);
}

void execSyscall(unsigned int op)
{
    printf("%s\n", instName[op]);
}

void execMfHiLo(unsigned int op, unsigned int rd)
{
    printf("%s $%d\n", instName[op], rd);
    int result;
    if (op == MFHI)
        result = ALU_hi;
    else if (op == MFLO)
        result = ALU_lo;
    REG(rd, result, ACCESS_WRITE);
}

void execMul(unsigned int op, unsigned int rs, unsigned int rt)
{
    printf("%s  $%d, $%d\n", instName[op], rs, rt);
    int v1 = REG(rs, 0, ACCESS_READ);
    int v2 = REG(rt, 0, ACCESS_READ);
    ALU(op, v1, v2);
}

void execALU(unsigned int op, unsigned int rd, unsigned int rs, unsigned int rt)
{
    printf("%s  $%d, $%d, $%d\n", instName[op], rd, rs, rt);
    int v1 = REG(rs, 0, ACCESS_READ);
    int v2 = REG(rt, 0, ACCESS_READ);
    int result = ALU(op, v1, v2);
    REG(rd, result, ACCESS_WRITE);
}

void execALUI(unsigned int op, unsigned int rt, unsigned int rs, unsigned int immediate)
{
    if (op == LUI)
        printf("%s  $%d, %d\n", instName[op], rt, immediate);
    else
        printf("%s  $%d, $%d, %d\n", instName[op], rt, rs, immediate);
    int v1 = REG(rs, 0, ACCESS_READ);
    int result = ALU(op, v1, immediate);
    REG(rt, result, ACCESS_WRITE);
}

void execBranch(unsigned int op, unsigned int rs, unsigned int rt, int offset)
{
    if (op == BLTZ)
    {
        printf("%s  $%d, %d\n", instName[op], rs, offset << 2); // print shifted offset
        int v1 = REG(rs, 0, ACCESS_READ);
        if (v1 < 0)
            PC = ALU(ADD, PC, offset << 2);
    }
    else
    {
        printf("%s  $%d, $%d, %d\n", instName[op], rs, rt, offset << 2); // print shifted offset
        int v1 = REG(rs, 0, ACCESS_READ);
        int v2 = REG(rt, 0, ACCESS_READ);
        int result = ALU(SUB, v1, v2);
        if (result < 0)
            PC = ALU(ADD, PC, offset << 2);
    }
}

void execJump(unsigned int op, unsigned int offset)
{
    printf("%s 0x%08X\n", instName[op], offset << 2); // print shifted offset
    PC = ALU(ADD, PC, offset << 2);
}

void execLoadStore(unsigned int op, unsigned int rt, unsigned int rs, int offset)
{
    printf("%s  $%d, %d($%d)\n", instName[op], rt, offset, rs);
    if (instName[op] == "lb")
    {
        unsigned int addr = REG(rs, 0, ACCESS_READ);
        addr = ALU(ADD, addr, offset);
        int result = MEM(addr, 0, ACCESS_READ, BYTE_SIZE);
        char sign = result >> 7; // sign extention
        result = ((sign << 31) | 0x7F) & result;
        REG(rt, result, ACCESS_WRITE);
    }
    else if (instName[op] == "lbu")
    {
        unsigned int addr = REG(rs, 0, ACCESS_READ);
        addr = ALU(ADD, addr, offset);
        int result = MEM(addr, 0, ACCESS_READ, BYTE_SIZE);
        REG(rt, result, ACCESS_WRITE);
    }
    else if (instName[op] == "lw")
    {
        unsigned int addr = REG(rs, 0, ACCESS_READ);
        addr = ALU(ADD, addr, offset);
        int result = MEM(addr, 0, ACCESS_READ, WORD_SIZE);
        REG(rt, result, ACCESS_WRITE);
    }
    else if (instName[op] == "sb")
    {
        unsigned int addr = REG(rs, 0, ACCESS_READ);
        addr = ALU(ADD, addr, offset);
        int result = REG(rt, 0, ACCESS_READ);
        MEM(addr, result, ACCESS_WRITE, BYTE_SIZE);
    }
    else if (instName[op] == "sw")
    {
        unsigned int addr = REG(rs, 0, ACCESS_READ);
        addr = ALU(ADD, addr, offset);
        int result = REG(rt, 0, ACCESS_READ);
        MEM(addr, result, ACCESS_WRITE, WORD_SIZE);
    }
}
