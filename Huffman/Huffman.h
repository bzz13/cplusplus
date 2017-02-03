#pragma once
#include <string>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include <memory>
#include <algorithm>
#include "huffman_tree_node.h"
#include "bitvector.h"

class huffman
{
    std::unordered_map<char, bitvector> translations;
    node root;

    std::unordered_map<char, size_t> recalculate_counters(const char* buffer, size_t size);
    std::vector<node> build_prefix_tree(const std::unordered_map<char, size_t>& counters);
    std::unordered_map<char, bitvector> build_translation_table(const std::vector<node>& nodes);
    bitvector translate(const char* buffer, size_t size);

public:
    bitvector encode(const char* buffer, size_t size);
    bitvector encode(const std::string& inputToEncode);

    size_t decode(const bitvector& encodedInput, char* outbuffer, size_t size);
    std::string decode(const bitvector& encodedInput);

    const std::unordered_map<char, bitvector>& get_translation_table() const;
    void set_translation_table(const std::unordered_map<char, bitvector>& table);

    void store_translateion_table_to_ostream(std::ostream& os) const;
    void load_translateion_table_from_istream(std::istream& is);
};