#ifndef WAREHOUSE_H

/*---------------------------------------------------------
	Definition of Numeric Parameters
---------------------------------------------------------*/
#define NUMSLOTS 5
#define NUMBINS 100
#define EMPTYBIN -1

/*---------------------------------------------------------
	Functions Defined in warehouse.c
---------------------------------------------------------*/
void getBin(int binNum,int slotNum); // Adds bin binNum to workbench at slot slotNum
int binInSlot(int slotNum); // Tells which bin is in slot slotNum
int onBench(int binNum); // Returns 1 if binNum is on the workbench, 0 otherwise
int binFromPart(int partNum); // Returns the bin number that contains the part in partNum
int partIndex(); // Returns the index of the current part in the order list
void swapSlots(int slot1,int slot2); // Swaps the bin in slot1 with the bin in slot2

#define WAREHOUSE_H
#endif