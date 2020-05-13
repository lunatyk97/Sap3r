#include <iostream>
#include <string>
#include <random>

#include "Field.h"
#include "Board.h"
#include "Game.h"

using namespace std;

Game::Game()
{
	this->lose = false;
	this->win = false;

	this->player_x = 0;
	this->player_y = 0;
	gameStart();
}

void Game::gameStart() 
{
	int sx, sy, b;

	cout << "New game started" << endl << endl;
	cout << "Podaj wymiary pola x y: ";
	cin >> sx >> sy;

	cout << "Podaj liczbe bomb: ";
	cin >> b;

	Board tablica(sx,sy,b);
	tablica.createBoard();
	tablica.fillBoard();

	this->tab = &tablica;

	while (!lose)
	{
		if (win)
			break;
		gameTurn();
	}
	
	cout << "END" << endl;
	gameOver();
}	

void Game::gameTurn() 
{
	cout << "Trun" << endl << endl;
	tab->showBoard();
	setPlayerXY();
	checkPlayerXY();

	if (tab->uncovered_num >= (tab->size_x * tab->size_y) - tab->bombs)
		setWin();

}

void Game::setWin() 
{
	this->win = true;
	cout << "you win!!!!!!!!" << endl;
}

void Game::setLose()
{
	this->lose = true;
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
	else 
	{
		cout << tab->plansza2d[player_y][player_x].value << endl;
		tab->plansza2d[player_y][player_x].uncoverValue();
		tab->addOneUncover();
	}

}

void Game::gameOver() 
{
	tab->showBoard();
	cout << "GAME OVER" << endl;
	tab->destroyBoard();
}