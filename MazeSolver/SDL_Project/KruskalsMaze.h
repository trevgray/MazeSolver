#pragma once
#include "Maze.h"
class KruskalsMaze : public Maze {
private:

public:
	KruskalsMaze();
	KruskalsMaze(int n);
	void Generate();
};