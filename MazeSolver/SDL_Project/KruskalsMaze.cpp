#include "KruskalsMaze.h"
#include <iostream>
#include <cstdlib>

KruskalsMaze::KruskalsMaze() {
	lastNodeSetIndex = -1;
	mazeSize = 5;
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			nodeArray[x][y] = Node(x, y);

			std::vector<Node> newSet;
			newSet.push_back(nodeArray[x][y]);
			nodeSets.push_back(newSet);
		}
	}
}

KruskalsMaze::KruskalsMaze(int n) {
	lastNodeSetIndex = -1;
	mazeSize = n; //generate the maze

	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			nodeArray[x][y] = Node(x, y);

			std::vector<Node> newSet;
			newSet.push_back(nodeArray[x][y]);
			nodeSets.push_back(newSet);

			//Unique Randoms
			uniqueRandomNums.push_back(x + (10 * y));
			//std::cout << x + (10 * y) << std::endl;
		}
	}
}

void KruskalsMaze::Generate() {
	srand((unsigned int)time(NULL));

	//mark random node
	/*while (nodeSets.size() > 1) {
		int randomIndex = uniqueRandomNums[rand() % uniqueRandomNums.size()];


	}*/
	//completely random
	while (nodeSets.size() > 1) {
		//std::cout << nodeSets.size() << std::endl;
		int x = rand() % mazeSize;
		int y = rand() % mazeSize;
		MarkNode(nodeArray[x][y]);
	}
}

void KruskalsMaze::MarkNode(Node markedNode) {
	//nodeArray[markedNode.x][markedNode.y].traversed = true;
	nodeArray[markedNode.x][markedNode.y].nodeState.SetState(1);

	std::vector<Node> workingNeightbors = NeighborNodes(markedNode, 1); //get working neighbors around the random node
	if (workingNeightbors.size() > 0) { //if empty don't do anything
		std::vector<Node> differentSetNeightbors = NodeSetCheck(workingNeightbors, markedNode);
		if (differentSetNeightbors.size() > 0) { //if empty don't do anything
			int randomDifferentSetNeightborIndex = rand() % differentSetNeightbors.size();
			Node randomDifferentSetNeightbor = differentSetNeightbors[randomDifferentSetNeightborIndex];
			RemoveWall(markedNode, randomDifferentSetNeightbor);
		}
	}
}

//void KruskalsMaze::RemoveNodeFromSet(Node node) {
//	std::vector<Node> currentSetNodeSet = GetNodeSet(node);
//	for (int index = 0; index < currentSetNodeSet.size(); index++) {
//		if (currentSetNodeSet[index].x == node.x && currentSetNodeSet[index].y == node.y) {
//			for (int x = 0; x < nodeSets[lastNodeSetIndex].size(); x++) {
//				currentSetNodeSet.push_back(nodeSets[lastNodeSetIndex][x]);
//			}
//			nodeSets.erase(nodeSets.begin() + lastNodeSetIndex);
//			break;
//			nodeSets[lastNodeSetIndex].erase(nodeSets[lastNodeSetIndex].begin() + index); //fix this!!!!! - convert all in a set to the other set
//			if (nodeSets[lastNodeSetIndex].size() == 0) {
//				nodeSets.erase(nodeSets.begin() + lastNodeSetIndex);
//			}
//		}
//	}
//}

void KruskalsMaze::AddNodeToSet(Node nodeToAdd, Node nodeWithSet) {
	std::vector<Node> currentSetNodeSet = GetNodeSet(nodeToAdd);
	int previousNodeSet = lastNodeSetIndex;

	GetNodeSet(nodeWithSet);
	int newNodeSet = lastNodeSetIndex;

	for (int index = 0; index < currentSetNodeSet.size(); index++) {
		nodeSets[newNodeSet].push_back(nodeSets[previousNodeSet][index]);
	}

	nodeSets[newNodeSet].push_back(nodeArray[nodeToAdd.x][nodeToAdd.y]);

	nodeSets[previousNodeSet].clear();
	nodeSets.erase(nodeSets.begin() + previousNodeSet);
}

void KruskalsMaze::RemoveWall(Node markedNode, Node randomDifferentSetNeightbor) {
	//start setting the set of the removed wall nodes to each other
	//RemoveNodeFromSet(randomDifferentSetNeightbor);
	AddNodeToSet(randomDifferentSetNeightbor, markedNode);

	GetNodeSet(markedNode);
	if (randomDifferentSetNeightbor.x < markedNode.x) { //if the neighbor is to the left of the node - remove the walls between the nodes
		nodeArray[markedNode.x][markedNode.y].leftWall = false;
		nodeArray[randomDifferentSetNeightbor.x][randomDifferentSetNeightbor.y].rightWall = false;
	}
	else if (randomDifferentSetNeightbor.x > markedNode.x) { //if the neighbor is to the right of the node - remove the walls between the nodes
		nodeArray[markedNode.x][markedNode.y].rightWall = false;
		nodeArray[randomDifferentSetNeightbor.x][randomDifferentSetNeightbor.y].leftWall = false;
	}
	else if (randomDifferentSetNeightbor.y < markedNode.y) { //if the neighbor is to the top of the node - remove the walls between the nodes
		nodeArray[markedNode.x][markedNode.y].topWall = false;
		nodeArray[randomDifferentSetNeightbor.x][randomDifferentSetNeightbor.y].bottomWall = false;
	}
	else if (randomDifferentSetNeightbor.y > markedNode.y) { //if the neighbor is to the bottom of the node - remove the walls between the nodes
		nodeArray[markedNode.x][markedNode.y].bottomWall = false;
		nodeArray[randomDifferentSetNeightbor.x][randomDifferentSetNeightbor.y].topWall = false;
	}
}

std::vector<Node> KruskalsMaze::NodeSetCheck(std::vector<Node> unCheckedNodes, Node markedNode) {
	std::vector<Node> markedNodeSet = GetNodeSet(markedNode);

	std::vector<Node> checkedNodes;
	for (Node currentNode : unCheckedNodes) {
		std::vector<Node> currentNodeSet = GetNodeSet(currentNode);
		//if (GetNodeSet(currentNode) != GetNodeSet(markedNode)) {
		if (currentNodeSet[0].x != markedNodeSet[0].x || currentNodeSet[0].y != markedNodeSet[0].y) {
			//std::cout << "currentNodeSet = " << currentNodeSet[0].x << " " << currentNodeSet[0].y << " " << "markedNodeSet = " << markedNodeSet[0].x << " " << markedNodeSet[0].y << std::endl;
			checkedNodes.push_back(currentNode);
		}
	}
	return checkedNodes;
}

std::vector<Node> KruskalsMaze::GetNodeSet(Node node) { //easy optimzation
	lastNodeSetIndex = -1;
	for (std::vector<Node> nodeArray : nodeSets) {
		lastNodeSetIndex++;
		for (Node currentNode : nodeArray) {
			if (currentNode.x == node.x && currentNode.y == node.y) {
				return nodeArray;
			}
		}
	}
	return std::vector<Node>();
}

std::vector<Node> KruskalsMaze::NeighborNodes(Node markedNode, int function) {
	std::vector<Node> neighbors; //declare a array of neighbors
	if (markedNode.x - 1 >= 0 && nodeArray[markedNode.x - 1][markedNode.y].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the -x direction
		neighbors.push_back(nodeArray[markedNode.x - 1][markedNode.y]);
	}
	if (markedNode.x + 1 < mazeSize && nodeArray[markedNode.x + 1][markedNode.y].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the +x direction
		neighbors.push_back(nodeArray[markedNode.x + 1][markedNode.y]);
	}
	if (markedNode.y - 1 >= 0 && nodeArray[markedNode.x][markedNode.y - 1].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the -y direction
		neighbors.push_back(nodeArray[markedNode.x][markedNode.y - 1]);
	}
	if (markedNode.y + 1 < mazeSize && nodeArray[markedNode.x][markedNode.y + 1].nodeState.GetState() == function) { //check if the neigbor is vaild then get the neighbor to the +y direction
		neighbors.push_back(nodeArray[markedNode.x][markedNode.y + 1]);
	}
	return neighbors;
}