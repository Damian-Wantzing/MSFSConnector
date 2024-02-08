#pragma once

#include <vector>
#include <mutex>

namespace Atomics
{
    template <typename T>
    class AtomicVector
    {
    public:
        void push_back(T value)
        {
            std::lock_guard<std::mutex> lock(mutex);
            vec.push_back(value);
        }

        T& operator[](size_t index)
        {
            std::lock_guard<std::mutex> lock(mutex);
            return vec[index];
        }

        const T& operator[](size_t index) const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return vec[index];
        }

        size_t size() const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return vec.size();
        }

    private:
        std::vector<T> vec;
        mutable std::mutex mutex;
    };
}