#pragma once 
#include <stdexcept>
// This must be the last included header
#include "k.h"

// String copy, allows portability between Visual and UNIX compilers
#if defined(WIN32) || defined(_WIN32) 
#define strdup _strdup
#endif


// Creates a C string from a kdb+ string
// Taken from https://code.kx.com/wiki/Cookbook/regex
static __inline S c2s(S s, J n) { S r = (S)malloc(n + 1); R r ? memcpy(r, s, n), r[n] = 0, r : (S)krr((S)"wsfull (string memory management)"); }

// Converts a kdb+ sym or string to a C string
static S kdb2str(K kdbStr) {
    return kdbStr->t == -KS ? kdbStr->s : c2s((S)kC(kdbStr), kdbStr->n);
}

// Converts C++ exception to kdb+ error
static K exception2krr(std::exception &e){
    return krr(strdup(e.what()));
}

// Type check, one allowed
static bool checkType(K kObject, int allowedType) {
	return (kObject->t == allowedType) ? true : throw std::runtime_error("type");
}

// Type check, two allowed
static bool checkType(K kObject, int allowedType1, int allowedType2) {
	return ((kObject->t == allowedType1) || (kObject->t == allowedType2)) ? true : throw std::runtime_error("type");
}

// Check valid date
static bool checkDate(K kDate) {
	checkType(kDate, -KD);
	return kDate->i != ni ? true : throw std::runtime_error("invalid date");
}
