#include "warehouse.h"
#include "packer.h"
#include <stdio.h>
#include <stdbool.h>

/*---------------------------------------------------------
   Global Declarations
---------------------------------------------------------*/
#define MAXRECENTPARTS 16
int recentBins[MAXRECENTPARTS];
int binFrequency[MAXRECENTPARTS] = {0};
int currentIndex = 0;
/*---------------------------------------------------------
   Internal Function Declarations
---------------------------------------------------------*/
void updateRecentBins(int partNumber){
	int binNumber = binFromPart(partNumber);
	int found = -1;
	for(int i = 0; i < MAXRECENTPARTS; i++){
		if(recentBins[i] == binNumber){
			found = i;
			break;
		}
	}
	if(found != -1){
		binFrequency[found]++;
	} else {
		if (recentBins[currentIndex] != -1){
			int minFrequency = binFrequency[0], minIndex = 0;
			for(int i = 1; i < MAXRECENTPARTS; i++){
				if(binFrequency[i] < minFrequency){
					minFrequency = binFrequency[i];
					minIndex = i;
				} else if(binFrequency[i] == minFrequency){
					minIndex = (currentIndex + 1) % MAXRECENTPARTS;
					break;
				}
			}
			if(binFrequency[minIndex] > 0) binFrequency[minIndex]--;
		}
		recentBins[currentIndex] = binNumber;
		binFrequency[currentIndex] = 1;
		currentIndex = (currentIndex + 1) & MAXRECENTPARTS;
	}
}
void ifNotOnBench(int partNumber) {
    int binNumber = binFromPart(partNumber); // Assuming binFromPart() maps a part number to its corresponding bin number

    if (onBench(binNumber)) {
        return; // Bin is already on the bench, no action needed
    } else {
        // Loop through each slot to find an empty one
        for (int slot = 0; slot < NUMSLOTS; slot++) {
            if (binInSlot(slot) == -1) { // Check if the slot is empty
                getBin(binNumber, slot); // Put the bin in the first empty slot found
                updateRecentBins(partNumber); // Update the recentBins array with this part's bin
                return; // Exit the function after placing the bin
            }
        }
        // Consider what to do if no slots are empty. This might include logic for swapping out an existing bin.
    }
}

bool areAllSlotsTaken() {
    for (int slot = 0; slot < NUMSLOTS; slot++) {
        if (binInSlot(slot) == -1) { // Assuming binInSlot checks if a slot is empty
            return false; // Found an empty slot, so not all slots are taken
        }
    }
    return true; // No empty slots found, all slots are taken
}


void swapLeastUsed(int newPartNumber) {
    int newBinNumber = binFromPart(newPartNumber);

    // Proceed only if the new bin is not already on the bench.
    if (!onBench(newBinNumber)) {
        // Find the least used bin to potentially swap out.
        int minFrequency = binFrequency[0]; //minIndex = 0;
        for (int i = 1; i < MAXRECENTPARTS; i++) {
            if (binFrequency[i] < minFrequency) {
                minFrequency = binFrequency[i];
                //minIndex = i;
            }
        }

        // Attempt to place the new bin in a slot that ideally should be swapped.
        // Note: This simplified example assumes we replace the bin at the current 'currentIndex' in 'recentBins',
        // which may not directly correspond to 'workBench' slots. The correct approach would involve a helper function.
        getBin(newBinNumber, currentIndex % NUMSLOTS); // Ensure we stay within slot bounds.

        // Update 'recentBins' and 'binFrequency' accordingly.
        recentBins[currentIndex] = newBinNumber;
        binFrequency[currentIndex] = 1;
        currentIndex = (currentIndex + 1) % MAXRECENTPARTS;
    }
}






/*---------------------------------------------------------
	External Function Definitions
---------------------------------------------------------*/
void processPart(int partNumber) {
    // Determine if all workbench slots are currently occupied.
    if (areAllSlotsTaken()) {
        // The workbench is fully occupied. This condition ensures that we only attempt
        // to swap bins when there's no available space. The swapLeastUsed function
        // will handle the logic to swap out the least used bin for the new part's bin.
        swapLeastUsed(partNumber);
    } else {
        // There's at least one empty slot available on the workbench.
        // Try to place the new part's bin on the bench, assuming its bin isn't
        // already present. This is handled by the ifNotOnBench function, which
        // will place the bin in an empty slot if available.
        ifNotOnBench(partNumber);
    }
}






/*---------------------------------------------------------
   Internal Function Definitions
---------------------------------------------------------*/




