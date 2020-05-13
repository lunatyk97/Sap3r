#pragma once
#include <iostream>

using namespace std;

class Field
{
	private:
		bool covered;
	
	public:
		int value;
		int x;
		int y;

		bool isCovered();
		void setCover(bool value);
		void uncoverValue();

		bool isBoomb();
		bool isZero();
};

