#pragma once
#include "MenuState.h"

class StateFactory
{
public:
	static std::shared_ptr<State> create(std::shared_ptr<AssetContext> assets, const std::string& type)
	{
		if (type == "MenuState")
		{
			MenuState menu_state(assets);
			return std::make_shared<MenuState>(menu_state);
		}
		return nullptr;
	}
};
