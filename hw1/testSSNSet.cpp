#include "SSNSet.h"
#include "Set.h"
#include <cassert>
#include <iostream>
using namespace std;

int main()
{
	SSNSet s;
	bool b = s.add(123456789);
	assert(b);
	assert(s.size() == 1);
	b = s.add(122345678);
	assert(b);
	assert(s.size() == 2);
	b = s.add(123456789);
	assert(!b);
	assert(s.size() == 2);

	s.print();
}