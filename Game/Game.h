#pragma once
#include "AssetContext.h"
#include "Utils.h"
#include "StateFactory.h"
#include "StateManager.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "json/json.h"
#include "TextureManager.h"
#include "FontManager.h"

class Game
{
public:
	Game() {}
	~Game() {}

	void start()
	{
		init(800, 600, "SFML Game");

		sf::Clock clock;
		auto frameTime = clock.restart();
		while (m_window.isOpen())
		{
			sf::Event e;
			if (m_window.pollEvent(e))
			{
				switch (e.type)
				{
				case e.Closed:
					m_window.close();
					break;
				case e.KeyPressed:
					if (e.key.code == sf::Keyboard::Escape)
					{
						m_window.close();
					}
					break;
				default:
					m_state_manager.handleInput(e);
					break;
				}
			}

			/*level.handleInput();
			level.update(frameTime);*/
			m_state_manager.update(frameTime);

			m_window.clear(sf::Color::Black);
			//level.draw(window);
			m_state_manager.draw(&m_window);
			m_window.display();

			frameTime = clock.restart();
		}
	}
private:

	void init(int screen_width, int screen_height, std::string window_title)
	{
		m_context = std::make_shared<AssetContext>();
		m_context->texture_manager = std::make_unique<TextureManager>(m_texture_manager);
		m_context->font_manager = std::make_unique<FontManager>(m_font_manager);
		m_context->state_manager = std::make_unique<StateManager>(m_state_manager);

		m_window.create(sf::VideoMode(screen_width, screen_height), window_title, sf::Style::Default);

		auto states_node = Utils::openFile("Assets/states.json");

		/*auto textures = root["textures"];
		for (auto i = 0; i < textures.size(); i++)
		{
		auto name = textures[i].get("name", "").asString();
		auto path = textures[i].get("path", "").asString();
		_texture_manager.load(name, path);
		}
		auto fonts = root["fonts"];
		for (auto i = 0; i < fonts.size(); i++)
		{
		auto name = fonts[i].get("name", "").asString();
		auto path = fonts[i].get("path", "").asString();
		m_font_manager.load(name, path);
		}
		Level level;
		level.load(texture_manager, root);*/

		auto states = states_node["states"];
		for (unsigned int i = 0; i < states.size(); i++)
		{
			auto name = states[i].get("name", "").asString();
			auto path = states[i].get("path", "").asString();
			auto type = states[i].get("type", "").asString();
			auto state = StateFactory::create(m_context, type);
			state->init(name, path);
			m_state_manager.pushState(state);
		}
	}

	sf::RenderWindow m_window;
	StateManager m_state_manager;
	TextureManager m_texture_manager;
	FontManager m_font_manager;
	std::shared_ptr<AssetContext> m_context;
};
