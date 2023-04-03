#pragma once
#include <SFML/Graphics.hpp>
#include "Mesh.h"
#include "Cell.h"
#include <stack>

constexpr int WIDTH = 1000, HEIGHT = 1000;

class Generator
{
public:
	Generator(int mazeSize);
	~Generator();
	void run();
private:
	void draw();
	void pollEvent();
	void update();
	int turnPositionToVector(sf::Vector2i position);
private:
	Mesh mesh;
	int mazeSize;
	sf::RenderWindow window;
	std::vector<Cell> cellList;
	std::stack<Cell*> cellStack;
	Cell * currentCell;
private:
	bool mazegenerator = true;
	bool pathfinder = false;
	bool simulation = true;
private:
	Cell *start;
	Cell *end;
	std::vector<Cell> openSet;
	std::vector<Cell> closedSet;
};