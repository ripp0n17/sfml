#include "stdafx.h"
#include "UiButton.h"
#include "TextureManager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "json/json.h"

UiButton::UiButton()
{}

UiButton::~UiButton()
{}

sf::FloatRect UiButton::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

void UiButton::deserialise(TextureManager& textures, const Json::Value & data)
{
	auto texture_name = data.get("texture_name", "").asString();
	auto size_x = data.get("size_x", 0).asInt();
	auto size_y = data.get("size_y", 0).asInt();
	auto texture_x = data.get("texture_x", 0).asInt();
	auto texture_y = data.get("texture_y", 0).asInt();

	sf::Vector2f position;
	auto pos_x_type = data.get("pos_x_type", "").asString();
	if (pos_x_type == "auto")
	{
		m_sprite.setOrigin(m_size.x / 2, m_sprite.getOrigin().y);
		position.x = 400; //TODO: get screen size
	}
	else
	{
		position.x = data.get("pos_x", 0).asInt();
	}

	auto pos_y_type = data.get("pos_y_type", "").asString();
	if (pos_y_type == "auto")
	{
		m_sprite.setOrigin(m_sprite.getOrigin().x, m_size.y / 2);
		position.y = 300; //TODO: get screen size
	}
	else
	{
		position.y = data.get("pos_y", 0).asInt();
	}

	m_action = data.get("action", "").asString();
	m_size = sf::Vector2i(size_x, size_y);
	m_texture_position = sf::Vector2i(texture_x, texture_y);
	m_texture = textures.get(texture_name);
	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect(sf::IntRect(m_texture_position.x, m_texture_position.y, m_size.x, m_size.y));
	m_sprite.setPosition(position);
}

void UiButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}

void UiButton::update(sf::Time)
{
	
}

void UiButton::mouseEnter()
{
	auto texture_rect = sf::IntRect(m_texture_position.x, m_texture_position.y, m_size.x, m_size.y);
	texture_rect.top = m_texture_position.y + m_size.y;

	m_sprite.setTextureRect(texture_rect);
}

void UiButton::mouseLeave()
{
	auto texture_rect = sf::IntRect(m_texture_position.x, m_texture_position.y, m_size.x, m_size.y);

	m_sprite.setTextureRect(texture_rect);
}

void UiButton::click()
{
	auto texture_rect = sf::IntRect(m_texture_position.x, m_texture_position.y, m_size.x, m_size.y);
	texture_rect.top = m_texture_position.y + (m_size.y * 2);

	m_sprite.setTextureRect(texture_rect);

	std::cout << m_action << std::endl;
}
