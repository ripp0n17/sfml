#include "stdafx.h"
#include "Level.h"
#include "EntityFactory.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "json/json.h"

void Level::load(ResourceManager<sf::Texture>& textures, const Json::Value& data)
{
	EntityFactory factory;
	auto entities = data["entities"];
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		auto type = entities[i].get("type", "").asString();
		auto entity = factory.create(type);
		entity->deserialise(textures, entities[i]["properties"]);
		m_entities.push_back(entity);
	}
	m_map.deserialise(textures, data["map"]);
}
void Level::handleInput() const
{
	static_cast<Player*>(m_entities[0].get())->handleInput();
}
void Level::update(sf::Time dt) const
{
	for each (auto entity in m_entities)
	{
		entity->update(dt);
		m_map.checkCollisions(*entity);
	}
}
void Level::draw(sf::RenderWindow& window) const
{
	window.draw(m_map);
	for each (auto entity in m_entities)
	{
		window.draw(*entity);
	}
}
void Level::dispose()
{
	for each (auto entity in m_entities)
	{
		entity.reset();
	}
	m_entities.clear();
}