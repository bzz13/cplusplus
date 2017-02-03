#pragma once
#include <vector>
#include <bitset>
#include <ostream>
#include <istream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>


class bitvector
{
    std::vector<unsigned char> values;
    size_t m_size;
public:
    static const int size_b = 8;

    struct bitvector_reference
    {
        size_t      m_position;
        bitvector*  m_bitvector;

        bitvector_reference(bitvector*, size_t);
        unsigned char get_bitvector_bit_mask(int bit_index) const;
        bitvector_reference& operator=(bool);
        operator bool() const;
        friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
    };

    struct bitvector_iterator: public bitvector_reference, public std::iterator<std::forward_iterator_tag, bool>
    {
        bitvector_iterator(bitvector* bv, size_t pos = 0);
        bitvector_iterator& operator++(); //prefix increment
        bitvector_iterator operator++(int); //postfix increment
        bitvector_reference& operator*();
        bool operator==(const bitvector_iterator& other) const;
        bool operator!=(const bitvector_iterator& other) const;
        operator bool() const;
    };

    struct const_bitvector_iterator: public bitvector_reference, public std::iterator<std::forward_iterator_tag, bool>
    {
        const_bitvector_iterator(bitvector* bv, size_t pos = 0);
        // const_bitvector_iterator(iterator& it);
        const_bitvector_iterator& operator++(); //prefix increment
        const_bitvector_iterator operator++(int); //postfix increment;
        const bitvector_reference& operator*() const;
        bool operator==(const const_bitvector_iterator& other) const;
        bool operator!=(const const_bitvector_iterator& other) const;
        operator bool() const;
    };

    bitvector();
    bitvector(size_t, bool initial_value = false);
    bitvector(std::string);
    bitvector(const char*);

    size_t size() const;
    void clear();
    void push_back(bool);
    void push_back(const bitvector&);

    const bitvector_reference operator[](size_t) const;
          bitvector_reference operator[](size_t);

    friend std::ostream& operator<<(std::ostream&, const bitvector_reference&);
    friend std::ostream& operator<<(std::ostream&, const bitvector&);
    friend std::istream& operator>>(std::istream&, bitvector&);

    bitvector_iterator begin();
    bitvector_iterator end();

    const_bitvector_iterator begin() const;
    const_bitvector_iterator end() const;
};