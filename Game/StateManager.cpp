#include "stdafx.h"
#include "StateManager.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

StateManager::StateManager() {}
StateManager::~StateManager() {}

void StateManager::pushState(std::shared_ptr<State> state)
{
	m_states.push_back(state);
	m_states.back()->load();
}

void StateManager::popState()
{
	if (m_states.size() > 0)
	{
		m_states.back()->dispose();
		m_states.pop_back();
	}
}

void StateManager::changeState(std::shared_ptr<State> state)
{
	popState();
	pushState(state);
}

void StateManager::handleInput(sf::Event e)
{
	if (m_states.size() > 0)
	{
		m_states.back()->handleInput(e);
	}
}

void StateManager::update(sf::Time dt)
{
	if (m_states.size() > 0)
	{
		m_states.back()->update(dt);
	}
}
void StateManager::draw(sf::RenderWindow* window) const
{
	if (m_states.size() > 0)
	{
		m_states.back()->draw(window);
	}
}