#pragma once
#include <SDL.h>
#include "Board.h"

class Game
{
private:
	bool win;
	bool lose;
	bool runs;

	int player_x;
	int player_y;

	int mouse_x;
	int mouse_y;

	int size_x;
	int size_y;

	Board* tab;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Game();
	//~Game();

	void gameStart();
	void gameTurn();
	
	void setWin();
	void setLose();
	bool hadWon();
	bool hadLost();

	void drawBoard();
	void setPlayerXY();
	void setMouseXY();
	void checkPlayerXY();

	void gameOver();
	
	void gameRender();
	void clean();
	void update();
	void init(int width, int height);
	void handleEvents();

	bool running();
};

