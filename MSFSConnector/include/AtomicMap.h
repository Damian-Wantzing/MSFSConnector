#pragma once

#include <map>
#include <shared_mutex>

namespace Atomics
{
	template <typename K, typename V>
	class AtomicMap
	{
	public:
		V get(const K& key)
		{
			std::shared_lock<std::shared_mutex> lock(rwMutex);
			return map[key];
		}

		void set(const std::pair<K, V>& value)
		{
			std::unique_lock<std::shared_mutex> lock(rwMutex);
			map[value.first] = value.second;
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
}