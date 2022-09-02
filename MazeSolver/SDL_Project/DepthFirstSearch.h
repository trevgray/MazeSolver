#pragma once
#include "SolvingAlgorithm.h"
#include <vector>
class DepthFirstSearch : public SolvingAlgorithm {
public:
	void SolveMaze(Maze* maze, int startVertexX, int startVertexY, int endVertexX, int endVertexY);
	std::vector<Node> getUnvisitedNode(Node index);
	//Node getNextUnvisitedNode(Node index);
	DepthFirstSearch();
	//~DepthFirstSearch();
};

