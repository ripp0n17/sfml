#pragma once
#include "Entity.h"
#include "Animation.h"

class AnimatedEntity : public Entity
{
public:
	explicit AnimatedEntity();
	virtual ~AnimatedEntity();

	void deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data) override;

	bool hasAnimation(const std::string& name);

	void addAnimation(const std::string& name, int frame_count, sf::Vector2i start, sf::Vector2i size, float speed);

	void setAnimation(const std::string& name);

protected:
	AnimationMap m_animations;
	std::shared_ptr<Animation> m_current_animation;
};
