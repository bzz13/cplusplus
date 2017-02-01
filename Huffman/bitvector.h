#pragma once
#include <vector>
#include <bitset>
#include <ostream>
#include <stdexcept>
#include <string>
#include <memory>

#include <iostream>

const int size_b = 8;
unsigned char get_bitvector_bit_mask(int bit_index)
{
    if (bit_index > size_b)
    {
        throw std::out_of_range(std::to_string(bit_index) + " is too big. max value is " + std::to_string(size_b - 1));
    }
    return (unsigned char)(1 << (size_b - 1 - bit_index));
}

class bitvector;

struct bitvector_reference
{
    unsigned int m_position;
    bitvector*   m_bitvector;

    bitvector_reference(bitvector*, unsigned int);
    bitvector_reference& operator=(bool);

    operator bool() const;

    friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
};

class iterator: public bitvector_reference, public std::iterator<std::forward_iterator_tag, bool>
{
public:
    iterator(bitvector* bv, unsigned int pos = 0): bitvector_reference(bv, pos) { }
    iterator& operator++() //prefix increment
    {
        m_position ++;
        return (*this);
    }
    iterator operator++(int) //postfix increment
    {
        auto tmp = *this;
        m_position ++;
        return iterator(tmp);
    }
    bitvector_reference& operator*()
    {
        return *((bitvector_reference*)this);
    }
    bool operator==(const iterator& other) const
    {
        return (m_bitvector == other.m_bitvector && m_position == other.m_position);
    }
    bool operator!=(const iterator& other) const
    {
        return (m_bitvector != other.m_bitvector || m_position != other.m_position);
    }
    operator bool() const
    {
        return (bool)((bitvector_reference)(*this));
    }
};


class const_iterator: public bitvector_reference, public std::iterator<std::forward_iterator_tag, bool>
{
public:
    const_iterator(bitvector* bv, unsigned int pos = 0): bitvector_reference(bv, pos) { }
    // const_iterator(iterator& it): bitvector_reference( it.m_bitvector, it.m_position) { }

    const_iterator& operator++() //prefix increment
    {
        m_position ++;
        return (*this);
    }
    const_iterator operator++(int) //postfix increment
    {
        auto tmp = *this;
        m_position ++;
        return const_iterator(tmp);
    }
    const bitvector_reference& operator*() const
    {
        return *((bitvector_reference*)this);
    }
    bool operator==(const const_iterator& other) const
    {
        return (m_bitvector == other.m_bitvector && m_position == other.m_position);
    }
    bool operator!=(const const_iterator& other) const
    {
        return (m_bitvector != other.m_bitvector || m_position != other.m_position);
    }
    operator bool() const
    {
        return (bool)((bitvector_reference)(*this));
    }
};

class bitvector
{
    friend class bitvector_reference;

    std::vector<unsigned char> values;
    unsigned int m_size;
public:
    bitvector();
    bitvector(unsigned int, bool initial_value = false);
    bitvector(std::string);
    bitvector(const char*);

    unsigned int size() const;
    void push_back(bool);

    const bitvector_reference operator[](unsigned int) const;
          bitvector_reference operator[](unsigned int);

    friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
    friend std::ostream& operator<<(std::ostream&, const bitvector&);


    iterator begin()
    {
        return iterator(this, 0);
    }
    const_iterator begin() const
    {
        auto tmp = const_cast<bitvector*>(this);
        return const_iterator(tmp, 0);
    }
    iterator end()
    {
        return iterator(this, m_size);
    }
    const_iterator end() const
    {
        auto tmp = const_cast<bitvector*>(this);
        return const_iterator(tmp, m_size);
    }
};


/* bitvector_reference impl*/

bitvector_reference::bitvector_reference(bitvector* vector, unsigned int position):
    m_bitvector(vector), m_position(position)
{
}

bitvector_reference& bitvector_reference::operator=(bool value)
{
    auto byte_index = m_position / size_b;
    auto bit_index = m_position % size_b;
    unsigned char mask = get_bitvector_bit_mask(bit_index);
    unsigned char current = m_bitvector->values[byte_index] & mask;

    if(value)
    {
        m_bitvector->values[byte_index] |= mask;
    }
    else
    {
        if (current && !value)
        {
            m_bitvector->values[byte_index] ^= mask;
        }
    }
    return *this;
}

bitvector_reference::operator bool() const
{
    auto byte_index = m_position / size_b;
    auto bit_index = m_position % size_b;
    auto mask = get_bitvector_bit_mask(bit_index);
    return (bool)(m_bitvector->values[byte_index] & mask);
}

std::ostream& operator<<(std::ostream& os, const bitvector_reference& reference)
{
    return (os << ((bool)reference));
}



/* bitvector impl */


bitvector::bitvector(): m_size(0)
{
}
bitvector::bitvector(unsigned int initial_length, bool initial_value)
    : m_size((initial_length / size_b) * size_b),
      values(initial_length / size_b, initial_value ? (unsigned char)(~0) : (unsigned char)(0))
{

    for (int i = values.size() * size_b; i < initial_length; ++i)
    {
        push_back(initial_value);
    }
}
bitvector::bitvector(std::string str): bitvector()
{
    for (auto c: str)
    {
        push_back(c - '0');
    }
}
bitvector::bitvector(const char* str): bitvector(std::string(str))
{
}

unsigned int bitvector::size() const
{
    return m_size;
}

void bitvector::push_back(bool v)
{
    m_size++;
    auto bit_index = m_size % size_b;
    if (bit_index == 1)
    {
        values.push_back(0);
    }
    auto tmp = const_cast<bitvector*>(this);
    bitvector_reference ref(tmp, m_size - 1);
    ref = v;
}

const bitvector_reference bitvector::operator[](unsigned int pos) const
{
    if (pos >= m_size)
    {
        throw std::out_of_range(std::to_string(pos) + " is too big. size() is " + std::to_string(m_size));
    }
    auto tmp = const_cast<bitvector*>(this);
    return bitvector_reference(tmp, pos);
}

bitvector_reference bitvector::operator[](unsigned int pos)
{
    if (pos >= m_size)
    {
        throw std::out_of_range(std::to_string(pos) + " is too big. size() is " + std::to_string(m_size));
    }
    return bitvector_reference(this, pos);
}

std::ostream& operator<<(std::ostream& os, const bitvector& bv)
{
    for(auto v: bv)
    {
        os << v;
    }
    return (os << " size: " << bv.size());
}