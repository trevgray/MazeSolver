#pragma once
#include "SolvingAlgorithm.h"
#include <vector>
#include <stack>
class DepthFirstSearch : public SolvingAlgorithm {
private:
	bool firstLoop;
	std::stack<Node> depthStack;
	std::vector<Node> getUnvisitedNode(Node index);
public:
	void SolveMaze(Maze* maze, int startVertexX, int startVertexY, int endVertexX, int endVertexY, bool gradualGenerationBool);
	//Node getNextUnvisitedNode(Node index);
	DepthFirstSearch();
	~DepthFirstSearch();
};

