#pragma once
#include "UiButton.h"
#include "TextureManager.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "json/json.h"

class UiMenu : public sf::Transformable, public sf::Drawable
{
public:
	UiMenu() {}
	~UiMenu() {}

	void deserialise(TextureManager& textures, const Json::Value& data)
	{
		auto texture_name = data.get("texture_name", "").asString();
		m_texture_position.x = data.get("texture_x", 0).asInt();
		m_texture_position.y = data.get("texture_y", 0).asInt();

		m_size.x = data.get("size_x", 0).asInt();;
		m_size.y = data.get("size_y", 0).asInt();
		m_texture = textures.get(texture_name);
		m_sprite.setTexture(*m_texture);
		m_sprite.setPosition(0.f, 0.f);

		auto buttons = data["buttons"];
		for (unsigned int i = 0; i < buttons.size(); i++)
		{
			UiButton button;
			button.deserialise(textures, buttons[i]);
			m_buttons.push_back(std::make_shared<UiButton>(button));
		}
	}

	void handleUpdate(sf::Event e)
	{
		if (e.type == e.MouseMoved)
		{
			auto index = getButtonByCoords(sf::Vector2f(e.mouseMove.x, e.mouseMove.y));
			if (index > -1)
			{
				m_buttons[index]->mouseEnter();
			}
			else
			{
				for (auto button : m_buttons)
				{
					button->mouseLeave();
				}
			}
		}
		else if (e.type == e.MouseButtonPressed)
		{
			auto index = getButtonByCoords(sf::Vector2f(e.mouseButton.x, e.mouseButton.y));
			if (index > -1)
			{
				m_buttons[index]->click();
			}
		}
	}

	void update(sf::Time dt)
	{
		for (auto button : m_buttons)
		{
			button->update(dt);
		}
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_sprite);
		for (auto button : m_buttons)
		{
			target.draw(*button);
		}
	}
private:
	std::shared_ptr<sf::Texture> m_texture;
	sf::Vector2i m_texture_position;
	sf::Sprite m_sprite;
	sf::Vector2i m_size;
	std::vector<std::shared_ptr<UiButton>> m_buttons;

	int getButtonByCoords(const sf::Vector2f mousePos)
	{
		if (m_buttons.size() == 0) return -1;

		for (auto i = 0; i < m_buttons.size(); ++i)
		{
			if (m_buttons[i]->getGlobalBounds().contains(mousePos))
			{
				return i;
			}
		}
		return -1;
	}
};
