#ifndef SNNSET_H
#define SNNSET_H
#include "Set.h"

class SSNSet
{
public:
	SSNSet();    

	bool add(unsigned long ssn);
	// Add an SSN to the SSNSet.  Return true if and only if the SSN
	// was actually added.

	int size() const;  // Return the number of SSNs in the SSNSet.

	void print() const;
	// Write every SSN in the SSNSet to cout exactly once, one per
	// line.  Write no other text.

private:
	Set mSNN;
};

#endif