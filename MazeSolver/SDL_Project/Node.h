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
	bool traversed;
	Node();
	Node(int x_, int y_);
	~Node();
};

