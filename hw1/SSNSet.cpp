#include "SSNSet.h"
#include "Set.h"
#include <iostream>
using namespace std;

SSNSet::SSNSet() {
}


bool SSNSet::add(unsigned long snn) {
	return mSNN.insert(snn);
}

int SSNSet::size() const {
// Return the number of ids in the SSNSet.
	return mSNN.size();
}

void SSNSet::print() const {
	ItemType temp;

	for (int i = 0; i < mSNN.size(); i++)
	{
		mSNN.get(i, temp);
		cout << temp <<endl;
	}
}