// Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "json/json.h"
#include <cassert>
#include <fstream>

using TextureMap = std::unordered_map<std::string, std::shared_ptr<sf::Texture>>;
class TextureManager
{
public:
	TextureManager() {}
	~TextureManager() {}

	bool hasTexture(std::string name)
	{
		return (m_textures.find(name) != m_textures.end());
	}

	bool loadTexture(std::string name, std::string path)
	{
		if (!hasTexture(name))
		{
			auto texture = new sf::Texture();
			if (!texture->loadFromFile(path))
			{
				std::cout << "ERROR: loading texture " << path << std::endl;
				return false;
			}
			m_textures[name] = std::shared_ptr<sf::Texture>(texture);
		}
		return true;
	}

	std::shared_ptr<sf::Texture> getTexture(std::string name)
	{
		return m_textures[name];
	}
private:
	TextureMap m_textures;
};

class Animation
{
public:
	Animation(int frame_count, sf::Vector2f start, sf::Vector2f size, float speed)
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
	~Animation() {}

	void update(sf::Sprite* sprite, sf::Time dt)
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

private:
	std::vector<sf::IntRect> m_frames;

	int m_frame_count;
	int m_current_frame;
	float m_speed;
	sf::Time m_elapsed_time;
	sf::Vector2f m_size;
};
using AnimationMap = std::unordered_map<std::string, std::shared_ptr<Animation>>;

class Entity : public sf::Drawable
{
public:
	explicit Entity() {}
	virtual ~Entity() {}

	virtual void deserialise(TextureManager& textures, const Json::Value& data)
	{
		auto texture_name = data.get("texture_name", "").asString();
		auto size_x = data.get("size_x", 0.f).asFloat();
		auto size_y = data.get("size_y", 0.f).asFloat();
		auto pos_x = data.get("pos_x", 0.f).asFloat();
		auto pos_y = data.get("pos_y", 0.f).asFloat();
		auto scale = data.get("scale", 0.f).asFloat();

		m_size = sf::Vector2f(size_x, size_y);
		m_texture = textures.getTexture(texture_name);
		m_sprite.setTexture(*m_texture);
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_size.x, m_size.y));
		m_sprite.setScale(scale, scale);
		m_sprite.setPosition(pos_x, pos_y);
	}
	
	virtual void update(sf::Time dt)
	{
		m_sprite.move(m_velocity * dt.asSeconds());
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		target.draw(m_sprite);
	}
protected:
	std::shared_ptr<sf::Texture> m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_velocity;
	sf::Vector2f m_size;
};

class AnimatedEntity : public Entity
{
public:
	explicit AnimatedEntity()
		: Entity()
		, m_current_animation(nullptr) {}
	virtual ~AnimatedEntity() {}

	void deserialise(TextureManager& textures, const Json::Value& data) override
	{
		Entity::deserialise(textures, data);
		auto default_animation = data.get("default_animation", "").asString();

		auto animations = data["animations"];
		for (auto i = 0; i < animations.size(); i++)
		{
			auto name = animations[i].get("name", "").asString();
			auto frame_count = animations[i].get("frame_count", 0).asInt();
			auto start_x = animations[i].get("start_x", 0.f).asFloat();
			auto start_y = animations[i].get("start_y", 0.f).asFloat();
			auto speed = animations[i].get("speed", 0.f).asFloat();
			addAnimation(name, frame_count, sf::Vector2f(start_x, start_y), m_size, speed);
		}
		setAnimation(default_animation);
	}

	bool hasAnimation(std::string name)
	{
		return (m_animations.find(name) != m_animations.end());
	}

	void addAnimation(const std::string name, int frame_count, sf::Vector2f start, sf::Vector2f size, float speed)
	{
		if (!hasAnimation(name))
		{
			m_animations[name] = std::make_shared<Animation>(frame_count, start, size, speed);
		}
	}

	void setAnimation(const std::string name)
	{
		m_current_animation = m_animations[name];
	}

protected:
	AnimationMap m_animations;
	std::shared_ptr<Animation> m_current_animation;
};

class Player : public AnimatedEntity
{
public:
	explicit Player()
		: AnimatedEntity()
		, m_acceleration(0.f)
		, m_run_speed(0.f)
		, m_jump_power(0.f)
	{
	}
	~Player() {}

	void deserialise(TextureManager& textures, const Json::Value& data) override
	{
		AnimatedEntity::deserialise(textures, data);

		m_acceleration = data.get("acceleration", 0.f).asFloat();
		m_run_speed = data.get("run_speed", 0.f).asFloat();
		m_jump_power = data.get("jump_power", 0.f).asFloat();
	}

	void handleInput()
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

	void update(sf::Time dt) override
	{
		m_current_animation->update(&m_sprite, dt);

		m_velocity.y += 0.75f;
		if (m_velocity.x > 0) m_velocity.x -= 0.5f;
		if (m_velocity.x < 0) m_velocity.x += 0.5f;
		Entity::update(dt);

		auto currentPos = m_sprite.getPosition();
		if (currentPos.y > (600 - 139))
		{
			m_sprite.setPosition(currentPos.x, (600 - 139));
			m_velocity.y = 0;
		}
	}
	
private:
	float m_acceleration;
	float m_run_speed;
	float m_jump_power;
};

using EntityVector = std::vector<std::shared_ptr<Entity>>;
class EntityManager
{
public:
private:
	EntityVector m_entities;
};

class TileModel : public sf::Drawable
{
public:
	TileModel()
		: m_texture(nullptr)
	{
		
	}
	~TileModel() {}

	void setPosition(sf::Vector2f position)
	{
		m_sprite.setPosition(position);
	}

	virtual void deserialise(TextureManager& textures, const Json::Value& data)
	{
		auto texture_name = data.get("texture_name", "").asString();
		auto tile_size = data.get("tile_size", 0.f).asFloat();
		auto pos_x = data.get("pos_x", 0.f).asFloat();
		auto pos_y = data.get("pos_y", 0.f).asFloat();

		m_texture = textures.getTexture(texture_name);
		m_sprite.setTexture(*m_texture);
		m_sprite.setTextureRect(sf::IntRect(pos_x, pos_y, tile_size, tile_size));
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		target.draw(m_sprite);
	}
private:
	std::shared_ptr<sf::Texture> m_texture;
	sf::Sprite m_sprite;
};
using TileModelMap = std::unordered_map<std::string, std::shared_ptr<TileModel>>;

class Tile : public sf::Drawable
{
public:
	Tile() 
		: m_model(nullptr)
		, m_size(0)
		, m_position(0,0)
		, m_is_solid(false) 
	{}
	virtual ~Tile() {}

	virtual void deserialise(TileModelMap& models, const Json::Value& data)
	{
		auto tile_model_name = data.get("tile_model_name", "").asString();
		auto column = data.get("column", 0.f).asFloat();
		auto row = data.get("row", 0.f).asFloat();

		auto model_itr = models.find(tile_model_name);
		if (model_itr == models.end())
		{
			return;
		}
		m_model = model_itr->second;
		m_size = data.get("size", 0).asInt();
		m_is_solid = data.get("is_solid", false).asBool();
		m_position.x = column;
		m_position.y = row;
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		m_model->setPosition(sf::Vector2f(m_position.x * m_size, m_position.y * m_size));
		target.draw(*m_model);
	}

private:
	std::shared_ptr<TileModel> m_model;
	int m_size;
	sf::Vector2f m_position;
	bool m_is_solid;
};
using TileVector = std::vector<std::shared_ptr<Tile>>;

class Map : public sf::Drawable
{
public:

	virtual void deserialise(TextureManager& textures, const Json::Value& data)
	{
		auto tile_models = data["tile_models"];
		for (auto i = 0; i < tile_models.size(); i++)
		{
			TileModel model;
			auto name = tile_models[i].get("name", "").asString();
			model.deserialise(textures, tile_models[i]["model"]);
			m_models[name] = std::make_shared<TileModel>(model);
		}

		auto tiles = data["tiles"];
		for (auto i = 0; i < tiles.size(); i++)
		{
			Tile tile;
			tile.deserialise(m_models, tiles[i]);
			m_tiles.push_back(std::make_shared<Tile>(tile));
		}
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		for each (auto tile in m_tiles)
		{
			target.draw(*tile);
		}
	}
private:
	TileModelMap m_models;
	TileVector m_tiles;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game", sf::Style::Default);

	Json::Reader reader;
	Json::Value root;
	std::ifstream file;
	file.open("Assets/data.json");

	if (file.is_open())
	{
		if (!reader.parse(file, root, false))
		{
			assert(false);
		}
		file.close();
	}

	TextureManager texture_manager;
	auto textures = root["textures"];
	for (auto i = 0; i < textures.size(); i++)
	{
		auto name = textures[i].get("name", "").asString();
		auto path = textures[i].get("path", "").asString();
		texture_manager.loadTexture(name, path);
	}

	Map map;
	map.deserialise(texture_manager, root["map"]);

	Player player;
	player.deserialise(texture_manager, root["entities"][0]);

	sf::Clock clock;
	auto frameTime = clock.restart();

	while (window.isOpen())
	{
		sf::Event e;
		if (window.pollEvent(e))
		{
			switch (e.type)
			{
			case e.Closed:
				window.close();
				break;
			case e.KeyPressed:
				if (e.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				break;
			default:
				break;
			}
		}

		player.handleInput();
		player.update(frameTime);

		window.clear(sf::Color::Black);
		window.draw(map);
		window.draw(player);
		window.display();

		frameTime = clock.restart();
	}

	return 0;
}

