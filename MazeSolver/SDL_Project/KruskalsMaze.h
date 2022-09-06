#pragma once
#include "Maze.h"
#include <vector>

class KruskalsMaze : public Maze {
private:
	std::vector<int> uniqueRandomXValues;
	std::vector<int> uniqueRandomYValues;

	std::vector<std::vector<Node>> nodeSets;
	int lastNodeSetIndex;

	void MarkNode(Node markedNode, int randomIndex);
	//void RemoveNodeFromSet(Node node);
	void AddNodeToSet(Node nodeToAdd, Node nodeWithSet);
	void RemoveWall(Node markedNode, Node randomDifferentSetNeightbor);
	std::vector<Node> NodeSetCheck(std::vector<Node> unCheckedNodes, Node markedNode);
	std::vector<Node> GetNodeSet(Node node);
	std::vector<Node> NeighborNodes(Node markedNode, int function);
public:
	KruskalsMaze();
	KruskalsMaze(int n);
	void Generate(bool gradualGenerationBool);
	~KruskalsMaze();
};