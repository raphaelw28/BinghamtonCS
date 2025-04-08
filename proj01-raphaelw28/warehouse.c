#include "warehouse.h"
#include "packer.h"
#include <stdio.h>
#include <assert.h>

/*---------------------------------------------------------
   Global Declarations
---------------------------------------------------------*/
int partNumber=-1;
int workBench[NUMSLOTS]={0};
float cost=0.0;
int packed=0;
FILE * order = NULL;
#define foreachslot(X) for(X=0;X<NUMSLOTS;X++)

/*---------------------------------------------------------
   Internal Function Declarations
---------------------------------------------------------*/
int openOrder(char * orderFile); // Returns 1 if order is opened, 0 otherwise
void closeOrder(); // Closes the currently open order file
void nextPart(); // Retrieves the previous part from the workbench (fails if part is not on workbench)
	// Then, returns the next part number in the order

/*---------------------------------------------------------
   main function definition
---------------------------------------------------------*/
int main(int argc,char **argv) {
	if (argc<2) {
		printf("Invoke as %s <order file name(s)>\n",argv[0]);
		return 1; // Failed
	}
	for(int i=1; i<argc;i++) {
		if (openOrder(argv[i])) {
			while(partNumber>-1) {
				processPart(partNumber);
				nextPart();
			}
			closeOrder();
		}
	}

	return 0; // Indicate success
}

/*---------------------------------------------------------
   Internal Function Definitions
---------------------------------------------------------*/
int openOrder(char * orderFile) {
	printf("Working on order: %s\n",orderFile);
	order=fopen(orderFile,"r");
	if (order==NULL) {
		perror("  Unable to open order file");
		return 0;
	}
	// Reset global statistics
	int slot;
	foreachslot(slot) { workBench[slot]=-1; }
	cost=0.0;
	packed=0;
	partNumber=-1;
	// Get first part number
	if (1!=fscanf(order,"%d",&partNumber)) {
			printf("   Unable to find first part number in order\n");
			return 0;
	}
	return 1;
}

void closeOrder() {
	printf("   You packed %d items at a cost of %.2f for a net earnings of %.2f\n",
		packed,cost,(packed/10.0) - cost);
	fclose(order);
}

void nextPart() {
	// Retrieves the previous part from the workBench (fails if part is not on workBench)
	if (!onBench(binFromPart(partNumber))) {
		printf("Previous part number: %d is not in a bin on the workBench\n",partNumber);
		assert(onBench(binFromPart(partNumber)));
	}
	packed++;
	// Then, returns the next part number in the order
	partNumber=-1;
	fscanf(order,"%d",&partNumber);
	// Note... if no more parts available, partNumber will not get updated
}

/*---------------------------------------------------------
	External Function Definitions
---------------------------------------------------------*/
void getBin(int binNum,int slotNum) {
	// Adds bin binNum to workBench at slot slotNum
	assert(binNum<NUMBINS);
	assert(slotNum<NUMSLOTS);
	if (workBench[slotNum]!=-1) cost+=0.05; // It costs 5 cents to return a bin to the warehouse
	workBench[slotNum]=binNum;
	cost+=0.10; // It costs 10 cents to retrieve a new bin
#ifdef SHOWBENCH
	printf("   Bench: ");
	foreachslot(slotNum) printf("%2d ",workBench[slotNum]);
	printf("\n");
#endif
}

int binInSlot(int slotNum) {
	// Tells which bin is in slot slotNum
	assert(slotNum<NUMSLOTS);
	return workBench[slotNum];
}

int onBench(int binNum) {
	// Returns 1 if binNum is on the workBench, 0 otherwise
	int slot;
	assert(binNum<NUMBINS);
	foreachslot(slot) {
		if (workBench[slot]==binNum) return 1;
	}
	return 0;
}

int binFromPart(int partNum) {
	// Returns the bin number that contains the part in partNum
	int binNum=partNum/10;
	if (binNum>=NUMBINS) {
		printf("Invalid part number: %d\n",partNum);
		assert(binNum<NUMBINS);
	}
	return binNum;
}

int partIndex() {
	// Returns the index of the current part in the order list
	return packed;
}

void swapSlots(int slot1,int slot2) {
	// Swaps the bin in slot1 with the bin in slot2
	assert(slot1<NUMSLOTS);
	assert(slot2<NUMSLOTS);
	int bin1=workBench[slot1];
	workBench[slot1]=workBench[slot2];
	workBench[slot2]=bin1;
#ifdef SHOWBENCH
	printf("   Swap : ");
	foreachslot(slot1) printf("%2d ",workBench[slot1]);
	printf("\n");
#endif
}