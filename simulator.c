#include <stdio.h>
#include "instruction.c"

unsigned int PC, IR;
void main()
{
    while (1)
    {
        char command;
        scanf(" %c", &command);
        switch (command)
        {
        case 'l':
            //load program
            break;
        case 'j':
            break;
        case 'g':
            break;
        case 's':
            break;
        case 'm':
            break;
        case 'r':
            break;
        case 'x':
            //exit program
            break;
        }
    }
}
