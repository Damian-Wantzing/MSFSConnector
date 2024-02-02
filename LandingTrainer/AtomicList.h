#pragma once
#include <List>

template <typename T>
class AtomicList
{
public:
    typedef typename std::list<T>::iterator iterator;
    typedef typename std::list<T>::const_iterator const_iterator;

    iterator begin() { return list.begin(); }
    const_iterator begin() const { return list.begin(); }
    iterator end() { return list.end(); }
    const_iterator end() const { return list.end(); }
    
    typename void push_back(const T& value) { list.push_back(value); }
    bool empty() { return list.empty(); };
    iterator erase(const const_iterator _Where) { return list.erase(_Where); };

private:
	std::list<T> list;
};

