#pragma once
#include "ResourceManager.h"
#include "Map.h"
#include "Entity.h"

#include <SFML/Graphics/Texture.hpp>

namespace sf {
	class RenderWindow;
	class Time;
}

namespace Json {
	class Value;
}

class Level
{
public:
	void load(ResourceManager<sf::Texture>& textures, const Json::Value& data);
	void handleInput() const;
	void update(sf::Time dt) const;
	void draw(sf::RenderWindow& window) const;
	void dispose();

private:
	Map m_map;
	EntityVector m_entities;
};
