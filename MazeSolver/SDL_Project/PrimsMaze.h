#pragma once
#include "Maze.h"
#include <vector>
class PrimsMaze : public Maze {
private:
	std::vector<Node> frontier;
	void MarkNode(Node markedNode);
	std::vector<Node> NeighborNodes(Node markedNode, int function);
	void addFrontier(Node addedNode);
	void SetNodeWalls(Node markedNode);

	bool firstLoop;
public:
	PrimsMaze();
	PrimsMaze(int n);
	void Generate(bool gradualGenerationBool);
};