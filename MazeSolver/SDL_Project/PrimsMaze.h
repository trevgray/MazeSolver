#pragma once
#include "Maze.h"
class PrimsMaze : public Maze {
public:
	PrimsMaze();
	PrimsMaze(int n);
	void Generate(int n);
};

