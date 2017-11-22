#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
using namespace std;

class GameImpl
{
public:
	GameImpl(int nRows, int nCols);
	int rows() const;
	int cols() const;
	bool isValid(Point p) const;
	Point randomPoint() const;
	bool addShip(int length, char symbol, string name);
	int nShips() const;
	int shipLength(int shipId) const;
	char shipSymbol(int shipId) const;
	string shipName(int shipId) const;
	Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
	~GameImpl();
private:
	int mRows;
	int mCols;
	int loc;

	class Ship {
	public:
		Ship(int length, char symbol, string name, int loc) : n_shipType(name), n_sLength(length), n_IDs(loc), n_sSymbol(symbol) {}
		string n_shipType;
		int n_sLength;
		int n_IDs;
		char n_sSymbol;
	};
	vector <Ship*> vShips;
};

GameImpl::~GameImpl() {

	for (int i = 0; i < vShips.size(); i++)
		delete vShips[i];
}

void waitForEnter()
{
	cout << "Press enter to continue: ";
	cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols) : mRows(nRows), mCols(nCols), loc(-1) {}

int GameImpl::rows() const
{
	return mRows;
}

int GameImpl::cols() const
{
	return mCols;
}

bool GameImpl::isValid(Point p) const
{
	return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols(); //make sure it's on the map
}

Point GameImpl::randomPoint() const
{
	return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
	loc++;
	vShips.push_back(new Ship(length, symbol, name, loc)); //add a ship by making a new ship
	return true;
}

int GameImpl::nShips() const
{
	return vShips.size();
}

int GameImpl::shipLength(int shipId) const
{
	for (int i = 0; i<vShips.size(); i++)
	{
		if (vShips[i]->n_IDs == shipId)
			return vShips[i]->n_sLength; //find the length given the id
	}
}

char GameImpl::shipSymbol(int shipId) const
{
	for (int i = 0; i<vShips.size(); i++)
	{
		if (vShips[i]->n_IDs == shipId)
			return vShips[i]->n_sSymbol; //get the letter you need to display the ship
	}
}

string GameImpl::shipName(int shipId) const
{
	for (int i = 0; i < vShips.size(); i++)
	{
		if (vShips[i]->n_IDs == shipId)
			return vShips[i]->n_shipType; //give id tell me the name of the ship
	}
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause) //connect the pieces and battle!
{
	//place the ships

	if (p2->isHuman() == true)
	{
		cout << p2->name() << " must place " << (*this).nShips() << " ships." << endl;
		b2.display(false);
	}


	if (!(p1->placeShips(b1)) || !(p2->placeShips(b2))) {
		return nullptr;
	}

	while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed())
	{
		bool ShotsOnly1 = false;
		bool ShotsOnly2 = false;
		bool ShotHit1 = false;
		bool ShipDestroyed1 = false;
		int shipid = 0;

		if (p1->isHuman())
			ShotsOnly1 = true;

		if (p2->isHuman())
			ShotsOnly2 = true;

		if ((shouldPause == true) && (p2->isHuman())) { //for it to display correctly
			cin.ignore(10000, '\n');
		}

		//first player's attack
		cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
		b2.display(ShotsOnly1);

		Point p = p1->recommendAttack();
		if (b2.attack(p, ShotHit1, ShipDestroyed1, shipid) == true)
		{
			if (!ShotHit1 && !ShipDestroyed1)
				cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and missed, resulting in:" << endl;

			if (ShotHit1 && !ShipDestroyed1)
				cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and hit something, resulting in:" << endl;

			if (ShipDestroyed1)
				cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and destroyed the " << shipName(shipid) << " resulting in:" << endl;


			b2.display(ShotsOnly1);
			p1->recordAttackResult(p, isValid(p), ShotHit1, ShipDestroyed1, shipid);
		}
		else
		{
			cout << p1->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;
			p1->recordAttackResult(p, isValid(p), false, false, shipid);
		}

		if (b2.allShipsDestroyed())
		{
			cout << p1->name() << " wins!" << endl;
			if (p2->isHuman())
			{
				cout << "Winner's Board:" << endl;
				b1.display(false);
			}
			return p1;
		}
		if (b1.allShipsDestroyed())
		{
			cout << p2->name() << " wins!" << endl;
			if (p1->isHuman())
			{
				cout << "Winner's Board:" << endl;
				b2.display(false);
			}
			return p2;
		}

		if (shouldPause == true) {
			waitForEnter();
		}

		bool ShotHit2 = false;
		bool ShipDestroyed2 = false;

		if ((shouldPause == true) && (p1->isHuman())) { //for it to display correctly
			cin.ignore(10000, '\n');
		}

		//second player's attack
		cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
		b1.display(ShotsOnly2);
		Point q = p2->recommendAttack();

		if (b1.attack(q, ShotHit2, ShipDestroyed2, shipid) == true)
		{
			if (!ShotHit2 && !ShipDestroyed2)
				cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and missed, resulting in:" << endl;

			if (ShotHit2 && !ShipDestroyed2)
				cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and hit something, resulting in:" << endl;

			if (ShipDestroyed2)
				cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and destroyed the " << shipName(shipid) << " resulting in:" << endl;

			b1.display(ShotsOnly2);
			p2->recordAttackResult(q, isValid(q), ShotHit2, ShipDestroyed2, shipid);
		}
		else
		{
			cout << p2->name() << " wasted a shot at (" << q.r << "," << q.c << ")." << endl;
			p2->recordAttackResult(q, isValid(q), ShotHit2, ShipDestroyed2, shipid);
		}

		if (b1.allShipsDestroyed())
		{
			cout << p2->name() << " wins!" << endl;
			if (p1->isHuman())
			{
				cout << "Winner's Board:" << endl;
				b2.display(false);
			}
			return p2;
		}

		if (b2.allShipsDestroyed())
		{
			cout << p1->name() << " wins!" << endl;
			if (p2->isHuman())
			{
				cout << "Winner's Board:" << endl;
				b1.display(false);
			}
			return p1;
		}

		if (shouldPause == true)
			waitForEnter();
	}
}


//******************** Game functions *******************************
// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
	if (nRows < 1 || nRows > MAXROWS)
	{
		cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
		exit(1);
	}
	if (nCols < 1 || nCols > MAXCOLS)
	{
		cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
		exit(1);
	}
	m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
	delete m_impl;
}

int Game::rows() const
{
	return m_impl->rows();
}

int Game::cols() const
{
	return m_impl->cols();
}

bool Game::isValid(Point p) const
{
	return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
	return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
	if (length < 1)
	{
		cout << "Bad ship length " << length << "; it must be >= 1" << endl;
		return false;
	}
	if (length > rows() && length > cols())
	{
		cout << "Bad ship length " << length << "; it won't fit on the board"
			<< endl;
		return false;
	}
	if (!isascii(symbol) || !isprint(symbol))
	{
		cout << "Unprintable character with decimal value " << symbol
			<< " must not be used as a ship symbol" << endl;
		return false;
	}
	if (symbol == 'X' || symbol == '.' || symbol == 'o')
	{
		cout << "Character " << symbol << " must not be used as a ship symbol"
			<< endl;
		return false;
	}
	int totalOfLengths = 0;
	for (int s = 0; s < nShips(); s++)
	{
		totalOfLengths += shipLength(s);
		if (shipSymbol(s) == symbol)
		{
			cout << "Ship symbol " << symbol
				<< " must not be used for more than one ship" << endl;
			return false;
		}
	}
	if (totalOfLengths + length > rows() * cols())
	{
		cout << "Board is too small to fit all ships" << endl;
		return false;
	}
	return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
	return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
	if (p1 == nullptr || p2 == nullptr || nShips() == 0)
		return nullptr;

	Board b1(*this);
	Board b2(*this);
	return m_impl->play(p1, p2, b1, b2, shouldPause);
}