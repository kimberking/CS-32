#include "History.h"
#include <iostream>
#include <string>

using namespace std;

History::History(int nRows, int nCols) {

	mRow = nRows;
	mColumn = nCols;

}

bool History::record(int r, int c) {

	if (r > mRow || c > mColumn) {
	return false;
	}

	mBoard[r][c] ++;
	return true;
}

void History::display() const {

	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

		// Fill displayGrid with dots (empty) and stars (poison pellets)
	for (r = 1; r <= mRow; r++)
		for (c = 1; c <= mColumn; c++)
				displayGrid[r][c] = ('.');

		//covert numbers with letters
	for (int i = 1; i <= mRow; i++) {
		for (int j = 1; j <= mColumn; j++) {
			if (mBoard[i][j] != 0) {
				int x = mBoard[i][j];
				char marker = 64 + x;
				displayGrid[i][j] = marker;
			}
		}
	}

		// Draw the grid
	clearScreen();
	for (r = 1; r <= mRow; r++)
	{
		for (c = 1; c <= mColumn; c++)
			cout << displayGrid[r][c];
		cout << endl;
	}
	cout << endl;


}