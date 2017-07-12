#include "stdafx.h"
#include "Player.h"

#include <SFML/Window/Keyboard.hpp>
#include "json/json.h"

Player::Player()
	: AnimatedEntity()
	, m_acceleration(0.f)
	, m_run_speed(0.f)
	, m_jump_power(0.f)
{
}
Player::~Player() {}

void Player::deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data)
{
	AnimatedEntity::deserialise(textures, data);

	m_acceleration = data.get("acceleration", 0.f).asFloat();
	m_run_speed = data.get("run_speed", 0.f).asFloat();
	m_jump_power = data.get("jump_power", 0.f).asFloat();
}

void Player::handleInput()
{
	setAnimation("idle");
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		setAnimation("run");
		m_velocity.x += m_acceleration;
		if (m_velocity.x > m_run_speed)
		{
			m_velocity.x = m_run_speed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		setAnimation("run");
		m_velocity.x -= m_acceleration;
		if (m_velocity.x < -m_run_speed)
		{
			m_velocity.x = -m_run_speed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_sprite.getPosition().y == (600 - 139))
	{
		m_velocity.y -= m_jump_power;
	}
}

void Player::update(sf::Time dt)
{
	m_current_animation->update(&m_sprite, dt);

	m_velocity.y += 0.75f;
	if (m_velocity.x > 0) m_velocity.x -= 0.5f;
	if (m_velocity.x < 0) m_velocity.x += 0.5f;
	Entity::update(dt);
	/*
	auto currentPos = m_sprite.getPosition();
	if (currentPos.y > (600 - 200))
	{
	m_sprite.setPosition(currentPos.x, (600 - 200));
	m_velocity.y = 0;
	}*/
}