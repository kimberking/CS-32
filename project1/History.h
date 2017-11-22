#ifndef PIZZA2
#define PIZZA2
#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;

private:
	int mRow;
	int mColumn;
	int mBoard[MAXROWS][MAXCOLS];
};

#endif