#pragma once
#include "Node.h"

class Maze {
protected:
	int mazeSize;
public:
	Node nodeArray[50][50];
	virtual void Generate() = 0;
	int GetSize() {
		return mazeSize;
	}
	~Maze() { 
		/*for (int x = 0; x <= mazeSize; x++) {
			for (int y = 0; y <= mazeSize; y++) {
				nodeArray[x][y] = Node();
			}
		}*/
	};
};

