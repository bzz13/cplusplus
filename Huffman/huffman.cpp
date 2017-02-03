#include "huffman.h"

void huffman::recalculate_counters(const char* buffer, size_t size)
{
    counters.clear();
    for(size_t i = 0; i < size; ++i)
    {
        counters[buffer[i]]++;
    }
    // for(auto p: counters)
    //      std::cout << p.first << "=" << p.second << std::endl;
}

void huffman::build_prefix_tree()
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

void huffman::build_translation_table()
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

bitvector huffman::translate(const char* buffer, size_t size)
{
    bitvector result;
    for(size_t i = 0; i < size; ++i)
    {
        result.push_back(translations[buffer[i]]);
    }
    return result;
}

bitvector huffman::encode(const char* buffer, size_t size)
{
    recalculate_counters(buffer, size);
    build_prefix_tree();
    build_translation_table();
    return translate(buffer, size);
}

bitvector huffman::encode(const std::string& inputToEncode)
{
    return encode(inputToEncode.c_str(), inputToEncode.size());
}

size_t huffman::decode(const bitvector& encodedInput, char* outbuffer, size_t size)
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

std::string huffman::decode(const bitvector& encodedInput)
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