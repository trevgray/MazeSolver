#pragma once
#include "Node.h"

class Maze {
protected:
	int mazeSize;
public:
	Node nodeArray[100][100];
	virtual void Generate() = 0;
	int GetSize() {
		return mazeSize;
	}
	//~Maze() { delete nodeArray; };
};

