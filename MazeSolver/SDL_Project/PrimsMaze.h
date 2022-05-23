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
	int startX;
	int startY;
	bool frontierLoop;
public:
	PrimsMaze();
	PrimsMaze(int n);
	void Generate();
	void SetupMaze(int x, int y) {
		startX = x;
		startY = y;
		frontierLoop = true;
		MarkNode(nodeArray[startX][startY]);
		mazeDone = false;
	}
};

