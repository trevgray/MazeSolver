#pragma once
#include "Node.h"
class Maze {
public:
	//Maze();
	//Maze(int n);
	Node nodeArray[100][100];
	virtual void Generate(int n) = 0;
};

