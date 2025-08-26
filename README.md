## **Introduction**

This is a C++ implementation of a Compressed Binary Search Tree (CBST) data structure. A CBST is a type of binary search tree that stores multiple values in each node, allowing for efficient storage and retrieval of data.

## **Overview**

The CBSTree class is a template class that can store any type of data. It provides methods for inserting, deleting, and searching for values in the tree. The tree is self-balancing, meaning that it will automatically adjust its structure to maintain a balance between the height of the tree and the number of nodes.

## **Files**

* ```
  main.cpp
  ```

  : A test driver program that exercises the CBSTree class.
* ```
  cbstree.cpp
  ```

  : The implementation of the CBSTree class.
* ```
  cbstree.hpp
  ```

  : The header file for the CBSTree class.
* ```
  ctreenode.hpp
  ```

  : The header file for the CTreeNode class, which represents a single node in the CBST.

## **Usage**

To use the CBSTree class, simply include the

```
cbstree.hpp
```

 header file in your program and create an instance of the class. You can then use the various methods provided by the class to insert, delete, and search for values in the tree.



## **Compiling and Running**

To compile and run the program, use the following commands:

g++ -o cbstree main.cpp cbstree.cpp
./cbstree


# **Applications of CBSTree in this Project**

The CBSTree data structure is being used to manage a binary search tree of integers. The tree is implemented using a compressed binary search tree (CBST) data structure, which allows for efficient storage and retrieval of data.

Some possible applications of the CBSTree in this project include:

### 1. **Data Storage and Retrieval**

The CBSTree can be used to store and retrieve integers in an efficient manner. The tree can be used to store a large dataset, and the CBSTree's compressed structure allows for fast search, insertion, and deletion operations.

### 2. **Sorting and Searching**

The CBSTree can be used to sort and search a dataset of integers. The tree's compressed structure allows for fast search operations, making it suitable for applications where data needs to be quickly retrieved.


### **3. Data Compression**

The CBSTree can be used to compress a dataset of integers. The tree's compressed structure allows for efficient storage of data, making it suitable for applications where data needs to be stored in a compact form.
