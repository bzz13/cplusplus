#include <iostream>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <time.h>

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

    void insert(const string& str)
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

    bool find(const string& str) const
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


class trie_str
{
    struct trie_node
    {
        bool is_terminal;
        unordered_map<char, pair<string, unique_ptr<trie_node>>> children;
    };

    unique_ptr<trie_node> root;
    bool is_freezed;

    void print_node(trie_node* node, const string& padding) const
    {
        if(!node) return;
        for(auto& p: node->children)
        {
            cout << padding << node << "(" << node->is_terminal << ") " 
                        << " " << p.first << ": " << p.second.first << ", " << p.second.second.get() << endl;
            print_node(p.second.second.get(), padding + "\t");
        }
    }

    void _freeze(trie_node* node, pair<const char, pair<string, unique_ptr<trie_node>>>& branch)
    {
        if (node->children.size() == 0)
        {
            delete branch.second.second.release();
            return;
        }
        if (node->children.size() == 1 && !node->is_terminal)
        {
            branch.second.first += node->children.begin()->first; // доростил строку символом
            branch.second.second = move(node->children.begin()->second.second);
            _freeze(branch.second.second.get(), branch);
        }
        else
        {
            for(auto& p: node->children)
            {
                _freeze(p.second.second.get(), p);
            }
        }
    }

public:
    trie_str(): root(new trie_node{}), is_freezed(false)
    {}

    void insert(const string& str)
    {
        auto tmp = root.get();
        for (auto c: str)
        {
            if(tmp->children.find(c) == tmp->children.end())
            {
                tmp->children[c] = make_pair<string, unique_ptr<trie_node>>(c+string(""), make_unique<trie_node>());
            }
            tmp = tmp->children[c].second.get();
        }
        tmp->is_terminal = true;
    }

    void freeze()
    {
        for(auto& p: root->children)
        {
            _freeze(p.second.second.get(), p);
        }
        // print_node(root.get(), "");
        is_freezed = true;
    }

    bool find(const string& str) const
    {
        if (!is_freezed)
            return false;

        auto tmp = root.get();
        int i = 0;
        while (i < str.size() && tmp)
        {
            auto c = str[i];
            auto find = tmp->children.find(c);
            if(find == tmp->children.end())
            {
                return false;
            }
            if (str.find(find->second.first, i) != i)
            {
                return false;
            }
            else
            {
                i += find->second.first.size();
                tmp = find->second.second.get();
            }
        }
        return (i == str.size()) && ( !tmp || (tmp && tmp->is_terminal));
    }
};



void t1()
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


void t2()
{
    trie_str t;
    t.insert("qwe");
    t.insert("qwj");
    t.insert("qwerty");
    t.insert("abc");
    t.freeze();
    cout << (t.find("qw") == false) << endl;
    cout << (t.find("qwj") == true) << endl;
    cout << (t.find("qwp") == false) << endl;
    cout << (t.find("qwe") == true) << endl;
    cout << (t.find("qwer") == false) << endl;
    cout << (t.find("qwerty") == true) << endl;
    cout << (t.find("ab") == false) << endl;
    cout << (t.find("abc") == true) << endl;
    cout << (t.find("abcd") == false) << endl;
}

vector<string> prepare()
{
    vector<string> v;
    // ifstream fin("20k.txt");
    ifstream fin("words.txt");
    while(!fin.eof())
    {
        string str;
        fin >> str;
        v.push_back(str);
    }
    fin.close();
    return v;
}

void t3(const std::vector<string>& v)
{
    cout << "char trie" << endl;
    trie t;
    clock_t tStart = clock();
    for(auto s: v)
    {
        t.insert(s);
    }
    printf("Time taken to insert: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    tStart = clock();
    for(auto s: v)
    {
        if(!t.find(s))
        {
            cout << "FUCK: " << s << endl;
        }
    }
    printf("Time taken to find: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void t4(const std::vector<string>& v)
{
    cout << "string trie" << endl;
    trie_str t;
    clock_t tStart = clock();
    for(auto s: v)
    {
        t.insert(s);
    }
    printf("Time taken to insert: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    tStart = clock();
    t.freeze();
    printf("Time taken to freeze: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    tStart = clock();
    for(auto s: v)
    {
        if(!t.find(s))
        {
            cout << "FUCK: " << s << endl;
        }
    }
    // cout << t.find("blablabla") << endl;
    // cout << t.find("father") << endl;
    printf("Time taken to find: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

int main()
{
    std::vector<string> words = prepare();
    t4(words);
    t3(words);
}