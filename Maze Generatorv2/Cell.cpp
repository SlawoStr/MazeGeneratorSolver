#include "Cell.h"

Cell::Cell(int i, int j)
{
	this->row = i;
	this->column = j;
	for (int k = 0; k < 4; k++)
	{
		walls[k] = true;
	}
	f = 0;
	g = 0;
	h = 0;
	previous = nullptr;
}

bool Cell::getWall(int i)
{
	return walls[i];
}

sf::Vector2i Cell::getPosition()
{
	return sf::Vector2i(column, row);
}

void Cell::destroyWall(int i)
{
	walls[i] = false;
}
