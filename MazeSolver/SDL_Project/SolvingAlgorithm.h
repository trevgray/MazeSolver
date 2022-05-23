#pragma once
#include "Maze.h"
class SolvingAlgorithm {
protected:
	Maze* maze;
public:
	virtual void SolveMaze(Maze* maze, int startVertexX, int startVertexY, int endVertexX, int endVertexY) = 0;
	~SolvingAlgorithm() { if (maze) { delete maze; } }
};