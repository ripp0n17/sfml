#pragma once
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include "TileModel.h"

class Tile : public sf::Drawable
{
public:
	Tile();
	virtual ~Tile();

	bool isSolid() const;

	virtual void deserialise(TileModelMap& models, const Json::Value& data);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	sf::FloatRect getBoundingRect() const;

private:
	std::shared_ptr<TileModel> m_model;
	int m_size;
	sf::Vector2i m_position;
	bool m_is_solid;
};
using TileVector = std::vector<std::shared_ptr<Tile>>;