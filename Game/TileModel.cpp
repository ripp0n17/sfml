#include "stdafx.h"
#include "TileModel.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "json/json.h"

TileModel::TileModel()
	: m_texture(nullptr)
{

}
TileModel::~TileModel() {}

void TileModel::setPosition(sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

void TileModel::deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data)
{
	auto texture_name = data.get("texture_name", "").asString();
	auto tile_size = data.get("tile_size", 0.f).asFloat();
	auto pos_x = data.get("pos_x", 0.f).asFloat();
	auto pos_y = data.get("pos_y", 0.f).asFloat();

	m_texture = textures.get(texture_name);
	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect(sf::IntRect(pos_x, pos_y, tile_size, tile_size));
}

void TileModel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}