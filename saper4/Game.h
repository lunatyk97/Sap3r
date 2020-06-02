#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "Board.h"

class Game
{
private:
	bool win;
	bool lose;
	bool runs;
	bool play_again;
	
	bool menu;
	bool end_menu;
	bool difficulty_menu;

	int player_x; //przypisywane jako ruch gracza
	int player_y;

	int mouse_x; //przechwytywanie z events
	int mouse_y;

	int size_x; //w odniesieniu do struktury
	int size_y;

	int windwow_size_x;
	int window_size_y;

	int points = 0;
	int bombs;
	int difficulty;

	Board* tab;
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	SDL_Rect bombs_window;
	SDL_Rect points_window;
	SDL_Rect end_window;
	SDL_Rect end_message_window;

	SDL_Rect menu_window;
	SDL_Rect small_board_button;
	SDL_Rect medium_board_button;
	SDL_Rect large_board_button;
	SDL_Rect yes;
	SDL_Rect no;

	SDL_Color black_color = { 0, 0, 0 };
	SDL_Color white_color = { 255, 255, 255 };
	SDL_Color red_color = { 255, 0, 0 };
	SDL_Color green_color = { 0, 255, 50 };

	TTF_Font* font;
	TTF_Font* font1;

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
	void setMarkXY(int mx, int my);
	void setMouseOnDiffButton();
	void setMouseOnYesNo();
	void setMouseOnButton();
	void setMouseXY();
	void checkPlayerXY();

	void gameOver();
	
	void menuDiffRender();
	void endMenuRender();
	void menuRender();
	void gameRender();
	void winRender();
	void loseRender();

	void clean();
	void update();
	void init(int width, int height);
	void handleEvents();

	bool running();
};

