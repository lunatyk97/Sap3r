#include <iostream>
#include <string>
#include <random>
#include <SDL.h>

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
}

void Game::gameStart()
{
	int sx, sy, b;

	// poki co maks dzialajacy rozmiar to 60 x 60

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

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	drawBoard();
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
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
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
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	cout << "Game cleaned" << endl;
}

void Game::gameTurn() 
{
	cout << "Trun" << endl << endl;
	tab->showBoard();
	//setPlayerXY();
	setMouseXY();
	checkPlayerXY();

	cout << "uncovered: " << tab->uncovered_num << "full size: " << (tab->size_x * tab->size_y) << "bombs: " << tab->bombs << endl;
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

void Game::checkPlayerXY()
{
	//Warunek wyjœcia z funkcji
	if (player_x >= tab->size_x || player_x < 0 || player_y >= tab->size_y || player_y < 0)
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
				SDL_SetRenderDrawColor(renderer, tab->plansza2d[y][x].color.r, tab->plansza2d[y][x].color.g, tab->plansza2d[y][x].color.b, 255);
				SDL_RenderFillRect(renderer, &tab->plansza2d[y][x].rect);
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
