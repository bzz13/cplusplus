#pragma once
#include <vector>
#include <bitset>
#include <ostream>
#include <istream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>

const int size_b = 8;

class bitvector;

struct bitvector_reference
{
    unsigned int m_position;
    bitvector*   m_bitvector;

    bitvector_reference(bitvector*, unsigned int);

    unsigned char get_bitvector_bit_mask(int bit_index) const;

    bitvector_reference& operator=(bool);

    operator bool() const;

    friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
};

struct bitvector_iterator: public bitvector_reference, public std::iterator<std::forward_iterator_tag, bool>
{
    bitvector_iterator(bitvector* bv, unsigned int pos = 0): bitvector_reference(bv, pos) { }
    bitvector_iterator& operator++() //prefix increment
    {
        m_position ++;
        return (*this);
    }
    bitvector_iterator operator++(int) //postfix increment
    {
        auto tmp = *this;
        m_position ++;
        return bitvector_iterator(tmp);
    }
    bitvector_reference& operator*()
    {
        return *((bitvector_reference*)this);
    }
    bool operator==(const bitvector_iterator& other) const
    {
        return (m_bitvector == other.m_bitvector && m_position == other.m_position);
    }
    bool operator!=(const bitvector_iterator& other) const
    {
        return (m_bitvector != other.m_bitvector || m_position != other.m_position);
    }
    operator bool() const
    {
        return (bool)((bitvector_reference)(*this));
    }
};


struct const_bitvector_iterator: public bitvector_reference, public std::iterator<std::forward_iterator_tag, bool>
{
    const_bitvector_iterator(bitvector* bv, unsigned int pos = 0): bitvector_reference(bv, pos) { }
    // const_bitvector_iterator(iterator& it): bitvector_reference( it.m_bitvector, it.m_position) { }

    const_bitvector_iterator& operator++() //prefix increment
    {
        m_position ++;
        return (*this);
    }
    const_bitvector_iterator operator++(int) //postfix increment
    {
        auto tmp = *this;
        m_position ++;
        return const_bitvector_iterator(tmp);
    }
    const bitvector_reference& operator*() const
    {
        return *((bitvector_reference*)this);
    }
    bool operator==(const const_bitvector_iterator& other) const
    {
        return (m_bitvector == other.m_bitvector && m_position == other.m_position);
    }
    bool operator!=(const const_bitvector_iterator& other) const
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
    void clear();
    void push_back(bool);
    void push_back(const bitvector&);

    const bitvector_reference operator[](unsigned int) const;
          bitvector_reference operator[](unsigned int);

    friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
    friend std::ostream& operator<<(std::ostream&, const bitvector&);
    friend std::istream& operator>>(std::istream&, bitvector&);

    bitvector_iterator begin();
    bitvector_iterator end();

    const_bitvector_iterator begin() const;
    const_bitvector_iterator end() const;
};