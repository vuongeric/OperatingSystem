#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcb.h"

PCB *makePCB(FILE *p, int ramCell);

PCB *makePCB(FILE *p, int ramCell)
{
    printf("DEBUG: Making a new PCB...\n");
    PCB *pcb = malloc(sizeof(PCB));
    pcb->PC = p;
    pcb->ramCell = ramCell;

    return pcb;
}