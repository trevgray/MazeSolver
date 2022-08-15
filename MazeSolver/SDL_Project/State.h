#pragma once
class State {
private:
	//0 is incomplete - 1 is working and 2 is complete
	//states 3+ can be and have been used in solving algorithms
	int stateNum;
public:
	State() : stateNum(-1) {}
	int GetState() {
		return stateNum;
	}
	void SetState(int newState) {
		stateNum = newState;
	}
};

