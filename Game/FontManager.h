#pragma once
#include <SFML/Graphics/Font.hpp>
#include "ResourceManager.h"

class FontManager : public ResourceManager<sf::Font>
{
public:
	FontManager()
		: ResourceManager()
	{}
	virtual ~FontManager()
	{}
};