#pragma once
#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include "KruskalsMaze.h"
#include "PrimsMaze.h"
#include "DepthFirstSearch.h"

#include "UserInterface.h"

using namespace MATH;


class MazeDisplay : public Scene {
private:
	SDL_Window* window;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	SDL_Renderer* renderer;
	SDL_Texture* wallTexture, *traversedTexture;

	Maze* maze;
	SolvingAlgorithm* algorithm;

	UserInterface interface;
public:
	MazeDisplay(SDL_Window* sdlWindow, UserInterface& interfaceRef);
	~MazeDisplay();
	void SetUpMazeAndAlgorithm();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};
