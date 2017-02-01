#pragma once
#include <vector>
#include <bitset>
#include <ostream>
#include <stdexcept>
#include <string>
#include <memory>

#include <iostream>

const int size_b = 8;

class bitvector;

class bitvector_reference
{
    unsigned int m_position;
    bitvector*   m_bitvector;
public:
    bitvector_reference(bitvector*, unsigned int);
    bitvector_reference& operator=(bool);

    friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
};

class bitvector
{
    friend class bitvector_reference;

    std::vector<unsigned char> values;
    unsigned int m_size;

    unsigned char get_mask(int bit_index) const
    {
        if (bit_index > size_b)
        {
            throw std::out_of_range(std::to_string(bit_index) + " is too big. max value is " + std::to_string(size_b - 1));
        }
        return (unsigned char)((1) << (size_b - bit_index));
    }
public:
    bitvector();
    bitvector(unsigned int initial_length, bool initial_value);

    unsigned int size() const;
    void push(bool v);

    const bitvector_reference operator[](unsigned int pos) const;
          bitvector_reference operator[](unsigned int pos);

    friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
    friend std::ostream& operator<<(std::ostream& os, const bitvector& bv);
};


/* bitvector_reference impl*/

bitvector_reference::bitvector_reference(bitvector* vector, unsigned int position):
    m_bitvector(vector), m_position(position)
{
}

bitvector_reference& bitvector_reference::operator=(bool value)
{
    auto byte_index = m_bitvector->m_size / size_b;
    auto bit_index = m_bitvector->m_size % size_b;
    unsigned char mask = m_bitvector->get_mask(bit_index);
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

std::ostream& operator<<(std::ostream& os, const bitvector_reference& reference)
{
    auto byte_index = reference.m_bitvector->m_size / size_b;
    auto bit_index = reference.m_bitvector->m_size % size_b;
    auto mask = reference.m_bitvector->get_mask(bit_index);
    return (os << (reference.m_bitvector->values[byte_index] & mask ? true : false));
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
        push(initial_value);
    }
}

unsigned int bitvector::size() const
{
    return m_size;
}

void bitvector::push(bool v)
{
    m_size++;
    auto bit_index = m_size % size_b;
    if (bit_index == 1)
    {
        values.push_back(0);
    }
    auto tmp = const_cast<bitvector*>(this);
    bitvector_reference ref(tmp, m_size);
    ref = v;
}

const bitvector_reference bitvector::operator[](unsigned int pos) const
{
    if (pos > m_size)
    {
        throw std::out_of_range(std::to_string(pos) + " is too big. size() is " + std::to_string(m_size));
    }
    auto tmp = const_cast<bitvector*>(this);
    return bitvector_reference(tmp, m_size);
}

bitvector_reference bitvector::operator[](unsigned int pos)
{
    if (pos > m_size)
    {
        throw std::out_of_range(std::to_string(pos) + " is too big. size() is " + std::to_string(m_size));
    }
    return bitvector_reference(this, pos);
}

std::ostream& operator<<(std::ostream& os, const bitvector& bv)
{
    os << "size: " << bv.size() << " value: ";
    for(auto v: bv.values)
    {
        os << std::bitset<size_b>(v) << " ";
    }
    return os;
}