#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>

#include <SFML/Window/Event.hpp>

namespace sf {
	class Event;
}

class TextureManager;

namespace Json {
	class Value;
}

class UiButton : public sf::Drawable
{
public:
	explicit UiButton();
	virtual ~UiButton();

	sf::FloatRect getGlobalBounds() const;

	void deserialise(TextureManager& textures, const Json::Value& data);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void update(sf::Time);

	void mouseEnter();
	void mouseLeave();
	void click();

protected:
	std::shared_ptr<sf::Texture> m_texture;
	sf::Vector2i m_texture_position;
	sf::Sprite m_sprite;
	sf::Vector2i m_size;
	std::string m_action;
};
