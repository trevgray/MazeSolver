#include "DepthFirstSearch.h"
#include <stack>
#include <iostream>


void DepthFirstSearch::SolveMaze(Maze* maze_, int startVertexX, int startVertexY, int endVertexX, int endVertexY) {
	srand((unsigned int)time(NULL));
	maze = maze_;
	//reset node states to 0 in nodeArray
	for (int x = 0; x < maze->GetSize(); x++) {
		for (int y = 0; y < maze->GetSize(); y++) {
			maze->nodeArray[x][y].nodeState.SetState(0);
		}
	}
	// Set the startIndex to visited
	maze->nodeArray[startVertexX][startVertexY].nodeState.SetState(1);
	maze->nodeArray[startVertexX][startVertexY].traversed = true;
	// Declare a stack for searching
	std::stack<Node> depthStack;
	// Push start index into stack
	depthStack.push(maze->nodeArray[startVertexX][startVertexY]);
	// Loop while stack is not empty
	while (!depthStack.empty()) {
		// Use getNextUnvisitedVertex() to check if the top of the stack is an unvisited vertex
		std::vector<Node> unVisitedNodes = getUnvisitedNode(depthStack.top());
		if (unVisitedNodes.empty() == false) {
			int randomUnVisitedNode = rand() % unVisitedNodes.size();
			Node currentNode = unVisitedNodes[randomUnVisitedNode];
			if (currentNode.nodeState.GetState() == 0) {
				// Set visited node at the found index to visited
				maze->nodeArray[currentNode.x][currentNode.y].nodeState.SetState(1);
				maze->nodeArray[currentNode.x][currentNode.y].traversed = true;
				// Push vertex index onto stack
				depthStack.push(currentNode);
			}
			if (currentNode.x == endVertexX && currentNode.y == endVertexY) {
				//node got the the end
				return;
			}
		}
		// If node is finished - pop it
		else {
			maze->nodeArray[depthStack.top().x][depthStack.top().y].traversed = false;
			depthStack.pop();
		}
	}
	//no path
	return;
}

std::vector<Node> DepthFirstSearch::getUnvisitedNode(Node index) {
	std::vector<Node> neighbors; //declare a array of neighbors
	if (index.x + 1 < maze->GetSize() && maze->nodeArray[index.x + 1][index.y].nodeState.GetState() == 0 && maze->nodeArray[index.x ][index.y].rightWall == false) { //check if the neigbor is vaild then get the neighbor to the +x direction
		neighbors.push_back(maze->nodeArray[index.x + 1][index.y]);
	}
	if (index.x - 1 >= 0 && maze->nodeArray[index.x - 1][index.y].nodeState.GetState() == 0 && maze->nodeArray[index.x][index.y].leftWall == false) { //check if the neigbor is vaild then get the neighbor to the -x direction
		neighbors.push_back(maze->nodeArray[index.x - 1][index.y]);
	}
	if (index.y - 1 >= 0 && maze->nodeArray[index.x][index.y - 1].nodeState.GetState() == 0 && maze->nodeArray[index.x][index.y].topWall == false) { //check if the neigbor is vaild then get the neighbor to the -y direction
		neighbors.push_back(maze->nodeArray[index.x][index.y - 1]);
	}
	if (index.y + 1 < maze->GetSize() && maze->nodeArray[index.x][index.y + 1].nodeState.GetState() == 0 && maze->nodeArray[index.x][index.y].bottomWall == false) { //check if the neigbor is vaild then get the neighbor to the +y direction
		neighbors.push_back(maze->nodeArray[index.x][index.y + 1]);
	}
	return neighbors;
}

DepthFirstSearch::DepthFirstSearch() {
	maze = nullptr;
}

//DepthFirstSearch::~DepthFirstSearch() {
//	if (maze) { delete maze; }
//}
