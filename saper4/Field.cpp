#include "Field.h"
#include "Col.h"
#include <iostream>

using namespace std;

bool Field::isCovered()
 {
	if (Field::covered == true)
		return true;
	else
		return false;
}

void Field::setCover(bool value)
{
	Field::covered = value;
}

void Field::uncoverValue()
{
	Field::covered = false;
}

bool Field::isBoomb()
{
	if (Field::value == 9)
		return true;
	return false;
}

bool Field::isZero()
{
	if (Field::value == 0)
		return true;
	return false;
}
