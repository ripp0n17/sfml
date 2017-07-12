#include "stdafx.h"

#include <SFML/Graphics/Sprite.hpp>

#include "Animation.h"

Animation::Animation(int frame_count, sf::Vector2i start, sf::Vector2i size, float speed)
	: m_frame_count(frame_count)
	, m_current_frame(-1)
	, m_speed(60.f)
	, m_size(size)
{
	for (auto i = 0; i < m_frame_count; i++)
	{
		m_frames.push_back(sf::IntRect(start.x + (m_size.x * i), start.y, m_size.x, m_size.y));
	}
}
Animation::~Animation() {}

void Animation::update(sf::Sprite* sprite, sf::Time dt)
{
	m_elapsed_time += dt;
	if (m_elapsed_time.asSeconds() >= (m_speed / 1000))
	{
		m_elapsed_time = dt;
		m_current_frame++;
		if (m_current_frame >= m_frame_count)
		{
			m_current_frame = 0;
		}
		sprite->setTextureRect(m_frames[m_current_frame]);
	}
}