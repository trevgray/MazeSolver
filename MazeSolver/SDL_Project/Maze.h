#pragma once
#include "Node.h"
#include <iostream>

class Maze {
protected:
	int mazeSize;
	bool mazeCompleted;
public:
	Node nodeArray[51][51];
	virtual void Generate(bool gradualGeneration) = 0;
	int GetSize() {
		return mazeSize;
	}
	bool GetCompleted() {
		return mazeCompleted;
	}
	virtual ~Maze() {
		for (int x = 0; x <= mazeSize; x++) {
			for (int y = 0; y <= mazeSize; y++) {
				nodeArray[x][y].~Node();
			}
		}
	};
};

