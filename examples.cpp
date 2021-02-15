// This must be the last included header
#include "k.h"

extern "C" {

    // Basic add function from KX website
    // https://code.kx.com/wiki/Cookbook/ExtendingWithC
    K add(K x, K y)
    {
        if (x->t != -KJ || y->t != -KJ) return krr((S)"type");
        return kj(x->j + y->j);
    }
    
    // Takes a string and returns its length, creates a memory leak
    K memoryLeakExample(K str)
    {
        // Check input type is string
        if (str->t != KC) return krr((S)"type");
		
        // Create an unused kdb+ long array object the same length as the string input
        K unused = ktn(KJ, str->n);
		
	    // Return length of input string as a long
	    return kj(str->n);
    }

	// Removes the reference that has been assigned to the input, returns 1b
	K nullPointerExample(K obj) 
    {
		// Remove the reference
        r0(obj);
        
        // Return boolean true
        return kb(1);
    }
}