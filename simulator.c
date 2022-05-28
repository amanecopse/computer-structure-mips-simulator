#include <stdio.h>
#include <string.h>
#include "instruction.c"
#include "defines.h"
#include "units.c"

unsigned int PC;
union instructionRegister IR;

unsigned int invertEndian(unsigned int data);
void loadProgram(char *fileName);

void main()
{
    while (1)
    {
        char command[3];
        char fileName[50];
        unsigned int uInput1;
        unsigned int uInput2;

        printf("command: ");
        scanf(" %s", &command);
        if (strncmp(command, "l", 2) == 0)
        { // load program
            scanf(" %s", fileName);
            loadProgram(fileName);
        }
        else if (strncmp(command, "j", 2) == 0)
        {
            scanf("%x", &uInput1);
            printf("PC가 0x%x로 변경되었습니다.\n", uInput1);
            PC = uInput1;
        }
        else if (strncmp(command, "g", 2) == 0)
        {
            do
                step();
            while (PC);
        }
        else if (strncmp(command, "s", 2) == 0)
        {
            step();
        }
        else if (strncmp(command, "m", 2) == 0)
        {
            scanf("%x", &uInput1);
            scanf("%x", &uInput2);
            for (unsigned int i = uInput1; i <= uInput2; i++)
            {
                int value = MEM(i, 0, ACCESS_READ, BYTE_SIZE);
                printf("0x%x번지의 메모리 값: %d(10진수), %x(16진수)\n", i, value, value);
            }
        }
        else if (strncmp(command, "r", 2) == 0)
        {
            for (int i = 0; i < 32; i++)
            {
                int value = REG(i, 0, ACCESS_READ);
                printf("%d번 레지스터의 값: %d(10진수), %x(16진수)\n", i, value, value);
            }
        }
        else if (strncmp(command, "sm", 2) == 0)
        {
            scanf("%x", &uInput1);
            scanf("%x", &uInput2);
            MEM(uInput1, uInput2, ACCESS_WRITE, BYTE_SIZE);
            printf("0x%x번지의 메모리 값이 다음으로 변경됨: %d(10진수), %x(16진수)\n", uInput1, uInput2, uInput2);
        }
        else if (strncmp(command, "sr", 2) == 0)
        {
            scanf("%d", &uInput1);
            scanf("%x", &uInput2);
            REG(uInput1, uInput2, ACCESS_WRITE);
            printf("%d번 레지스터의 값이 다음으로 변경됨: %d(10진수), %x(16진수)\n", uInput1, uInput2, uInput2);
        }
        else if (strncmp(command, "x", 2) == 0)
        {
            printf("MIPS simulator를 종료합니다.\n");
            return;
        }

        while (getchar() != '\n') //버퍼 비우기
            ;
    }
}

unsigned int invertEndian(unsigned int data)
{
    unsigned char c[4];

    c[3] = (unsigned char)data;
    data = data >> 8;
    c[2] = (unsigned char)data;
    data = data >> 8;
    c[1] = (unsigned char)data;
    data = data >> 8;
    c[0] = (unsigned char)data;

    return *(unsigned int *)c;
}

void loadProgram(char *fileName)
{
    FILE *pFile;
    unsigned int data;
    unsigned int addr;
    unsigned int iCount;
    unsigned int dCount;

    int err = fopen_s(&pFile, fileName, "rb");
    if (err)
    {
        printf("Cannot open file: %s\n", fileName);
        return;
    }

    // 명령어와 데이터의 개수를 각각 읽는다.
    fread(&data, sizeof(data), 1, pFile);
    iCount = invertEndian(data);
    fread(&data, sizeof(data), 1, pFile);
    dCount = invertEndian(data);
    printf("Number of Instructions: %d, Number of Data: %d\n", iCount, dCount);

    // 명령어를 메모리에 로드한다.
    addr = 0x400000;
    for (int i = 0; i < (int)iCount; i++)
    {
        fread(&data, sizeof(unsigned int), 1, pFile);
        MEM(addr, invertEndian(data), ACCESS_WRITE, WORD_SIZE);
        addr += 4;
    }

    // 데이터를 메모리에 로드한다.
    addr = 0x10000000;
    for (int i = 0; i < (int)dCount; i++)
    {
        fread(&data, sizeof(unsigned int), 1, pFile);
        MEM(addr, invertEndian(data), ACCESS_WRITE, WORD_SIZE);
        addr += 4;
    }

    PC = 0x400000;                     // pc 초기화
    REG(29, 0x80000000, ACCESS_WRITE); // sp 초기화
    printf("프로그램을 메모리에 로드하였습니다.\n");
}