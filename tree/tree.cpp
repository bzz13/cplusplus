#include <iostream>
#include <memory>

using namespace std;

template<typename TKey, typename TValue>
class bst
{
    struct node
    {
        node(const TKey& k, const TValue& v): key(k), value(v)
        {}

        TKey key;
        TValue value;
        unique_ptr<node> left;
        unique_ptr<node> right;
    };

    unique_ptr<node> root;

public:
    void insert(const TKey& key, const TValue& value)
    {
        if (!root)
        {
            root = make_unique<node>(key, value);
        }
        else
        {
            auto tmp = root.get();
            while(true)
            {
                if (tmp->key == key)
                {
                    tmp->value = value;
                    break;
                }
                if (tmp->key > key)
                {
                    if (tmp->left)
                    {
                        tmp = tmp->left.get();
                    }
                    else
                    {
                        tmp->left = make_unique<node>(key, value);
                        break;
                    }
                }
                else
                {
                    if (tmp->right)
                    {
                        tmp = tmp->right.get();
                    }
                    else
                    {
                        tmp->right = make_unique<node>(key, value);
                        break;
                    }
                }
            }
        }
    }
    bool find(const TKey& key, TValue& value) const
    {
        auto tmp = root.get();
        while(tmp)
        {
            if(tmp->key == key)
            {
                value = tmp->value;
                return true;
            }
            if (tmp->key > key)
            {
                tmp = tmp->left.get();
            }
            else
            {
                tmp = tmp->right.get();
            }
        }
        return false;
    }
};

void Test1()
{
    bst<int, int> tree;
    tree.insert(3, 5);
    tree.insert(1, 2);
    tree.insert(4, 6);
    tree.insert(2, 7);

    int result;
    for(int i = 0; i < 6; ++i)
    {
        if (tree.find(i, result))
        {
            cout << i << ": " << result << endl;
        }
        else
        {
            cout << i << " not exists" << endl;
        }
    }
}

void Test2()
{
    bst<int, int> tree;
    tree.insert(3, 5);
    tree.insert(1, 2);
    tree.insert(4, 6);
    tree.insert(2, 7);
    tree.insert(4, 20000);

    int result;
    for(int i = 0; i < 6; ++i)
    {
        if (tree.find(i, result))
        {
            cout << i << ": " << result << endl;
        }
        else
        {
            cout << i << " not exists" << endl;
        }
    }
}

int main()
{
    Test1();
    Test2();
    return 0;
}