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


struct huffman_tree_node;

typedef std::shared_ptr<huffman_tree_node> node;

struct huffman_tree_node
{
    char c;
    unsigned int count;
    bool weight;
    node parent;
    node left;
    node right;

    huffman_tree_node(char _c, unsigned int _i, node _l = nullptr, node _r = nullptr, node _p = nullptr)
        : c(_c), count(_i), parent(_p), left(_l), right(_r) { }

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
        auto parent = std::make_shared<huffman_tree_node>('\0', left->count + right->count, left, right);
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
    std::unordered_map<char, unsigned int> counters;
    std::vector<node> nodes;
    std::unordered_map<char, std::string> translations;
    node root;

    void recalculate_counters(const std::string& str)
    {
        counters.clear();
        for(auto c: str)
        {
            counters[c]++;
        }
        // for(auto p: counters)
        //      std::cout << p.first << "=" << p.second << std::endl;
    }

    void build_prefix_tree()
    {
        nodes.clear();
        for(auto p: counters)
        {
            nodes.push_back(std::make_shared<huffman_tree_node>(p.first, p.second));
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
            std::stringstream ss;
            auto tmp = n;
            while(tmp->parent)
            {
                ss << tmp->weight;
                tmp = tmp->parent;
            }
            auto r = ss.str();
            std::reverse(r.begin(), r.end());
            translations[n->c] = r;

            std::cout << n->c << ": " << r << std::endl;
        }
    }

    std::string translate(const std::string& str)
    {
        std::stringstream result;
        for(auto c: str)
        {
            result << translations[c];
        }
        return result.str();
    }

public:
    std::string encode(const std::string& inputToEncode)
    {
        recalculate_counters(inputToEncode);
        build_prefix_tree();
        build_translation_table();
        auto result = translate(inputToEncode);
        return result;
    }

    std::string encode(const char* inputToEncode)
    {
        return encode(std::string(inputToEncode));
    }

    std::string decode(const std::string& encodedInput)
    {
        std::stringstream result;
        auto tmp = root;
        for(auto c: encodedInput)
        {
            tmp = c == '1' ? tmp->right : tmp->left;
            if (tmp->c != '\0')
            {
                result << tmp->c;
                tmp = root;
            }
        }
        return result.str();
    }
    std::string decode(const char* encodedInput)
    {
        return decode(std::string(encodedInput));
    }
};