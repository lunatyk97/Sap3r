#include <iostream>
#include <string>
#include <random>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Field.h"
#include "Board.h"
#include "Game.h"

using namespace std;

Game::Game()
{
	this->lose = false;
	this->win = false;

	this->player_x = 1000;
	this->player_y = 1000;

	init(600,400);
	gameStart();
}
 
void Game::init(int width, int height)
{
	//Sprawdzenie czy jest dostep do bibioteki SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "BLAD SDL" << endl;
		runs = false;
	}

	//Inicjalizacja Biblioteki
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	//Zbudowanie okna
	window = SDL_CreateWindow("SAP3R", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window)
		cout << "Window created" << endl;

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		cout << "Rendered created" << endl;
	}
	runs = true;

	//Budowanie poszczegolnych okienek gry
	bombs_window.x = 10;
	bombs_window.y = 10;
	bombs_window.w = 200;
	bombs_window.h = 20;

	points_window.x = 220;
	points_window.y = 10;
	points_window.w = 200;
	points_window.h = 20;

	end_window.x = 10;
	end_window.y = 10;
	end_window.w = width - (end_window.x * 2);
	end_window.h = height - (end_window.y * 2);

	//Inicjalizacja czcionki
	font = TTF_OpenFont("Dashboard.ttf", 18);
	font1 = TTF_OpenFont("MOMCAKE.ttf", 10);
}

void Game::gameStart()
{
	int sx, sy, b;

	// poki co maks dzialajacy rozmiar to 60 x 60

	// TUTAJ PRZEROBIÆ TEN POCZATEK NA MENU!
	cout << "New game started" << endl << endl;
	cout << "Podaj wymiary pola x y: ";
	cin >> sx >> sy;

	cout << "Podaj liczbe bomb: ";
	cin >> b;

	size_x = sx;
	size_y = sy;

	Board tablica(sx, sy, b);
	tablica.createBoard();
	tablica.fillBoard();

	this->tab = &tablica;


	while (running())
	{
		gameRender();
		handleEvents();
		update();
	}

	cout << "END" << endl;

	gameOver();
}

void Game::gameRender()
{
	SDL_RenderClear(renderer);

	//Bombs && Points Window
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &bombs_window);
	SDL_RenderFillRect(renderer, &points_window);
	
	drawBoard();
	
	if (running() == false)
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		cout << "Kaj ta rozowo kolumna" << endl;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
		SDL_RenderFillRect(renderer, &end_window);

		// <----
		/*Testy z tekstem
		SDL_Color black_color = { 0, 0, 0 };
		SDL_Surface* surface_message = TTF_RenderText_Solid(font, "text", black_color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_message);

		int texW = 20;
		int texH = 20;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		SDL_Rect dstrect = { 0, 0, texW, texH };

		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_Delay(5000);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface_message); 
		// ---> */
	}

	SDL_Color black_color = { 0, 0, 0 };
	SDL_Color white_color = { 255, 255, 255 };
	int texW = 20;
	int texH = 10;

	SDL_Surface* surface_message = TTF_RenderText_Solid(font, "punkty: " , black_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_message);
	SDL_QueryTexture(texture, NULL, NULL, &points_window.w, &points_window.h);
	SDL_Rect dstrect = { points_window.x, points_window.y, points_window.w, points_window.h };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface_message);


	char wt[10];
	_itoa_s(points, wt, 10); // pobieranie informacji o liczbie i zamiana jej na array
	SDL_Surface* surface_message1 = TTF_RenderText_Solid(font, wt, white_color);
	SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface_message1);
	SDL_QueryTexture(texture1, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect22 = { points_window.x + points_window.w + 10, points_window.y, texW, points_window.h };
	SDL_RenderCopy(renderer, texture1, NULL, &dstrect22);
	SDL_DestroyTexture(texture1);
	SDL_FreeSurface(surface_message1);
	

	SDL_Surface* surface_bomb = TTF_RenderText_Solid(font, "bomby: ", black_color);
	SDL_Texture* texture_bomb = SDL_CreateTextureFromSurface(renderer, surface_bomb);
	SDL_QueryTexture(texture_bomb, NULL, NULL, &bombs_window.w, &bombs_window.h);
	SDL_Rect dstrect1 = { bombs_window.x, bombs_window.y,bombs_window.w,bombs_window.h };
	SDL_RenderCopy(renderer, texture_bomb, NULL, &dstrect1);
	SDL_DestroyTexture(texture_bomb);
	SDL_FreeSurface(surface_bomb);

	char wt2[10];
	_itoa_s(tab->bombs, wt2, 10);
	SDL_Surface* surface_bomb1 = TTF_RenderText_Solid(font, wt2, white_color);
	SDL_Texture* texture_bomb1 = SDL_CreateTextureFromSurface(renderer, surface_bomb1);
	SDL_QueryTexture(texture_bomb1, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect12 = { bombs_window.x + bombs_window.w + 10, bombs_window.y, texW, bombs_window.h };
	SDL_RenderCopy(renderer, texture_bomb1, NULL, &dstrect12);
	SDL_DestroyTexture(texture_bomb1);
	SDL_FreeSurface(surface_bomb1);


	//Renderowanie elementow rendera
	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		runs = false;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_GetMouseState(&mouse_x, &mouse_y);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			int x;
			int y;
			SDL_GetMouseState(&x, &y);
			setMarkXY(x, y);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		}
		break;
	}
}
 
void Game::update()
{
	gameTurn();
}

void Game::clean()
{
	//Zakonczenie dzialania Biblioteki
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	cout << "Game cleaned" << endl;
}

void Game::gameTurn() 
{
	cout << "Trun" << endl << endl;
	//tab->showBoard();
	//setPlayerXY();
	setMouseXY();
	checkPlayerXY();

	//cout << "uncovered: " << tab->uncovered_num << "full size: " << (tab->size_x * tab->size_y) << "bombs: " << tab->bombs << endl;
	if (tab->uncovered_num >= ((tab->size_x * tab->size_y) - tab->bombs))
	{
		cout << "LOOK";
		cout << "uncovered: " << tab->uncovered_num << "full size: " << (tab->size_x * tab->size_y) << "bombs: " << tab->bombs << endl;
		setWin();
	}
}

void Game::setWin() 
{
	this->win = true;
	this->runs = false;
	cout << "you win!!!!!!!!" << endl;
}

void Game::setLose()
{
	this->lose = true;
	this->runs = false;
}

bool Game::hadWon()
{
	if (this->win == true)
		return true;
	return false;
}

bool Game::hadLost()
{
	if (this->lose == true)
		return true;
	return false;
}

void Game::setPlayerXY() {
	cout << "Podaj x: ";
	cin >> this->player_x;
	cout << "Podaj y: ";
	cin >> this->player_y;
}

void Game::setMouseXY()
{

	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (mouse_x >= tab->plansza2d[y][x].rect.x && mouse_x < (tab->plansza2d[y][x].rect.x + tab->plansza2d[y][x].rect.w)) {
				if (mouse_y >= tab->plansza2d[y][x].rect.y && mouse_y < (tab->plansza2d[y][x].rect.y + tab->plansza2d[y][x].rect.h))
				{

					this->player_x = x;
					this->player_y = y;
				
				}
			}
		}
	}
}


void Game::setMarkXY(int mx, int my)
{

	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (mx >= tab->plansza2d[y][x].rect.x && mx < (tab->plansza2d[y][x].rect.x + tab->plansza2d[y][x].rect.w)) {
				if (my >= tab->plansza2d[y][x].rect.y && my < (tab->plansza2d[y][x].rect.y + tab->plansza2d[y][x].rect.h))
				{
					if(!tab->plansza2d[y][x].marked())
						tab->plansza2d[y][x].setMark();
					else
						tab->plansza2d[y][x].unMark();
				}
			}
		}
	}
}

void Game::checkPlayerXY()
{

	//Warunek wyjœcia z funkcji
	if (player_x >= tab->size_x || player_x < 0 || player_y >= tab->size_y || player_y < 0 )
	{	
		cout << "Poza plansza, sproboj jeszcze raz!" << endl;
		return;
	}

	if (tab->plansza2d[player_y][player_x].isBoomb())
	{
		cout << "Trafiles na bombe" << endl;
		tab->plansza2d[player_y][player_x].uncoverValue();
		setLose();
		//zaznaczenie trafienia tej bomby na polu
	}
	else if (tab->plansza2d[player_y][player_x].isZero())
	{
		//cout << "Trafiles na puste pole" << endl;
		tab->uncoverNearZeros(player_x, player_y);
	}
	else if(tab->plansza2d[player_y][player_x].value > 0 && tab->plansza2d[player_y][player_x].value < 9 && tab->plansza2d[player_y][player_x].isCovered())
	{
		cout << tab->plansza2d[player_y][player_x].value << endl;
		tab->plansza2d[player_y][player_x].uncoverValue();
		tab->addOneUncover();
		points += tab->plansza2d[player_y][player_x].value;
	}

}

void Game::drawBoard()
{
	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (!tab->plansza2d[y][x].isCovered())
			{
				char wt[10];
				_itoa_s(tab->plansza2d[y][x].value, wt, 10); // pobieranie informacji o liczbie i zamiana jej na array

				SDL_SetRenderDrawColor(renderer, tab->plansza2d[y][x].color.r, tab->plansza2d[y][x].color.g, tab->plansza2d[y][x].color.b, 255);
				SDL_RenderFillRect(renderer, &tab->plansza2d[y][x].rect);
			
				if (tab->plansza2d[y][x].value > 0)
				{
					SDL_Color black_color = { 0, 0, 0 };
					SDL_Surface* surface_message = TTF_RenderText_Solid(font, wt, black_color);
					SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_message);

					int w = 0;
					int h = 0;

					SDL_QueryTexture(texture, NULL, NULL, &w, &h);
					SDL_Rect dstrect = { tab->plansza2d[y][x].rect.x,  tab->plansza2d[y][x].rect.y, w, h };

					SDL_RenderCopy(renderer, texture, NULL, &dstrect);

					SDL_DestroyTexture(texture);
					SDL_FreeSurface(surface_message);
				}

				if (!runs && tab->plansza2d[y][x].value == 9)
				{
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderDrawRect(renderer, &tab->plansza2d[y][x].rect);	
				}

			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &tab->plansza2d[y][x].rect);

				if (tab->plansza2d[y][x].marked())
				{
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					SDL_RenderFillRect(renderer, &tab->plansza2d[y][x].rect);
				}

				if (hadLost() && tab->plansza2d[y][x].value == 9)
				{
					SDL_Color red_color = { 255, 0, 0 };
					SDL_Surface* surface_message = TTF_RenderText_Solid(font, " X ", red_color);
					SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_message);

					int w = 0;
					int h = 0;

					SDL_QueryTexture(texture, NULL, NULL, &w, &h);
					SDL_Rect dstrect = { tab->plansza2d[y][x].rect.x,  tab->plansza2d[y][x].rect.y, w, h };

					SDL_RenderCopy(renderer, texture, NULL, &dstrect);

					SDL_DestroyTexture(texture);
					SDL_FreeSurface(surface_message);
				}
			}
		}
	}
}

void Game::gameOver() 
{
	gameRender();
	SDL_Delay(8000);
	tab->showBoard();
	cout << "GAME OVER" << endl;
	tab->destroyBoard();
}

bool Game::running(){ return runs; }
