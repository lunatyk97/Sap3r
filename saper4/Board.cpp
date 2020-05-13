#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

Board::Board()
{
    this->size_x = 10;
    this->size_y = 10;
    this->bombs = 10;
    this->bomb_val = 9;
    this->uncovered_num = 0;
    srand(static_cast<unsigned int>(time(NULL))); //srand(time(NULL)) - inna implementacja, bo ta robi warning debugera
}

Board::Board(int size_x, int size_y)
{
    if (size_x > min_size && size_y > min_size)
    {
        this->size_x = size_x;
        this->size_y = size_y;
    }
    else
    {
        cout << "Zbyt maly rozmiar" << endl;
        this->size_x = min_size;
        this->size_y = min_size;
    }
    this->uncovered_num = 0;
    this->bombs = 10;
    this->bomb_val = 9;

    srand(static_cast<unsigned int>(time(NULL)));
}

Board::Board(int size_x, int size_y, int bombs_number)
{
    this->size_x = size_x;
    this->size_y = size_y;

    if (bombs_number < size_x * size_y)
    {
        this->bombs = bombs_number;
    }
    else
    {
        this->bombs = (size_x * size_y) - 4;
    }
    this->bomb_val = 9;
    this->uncovered_num = 0;
    srand(static_cast<unsigned int>(time(NULL)));
}

Field **Board::createBoard()
{
	plansza2d = new Field * [size_y];

	if (plansza2d == NULL)
		cout << "Blad przydzialu pamieci tablicy" << endl;

    for (int i = 0; i < size_y; i++)
    {
        plansza2d[i] = new Field[size_x];
        if (plansza2d[i] == NULL)
        {
            cout << "Blad przydzialu pamieci #2!\n" << endl;
        }
    }

    return plansza2d;
}



void Board::fillBoard()
{
    fillBoardWithZeros();
    fillBoardWithBombs();
    fillBoardWithNum();
}

void Board::showBoard()
{
    cout << endl << "BOARD:" << endl;
    for (int y = 0; y < size_y; y++)
    {
        for (int x = 0; x < size_x; x++)
        {
            if (plansza2d[y][x].isCovered() == false)
                cout << plansza2d[y][x].value << " ";
            else
                cout << ". ";
        }
        cout << endl;
    }

    cout << "BOARD_END" << endl << endl;
}

void Board::showField(int x, int y)
{
    cout << "x: " << plansza2d[y][x].x << ", ";
    cout << "y: " << plansza2d[y][x].y << ", ";
    cout << "w: " << plansza2d[y][x].value << ", ";
    cout << "c: " << plansza2d[y][x].isCovered() << endl;
}

void Board::destroyBoard()
{
    delete[] plansza2d;
}

void Board::fillBoardWithZeros()
{
    int num = 0;
    for (int y = 0; y < size_y; y++)
    {
        for (int x = 0; x < size_x; x++)
        {
            plansza2d[y][x].x = x;
            plansza2d[y][x].y = y;
            plansza2d[y][x].setCover(true);
            plansza2d[y][x].value = 0;
        }
    }
}

void Board::fillBoardWithBombs()
{
    int counter = 0;
    int min = 0;
    int x;
    int y;

    while (counter < bombs) {
        
        x = min + (rand() % ((size_x - 1) + 1 - min));
        y = min + (rand() % ((size_y - 1) + 1 - min));
        if (plansza2d[y][x].value == 0) {
            plansza2d[y][x].value = bomb_val;
            counter++;
        }
    }
}

void Board::fillBoardWithNum()
{
    for (int y = 0; y < size_y; y++)
    {
        for (int x = 0; x < size_x; x++)
        {
            if (plansza2d[y][x].value != bomb_val) {
                int licznik = 0;
                if (y > 0 && x > 0)
                {
                    if (plansza2d[y - 1][x - 1].value == bomb_val) licznik++;
                }
                if (y > 0)
                {
                    if (plansza2d[y - 1][x].value == bomb_val) licznik++;
                }
                if (y > 0 && x < size_x - 1)
                {
                    if (plansza2d[y - 1][x + 1].value == bomb_val) licznik++;
                }
                if (x > 0)
                {
                    if (plansza2d[y][x - 1].value == bomb_val) licznik++;
                }
                if (x < size_x - 1)
                {
                    if (plansza2d[y][x + 1].value == bomb_val) licznik++;
                }
                if (y < size_y - 1 && x > 0)
                {
                    if (plansza2d[y + 1][x - 1].value == bomb_val) licznik++;
                }
                if (y < size_y - 1)
                {
                    if (plansza2d[y + 1][x].value == bomb_val) licznik++;
                }
                if (y < size_y - 1 && x < size_x - 1)
                {
                    if (plansza2d[y + 1][x + 1].value == bomb_val) licznik++;
                }
                if (licznik != 0) plansza2d[y][x].value = licznik;
            }
        }
    }
}

void Board::fillBoardWithSeries()
{
    int num = 0;
    for (int y = 0; y < size_y; y++)
    {
        for (int x = 0; x < size_x; x++)
        {
            plansza2d[y][x].x = x;
            plansza2d[y][x].y = y;
            plansza2d[y][x].setCover(false);
            plansza2d[y][x].value = num++;
        }
    }
}

void Board::uncoverNearZeros(int x, int y) {

    if (plansza2d[y][x].isCovered()) {

        
        plansza2d[y][x].setCover(false);
        addOneUncover();
        //showBoard();
        //cout << "x: " << plansza2d[y][x].x <<", y: " << plansza2d[y][x].y<< endl;
        
        if (plansza2d[y][x].value == 0) {
            
            if (x + 1 < size_x)
                uncoverNearZeros(x + 1, y);
            if (x - 1 >= 0)
                uncoverNearZeros(x - 1, y);
            if (y + 1 < size_y)
                uncoverNearZeros(x, y + 1);
            if (y - 1 >= 0)
                uncoverNearZeros(x, y - 1);
            
        }
    }
}

void Board::addOneUncover()
{
    uncovered_num++;
}