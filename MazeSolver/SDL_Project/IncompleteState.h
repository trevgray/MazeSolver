#pragma once
#include "State.h"
class IncompleteState : public State {
	int GetState() {
		return 0;
	}
};