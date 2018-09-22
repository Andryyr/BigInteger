#include "bastard.h"
#include <iostream>

bastard::bastard() : _size(0), isSmall(true)
{
    data.small = 0;
}

void bastard::make_big()
{
    if (!isSmall)
        return;

    isSmall = false;

    uint32_t mas;

    mas = data.small;

    vector<uint32_t> *vect = new vector<uint32_t>(_size);

    new(&data.big) std::shared_ptr<std::vector<uint32_t>>(vect);

    (*data.big.get())[0] = mas;
}

bastard::bastard(bastard const &other)
{
    _size = other._size;
    isSmall = other.isSmall;
    if (other.isSmall)
    {
        data.small = other.data.small;
    } else
    {
        new(&data.big) std::shared_ptr<vector<uint32_t>>(other.data.big);
    }
}

bastard &bastard::operator=(bastard const &other)
{
    _size = other._size;

    if (!isSmall)
    {
        data.big.reset();
    }
    if (other.isSmall)
    {
        data.small = other.data.small;
    } else
    {
        new(&data.big) std::shared_ptr<vector<uint32_t>>(other.data.big);
    }
    isSmall = other.isSmall;
    return *this;
}

size_t bastard::size() const
{
    return _size;
}

uint32_t bastard::back() const
{
    assert(_size > 0);
    return (isSmall ? data.small : (*data.big.get())[_size - 1]);
}

void bastard::push_back(uint32_t const x)
{
    if (isSmall && _size < 1)
    {
        data.small = x;
        _size++;
        return;
    }
    make_unique();
    make_big();
    (*data.big.get()).push_back(x);
    _size++;
}

void bastard::pop_back()
{
    assert(_size > 0);
    _size--;
    if (isSmall)
    {
        return;
    }
    make_unique();
    (*data.big.get()).pop_back();
    if (data.big->size() == 1)
    {
        uint32_t tmp = data.big->back();
        data.big.~shared_ptr();
        isSmall = true;
        data.small = tmp;
    }
}

uint32_t &bastard::operator[](size_t ind)
{
    assert(ind < _size);
    if (isSmall)
        return data.small;
    make_unique();
    return (*data.big.get())[ind];
}

uint32_t const &bastard::operator[](size_t ind) const
{
    assert(ind < _size);
    return (isSmall ? data.small : (*data.big.get())[ind]);
}

void bastard::clear() {
    make_unique();
    if (!isSmall) {
        data.big.reset();
        data.big.~shared_ptr();
        isSmall = true;
    }
    memset(&data.small, 0, sizeof(uint32_t));
    _size = 0;
}

void bastard::make_unique()
{
    if (isSmall || data.big.unique())
    {
        return;
    }
    data.big = std::make_shared<std::vector<uint32_t >>(*data.big);
}

bastard::~bastard()
{
    if (!isSmall)
    {
        data.big.reset();
        data.big.~shared_ptr();
    }
}