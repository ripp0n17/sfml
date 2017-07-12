#pragma once
#include "Tile.h"

class Entity;

class Map : public sf::Drawable
{
public:

	virtual void deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data);

	void checkCollisions(Entity& entity) const;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
	TileModelMap m_models;
	TileVector m_tiles;
};
