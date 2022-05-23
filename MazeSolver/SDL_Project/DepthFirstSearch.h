#pragma once
#include "SolvingAlgorithm.h"
#include <vector>
#include <stack>
class DepthFirstSearch : public SolvingAlgorithm {
public:
	void SolveMaze();
	void SetupAlgorithm(Maze* maze_, int startVertexX, int startVertexY, int endVertexX, int endVertexY, bool mazeDone);
	std::vector<Node> getUnvisitedNode(Node index);
	std::stack<Node> depthStack;
	int endX, endY;
	bool endAlgorithm;
	//Node getNextUnvisitedNode(Node index);
	DepthFirstSearch();
	~DepthFirstSearch();
};

