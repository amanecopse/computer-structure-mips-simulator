#include <stdio.h>
#include "units.c"
#include "defines.h"
#include "disassemble.c"

extern unsigned int PC;
extern union instructionRegister IR;

void decodeRtype(unsigned int fct);

void step(void)
{
    // instruction fetch
    // IR = MEM(PC, 0, 0, 2);
    IR.I = 0xAC620014; // 테스트 명령어 1010_1100_0110_0010_0000_0000_0001_0100
    PC += 4;

    // instruction decode
    unsigned int opc, fct;

    unsigned int opch, opcl;

    opc = IR.RI.opc;
    fct = IR.RI.fct;

    printf("Opc: %2x, Fct: %2x, Inst: ", opc, fct);

    opcl = opc & 0x7;
    opch = (opc & 0x38) >> 3;

    if (opch == 0)
    {
        if (opcl == 0)
        {
            decodeRtype(fct);
        }
        else if (opcl == 1)
            execBranch(BLTZ, IR.II.rs, IR.II.rt, IR.II.offset);
        else if (opcl == 2)
            execJump(J, IR.JI.jval);
        else if (opcl == 3)
            execJump(JAL, IR.JI.jval);
        else if (opcl == 4)
            execBranch(BEQ, IR.II.rs, IR.II.rt, IR.II.offset);
        else if (opcl == 5)
            execBranch(BNE, IR.II.rs, IR.II.rt, IR.II.offset);
        else
            printf("Undefined instruction\n");
    }
    else if (opch == 1)
    {
        if (opcl == 0)
            execALUI(ADDI, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 2)
            execALUI(SLTI, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 4)
            execALUI(ANDI, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 5)
            execALUI(ORI, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 6)
            execALUI(XORI, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 7)
            execALUI(LUI, IR.II.rt, IR.II.rs, IR.II.offset);
        else
            printf("Undefined instruction\n");
    }
    else if (opch == 4)
    {
        if (opcl == 0)
            execLoadStore(LB, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 3)
            execLoadStore(LW, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 4)
            execLoadStore(LBU, IR.II.rt, IR.II.rs, IR.II.offset);
        else
            printf("Undefined instruction\n");
    }
    else if (opch == 5)
    {
        if (opcl == 0)
            execLoadStore(SB, IR.II.rt, IR.II.rs, IR.II.offset);
        else if (opcl == 3)
            execLoadStore(SW, IR.II.rt, IR.II.rs, IR.II.offset);
        else
            printf("Undefined instruction\n");
    }
    else
        printf("Undefined instruction\n");
}

void decodeRtype(unsigned int fct)
{
    unsigned int fcth, fctl;

    fctl = fct & 0x7;
    fcth = (fct & 0x38) >> 3;

    if (fcth == 0)
    {
        if (fctl == 0)
            execShift(SLL, IR.RI.rd, IR.RI.rt, IR.RI.sht);
        else if (fctl == 2)
            execShift(SRL, IR.RI.rd, IR.RI.rt, IR.RI.sht);
        else if (fctl == 3)
            execShift(SRA, IR.RI.rd, IR.RI.rt, IR.RI.sht);
        else
        {
            printf("Undefined instruction\n");
            return;
        }
    }
    else if (fcth == 1)
    {
        if (fctl == 0)
            execJr(JR, IR.RI.rs);
        else if (fctl == 4)
            execSyscall(SYSCALL);
        else
            printf("Undefined instruction\n");
    }
    else if (fcth == 2)
    {
        if (fctl == 0)
            execMfHiLo(MFHI, IR.RI.rd);
        else if (fctl == 2)
            execMfHiLo(MFLO, IR.RI.rd);
        else
            printf("Undefined instruction\n");
    }
    else if (fcth == 3)
    {
        if (fctl == 0)
            execMul(MUL, IR.RI.rs, IR.RI.rt);
        else
            printf("Undefined instruction\n");
    }
    else if (fcth == 4)
    {
        if (fctl == 0)
            execALU(ADD, IR.RI.rd, IR.RI.rs, IR.RI.rt);
        else if (fctl == 2)
            execALU(SUB, IR.RI.rd, IR.RI.rs, IR.RI.rt);
        else if (fctl == 4)
            execALU(AND, IR.RI.rd, IR.RI.rs, IR.RI.rt);
        else if (fctl == 5)
            execALU(OR, IR.RI.rd, IR.RI.rs, IR.RI.rt);
        else if (fctl == 6)
            execALU(XOR, IR.RI.rd, IR.RI.rs, IR.RI.rt);
        else if (fctl == 7)
            execALU(NOR, IR.RI.rd, IR.RI.rs, IR.RI.rt);
        else
            printf("Undefined instruction\n");
    }
    else if (fcth == 5)
    {
        if (fctl == 2)
            execALU(SLT, IR.RI.rd, IR.RI.rs, IR.RI.rt);
        else
            printf("Undefined instruction\n");
    }
    else
        printf("Undefined instruction\n");
}