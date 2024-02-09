#pragma once
#include <List>
#include <shared_mutex>

namespace Atomics
{
    template <typename T>
    class AtomicList
    {
    public:
        typedef typename std::list<T>::iterator iterator;
        typedef typename std::list<T>::const_iterator const_iterator;

        iterator begin()
        {
            std::shared_lock<std::shared_mutex> lock(rwMutex);
            return list.begin();
        }

        const_iterator begin() const
        {
            std::shared_lock<std::shared_mutex> lock(rwMutex);
            return list.begin();
        }

        iterator end()
        {
            std::shared_lock<std::shared_mutex> lock(rwMutex);
            return list.end();
        }

        const_iterator end() const
        {
            std::shared_lock<std::shared_mutex> lock(rwMutex);
            return list.end();
        }

        typename void push_back(const T& value)
        {
            std::unique_lock<std::shared_mutex> lock(rwMutex);
            list.push_back(value);
        }

        bool empty() const
        {
            std::shared_lock<std::shared_mutex> lock(rwMutex);
            return list.empty();
        }

        iterator erase(const const_iterator _Where)
        {
            std::unique_lock<std::shared_mutex> lock(rwMutex);
            return list.erase(_Where);
        }

    private:
        std::list<T> list;
        mutable std::shared_mutex rwMutex;
    };
}