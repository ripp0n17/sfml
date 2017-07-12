#include "stdafx.h"
#include "Map.h"
#include "Entity.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "json/json.h"

	void Map::deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data)
	{
		auto tile_models = data["tile_models"];
		for (unsigned int i = 0; i < tile_models.size(); i++)
		{
			TileModel model;
			auto name = tile_models[i].get("name", "").asString();
			model.deserialise(textures, tile_models[i]["model"]);
			m_models[name] = std::make_shared<TileModel>(model);
		}

		auto tiles = data["tiles"];
		for (unsigned int i = 0; i < tiles.size(); i++)
		{
			Tile tile;
			tile.deserialise(m_models, tiles[i]);
			m_tiles.push_back(std::make_shared<Tile>(tile));
		}
	}

	void Map::checkCollisions(Entity& entity) const
	{
		for each (auto tile in m_tiles)
		{
			if (tile->isSolid())
			{
				sf::FloatRect intersection;
				if (entity.getBoundingRect().intersects(tile->getBoundingRect(), intersection))
				{
					entity.handleCollision(&entity, intersection);
				}
			}
		}
	}

	void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for each (auto tile in m_tiles)
		{
			target.draw(*tile);
		}
	}