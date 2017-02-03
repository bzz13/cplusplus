#include "huffman_tree_node.h"

huffman_tree_node::huffman_tree_node(char _char, size_t _count, bool _terminated, node _left, node _right, node _parent)
    : c(_char), count(_count), terminated(_terminated), parent(_parent), left(_left), right(_right)
{
}

std::string huffman_tree_node::to_string()
{
    std::stringstream ss;
    ss << this << "{" << c << ":" << count;
    if (parent)
        ss << "--" << weight << "--> " << parent->to_string();
    ss << "}";
    return ss.str();
}

node huffman_tree_node::join(node left, node right)
{
    auto parent = std::make_shared<huffman_tree_node>('\0', left->count + right->count, false, left, right);
    left->parent = parent;
    left->weight = false;
    right->parent = parent;
    right->weight = true;
    return parent;
}

node_comparison::node_comparison(const bool& revparam)
    : reverse(revparam)
{
}

bool node_comparison::operator() (const node& lhs, const node& rhs) const
{
    return reverse
        ? (lhs->count > rhs->count)
        : (lhs->count < rhs->count);
}