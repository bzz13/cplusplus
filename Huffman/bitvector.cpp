#include "bitvector.h"

/* bitvector_reference impl*/

bitvector_reference::bitvector_reference(bitvector* vector, unsigned int position):
    m_bitvector(vector), m_position(position)
{
}

unsigned char bitvector_reference::get_bitvector_bit_mask(int bit_index) const
{
    if (bit_index > size_b)
    {
        throw std::out_of_range(std::to_string(bit_index) + " is too big. max value is " + std::to_string(size_b - 1));
    }
    return (unsigned char)(1 << (size_b - 1 - bit_index));
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

void bitvector::clear()
{
    m_size = 0;
    values.clear();
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

void bitvector::push_back(const bitvector& other)
{
    for(auto v: other)
    {
        push_back(v);
    }
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
    if (&os == &(std::cout))
    {
        for(auto v: bv)
        {
            os << v;
        }
        os << " size: " << bv.size();
    }
    else
    {
        os << bv.size() << " ";
        for (auto c: bv.values)
        {
            os << c;
        }
    }
    return os;
}


std::istream& operator>>(std::istream& is, bitvector& bv)
{
    if (&is == &(std::cin))
    {
       std::string s;
       is >> s;
       bv = bitvector(s);
    }
    else
    {
        bv.clear();
        unsigned int size;
        is >> size;
        bv.m_size = size;

        auto byte_size = size % size_b == 0 ? size / size_b : size / size_b + 1;
        char c;
        is.get(c); // space
        for(auto i = 0; i < byte_size; ++i)
        {
            is.get(c);
            bv.values.push_back(c);
        }
    }
    return is;
}

bitvector_iterator bitvector::begin()
{
    return bitvector_iterator(this, 0);
}

bitvector_iterator bitvector::end()
{
    return bitvector_iterator(this, m_size);
}

const_bitvector_iterator bitvector::begin() const
{
    auto tmp = const_cast<bitvector*>(this);
    return const_bitvector_iterator(tmp, 0);
}

const_bitvector_iterator bitvector::end() const
{
    auto tmp = const_cast<bitvector*>(this);
    return const_bitvector_iterator(tmp, m_size);
}