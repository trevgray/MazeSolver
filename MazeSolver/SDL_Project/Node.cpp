#include "Node.h"

Node::Node() {
	x = y = 0;
	topWall = bottomWall = leftWall = rightWall = true;
	nodeState.SetState(0);
	traversed = false;
}

Node::Node(int x_, int y_) {
	x = x_;
	y = y_;
	topWall = bottomWall = leftWall = rightWall = true;
	nodeState.SetState(0);
	traversed = false;
}

Node::~Node() {
	x = y = 0;
	topWall = bottomWall = leftWall = rightWall = traversed = 0;
	nodeState.~State();
}
