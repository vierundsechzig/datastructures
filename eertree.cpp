#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

struct node
{
    int n[26]; // next node (state transition function)
    int l; // length of the corresponding palindrome
    int s_ptr; // points to its longest suffix palindromic string
    int num; // number of occurrences of subpalindromes in the corresponding string
};

int strl;
string input;
vector<node> tree;
int num; // total number of nodes
int suff; // maximum suffix palindrome

int main()
{
    getline(cin, input);
    int strl = input.length();
    int symbol; // characters in terms of integers 0 - 25
    int count = 0;

    // initialize node arrays
    tree.push_back({.n = {0}, .l = -1, .s_ptr = 0, .num = 0}); // node 0: palindrome of length -1
    tree.push_back({.n = {0}, .l = 0, .s_ptr = 0, .num = 0}); // node 1: palindrome of length 0
    num = 2;
    suff = 1; // the initial maximum suffix palindrome is the empty string (node 1)

    for (int i = 0; i < strl; ++i)
    {
        int cur = suff;
        int curlen = 0;
        symbol = input[i] - 'a';
        curlen = tree[cur].l;
        // find the maximum suffix palindrome of p + ch
        while (i - curlen - 1 < 0 || input[i - curlen - 1] != input[i])
        {
            cur = tree[cur].s_ptr;
            curlen = tree[cur].l;
        }
        // node of palindrome ch + p + ch already exists; simply update the maximum suffix palindrome
        if (tree[cur].n[symbol])
            suff = tree[cur].n[symbol];
        // the node does not exist; need to insert the new node
        else
        {
            ++num;
            suff = num - 1; // the new node is the current maximum suffix palindrome node
            tree.push_back({.n = {0}, .l = tree[cur].l + 2, .s_ptr = 0, .num = 0}); // insert the new node and set its string length
            tree[cur].n[symbol] = num - 1; // transition from current node with input symbol is the new node
            // the end character is a symbol that never occurs before
            if (tree[num-1].l == 1)
            {
                tree[num-1].s_ptr = 1;
                tree[num-1].num = 1;
            }
            // find the maximum suffix palindrome for the new node
            else
            {
                do
                {
                    cur = tree[cur].s_ptr;
                    curlen = tree[cur].l;
                } while (i - curlen - 1 < 0 || input[i - curlen - 1] != input[i]);
                tree[num-1].s_ptr = tree[cur].n[symbol];
                tree[num-1].num = 1 + tree[tree[num-1].s_ptr].num;
            }
        }
        count += tree[suff].num;
    }
    cout << "Number of distinct subpalindromes: " << num - 2 << endl;
    cout << "Number of subpalindromes: " << count << endl;

    return 0;
}