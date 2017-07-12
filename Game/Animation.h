#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

namespace sf {
	class Sprite;
}

class Animation
{
public:
	Animation(int frame_count, sf::Vector2i start, sf::Vector2i size, float speed);
	~Animation();

	void update(sf::Sprite* sprite, sf::Time dt);

private:
	std::vector<sf::IntRect> m_frames;

	int m_frame_count;
	int m_current_frame;
	float m_speed;
	sf::Time m_elapsed_time;
	sf::Vector2i m_size;
};
using AnimationMap = std::unordered_map<std::string, std::shared_ptr<Animation>>;