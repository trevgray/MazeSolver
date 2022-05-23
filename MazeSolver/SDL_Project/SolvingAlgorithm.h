#pragma once
#include "Maze.h"
class SolvingAlgorithm {
protected:
	Maze* maze;
public:
	virtual void SolveMaze() = 0;
	virtual void SetupAlgorithm(Maze* maze, int startVertexX, int startVertexY, int endVertexX, int endVertexY, bool mazeDone) = 0;
	~SolvingAlgorithm() { if (maze) { delete maze; } }
};