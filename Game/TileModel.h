#pragma once
#include <unordered_map>
#include <memory>

#include "ResourceManager.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Json {
	class Value;
}

class TileModel : public sf::Drawable
{
public:
	TileModel();
	~TileModel();

	void setPosition(sf::Vector2f position);

	virtual void deserialise(ResourceManager<sf::Texture>& textures, const Json::Value& data);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
	std::shared_ptr<sf::Texture> m_texture;
	sf::Sprite m_sprite;
};
using TileModelMap = std::unordered_map<std::string, std::shared_ptr<TileModel>>;