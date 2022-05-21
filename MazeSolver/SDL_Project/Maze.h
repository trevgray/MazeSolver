#pragma once
#include "Node.h"
#include "Graphs.h"
class Maze {
protected:
	int mazeSize;
public:
	Graph<int>* MazeGraph;
	Node nodeArray[50][50];
	virtual void Generate() = 0;
	int GetSize() {
		return mazeSize;
	}
};

