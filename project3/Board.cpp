#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

using namespace std;

class BoardImpl
{
public:
	BoardImpl(const Game& g);
	void clear();
	void block();
	void unblock();
	bool placeShip(Point topOrLeft, int shipId, Direction dir);
	bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
	void display(bool shotsOnly) const;
	bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
	bool allShipsDestroyed() const;
	~BoardImpl();

private:
	// TODO:  Decide what private members you need.  Here's one that's likely
	//        to be useful:
	const Game& m_game;
	int mRows;
	int mCols;
	char map[MAXROWS][MAXCOLS];
	int mnShips;

	class shipInformation {
	public:
		shipInformation(int shipID, Point topOrLeft, Direction dir) : nat_startPoint(topOrLeft), nat_shipID(shipID), nat_segmentsHit(0), nat_direction(dir) {}
		Point  nat_startPoint;
		int   nat_shipID;
		int nat_segmentsHit;
		Direction nat_direction;
	};
	vector <shipInformation*> trackInfo;
};

BoardImpl::~BoardImpl() {

	for (int k = 0; k < trackInfo.size(); k++)
		delete trackInfo[k];
}

BoardImpl::BoardImpl(const Game& g) : m_game(g), mRows(g.rows()), mCols(g.cols()), mnShips(g.nShips())
{
	for (int k = 0; k < mRows; k++)
		for (int i = 0; i < mCols; i++)
		{
			map[k][i] = '.';
		}
}

void BoardImpl::clear() //same as boardimpl
{
	for (int k = 0; k < mRows; k++)
		for (int i = 0; i < mCols; i++)
		{
			map[k][i] = '.';
		}
}

void BoardImpl::block()
{
	// Block cells with 50% probability
	for (int r = 0; r < m_game.cols(); r++)
		for (int c = 0; c < m_game.rows(); c++)
			if (randInt(2) == 0)
			{
				map[r][c] = '#'; //from the spec
			}
}

void BoardImpl::unblock()
{
	for (int r = 0; r < m_game.cols(); r++)
		for (int c = 0; c < m_game.rows(); c++)	{
			if (map[r][c] == '#')
				map[r][c] = '.';
		}
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	if (shipId<0 || shipId >= m_game.nShips() || topOrLeft.r<0 || topOrLeft.r >= m_game.rows() || topOrLeft.c < 0 || topOrLeft.c >= m_game.cols())
	{
		return false;
	}

	if ((dir == HORIZONTAL && topOrLeft.c + m_game.shipLength(shipId)>m_game.cols()) || (dir == VERTICAL && topOrLeft.r + m_game.shipLength(shipId)>m_game.rows()))
	{
		return false;
	}

	for (int k = 0; k<trackInfo.size(); k++) //check for already placed ship
	{
		if (trackInfo[k]->nat_shipID == shipId)
		{
			return false;
		}

	}
	if (dir == HORIZONTAL)
	{
		for (int k = topOrLeft.c; k<topOrLeft.c + m_game.shipLength(shipId); k++) //check if there is already a ship there
		{
			if (map[topOrLeft.r][k] != '.')
			{
				return false;
			}
		}

		for (int k = topOrLeft.c; k<topOrLeft.c + m_game.shipLength(shipId); k++) //place ship
		{
			map[topOrLeft.r][k] = m_game.shipSymbol(shipId);
		}
	}
	if (dir == VERTICAL)
	{
		for (int k = topOrLeft.r; k<topOrLeft.r + m_game.shipLength(shipId); k++) //make sure there isnt an existing ship
		{
			if (map[k][topOrLeft.c] != '.')
			{
				return false;
			}
		}

		for (int k = topOrLeft.r; k<topOrLeft.r + m_game.shipLength(shipId); k++) //place the ship
		{
			map[k][topOrLeft.c] = m_game.shipSymbol(shipId);
		}
	}

	trackInfo.push_back(new shipInformation(shipId, topOrLeft, dir));
	return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	bool validShip = false;                           //if ship has an id then place is left or top
	for (int k = 0; k<trackInfo.size(); k++)
	{
		if ((trackInfo[k]->nat_shipID) == shipId && (trackInfo[k]->nat_startPoint.r == topOrLeft.r) && (trackInfo[k]->nat_startPoint.c == topOrLeft.c) && (trackInfo[k]->nat_direction == dir)) {
			validShip = true;
		}
	}
		
	if (!validShip) {
		return(false);
	}

	if (dir == HORIZONTAL)
	{
		for (int k = topOrLeft.c; k < topOrLeft.c + m_game.shipLength(shipId); k++) //unplace ship
		{
			map[topOrLeft.r][k] = '.';
		}
	}
	if (dir == VERTICAL)
	{

		for (int k = topOrLeft.r; k < topOrLeft.r + m_game.shipLength(shipId); k++) //unplace ship
		{
			map[k][topOrLeft.c] = '.';
		}
	}
	for (int k = 0; k<trackInfo.size(); k++)
	{
		if (trackInfo[k]->nat_shipID == shipId)
		{
			delete trackInfo[k];    
			trackInfo.erase(trackInfo.begin() + k);   //remove id
			return true;
		}
	}
	return true;
}

void BoardImpl::display(bool shotsOnly) const
{
	cout << "  ";                                //numbers on the board
	
	for (int k = 0; k < m_game.cols(); k++) {
		cout << k;
	}
	cout << endl;

	int k = 0;
	int i = 0;

	for (k = 0; k < m_game.rows(); k++) {
		for (i = 0; i < m_game.cols(); i++) {
			if (i == 0)
				cout << k << " ";       //output row numbers

			if (!shotsOnly)
				cout << map[k][i];   //if shotsOnly is false, show everything
			else
			{
				if (map[k][i] != 'X' && map[k][i] != 'o' && map[k][i] != '.')  //if it's a letter, show a dot
					cout << '.';
				else
					cout << map[k][i];  //if it's not a letter, show what is really there
			}
		}
		cout << endl;
	}
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	if (p.r<0 || p.c<0 || p.r >= mRows || p.c >= mCols || map[p.r][p.c] == 'o' || map[p.r][p.c] == 'X') //out of bounds
	{
		shotHit = false;
		shipDestroyed = false;
		return false;
	}


	if (map[p.r][p.c] == '.')
	{
		map[p.r][p.c] = 'o'; //miss
		shotHit = false;
	}

	else
	{

		shotHit = true;
		for (int k = 0; k<trackInfo.size(); k++) //type of ship it hits
		{
			if (map[p.r][p.c] == m_game.shipSymbol(trackInfo[k]->nat_shipID))
			{
				(trackInfo[k]->nat_segmentsHit)++;
				if (trackInfo[k]->nat_segmentsHit >= m_game.shipLength(trackInfo[k]->nat_shipID))
				{
					shipDestroyed = true; //destoryed the ship
					shipId = trackInfo[k]->nat_shipID;
					mnShips--;
					break;
				}
				else {
					shipDestroyed = false;
					break;
				}
			}
		}

		map[p.r][p.c] = 'X'; //mark a hit
	}
	return true;
}

bool BoardImpl::allShipsDestroyed() const //if everything is destoryed
{
	if (mnShips == 0)
		return true;
	else 
		return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
	m_impl = new BoardImpl(g);
}

Board::~Board()
{
	delete m_impl;
}

void Board::clear()
{
	m_impl->clear();
}

void Board::block()
{
	return m_impl->block();
}

void Board::unblock()
{
	return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
	m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
	return m_impl->allShipsDestroyed();
}
