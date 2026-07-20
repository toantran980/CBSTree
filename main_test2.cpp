#include <cassert>
#include <iostream>
#include "cbstreeV2.hpp"


static int g_sum = 0;
static void acc(const int& v){ g_sum += v; }

int main(){
CBSTreeV2<int> t;
    for(int i=1;i<=10;i++) assert(t.InsertItem(i));
    assert(!t.InsertItem(5));

    int n=0,h=0;
    t.GetTreeInfo(n,h);
    assert(n==10);

    // rebalance shouldn't drop nodes
    t.RebalanceTree();
    t.GetTreeInfo(n,h);
    assert(n==10);

    // traversals shouldn't crash
    t.InOrderTraverse(acc);
    assert(g_sum>0);

    std::cout<<"Basic traversal+rebalance tests passed.\n";
    return 0;
}

