#pragma once
#include "ID.hh"
#include "Renderer.hh"
#include "Transform.hh"

// Sprite class that serves as part of the Model
struct Sprite {
	Transform transform;
	ObjectID objectID;
	double angle;

	void Draw() { R.Push(objectID, transform); };
	void DrawPlayer() { R.Push(objectID, transform, angle, NULL, SDL_FLIP_NONE);};
};