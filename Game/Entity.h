#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ResourceManager.h"

namespace sf {
	class Time;
}

namespace Json {
	class Value;
}

class Entity : public sf::Drawable
{
public:
	explicit Entity();
	virtual ~Entity();

	sf::FloatRect getBoundingRect() const;

	virtual void deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data);

	virtual void update(sf::Time dt);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	void handleCollision(Entity* entity, sf::FloatRect intersection);

protected:
	std::shared_ptr<sf::Texture> m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_velocity;
	sf::Vector2i m_size;

	sf::RectangleShape getDebugRect() const;
};

using EntityVector = std::vector<std::shared_ptr<Entity>>;