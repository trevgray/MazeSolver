#pragma once
#include "Node.h"
class Maze {
public:
	//Maze();
	//Maze(int n);
	Node nodeArray[50][50];
	int mazeSize;
	virtual void Generate(int n) = 0;
};

