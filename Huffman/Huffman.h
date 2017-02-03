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
#include "bitvector.h"


struct huffman_tree_node;
typedef std::shared_ptr<huffman_tree_node> node;

struct huffman_tree_node
{
    char c;
    size_t count;
    bool weight;
    bool terminated;
    node parent;
    node left;
    node right;

    huffman_tree_node(char _c, size_t _i, bool _term, node _l = nullptr, node _r = nullptr, node _p = nullptr)
        : c(_c), count(_i), terminated(_term), parent(_p), left(_l), right(_r) { }

    std::string to_string()
    {
        std::stringstream ss;
        ss << this << "{" << c << ":" << count;
        if (parent)
            ss << "--" << weight << "--> " << parent->to_string();
        ss << "}";
        return ss.str();
    }

    static node join(node left, node right)
    {
        auto parent = std::make_shared<huffman_tree_node>('\0', left->count + right->count, false, left, right);
        left->parent = parent;
        left->weight = false;
        right->parent = parent;
        right->weight = true;
        return parent;
    }
};

class node_comparison
{
    bool reverse;
public:
    node_comparison(const bool& revparam=false)
        : reverse(revparam){ }

    bool operator() (const node& lhs, const node& rhs) const
    {
        return reverse
            ? (lhs->count > rhs->count)
            : (lhs->count < rhs->count);
    }
};

class huffman
{
    std::unordered_map<char, size_t> counters;
    std::vector<node> nodes;
    std::unordered_map<char, bitvector> translations;
    node root;

    void recalculate_counters(const char* buffer, size_t size)
    {
        counters.clear();
        for(size_t i = 0; i < size; ++i)
        {
            counters[buffer[i]]++;
        }
        // for(auto p: counters)
        //      std::cout << p.first << "=" << p.second << std::endl;
    }

    void build_prefix_tree()
    {
        nodes.clear();
        for(auto p: counters)
        {
            nodes.push_back(std::make_shared<huffman_tree_node>(p.first, p.second, true));
        }

        std::priority_queue<node, std::vector<node>, node_comparison> 
            q(nodes.begin(), nodes.end(), node_comparison(true));

        while (q.size() > 1)
        {
            auto left = q.top(); q.pop();
            auto right = q.top(); q.pop();
            auto p = huffman_tree_node::join(left, right);
            q.push(p);
        }

        root = q.top(); q.pop();
    }

    void build_translation_table()
    {
        translations.clear();
        for(auto n: nodes)
        {
            // std::cout << n->to_string() << std::endl;
            std::stack<bool> path;
            auto tmp = n;
            while(tmp->parent)
            {
                path.push(tmp->weight);
                tmp = tmp->parent;
            }
            bitvector r;
            while(!path.empty())
            {
                r.push_back(path.top());
                path.pop();
            }
            translations[n->c] = r;

            if (n->c == '\0')
                std::cout << "\\0: " << r << std::endl;
            else
                std::cout << n->c << ": " << r << std::endl;
        }
    }

    bitvector translate(const char* buffer, size_t size)
    {
        bitvector result;
        for(size_t i = 0; i < size; ++i)
        {
            result.push_back(translations[buffer[i]]);
        }
        return result;
    }

public:
    bitvector encode(const char* buffer, size_t size)
    {
        recalculate_counters(buffer, size);
        build_prefix_tree();
        build_translation_table();
        return translate(buffer, size);
    }

    bitvector encode(const std::string& inputToEncode)
    {
        return encode(inputToEncode.c_str(), inputToEncode.size());
    }

    bitvector encode(const char* inputToEncode)
    {
        return encode(std::string(inputToEncode));
    }

    size_t decode(const bitvector& encodedInput, char* outbuffer, size_t size)
    {
        size_t decodedCount = 0;
        auto tmp = root;

        for(auto b: encodedInput)
        {
            tmp = b ? tmp->right : tmp->left;
            if (tmp->terminated)
            {
                outbuffer[decodedCount] = tmp->c;
                tmp = root;
                decodedCount++;
                if (decodedCount == size)
                {
                    break;
                }
            }
        }
        if (tmp != root)
        {
            throw std::invalid_argument("inconsistent state, wrong input");
        }
        return decodedCount;
    }

    std::string decode(const bitvector& encodedInput)
    {
        std::stringstream result;

        auto tmp = root;
        for(auto b: encodedInput)
        {
            tmp = b ? tmp->right : tmp->left;
            if (tmp->terminated)
            {
                result << tmp->c;
                tmp = root;
            }
        }
        if (tmp != root)
        {
            throw std::invalid_argument("inconsistent state, wrong input");
        }

        return result.str();
    }
};