#pragma once
#include "Board.h"

class Game
{
private:
	bool win;
	bool lose;
	int player_x;
	int player_y;
	Board* tab;

public:
	Game();
	void gameStart();
	void gameTurn();
	void setWin();
	void setLose();
	bool hadWon();
	bool hadLost();

	void setPlayerXY();
	void checkPlayerXY();

	void gameOver();
};

