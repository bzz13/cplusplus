#include "huffman.h"

std::unordered_map<char, size_t> huffman::recalculate_counters(const char* buffer, size_t size)
{
    std::unordered_map<char, size_t> counters;
    for(size_t i = 0; i < size; ++i)
    {
        counters[buffer[i]]++;
    }
    return counters;
}

std::vector<node> huffman::build_prefix_tree(const std::unordered_map<char, size_t>& counters)
{
    std::vector<node> nodes;
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
    return nodes;
}

std::unordered_map<char, bitvector> huffman::build_translation_table(const std::vector<node>& nodes)
{
    std::unordered_map<char, bitvector> table;
    for(auto n: nodes)
    {
        std::stack<bool> path;
        auto tmp = n;
        while(tmp->parent)
        {
            path.push(tmp->weight);
            tmp = tmp->parent;
        }
        bitvector result;
        while(!path.empty())
        {
            result.push_back(path.top());
            path.pop();
        }
        table[n->c] = result;
    }
    return table;
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
    translations = build_translation_table(build_prefix_tree(recalculate_counters(buffer, size)));
    init = true;
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

const std::unordered_map<char, bitvector>& huffman::get_translation_table() const
{
    if (!init)
    {
        throw std::logic_error("translation table not init yet");
    }
    return translations;
}
void huffman::set_translation_table(const std::unordered_map<char, bitvector>& table)
{
    root = std::make_shared<huffman_tree_node>('\0', 0, false);
    for(auto p: table)
    {
        auto tmp = root;
        for(auto bit: p.second)
        {
            if (bit)
            {
                if(!(tmp->right))
                {
                    tmp->right = std::make_shared<huffman_tree_node>('\0', 0, false);
                }
                tmp = tmp->right;
            }
            else
            {
                if(!(tmp->left))
                {
                    tmp->left = std::make_shared<huffman_tree_node>('\0', 0, false);
                }
                tmp = tmp->left;
            }
        }
        tmp->c = p.first;
        tmp->terminated = true;
    }
    init = true;
}

void huffman::store_translateion_table_to_ostream(std::ostream& os) const
{
    os << translations.size() << std::endl;
    for(auto p: translations)
    {
        os << p.first << " " << p.second << std::endl;
    }
}

void huffman::load_translateion_table_from_istream(std::istream& is)
{
    char c;
    size_t size, count = 0;
    is >> size;
    is.get(c); // endl
    std::unordered_map<char, bitvector> table;
    while(!is.eof() && count < size)
    {
        is.get(c);
        if (is.eof())
        {
            break;
        }
        is >> table[c];
        is.get(c); // endl
        count++;
    }
    set_translation_table(table);
}