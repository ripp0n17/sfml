#pragma once
#include "AnimatedEntity.h"

class Player : public AnimatedEntity
{
public:
	explicit Player();
	virtual ~Player();

	void deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data) override;

	void handleInput();

	void update(sf::Time dt) override;

private:
	float m_acceleration;
	float m_run_speed;
	float m_jump_power;
};
