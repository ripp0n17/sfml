#include "stdafx.h"
#include "AssetContext.h"
#include "MenuState.h"
#include "Utils.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "json/json.h"

MenuState::MenuState(std::shared_ptr<AssetContext> assets)
	: State(assets)
{
}

MenuState::~MenuState()
{
}

void MenuState::dispose()
{
}

void MenuState::handleInput(sf::Event e)
{
	m_menu.handleUpdate(e);
}

void MenuState::load()
{
	/* open file */
	auto root = Utils::openFile(m_path);

	/* load fonts */
	auto fonts = root["fonts"];
	for (unsigned int i = 0; i < fonts.size(); i++)
	{
		auto name = fonts[i].get("name", "").asString();
		auto path = fonts[i].get("path", "").asString();
		m_assets->font_manager->load(name, path);
	}

	/* load textures */
	auto textures = root["textures"];
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		auto name = textures[i].get("name", "").asString();
		auto path = textures[i].get("path", "").asString();
		m_assets->texture_manager->load(name, path);
	}

	/* load menu */
	m_menu.deserialise(*m_assets->texture_manager, root["menu"]);
}

void MenuState::update(sf::Time dt)
{
	m_menu.update(dt);
}

void MenuState::draw(sf::RenderWindow* window)
{
	window->draw(m_menu);
}