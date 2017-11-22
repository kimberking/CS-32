#ifndef PIZZA4
#define PIZZA4
#include "globals.h"
#include <string>
using namespace std;
class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};


#endif