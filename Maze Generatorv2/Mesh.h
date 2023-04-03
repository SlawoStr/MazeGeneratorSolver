#pragma once
#include "SFML/Graphics.hpp"
#include "Cell.h"

class Mesh
{
public:
	Mesh(sf::Vector2f start, sf::Vector2f end, int squareSize);
	~Mesh();
	void draw(sf::RenderWindow & window,Cell cell);
private:
	sf::Vector2f startingPosition;
	sf::Vector2f endingPosition;
	const int squareSize;
};