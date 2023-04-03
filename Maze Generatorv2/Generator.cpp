#include "Generator.h"
#include <time.h>
#include <iostream>
#include <algorithm>



double distanceCalculate(int x1, int y1, int x2, int y2)
{
	double x = x1 - x2; //calculating number to square in next step
	double y = y1 - y2;
	double dist;

	dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
	dist = sqrt(dist);

	return (int)dist;
}




Generator::Generator(int mazeSize) : window(sf::VideoMode(WIDTH, HEIGHT), "Maze Generator", sf::Style::Close | sf::Style::Titlebar), mesh(sf::Vector2f(0, 0), sf::Vector2f(HEIGHT, WIDTH), WIDTH / mazeSize)
{
	this->mazeSize = mazeSize;
	for (int i = 0; i < mazeSize; i++)
	{
		for (int j = 0; j < mazeSize; j++)
		{
			cellList.push_back(Cell(i, j));
		}
	}
	currentCell = &cellList[0];
	srand(time(NULL));
	//window.setFramerateLimit(10);
}

Generator::~Generator()
{
}

void Generator::run()
{
	while (window.isOpen())
	{
		if (simulation)
		{
			update();
		}
		window.clear();
		draw();
		window.display();
		pollEvent();
	}
}

void Generator::draw()
{
	
	sf::Vector2i coordinates = currentCell->getPosition();
	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(coordinates.x*WIDTH / mazeSize, coordinates.y*WIDTH / mazeSize));
	shape.setSize(sf::Vector2f(WIDTH / mazeSize, WIDTH / mazeSize));
	shape.setFillColor(sf::Color::Magenta);
	window.draw(shape);

	if (pathfinder)
	{
		for (int i = 0; i < openSet.size(); i++)
		{
			coordinates = openSet[i].getPosition();
			shape.setPosition(sf::Vector2f(coordinates.x*WIDTH / mazeSize, coordinates.y*WIDTH / mazeSize));
			shape.setSize(sf::Vector2f(WIDTH / mazeSize, WIDTH / mazeSize));
			shape.setFillColor(sf::Color::White);
			window.draw(shape);
		}

		for (int i = 0; i < closedSet.size(); i++)
		{
			coordinates = closedSet[i].getPosition();
			shape.setPosition(sf::Vector2f(coordinates.x*WIDTH / mazeSize, coordinates.y*WIDTH / mazeSize));
			shape.setSize(sf::Vector2f(WIDTH / mazeSize, WIDTH / mazeSize));
			shape.setFillColor(sf::Color::Red);
			window.draw(shape);
		}
	}
	
	if (!simulation)
	{
		Cell *node = end;
		while (node != nullptr)
		{
			coordinates = node->getPosition();
			shape.setPosition(sf::Vector2f(coordinates.x*WIDTH / mazeSize, coordinates.y*WIDTH / mazeSize));
			shape.setSize(sf::Vector2f(WIDTH / mazeSize, WIDTH / mazeSize));
			shape.setFillColor(sf::Color::Blue);
			window.draw(shape);
			node = node->previous;
		}
	}
	
	
	for (int i = 0; i < cellList.size(); i++)
	{
		mesh.draw(window, cellList[i]);
	}
	
}

void Generator::pollEvent()
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

void Generator::update()
{

	if (mazegenerator)
	{
		currentCell->swapVisited();
		sf::Vector2i currentCellPosition = currentCell->getPosition();
		bool neighbor[4] = { false,false,false,false };

		if (currentCellPosition.y - 1 >= 0)
		{
			int vectorPosition = turnPositionToVector(sf::Vector2i(currentCellPosition.x, currentCellPosition.y - 1));
			if (!cellList[vectorPosition].isVisited())
			{
				neighbor[0] = true;
			}

		}

		if (currentCellPosition.x + 1 < mazeSize)
		{
			int vectorPosition = turnPositionToVector(sf::Vector2i(currentCellPosition.x + 1, currentCellPosition.y));
			if (!cellList[vectorPosition].isVisited())
			{
				neighbor[1] = true;
			}

		}

		if (currentCellPosition.y + 1 < mazeSize)
		{
			int vectorPosition = turnPositionToVector(sf::Vector2i(currentCellPosition.x, currentCellPosition.y + 1));
			if (!cellList[vectorPosition].isVisited())
			{
				neighbor[2] = true;
			}

		}

		if (currentCellPosition.x - 1 >= 0)
		{
			int vectorPosition = turnPositionToVector(sf::Vector2i(currentCellPosition.x - 1, currentCellPosition.y));
			if (!cellList[vectorPosition].isVisited())
			{
				neighbor[3] = true;
			}

		}

		if (neighbor[0] == false && neighbor[1] == false && neighbor[2] == false && neighbor[3] == false)
		{
			if (cellStack.size() != 0)
			{
				currentCell = cellStack.top();
				cellStack.pop();
			}
			else
			{
				mazegenerator = false;
				pathfinder = true;
				start = &cellList[0];
				end = &cellList[cellList.size() - 1];
				openSet.push_back(*start);
			}
		}
		else
		{
			cellStack.push(currentCell);
			bool next = true;
			while (next)
			{
				int random = rand() % 4;
				if (neighbor[random] == true)
				{
					switch (random)
					{
					case 0:
						currentCell->destroyWall(0);
						currentCell = &cellList[turnPositionToVector(sf::Vector2i(currentCellPosition.x, currentCellPosition.y - 1))];
						currentCell->destroyWall(2);
						break;
					case 1:
						currentCell->destroyWall(1);
						currentCell = &cellList[turnPositionToVector(sf::Vector2i(currentCellPosition.x + 1, currentCellPosition.y))];
						currentCell->destroyWall(3);
						break;
					case 2:
						currentCell->destroyWall(2);
						currentCell = &cellList[turnPositionToVector(sf::Vector2i(currentCellPosition.x, currentCellPosition.y + 1))];
						currentCell->destroyWall(0);
						break;
					case 3:
						currentCell->destroyWall(3);
						currentCell = &cellList[turnPositionToVector(sf::Vector2i(currentCellPosition.x - 1, currentCellPosition.y))];
						currentCell->destroyWall(1);
						break;
					}
					next = false;
				}
			}
		}
	}
	else if(pathfinder)
	{
		if (openSet.size() > 0)
		{
			int lowestIndex = 0;
			for (int i = 0; i < openSet.size(); i++)
			{
				if (openSet[i].f < openSet[lowestIndex].f)
				{
					lowestIndex = i;
				}
			}

			Cell current = openSet[lowestIndex];
			Cell *prevCell=nullptr;
			if (openSet[lowestIndex].getPosition().x == end->getPosition().x && openSet[lowestIndex].getPosition().y == end->getPosition().y)
			{
				std::cout << "Znaleziono rozwiazanie" << std::endl;
				end = &openSet[lowestIndex];
				simulation = false;
			}
			else
			{
				auto it = openSet.begin();
				std::advance(it, lowestIndex);
				openSet.erase(it);
				closedSet.push_back(current);

				for (int i = 0; i < 4; i++)
				{
					bool isBlocked = current.getWall(i);
					Cell * neighbourCell = nullptr;
					if (!isBlocked)
					{
						switch (i)
						{
						case 0:
							neighbourCell = &cellList[turnPositionToVector(sf::Vector2i(current.getPosition().x, current.getPosition().y - 1))];
							break;
						case 1:
							neighbourCell = &cellList[turnPositionToVector(sf::Vector2i(current.getPosition().x + 1, current.getPosition().y))];
							break;
						case 2:
							neighbourCell = &cellList[turnPositionToVector(sf::Vector2i(current.getPosition().x, current.getPosition().y + 1))];
							break;
						case 3:
							neighbourCell = &cellList[turnPositionToVector(sf::Vector2i(current.getPosition().x - 1, current.getPosition().y))];
							break;
						}
						bool contain = false;
						prevCell = &cellList[turnPositionToVector(sf::Vector2i(current.getPosition().x, current.getPosition().y))];
						for (int k = 0; k < closedSet.size(); k++)
						{
							if (closedSet[k].getPosition().x == neighbourCell->getPosition().x && closedSet[k].getPosition().y == neighbourCell->getPosition().y)
							{
								contain = true;
								break;
							}
						}
						if (contain)
						{
							continue;
						}
						else
						{
							int tempG = current.g + 1;
							contain = false;
							for (int k = 0; k < openSet.size(); k++)
							{
								if (openSet[k].getPosition().x == neighbourCell->getPosition().x && openSet[k].getPosition().y == neighbourCell->getPosition().y)
								{
									contain = true;
									break;
								}
							}
							if (contain)
							{
								if (tempG < neighbourCell->g)
								{
									neighbourCell->g = tempG;
								}
							}
							else
							{
								neighbourCell->g = tempG;
							}

							neighbourCell->h = distanceCalculate(neighbourCell->getPosition().x, neighbourCell->getPosition().y, end->getPosition().x, end->getPosition().y);
							neighbourCell->f = neighbourCell->g + neighbourCell->h;
							neighbourCell->previous = prevCell;
							openSet.push_back(*neighbourCell);
							
						}
					}
				}

			}
		}
		else
		{
			std::cout << "Nie znaleziono rozwiazania" << std::endl;
		}
	}
}

int Generator::turnPositionToVector(sf::Vector2i position)
{
	return position.x + position.y*mazeSize;
}

