#include "stdafx.h"

#include "Entity.h"

#include "json/json.h"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity() {}
Entity::~Entity() {}

sf::FloatRect Entity::getBoundingRect() const
{
	return m_sprite.getGlobalBounds();
}

void Entity::deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data)
{
	auto texture_name = data.get("texture_name", "").asString();
	auto size_x = data.get("size_x", 0).asInt();
	auto size_y = data.get("size_y", 0).asInt();
	auto pos_x = data.get("pos_x", 0.f).asFloat();
	auto pos_y = data.get("pos_y", 0.f).asFloat();
	auto scale = data.get("scale", 0.f).asFloat();

	m_size = sf::Vector2i(size_x, size_y);
	m_texture = textures.get(texture_name);
	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_size.x, m_size.y));
	m_sprite.setScale(scale, scale);
	m_sprite.setPosition(pos_x, pos_y);
}

void Entity::update(sf::Time dt)
{
	m_sprite.move(m_velocity * dt.asSeconds());
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite);
	target.draw(getDebugRect());
}

void Entity::handleCollision(Entity* entity, sf::FloatRect intersection)
{
	if (entity->getBoundingRect().left == intersection.left)
	{
		m_sprite.move(0, -intersection.height * 2);
		m_velocity.y = 0;
	}
	else if (entity->getBoundingRect().top == intersection.top)
	{
		m_sprite.move(-intersection.width * 2, 0);
		m_velocity.x = 0;
	}
}

sf::RectangleShape Entity::getDebugRect() const
{
	auto bounds = getBoundingRect();
	sf::RectangleShape debug;
	debug.setPosition(bounds.left, bounds.top);
	debug.setSize(sf::Vector2f(bounds.width, bounds.height));
	debug.setFillColor(sf::Color::Transparent);
	debug.setOutlineColor(sf::Color::Red);
	debug.setOutlineThickness(2.f);
	return debug;
}