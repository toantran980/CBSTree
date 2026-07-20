## Introduction

This is a C++ implementation of a **Compressed Binary Search Tree (CBST)** data structure. A CBST is a type of binary search tree that stores values in nodes while supporting efficient storage and retrieval.

## Overview

`CBSTree` is a **template class** that can store any type of data. It provides methods for:
- inserting values
- deleting values
- searching for values
- traversing the tree (preorder / inorder / postorder)
- rebalancing the tree

The project includes a console driver (`main.cpp`) that exercises the tree with random or sequential integers and prints tree contents and statistics.

## Files

- `main.cpp`
  - Test driver program that exercises `CBSTree<int>`.
- `cbstree.hpp`
  - Header file for the `CBSTree` class.
- `cbstree.cpp`
  - (Legacy) implementation of the `CBSTree` template.
- `cbstree_V2.cpp`
  - Current/alternate implementation (unique_ptr-based) intended for template usage in this repo.
- `ctreenode.hpp`
  - Defines `CTreeNode`, the node type used by `CBSTree`.

## Usage

To use `CBSTree`, include `cbstree.hpp` in your program and create an instance:

```cpp
#include "cbstree.hpp"

CBSTree<int> tree;
```

Then you can call:
- `tree.InsertItem(value)`
- `tree.DeleteItem(value)`
- `tree.ItemInTree(value)`
- traversals: `tree.PreOrderTraverse(...)`, `tree.InOrderTraverse(...)`, `tree.PostOrderTraverse(...)`
- `tree.RebalanceTree()`

## Compiling and Running

From the `CBSTree/CBSTree` folder:

### Build

```bat
g++ -std=c++17 main.cpp -o cbstree_demo.exe
```

### Run

```bat
cbstree_demo.exe
```

## Applications of CBSTree in this Project

1. **Data Storage and Retrieval**
   - Store and retrieve integers efficiently using BST operations.

2. **Sorting and Searching**
   - Inorder traversal returns values in sorted order, useful for verifying BST properties.

3. **Balancing / Maintaining Performance**
   - `RebalanceTree()` rebuilds the tree to improve height (especially after inserting ordered data).

