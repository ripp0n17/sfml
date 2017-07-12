#include "stdafx.h"
#include "AnimatedEntity.h"

#include "json/json.h"

AnimatedEntity::AnimatedEntity()
	: Entity()
	, m_current_animation(nullptr) {}
AnimatedEntity::~AnimatedEntity() {}

void AnimatedEntity::deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data)
{
	Entity::deserialise(textures, data);
	auto default_animation = data.get("default_animation", "").asString();

	auto animations = data["animations"];
	for (unsigned int i = 0; i < animations.size(); i++)
	{
		auto name = animations[i].get("name", "").asString();
		auto frame_count = animations[i].get("frame_count", 0).asInt();
		auto start_x = animations[i].get("start_x", 0).asInt();
		auto start_y = animations[i].get("start_y", 0).asInt();
		auto speed = animations[i].get("speed", 0.f).asFloat();
		addAnimation(name, frame_count, sf::Vector2i(start_x, start_y), m_size, speed);
	}
	setAnimation(default_animation);
}

bool AnimatedEntity::hasAnimation(const std::string& name)
{
	return (m_animations.find(name) != m_animations.end());
}

void AnimatedEntity::addAnimation(const std::string& name, int frame_count, sf::Vector2i start, sf::Vector2i size, float speed)
{
	if (!hasAnimation(name))
	{
		m_animations[name] = std::make_shared<Animation>(frame_count, start, size, speed);
	}
}

void AnimatedEntity::setAnimation(const std::string& name)
{
	m_current_animation = m_animations[name];
}
