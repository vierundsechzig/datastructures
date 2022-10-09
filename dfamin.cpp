#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <list>
#include <set>
#include <map>

using namespace std;

int a, na; // number of accept states and nonaccept states
map<int, string> state_name;
map<string, int> state_name_rev;
set<int> accept;
set<int> nonaccept;
set<char> alphabet;
map<pair<int, char>, int> transition; // state transition function
list<set<int> > w;
list<set<int> > pt; // partition is the final partition
map<int, int> repre; // representatives of each equivalence class in the final partition
map<pair<int, char>, int> final_transition; // simplified tate transition function

int main()
{
    string input, input2;
    char sgm;
    cout << "Number of Nonaccept states: ";
    cin >> na;
    cout << "Nonaccept: ";
    for (int i=0; i<na; ++i)
    {
        cin >> input;
        nonaccept.insert(i);
        state_name[i] = input;
        state_name_rev[input] = i;
    }
    cout << "Number of Accept states: ";
    cin >> a;
    cout << "Accept: ";
    for (int i=0; i<a; ++i)
    {
        cin >> input;
        accept.insert(na + i);
        state_name[na + i] = input;
        state_name_rev[input] = na + i;
    }
    cout << "State transitions (press Ctrl+D to end input):" << endl;
    while (cin >> input >> sgm >> input2)
    {
        transition[make_pair(state_name_rev[input], sgm)] = state_name_rev[input2];
        alphabet.insert(sgm);
    }
    // initialization
    w.push_back(nonaccept);
    w.push_back(accept);
    pt.push_back(nonaccept);
    pt.push_back(accept);
    while (w.size() > 0)
    {
        // select and remove a set from w
        auto s = w.front();
        w.pop_front();
        for (char ch : alphabet)
        {
            // find all states that convert to s on input ch
            set<int> x;
            for (auto& pr : transition)
                if (pr.first.second == ch && s.find(pr.second) != s.end())
                    x.insert(pr.first.first);
            auto itr=pt.begin();
            while (itr != pt.end())
            {
                set<int> y = *itr;
                set<int> itsc, diff;
                set_intersection(x.begin(), x.end(), y.begin(), y.end(), std::inserter(itsc, itsc.begin()));
                set_difference(y.begin(), y.end(), x.begin(), x.end(), std::inserter(diff, diff.begin()));
                // X intersect Y is not empty and Y - X is not empty
                if (!itsc.empty() && !diff.empty())
                {
                    pt.push_back(itsc);
                    pt.push_back(diff);
                    // figure out whether y is in w
                    bool belongto = false;
                    auto w_itr = w.begin();
                    for (; w_itr != w.end(); ++w_itr)
                        if (includes(w_itr->begin(), w_itr->end(), y.begin(), y.end()) && includes(y.begin(), y.end(), w_itr->begin(), w_itr->end()))
                        {
                            belongto = true;
                            break;
                        }
                    if (belongto)
                    {
                        w.push_back(itsc);
                        w.push_back(diff);
                        w.erase(w_itr);
                    }
                    else if (itsc.size() <= diff.size())
                        w.push_back(itsc);
                    else
                        w.push_back(diff);
                    itr = pt.erase(itr); // delete y
                }
                else
                    ++itr;
            }
        }
    }
    // pick a representative for each equivalence class
    for (auto& sg : pt)
    {
        int representative = *sg.begin();
        for (auto s : sg)
            repre[s] = representative;
    }
    // generetate the simplified state transition function
    for (auto& f : transition)
        final_transition[make_pair(repre[f.first.first], f.first.second)] = repre[f.second];
    for (auto& f : final_transition)
        cout << "d(" << state_name[f.first.first] << ", " << f.first.second << ") = " << state_name[f.second] << endl;
    return 0;
}
