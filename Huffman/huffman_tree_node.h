#pragma once
#include <memory>
#include <sstream>

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

    huffman_tree_node(char _c, size_t _i, bool _term, node _l = nullptr, node _r = nullptr, node _p = nullptr);

    std::string to_string();

    static node join(node left, node right);
};

struct node_comparison
{
    node_comparison(const bool& revparam = false);

    bool operator() (const node& lhs, const node& rhs) const;

private:
    bool reverse;
};