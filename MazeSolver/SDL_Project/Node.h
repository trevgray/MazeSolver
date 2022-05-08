#pragma once
#include "State.h"
class Node {
public:
	int x;
	int y;
	bool topWall;
	bool bottomWall;
	bool leftWall;
	bool rightWall;
	State nodeState;
	Node();
	Node(int x_, int y_);
};

