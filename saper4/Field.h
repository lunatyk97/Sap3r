#pragma once
#include <iostream>
#include <SDL.h>

#include "Col.h"

using namespace std;

class Field
{
	private:
		bool covered;
		bool mark = false;
	
	public:
		int value;
		int x;
		int y;
		SDL_Rect rect;
		Col color;

		bool isCovered();
		void setCover(bool value);
		void uncoverValue();

		bool isBoomb();
		bool isZero();
		
		bool marked();
		void setMark();
		void unMark();
};

