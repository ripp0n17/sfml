#pragma once
#include "UiMenu.h"
#include "State.h"

namespace sf {
	class RenderWindow;
}

class MenuState : public State
{
public:
	MenuState(std::shared_ptr<AssetContext> assets);
	virtual ~MenuState();

	void load() override;
	void dispose() override;
	void handleInput(sf::Event e) override;
	void update(sf::Time dt) override;
	void draw(sf::RenderWindow* window) override;

private:
	UiMenu m_menu;
};
