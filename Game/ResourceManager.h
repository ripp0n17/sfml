#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>

/*
 * Manages resources of a specified type.
 */
template <class T>
class ResourceManager
{
public:
	ResourceManager() {}
	virtual ~ResourceManager() {}

	/*
	 * Returns true if a resource with the specified name has
	 * been loaded into the internal storage, otherwise false.
	 */
	bool isLoaded(const std::string& name)
	{
		return (m_resources.find(name) != m_resources.end());
	}

	/*
	 * Loads and stores the resource located at the specified path
	 * with the specified name. Returns true if successful, 
	 * otherwise false.
	 */
	bool load(const std::string& name, const std::string& path)
	{
		if (!isLoaded(name))
		{
			auto resource = new T;
			if (!resource->loadFromFile(path))
			{
				std::cout << "ERROR: loading texture " << path << std::endl;
				return false;
			}
			m_resources[name] = std::shared_ptr<T>(resource);
		}
		return true;
	}

	/*
	 * Returns the resource stored with the specified name.
	 * If no such resource, returns a nullptr.
	 */
	std::shared_ptr<T> get(const std::string& name)
	{
		if (!isLoaded(name))
		{
			return nullptr;
		}
		return m_resources[name];
	}

protected:
	std::unordered_map<std::string, std::shared_ptr<T>> m_resources;
};
