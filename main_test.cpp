#include <cassert>
#include <vector>
#include <iostream>
#include "cbstreeV2.hpp"


static void noopPrint(const int&) {}

int main() {
CBSTreeV2<int> t;

    std::cout << "[1] Insert unique keys 1..10" << std::endl;
    for (int i = 1; i <= 10; i++) {
        bool ok = t.InsertItem(i);
        std::cout << "  InsertItem(" << i << ")=" << ok << std::endl;
        assert(ok);
    }

    std::cout << "[2] Duplicate insert 5 should fail" << std::endl;
    assert(t.InsertItem(5) == false);

    std::cout << "[3] Search checks" << std::endl;
    assert(t.ItemInTree(1) == true);
    assert(t.ItemInTree(11) == false);

    std::cout << "[4] Delete leaf 10" << std::endl;
    assert(t.DeleteItem(10) == true);
    assert(t.ItemInTree(10) == false);

    std::cout << "[5] Delete nodes 1 and 2" << std::endl;
    assert(t.DeleteItem(1) == true);
    assert(t.DeleteItem(2) == true);

    std::cout << "[6] GetTreeInfo" << std::endl;
    int numNodes = 0;
    int height = 0;
    t.GetTreeInfo(numNodes, height);
    std::cout << "  numNodes=" << numNodes << " height=" << height << std::endl;
    assert(numNodes > 0);

    std::cout << "[7] RebalanceTree" << std::endl;
    t.RebalanceTree();

    std::cout << "[8] Validate remaining keys 3..9 exist" << std::endl;
    for (int i = 3; i <= 9; i++) {
        bool inTree = t.ItemInTree(i);
        std::cout << "  ItemInTree(" << i << ")=" << inTree << std::endl;
        assert(inTree == true);
    }
    assert(t.ItemInTree(4) == true);
    assert(t.ItemInTree(5) == true);
    assert(t.ItemInTree(6) == true);

    std::cout << "[9] Traversals (should not crash)" << std::endl;
    t.InOrderTraverse(noopPrint);
    t.PreOrderTraverse(noopPrint);
    t.PostOrderTraverse(noopPrint);

    std::cout << "All CBSTree tests passed.\n";
    return 0;
}

