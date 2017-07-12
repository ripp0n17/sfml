#pragma once
#include <string>
#include <unordered_map>
#include <functional>

using CallbackContainer = std::unordered_map<std::string, std::function<void(void)>>;

class EventManager
{
public:
	EventManager() {}
	~EventManager() {}

	template <class T>
	void addCallback(const std::string& name, void(T::*l_func)(void), T* instance)
	{
		m_callbacks[name] = l_func;
	}
private:
	CallbackContainer m_callbacks;
};
