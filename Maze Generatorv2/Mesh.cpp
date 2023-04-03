#include "Mesh.h"

Mesh::Mesh(sf::Vector2f start, sf::Vector2f end, int squareSize) :squareSize(squareSize)
{
	this->startingPosition = start;
	this->endingPosition = end;

}

Mesh::~Mesh()
{
}

void Mesh::draw(sf::RenderWindow & window,Cell cell)
{
	sf::Vector2i coordinates = cell.getPosition();
	sf::Vertex line[2];
	for (int i = 0; i < 4; i++)
	{
		bool wall = cell.getWall(i);
		if (wall)
		{
			switch (i)
			{
			case 0:

				line[0] = sf::Vertex(sf::Vector2f(coordinates.x*squareSize, coordinates.y*squareSize), sf::Color::Yellow);
				line[1] = sf::Vertex(sf::Vector2f((coordinates.x*squareSize)+squareSize, coordinates.y*squareSize), sf::Color::Yellow);

				window.draw(line, 2, sf::Lines);
				break;
			case 1:
				line[0] = sf::Vertex(sf::Vector2f((coordinates.x*squareSize) + squareSize, coordinates.y*squareSize), sf::Color::Yellow);
				line[1] = sf::Vertex(sf::Vector2f((coordinates.x*squareSize) + squareSize, (coordinates.y*squareSize)+squareSize), sf::Color::Yellow);

				window.draw(line, 2, sf::Lines);
				break;
			case 2:
				line[0] = sf::Vertex(sf::Vector2f(coordinates.x*squareSize, coordinates.y*squareSize+squareSize), sf::Color::Yellow);
				line[1] = sf::Vertex(sf::Vector2f((coordinates.x*squareSize) + squareSize, (coordinates.y*squareSize) + squareSize), sf::Color::Yellow);

				window.draw(line, 2, sf::Lines);
				break;
			case 3:
				line[0] = sf::Vertex(sf::Vector2f(coordinates.x*squareSize, coordinates.y*squareSize), sf::Color::Yellow);
				line[1] = sf::Vertex(sf::Vector2f(coordinates.x*squareSize, coordinates.y*squareSize+squareSize), sf::Color::Yellow);

				window.draw(line, 2, sf::Lines);
				break;
			}
		}
	}
}