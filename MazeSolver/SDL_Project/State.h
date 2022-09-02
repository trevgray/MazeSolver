#pragma once
class State {
private:
	//0 is incomplete - 1 is working and 2 is complete in all maze classes
	//node states are reset in solving algorithms to then be used in solving the maze 
	int stateNum;
public:
	State() : stateNum(-1) {}
	int GetState() {
		return stateNum;
	}
	void SetState(int newState) {
		stateNum = newState;
	}
	~State() {
		stateNum = 0;
	}
};

