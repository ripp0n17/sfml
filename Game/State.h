#pragma once
#include <memory>
#include <vector>

struct AssetContext;

namespace sf {
	class Event;
	class RenderWindow;
	class Time;
}

class State
{
public:
	State(std::shared_ptr<AssetContext> assets)
		: m_assets(assets)
	{}
	virtual ~State() {}

	virtual void init(const std::string& name, const std::string& path)
	{
		m_name = name;
		m_path = path;
	}

	virtual void load() = 0;
	virtual void dispose() = 0;
	virtual void handleInput(sf::Event e) = 0;
	virtual void update(sf::Time dt) = 0;
	virtual void draw(sf::RenderWindow* window) = 0;

protected:
	std::string m_name;
	std::string m_path;
	std::shared_ptr<AssetContext> m_assets;
};
using StateVector = std::vector<std::shared_ptr<State>>;