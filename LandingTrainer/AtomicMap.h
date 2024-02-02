#pragma once

#include <map>
#include <shared_mutex>

template <typename K, typename V>
class AtomicMap
{
public:
	V get(const K& key)
	{
		std::shared_lock<std::shared_mutex> lock(rwMutex);
		return map[key];
	}

	std::pair<typename std::map<K, V>::iterator, bool> insert(const std::pair<K, V>& value)
	{
		std::unique_lock<std::shared_mutex> lock(rwMutex);
		return map.insert(value);
	}
	
	size_t count(const K& key) const
	{
		std::shared_lock<std::shared_mutex> lock(rwMutex);
		return map.count(key);
	}

	size_t erase(const K& key)
	{
		std::unique_lock<std::shared_mutex> lock(rwMutex);
		return map.erase(key);
	}

private:
	std::map<K, V> map;
	mutable std::shared_mutex rwMutex;

};

