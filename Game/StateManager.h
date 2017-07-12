#pragma once
#include <memory>
#include "State.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class StateManager
{
public:
	StateManager();
	~StateManager();

	void pushState(std::shared_ptr<State> state);
	void popState();
	void changeState(std::shared_ptr<State> state);

	void handleInput(sf::Event e);
	void update(sf::Time dt);
	void draw(sf::RenderWindow* window) const;
private:
	StateVector m_states;
};
