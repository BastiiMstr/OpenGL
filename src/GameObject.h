#pragma once
#include "../textures/Textures.h"
#include <glm/glm.hpp>

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture texture);
	
	glm::vec2 Position, Size;
	
	float rotation;
	Texture texture;

	void Draw();

};