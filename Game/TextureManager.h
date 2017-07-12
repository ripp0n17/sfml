#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "ResourceManager.h"

class TextureManager : public ResourceManager<sf::Texture>
{
public:
	TextureManager()
		: ResourceManager()
	{}

	virtual ~TextureManager()
	{}
};