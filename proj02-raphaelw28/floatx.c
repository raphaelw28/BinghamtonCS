#include "floatx.h"
#include "bitFields.h"

union hexDouble {
    double d;
    unsigned long lng;
};

floatx doubleToFloatx(double val, int totBits, int expBits) {
    union hexDouble u;
    u.d = val;
    
    unsigned long sign = u.lng >> 63;
    unsigned long exponentBits = (u.lng >> 52) & 0x7FF;
    unsigned long fraction = u.lng & ((1LL << 52) - 1);
    long exponent = exponentBits - 1023 + ((1 << (expBits - 1)) - 1); 
    int fracBits = totBits - expBits - 1;


    if (exponentBits == 0x7FF) {
        if (fraction != 0) { 
            return (sign << (totBits - 1)) | (((1LL << expBits) - 1) << fracBits) | (1LL << (fracBits - 1));
        } else { 
            return (sign << (totBits - 1)) | (((1LL << expBits) - 1) << fracBits);
        }
    }

    if(val == 0.0) return sign << (totBits - 1);

    if (exponent <= 0) {
        int shiftAmount = 1 - exponent;
        if (shiftAmount >= 52) { 
            fraction = 0;
        } else {
            fraction >>= shiftAmount;
            fraction &= ((1LL << fracBits) - 1);
        }
        exponent = 0;
    } else {
        fraction >>= (52 - fracBits);
    }

    if (exponent >= (1 << expBits)) {
        exponent = (1 << expBits) - 1; 
        fraction = 0; 
    }

    unsigned long floatxVal = (sign << (totBits - 1)) | ((exponent & ((1 << expBits) - 1)) << fracBits) | (fraction & ((1LL << fracBits) - 1));
    
    return floatxVal;
}

