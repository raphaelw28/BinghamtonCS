#include "bitFields.h"
#include <assert.h>

int getBit(int bn,unsigned long val) {
	assert(bn>=0 && bn<64);
	if (1UL<<bn & val) return 1;
	return 0;
}

void setBit(int bn,int new,unsigned long *val) {
	assert(bn>=0 && bn<64);
	if (new) (*val)|=1UL<<bn;
	else (*val)&=~(1L<<bn);
}

long getBitFld(int bs,int len,unsigned long val) {
	assert(bs > 0 && bs < 64);
	assert(1 <= len && len <= bs+1);
	int leftShift = 64 - len;
	long signExtendedField = (long)(val << (bs + leftShift)) >> leftShift;
	return signExtendedField;
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		perform an unsigned getBitFldU to extract the unsigned version of the field
	//		get the high order bit of the result... that's the "sign" bit
	//		if the sign bit not zero, replace the 0 padding with 1 padding
}
unsigned long getBitFldU(int bs,int len,unsigned long val) {
	assert(bs >= 0 && bs < 64);
	assert(1 <= len && len <= bs+1);
	unsigned long mask = ((1UL << len) - 1) << bs;
	unsigned long bitField = (val & mask) >> bs;
	return bitField;
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		make a mask that has len 1's
	// 		Shift that mask left so it's leftmost 1 starts at bs
	//      bitwise and the mask and val to isolate the bit field bits
	//		Shift the result to the right so that the rightmost bit of the bit field is bit 0
	//			Note that shift right pads to the left with the sign bit if the field is signed, but
	//			for unsigned, it will pad to the left with 0

}

void setBitFld(int bs,int len,unsigned long new,unsigned long *val) {
	assert(bs >= 0 && bs < 64);
	assert(1 <= len && len <= bs+1);
	unsigned long mask = (1UL << len) - 1;
	new &= mask;
	new <<= bs;
	mask <<= bs;
	*val &= ~mask;
	*val |= new;
	
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	//      Make a mask that has len bits, right justified
	//		Use mask to turn off any bits to the left of the bitField values in new
	// 		Shift both the new value and the mask to the correct bit position
	//		Turn off all bits in *val under the mask by bitwise anding with ~mask
	//		Or *val with new.  (new has zeros for all non bit field positions, so won't affect val)
}




