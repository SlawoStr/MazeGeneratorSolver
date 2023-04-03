#pragma once
#include <SFML/Graphics.hpp>

class Cell
{
public:
	Cell(int i, int j);

	bool getWall(int i);
	sf::Vector2i getPosition();
	void swapVisited() { visited = true; }
	bool isVisited() { return visited == true ? true : false; }
	void destroyWall(int i);
	int f;
	int g;
	int h;
	Cell *previous;
private:
	int row;
	int column;
	bool walls[4];
	bool visited = false;
};