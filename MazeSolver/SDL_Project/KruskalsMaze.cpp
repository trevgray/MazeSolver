#include "KruskalsMaze.h"
#include <iostream>
#include <cstdlib>

KruskalsMaze::KruskalsMaze() {
	srand((unsigned int)time(NULL));
	lastNodeSetIndex = -1;
	mazeSize = 5;
	mazeCompleted = false;
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			nodeArray[x][y] = Node(x, y);
			//Add all nodes to their own array
			std::vector<Node> newSet;
			newSet.push_back(nodeArray[x][y]);
			nodeSets.push_back(newSet);
			//Setting up Unique Randoms Arrays
			uniqueRandomXValues.push_back(x);
			uniqueRandomYValues.push_back(y);
		}
	}
}

KruskalsMaze::KruskalsMaze(int n) {
	srand((unsigned int)time(NULL));
	lastNodeSetIndex = -1;
	mazeSize = n; //generate the maze
	mazeCompleted = false;
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			nodeArray[x][y] = Node(x, y);
			//Add all nodes to their own array
			std::vector<Node> newSet;
			newSet.push_back(nodeArray[x][y]);
			nodeSets.push_back(newSet);
			//Setting up Unique Randoms Arrays
			uniqueRandomXValues.push_back(x);
			uniqueRandomYValues.push_back(y);
		}
	}
}

void KruskalsMaze::Generate(bool gradualGenerationBool) {
	//generate using unique random numbers
	//loop until their is only 1 set left
	while (nodeSets.size() > 1) {
		//std::cout << nodeSets.size() << std::endl;
		//Get a random node
		int randomIndex = rand() % uniqueRandomXValues.size();
		//Mark the node for maze evaluation
		MarkNode(nodeArray[uniqueRandomXValues[randomIndex]][uniqueRandomYValues[randomIndex]], randomIndex);
		if (gradualGenerationBool == true) {
			return;
		}
	}
	//generate using completely random
	//while (nodeSets.size() > 1) {
	//	//std::cout << nodeSets.size() << std::endl;
	//	int x = rand() % mazeSize;
	//	int y = rand() % mazeSize;
	//	MarkNode(nodeArray[x][y]);
	//}
	mazeCompleted = true;
}

KruskalsMaze::~KruskalsMaze() {
	uniqueRandomXValues.clear();
	uniqueRandomYValues.clear();
	nodeSets.clear();
	lastNodeSetIndex = 0;
}

void KruskalsMaze::MarkNode(Node markedNode, int randomIndex) {
	//set the state of the node to working
	nodeArray[markedNode.x][markedNode.y].nodeState.SetState(1);
	//get working neighbors around the random node
	std::vector<Node> workingNeightbors = NeighborNodes(markedNode, 1);
	if (workingNeightbors.size() > 0) { //if empty don't do anything
		//get the neightbors that are contained in different sets
		std::vector<Node> differentSetNeightbors = NodeSetCheck(workingNeightbors, markedNode);
		if (differentSetNeightbors.size() > 0) { //if empty then there is no more need to evaluate this node again
			//Get a random different set neightbor
			int randomDifferentSetNeightborIndex = rand() % differentSetNeightbors.size();
			Node randomDifferentSetNeightbor = differentSetNeightbors[randomDifferentSetNeightborIndex];
			//Remove the wall between the two nodes and add the nodes to the same set
			RemoveWall(markedNode, randomDifferentSetNeightbor);
		}
		else {
			//Remove the node from being evaluating again
			uniqueRandomXValues.erase(uniqueRandomXValues.begin() + randomIndex);
			uniqueRandomYValues.erase(uniqueRandomYValues.begin() + randomIndex);
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
	//Get node set of both of the nodes inside the maze - the order does not matter
	std::vector<Node> currentSetNodeSet = GetNodeSet(nodeToAdd);
	int previousNodeSet = lastNodeSetIndex;

	GetNodeSet(nodeWithSet);
	int newNodeSet = lastNodeSetIndex;
	//Move one of the node sets into the other node's set - basically combine both of the sets together
	for (int index = 0; index < currentSetNodeSet.size(); index++) {
		nodeSets[newNodeSet].push_back(nodeSets[previousNodeSet][index]);
	}
	//clear and erase the duplicate obsolete node set
	nodeSets[previousNodeSet].clear();
	nodeSets.erase(nodeSets.begin() + previousNodeSet);
}

void KruskalsMaze::RemoveWall(Node markedNode, Node randomDifferentSetNeightbor) {
	//Add the two nodes to the same set
	AddNodeToSet(randomDifferentSetNeightbor, markedNode);
	//Remove the wall between the nodes
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
	//get the set of the marked node
	std::vector<Node> markedNodeSet = GetNodeSet(markedNode);
	//Make a array for the checked nodes to be stored in
	std::vector<Node> checkedNodes;
	//for each of the unchecked nodes
	for (Node currentNode : unCheckedNodes) {
		//get the set the node is contained in
		std::vector<Node> currentNodeSet = GetNodeSet(currentNode);
		//Check if the nodes are contained in the same set - we can't do "if (GetNodeSet(currentNode) != GetNodeSet(markedNode)) {" so we check the x and y of the first node in the set
		if (currentNodeSet[0].x != markedNodeSet[0].x || currentNodeSet[0].y != markedNodeSet[0].y) {
			//if the nodes are contained in different sets - add them to the array
			checkedNodes.push_back(currentNode);
		}
	}
	return checkedNodes;
}

std::vector<Node> KruskalsMaze::GetNodeSet(Node node) { //this running as On^2 - if we store the index of the node set inside the node, this maze would generate way faster
	//reset the lost node set index
	lastNodeSetIndex = -1;
	//loop though all node arrays inside the node sets
	for (std::vector<Node> nodeArray : nodeSets) {
		//increment the last node set index
		lastNodeSetIndex++;
		//loop though all nodes inside the node array
		for (Node currentNode : nodeArray) {
			//if the x and y of the node are equal to the node inside the node array - then we found the proper nodeSet
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