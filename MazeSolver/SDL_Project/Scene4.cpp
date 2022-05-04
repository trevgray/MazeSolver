#include "Scene4.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Randomizer.h"
#include <iostream>
#include "VMath.h"
using namespace MATH;

Scene4::Scene4(SDL_Window* sdlWindow_){
	initialVel = 15;
	Randomizer r;
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
		//planet
		gameObjects[0] = new Planet(Vec3(0.0f, 0.0f, 0.0f),
							    Vec3(0.0f, 0.0f, 0.0f), 
								Vec3(0.0f, 0.0f, 0.0f), 1.0f);
		//star1
		gameObjects[1] = new Star(Vec3(-5.0f, -10.0f, 0.0f),
								Vec3(0.0, 0.0f, 0.0f),
								Vec3(0.0f, 0.0f, 0.0f), 100.0f);
		//star2
		gameObjects[2] = new Star(Vec3(5.0f, 5.0f, 0.0f),
								Vec3(0.0, 0.0f, 0.0f),
								Vec3(0.0f, 0.0f, 0.0f), 150.0f);
	
}

Scene4::~Scene4(){
	for(int i = 0; i < NUM_OBJECTS; ++i){
		delete gameObjects[i];
	}
	SDL_DestroyRenderer(renderer);
}

bool Scene4::OnCreate() {
	int w, h;
	float xAxis = 15.0f;
	float yAxis = 20.0f;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-xAxis, xAxis, -yAxis, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	invProjectionMatrix = MMath::inverse(projectionMatrix);
	
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* starImage = IMG_Load("star.png");
	SDL_Texture* starTexture = SDL_CreateTextureFromSurface(renderer, starImage);
	SDL_Surface* planetImage = IMG_Load("planet.png");
	SDL_Texture* planetTexture = SDL_CreateTextureFromSurface(renderer, planetImage);
	if(starTexture == nullptr || planetTexture == nullptr) printf("%s\n",SDL_GetError());
	if(starImage == nullptr || planetTexture == nullptr){
		std::cerr << "Can't open the image" << std::endl;
		return false;
	} 
	else {
		
		/// Do some tricks with the image coords
		Vec3 upperLeft(0.0f,0.0f,0.0f); 
		Vec3 lowerRight(static_cast<float>(starImage->w), static_cast<float>(starImage->h),0.0f);
		Vec3 ulWorld = invProjectionMatrix * upperLeft;
		Vec3 lrWorld = invProjectionMatrix * lowerRight;
		Vec3 worldCoordsFromScreenCoords = lrWorld - ulWorld;
		///float r = worldCoordsFromScreenCoords.x / 2.0f;
		
		for(int i = 1; i < NUM_OBJECTS; i++){
			gameObjects[i]->setTexture(starTexture);
			gameObjects[i]->setImageSizeWorldCoords(worldCoordsFromScreenCoords);
		}
		gameObjects[0]->setTexture(planetTexture);
		gameObjects[0]->setImageSizeWorldCoords(worldCoordsFromScreenCoords);
		SDL_FreeSurface(starImage);
		SDL_FreeSurface(planetImage);
	}
	return true;
}

void Scene4::OnDestroy() {
	/// We really need to deal with the textures better, they are left dangling right now
	SDL_DestroyRenderer(renderer);
}

void Scene4::Update(const float deltaTime) {
	//star nf loop
	float forceGravityX[NUM_OBJECTS];
	float forceGravityY[NUM_OBJECTS];
	for (int i = 1; i < NUM_OBJECTS; i = i + 1) {
		float Gconstant = 1.0f;
		Vec3 dir = gameObjects[i]->getPos() - gameObjects[0]->getPos(); //float dis = VMath
		float mag = sqrt(pow(dir.x,2.0f) + pow(dir.y,2.0f));
		float Fg = ((Gconstant * gameObjects[0]->getMass() * gameObjects[i]->getMass())/pow(mag,2.0f));
		dir = dir / mag;
		forceGravityX[i] = Fg * dir.x;
		forceGravityY[i] = Fg * dir.y;
	}
	float forceX = forceGravityX[1] + forceGravityX[2];
	float forceY = forceGravityY[1] + forceGravityY[2];
	Vec3 forceGravity(forceX - initialVel, forceY + initialVel, 0);
	gameObjects[0]->ApplyForce(forceGravity);
	printf("%f %f %i \n", forceX, forceY, initialVel);
	if (initialVel != 0) {
		initialVel = initialVel - 1;
	}
	for(int i = 0; i < NUM_OBJECTS; i = i + 1){
		gameObjects[i]->Update(deltaTime);
	}
}


void Scene4::HandleEvents(const SDL_Event& sdlEvent){
	/*if (sdlEvent.type == SDL_MOUSEBUTTONDOWN && sdlEvent.button.button == SDL_BUTTON_LEFT) {
		Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y),0.0f);
		Vec3 worldCoords = invProjectionMatrix * mouseCoords;
		worldCoords.z = 0.0f;
		for (int i = 0; i < NUM_OBJECTS; ++i) {
			Vec3 pos = gameObjects[i]->getPos();
			float distance = VMath::distance(pos , worldCoords);
			float radius = gameObjects[i]->getRadius();
			if(distance < radius){
				printf("Gotcha ball %d\n",i);
			}
		}
	}*/
}


void Scene4::Render() {
	SDL_SetRenderDrawColor(renderer, 0,0,0,0);
	SDL_RenderClear(renderer);
	SDL_Rect square;
	Vec3 screenCoords;
	int w,h;
	
	for(int i = 0; i < NUM_OBJECTS; ++i){
		SDL_QueryTexture(gameObjects[i]->getTexture(), nullptr, nullptr, &w, &h);
		screenCoords = projectionMatrix * gameObjects[i]->getPos();
		square.x = static_cast<int>(screenCoords.x) - w/2;
		square.y = static_cast<int>(screenCoords.y) - h/2;
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, gameObjects[i]->getTexture(),nullptr, &square, gameObjects[i]->getRotation(), nullptr, SDL_FLIP_NONE);
		
	}
	SDL_RenderPresent(renderer);
}