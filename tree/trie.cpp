#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>

using namespace std;

class trie
{
    struct trie_node
    {
        trie_node(bool terminal = false) : is_terminal(terminal)
        {}

        bool is_terminal;
        unordered_map<char, unique_ptr<trie_node>> children;
    };

    unique_ptr<trie_node> root;

public:
    trie(): root(new trie_node{})
    {}

    void insert(string str)
    {
        auto tmp = root.get();
        for (auto c: str)
        {
            if(tmp->children.find(c) == tmp->children.end())
            {
                tmp->children[c] = make_unique<trie_node>();
            }
            tmp = tmp->children[c].get();
        }
        tmp->is_terminal = true;
    }

    bool find(string str)
    {
        auto tmp = root.get();
        for (auto c: str)
        {
            if(tmp->children.find(c) == tmp->children.end())
            {
                return false;
            }
            tmp = tmp->children[c].get();
        }
        return tmp->is_terminal;
    }
};

int main()
{
    trie t;
    cout << (t.find("qwe") == false) << endl;
    t.insert("qwe");
    t.insert("qwerty");
    cout << (t.find("qwe") == true) << endl;
    cout << (t.find("qwer") == false) << endl;
    cout << (t.find("qwerty") == true) << endl;
    t.insert("abc");
    cout << (t.find("ab") == false) << endl;
    cout << (t.find("abc") == true) << endl;
    cout << (t.find("abcd") == false) << endl;
}
