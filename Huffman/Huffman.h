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
    std::unordered_map<char, size_t> counters;
    std::vector<node> nodes;
    std::unordered_map<char, bitvector> translations;
    node root;

    void recalculate_counters(const char* buffer, size_t size);
    void build_prefix_tree();
    void build_translation_table();
    bitvector translate(const char* buffer, size_t size);

public:
    bitvector encode(const char* buffer, size_t size);
    bitvector encode(const std::string& inputToEncode);

    size_t decode(const bitvector& encodedInput, char* outbuffer, size_t size);
    std::string decode(const bitvector& encodedInput);
};