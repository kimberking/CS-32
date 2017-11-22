#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>
#include <chrono>
using namespace std;
bool getLineWithTwoIntegers(int& r, int& c);

//not sure if i have to use since it wont take five seconds
/* class Timer
{
public:
Timer()
{
start();
}
void start()
{
m_time = std::chrono::high_resolution_clock::now();
}
double elapsed() const
{
std::chrono::duration<double, std::milli> diff =
std::chrono::high_resolution_clock::now() - m_time;
return diff.count();
}
private:
std::chrono::high_resolution_clock::time_point m_time;
}; */


//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player //provided
{
public:
	AwfulPlayer(string nm, const Game& g) : Player(nm, g), m_lastCellAttacked(0, 0) {};

	virtual bool placeShips(Board& b) {
		// Clustering ships is bad strategy
		for (int k = 0; k < game().nShips(); k++)
			if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
				return false;
		return true;
	};

	virtual Point recommendAttack() {
		if (m_lastCellAttacked.c > 0)
			m_lastCellAttacked.c--;
		else
		{
			m_lastCellAttacked.c = game().cols() - 1;
			if (m_lastCellAttacked.r > 0)
				m_lastCellAttacked.r--;
			else
				m_lastCellAttacked.r = game().rows() - 1;
		}
		return m_lastCellAttacked;
	};

	virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {};

	virtual void recordAttackByOpponent(Point p) {};

	virtual ~AwfulPlayer() {}
private:
	Point m_lastCellAttacked;
};

//*********************************************************************
//  HumanPlayer
//*********************************************************************

class HumanPlayer : public Player
{
public:
	HumanPlayer(std::string nm, const Game& g) : Player(nm, g) {};
	virtual ~HumanPlayer() {}
	virtual bool isHuman() const { return true; } //always true for human
	virtual bool placeShips(Board& b);
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {};
	virtual void recordAttackByOpponent(Point p) {};
	virtual Point recommendAttack() {
		cout << "Enter the row and column to attack (e.g, 3 5): ";
		int r = 0;
		int c = 0;
		getLineWithTwoIntegers(r, c);
		return Point(r, c);
	};
private:
};

bool getLineWithTwoIntegers(int& r, int& c)
{
	bool result(cin >> r >> c);

	if (!result) {
		cin.clear();  // clear error state so can do more input operations
		cin.ignore(10000, '\n');
		return result;
	}
	return result;
}

bool HumanPlayer::placeShips(Board& b) {

	char direction = ' ';
	int numShips = game().nShips();
	for (int i = 0; i < game().nShips(); i++)
	{

		while (direction != 'v' && direction != 'h')
		{
			cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
			cin >> direction;
			cin.ignore(10000, '\n');
			if (direction != 'v' && direction != 'h')
			{
				cout << "Direction must be h or v." << endl;
			}
		}

		Direction dir;
		if (direction == 'v')
			dir = VERTICAL;
		else
			dir = HORIZONTAL;

		bool success = false;
		while (success != true)
		{
			cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
			int r, c;
			if (!getLineWithTwoIntegers(r, c))
				return false;
			else
			{

				if (b.placeShip(Point(r, c), i, dir) == true)
				{
					direction = 0; //reset direction
					success = true;

					if (i != numShips - 1) {
						b.display(false);
					}
				}
			}

			if (success != true) {
				cout << "The ship can not be placed there." << endl;;
			}
		}

	}
	return true;
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
public:
	MediocrePlayer(string nm, const Game& g) :Player(nm, g), m_lastCellAttacked(0, 0), state(1)

	{
		for (int i = 0; i<game().rows(); i++)
			for (int j = 0; j<game().cols(); j++)
			{
				mapAttack[i][j] = '.';
			}
	};

	virtual ~MediocrePlayer() {};
	virtual bool isHuman() const { return false; }
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p) {};
	bool recursiveAlgorithm(Board &b, int shipID);
private:
	int state;
	Point m_lastCellAttacked;
	char mapAttack[MAXROWS][MAXCOLS];
};

Point MediocrePlayer::recommendAttack()
{
	//if it doesnt hit anything
	if (state == 1)
	{

		for (int i = 0; i<game().rows() * game().cols(); i++)
		{
			Point p = game().randomPoint();
			if (mapAttack[p.r][p.c] == '.')
			{
				mapAttack[p.r][p.c] = 'H';
				return p;
			}
		}
	}
	//hit something
	if (state == 2)
	{
		for (int i = 0; i<game().rows(); i++)
			for (int j = 0; j<game().cols(); j++)
			{
				Point p(i, j);
				if (mapAttack[p.r][p.c] == '+')
				{
					mapAttack[p.r][p.c] = 'H';
					return p;
				}
			}
	}

}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {

	if (!shotHit)
		return;

	if (validShot && shotHit && !shipDestroyed && state == 1)
	{
		//horizontal
		for (int i = 1; (p.c + i)<game().cols() && i < 5; i++)
		{
			if (mapAttack[p.r][p.c + i] == '.')
				mapAttack[p.r][p.c + i] = '+';
		}
		for (int i = 1; (p.c - i) >= 0 && i < 5; i++)
		{
			if (mapAttack[p.r][p.c - i] == '.')
				mapAttack[p.r][p.c - i] = '+';
		}

		//vertical 
		for (int i = 1; (p.r - i) >= 0 && i < 5; i++)
		{
			if (mapAttack[p.r - i][p.c] == '.')
				mapAttack[p.r - i][p.c] = '+';
		}

		for (int i = 1; (p.r + i)<game().rows() && i < 5; i++)
		{
			if (mapAttack[p.r + i][p.c] == '.')
				mapAttack[p.r + i][p.c] = '+';
		}

		state = 2;
	}

	if (shipDestroyed == true)
	{
		for (int i = 0; i < game().rows(); i++)

			for (int j = 0; j<game().cols(); j++)
			{
				Point p(i, j);
				if (mapAttack[p.r][p.c] == '+')
				{
					mapAttack[p.r][p.c] = '.';
				}
			}

		state = 1;
	}
}

bool MediocrePlayer::recursiveAlgorithm(Board &b, int shipID) {

	if (shipID == -1)
	{
		return false;
	}

	if (shipID == game().nShips())
	{
		return true;
	}

	for (int i = 0; i<game().rows(); i++)
		for (int j = 0; j<game().cols(); j++)
		{
			Point p(i, j);

			if (b.placeShip(p, shipID, HORIZONTAL))
			{
				if (recursiveAlgorithm(b, shipID + 1))
					return true;

				b.unplaceShip(p, shipID, HORIZONTAL);
			}

			if (b.placeShip(p, shipID, VERTICAL))
			{
				if (recursiveAlgorithm(b, shipID + 1))
					return true;

				b.unplaceShip(p, shipID, VERTICAL);

			}
		}

	return false;
}

bool MediocrePlayer::placeShips(Board &b)
{
	int count = 0;
	bool f = false;
	while (count != 50 && (!f))
	{
		b.block();
		f = recursiveAlgorithm(b, 0);
		b.unblock();
		count++;
	}
	return f;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class GoodPlayer : public Player
{
public:
	GoodPlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p) {};
	bool recursiveAlgorithm2(Board &b, int shipID);
private:
	Point m_lastCellAttacked;
	char mapAttack[MAXROWS][MAXCOLS];
	int state;
	int turn;
	stack <Point> solve;
};

GoodPlayer::GoodPlayer(string nm, const Game& g) :Player(nm, g), m_lastCellAttacked(0, 0), state(1), turn(1)
{
	for (int i = 0; i<game().rows(); i++)
		for (int j = 0; j<game().cols(); j++)
		{
			mapAttack[i][j] = '.';
		}
};

bool GoodPlayer::recursiveAlgorithm2(Board &b, int shipID) {

	if (shipID == -1) {
		return false;
	}

	if (shipID == game().nShips())
	{
		return true;
	}

	for (int i = 0; i<game().rows(); i++)
		for (int j = 0; j<game().cols(); j++)
		{
			Point p(i, j);

			if (b.placeShip(p, shipID, HORIZONTAL))
			{
				if (recursiveAlgorithm2(b, shipID + 1))
					return true;

				b.unplaceShip(p, shipID, HORIZONTAL);
			}

			if (b.placeShip(p, shipID, VERTICAL))
			{
				if (recursiveAlgorithm2(b, shipID + 1))
					return true;

				b.unplaceShip(p, shipID, VERTICAL);

			}
		}

	return false;
}

bool GoodPlayer::placeShips(Board &b) {

	int count = 0;
	bool f = false;

	while (count != 50 && (!f))
	{
		b.block();
		f = recursiveAlgorithm2(b, 0);
		b.unblock();
		count++;
	}
	return f;
}

Point GoodPlayer::recommendAttack() {
	//state 1
	if (state == 1)
	{
		if (turn == 1)
		{
			Point p(game().rows() / 2, game().cols() / 2);
			mapAttack[p.r][p.c] = 'H';
			turn++;
			return p;
		}

		if (turn == 2)
		{
			Point p(game().rows() / 4, 0);
			if (mapAttack[p.r][p.c] == '.')
			{
				mapAttack[p.r][p.c] = 'H';
				turn++;
				return p;
			}
		}

		if (turn == 3)
		{
			Point p((game().rows()) - (game().rows() / 4), (game().cols()) - 1);
			if (mapAttack[p.r][p.c] == '.')
			{
				mapAttack[p.r][p.c] = 'H';
				turn++;
				return p;
			}
		}
		if (turn == 4)
		{
			Point p(game().rows() / 4, (game().cols()) - 1);
			if (mapAttack[p.r][p.c] == '.')
			{
				mapAttack[p.r][p.c] = 'H';
				turn++;
				return p;
			}

		}
		if (turn == 5)
		{
			Point p((game().rows()) - (game().rows() / 4), 0);
			if (mapAttack[p.r][p.c] == '.')
			{
				mapAttack[p.r][p.c] = 'H';
				turn++;
				return p;
			}
		}
		for (int i = 0; ; i++)
		{
			Point p = game().randomPoint();

			{
				if (p.r % 2 != 0 && (p.c % 2 == 0))
					p.c = p.c + 1;
				else if (p.r % 2 == 0 && p.c % 2 != 0)
					p.r = p.r + 1;
			}
			if (mapAttack[p.r][p.c] == '.')
			{
				mapAttack[p.r][p.c] = 'H';
				return p;
			}
		}

		for (int k = 0;; k++) {

			Point p = game().randomPoint(); //pick a random point

			if (p.c % 2 == 0 && p.r % 2 != 0) //if row is odd and col is even
				p.c = p.c - 1; //minus one to col

			else if (p.r % 2 == 0 && p.c % 2 != 0) //if row is even and col is odd
				p.r = p.r - 1; //minus one to row

			if (mapAttack[p.r][p.c] == '.') //miss
			{
				mapAttack[p.r][p.c] = 'H'; //hit
				return p;
			}
		}
	}

	if (state == 2)
	{
		while (!solve.empty())
		{
			Point p = solve.top();
			solve.pop();
			if (mapAttack[p.r][p.c] == '.')
			{
				mapAttack[p.r][p.c] = 'H';
				return p;
			}
		}

		for (int k = 0;; k++) {

			Point p = game().randomPoint(); //pick a random point

			if (p.c % 2 == 0 && p.r % 2 != 0) //if row is odd and col is even
				p.c = p.c - 1; //minus one to col

			else if (p.r % 2 == 0 && p.c % 2 != 0) //if row is even and col is odd
				p.r = p.r - 1; //minus one to row

			if (mapAttack[p.r][p.c] == '.') //miss
			{
				mapAttack[p.r][p.c] = 'H'; //hit
				return p;
			}
		}

	}

}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	if (validShot && shotHit && !shipDestroyed)
	{
		if (p.r + 1<game().rows())
			solve.push(Point(p.r + 1, p.c));
		if (p.c + 1<game().cols())
			solve.push(Point(p.r, p.c + 1));
		if (p.r - 1 >= 0)
			solve.push(Point(p.r - 1, p.c));
		if (p.c - 1 >= 0)
			solve.push(Point(p.r, p.c - 1));

		state = 2;
	}
	if (solve.size() == 0) //if it is empty
		state = 1;

	if (!shotHit)
		;
	if (shipDestroyed == true)
	{
		if (!solve.empty())
			state = 2;
		else
			state = 1;
	}
}
	
//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
	static string types[] = {
		"human", "awful", "mediocre", "good"
	};

	int loc;
	for (loc = 0; loc != sizeof(types) / sizeof(types[0]) &&
		type != types[loc]; loc++)
		;
	switch (loc)
	{
	case 0:  return new HumanPlayer(nm, g);
	case 1:  return new AwfulPlayer(nm, g);
	case 2:  return new MediocrePlayer(nm, g);
	case 3:  return new GoodPlayer(nm, g);
	default: return nullptr;
	}
}
