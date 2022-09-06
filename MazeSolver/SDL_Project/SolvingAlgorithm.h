#pragma once
#include "Maze.h"
class SolvingAlgorithm {
protected:
	Maze* maze;
	bool searchComplete;
public:
	virtual void SolveMaze(Maze* maze, int startVertexX, int startVertexY, int endVertexX, int endVertexY, bool gradualGenerationBool) = 0;
	virtual ~SolvingAlgorithm() { if (maze) { delete maze; } }
	bool GetCompleted() {
		return searchComplete;
	}
};