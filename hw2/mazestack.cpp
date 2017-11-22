#include <iostream>
#include <stack>
using namespace std;

class Coord { //the class provided in the spec
public:
	Coord(int rr, int cc) : mR(rr), mC(cc) {}
	int r() const { return mR; }
	int c() const { return mC; }
private:
	int mR;
	int mC;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	stack <Coord> coordStack;        // create stack
	Coord startpoint(sr, sc);
	coordStack.push(startpoint);    // push the starting point
	maze[sr][sc] = 'B';             // show that weve been on it

	int row = sr;
	int col = sc;

	while (!coordStack.empty()) // not empty
	{
		startpoint = coordStack.top();  // store the location
		row = startpoint.r();
		col = startpoint.c();
		coordStack.pop(); //pop coordinates
		cerr << row << col << endl;

		if (row == er && col == ec)     // if we start and end at the same place then return true
			return true;

		if ( (row != 0 && maze[row - 1][col] != 'X' ) && ( maze[row - 1][col] != 'B' ) )  //check north
		{
			Coord point(row - 1, col);
			coordStack.push(point);      // push north
			maze[row - 1][col] = 'B';     // show that weve been here
	
	
		}
		if ( (col != 9 && maze[row][col + 1] != 'X' ) && ( maze[row][col + 1] != 'B' ) ) //check east
		{
			Coord point(row, col + 1);
			coordStack.push(point);      // push east
			maze[row][col + 1] = 'B';     // show that weve been here
		
		}
		if ( (row != 9 && maze[row + 1][col] != 'X' ) && ( maze[row + 1][col] != 'B') ) //check south
		{
			Coord point(row + 1, col);
			coordStack.push(point);      // push south
			maze[row + 1][col] = 'B';     // show that weve been here
		
		}
		if ( (col != 0 && maze[row][col + -1] != 'X' ) && ( maze[row][col - 1] != 'B' ) ) // check west
		{
			Coord point(row, col - 1);
			coordStack.push(point);      // push west
			maze[row][col - 1] = 'B';     // show that weve been here
		
		}
	}
	return false;
}	

int main()
{
	char maze[10][10] = {
		{ 'X','X','X','X','X','X','X','X','X','X' },
		{ 'X','.','.','.','.','.','.','.','.','X' },
		{ 'X','X','.','X','.','X','X','X','X','X' },
		{ 'X','.','.','X','.','X','.','.','.','X' },
		{ 'X','.','.','X','.','.','.','X','.','X' },
		{ 'X','X','X','X','.','X','X','X','.','X' },
		{ 'X','.','X','.','.','.','.','X','X','X' },
		{ 'X','.','X','X','X','.','X','X','.','X' },
		{ 'X','.','.','.','X','.','.','.','.','X' },
		{ 'X','X','X','X','X','X','X','X','X','X' }
	};

	if (pathExists(maze, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}