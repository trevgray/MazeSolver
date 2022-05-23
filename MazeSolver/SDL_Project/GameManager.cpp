#include "GameManager.h"
#include "Window.h"
#include "Timer.h"
#include "MazeDisplay.h"
#include <iostream>

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
}


/// In this OnCreate() method, fuction, subroutine, whatever the word, 
bool GameManager::OnCreate() {
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 1000;
	windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

	currentScene = new MazeDisplay(windowPtr->GetSDL_Window());
	if (currentScene == nullptr) {
		OnDestroy();
		return false;
	}

	if (currentScene->OnCreate() == false) {
		OnDestroy();
		return false;
	}
	return true;
}

/// Here's the whole game
void GameManager::Run() {
	SDL_Event sdlEvent;
	timer->Start();
	while (isRunning) {
		
		while (SDL_PollEvent(&sdlEvent)){
			if (sdlEvent.type == SDL_QUIT) {
				isRunning = false;
			}else if(sdlEvent.type == SDL_KEYDOWN){
				switch (sdlEvent.key.keysym.scancode){
					case SDL_SCANCODE_ESCAPE:
						isRunning = false;
						break;
					case SDL_SCANCODE_Q:
						isRunning = false;
						break;

					case SDL_SCANCODE_F10:
						currentScene->OnDestroy();
						delete currentScene;
						currentScene = new MazeDisplay(windowPtr->GetSDL_Window());
						currentScene->OnCreate();
						break;
					default:
						break;
				}
			}
			currentScene->HandleEvents(sdlEvent);
		}

		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();

		/// Keeep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(120)); ///60 frames per sec
	}
}

GameManager::~GameManager() {}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}