#pragma once
#include <iostream>
#include "Field.h"

using namespace std;


class Board
{
public:
	int size_x;
	int size_y;
	int uncovered_num;
	int bombs;

private:
	int bomb_val;
	int min_size = 4;

public:
	Field** plansza2d;

	Board();
	Board(int size_x, int size_y);
	Board(int size_x, int size_y, int bombs_number);

	Field** createBoard();
	void fillBoard();
	void showBoard();
	void showField(int x, int y);
	void destroyBoard();

	void fillBoardWithZeros();
	void fillBoardWithBombs();
	void fillBoardWithNum();
	void fillBoardWithSeries();
	void fillBoardWithRectangles(int x_pos, int y_pos, int w, int h);
	void fillBoardWithColors(int x, int y, int val);

	void uncoverNearZeros(int x, int y);
	void addOneUncover();
};

