#pragma once
#include "Maze.h"
#include <vector>
class PrimsMaze : public Maze {
public:
	std::vector<Node> frontier;
	PrimsMaze();
	PrimsMaze(int n);
	void Generate(int n);
	void MarkNode(Node markedNode);
	std::vector<Node> NeighborNodes(Node markedNode);
	void addFrontier(Node addedNode);
	void SetNodeWalls(int fx, int fy, int tx, int ty);
};

