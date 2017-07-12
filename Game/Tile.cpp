#include "stdafx.h"
#include "Tile.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "json/json.h"

Tile::Tile()
	: m_model(nullptr)
	, m_size(0)
	, m_position(0, 0)
	, m_is_solid(false)
{}
Tile::~Tile() {}

bool Tile::isSolid() const
{
	return m_is_solid;
}

void Tile::deserialise(TileModelMap& models, const Json::Value& data)
{
	auto tile_model_name = data.get("tile_model_name", "").asString();
	auto column = data.get("column", 0.f).asFloat();
	auto row = data.get("row", 0.f).asFloat();

	auto model_itr = models.find(tile_model_name);
	if (model_itr == models.end())
	{
		return;
	}
	m_model = model_itr->second;
	m_size = data.get("size", 0).asInt();
	m_is_solid = data.get("is_solid", false).asBool();
	m_position.x = column;
	m_position.y = row;
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	m_model->setPosition(sf::Vector2f(m_position.x * m_size, m_position.y * m_size));
	target.draw(*m_model);
}

sf::FloatRect Tile::getBoundingRect() const
{
	return sf::FloatRect(m_size * m_position.x, m_size * m_position.y, m_size, m_size);
}