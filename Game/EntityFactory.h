#pragma once
#include "Player.h"

class EntityFactory
{
public:
	std::shared_ptr<Entity> create(std::string type) const
	{
		if (type == "player")
		{
			Player player;
			return std::make_shared<Player>(player);
		}
		return nullptr;
	}
};
