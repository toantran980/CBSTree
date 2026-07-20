// ============================================================================
// This file contains the implementation of the CBSTree class. It uses the
// template parameter "NodeType" for the type of values that are stored in the
// tree.
// ============================================================================

#include    <fstream>
#include    <iostream>
#include    <cstdlib>
using namespace std;
#include    "cbstree.hpp"


// ==== CBSTree::CBSTree ======================================================
//
// This is the copy constructor for the CBSTree class, it just makes a call to
// the CopyTree member function and saves the return value in the root member
// of the calling object.
//
// Access: public
//
// Input:
//      other [IN]  -- a constant reference to a CBSTree object.
//
// ============================================================================

template    <typename  NodeType>
CBSTree<NodeType>::CBSTree(const CBSTree<NodeType>& other)
    : m_root(NULL) {
    // m_root must be initialized to NULL before CopyTree/InsertItem run,
    // since InsertItem checks "if (m_root == NULL)" to know whether this is
    // the first node being inserted.
    CopyTree(other.m_root);

}  // end of "CBSTree<NodeType>::CBSTree"



// ==== CBSTree::CopyTree =====================================================
//
// This recursive function creates a copy of a CBSTree. It receives a pointer
// to the source tree's root and uses a preorder traversal to make recursive
// calls and create a copy of the tree, and then returns a pointer to the root
// of the new copy.
//
// Access: private
//
// Input:
//      sourcePtr [IN]          -- a pointer to the source tree's root
//
//  Output:
//      A pointer to the root of the copied tree.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>* CBSTree<NodeType>::CopyTree(
    const CTreeNode<NodeType>* sourcePtr) {
    // Base case: nothing to copy.
    if (sourcePtr == NULL) {
        return NULL;
    }

    // Preorder: insert this node's value into the calling object's tree,
    // then recurse into the source's left and right subtrees.
    InsertItem(sourcePtr->m_value);
    CopyTree(sourcePtr->m_left);
    CopyTree(sourcePtr->m_right);

    return m_root;

}  // end of "CBSTree<NodeType>::CopyTree"



// ==== CBSTree::CountNodes ===================================================
//
// This recursive function derives the current height and number of nodes in
// the tree.  The height is a zero-based integer value, which represents the
// length of the longest path from the root to a leaf (counting the edges, not
// the nodes).  This function is called by public function CBSTree::GetTreeInfo
// so that the caller may determine the total number of nodes and the height of
// the tree.
//
// Access: protected
//
// Input:
//      nodePtr [IN]        -- a pointer to a tree node; initially this is the
//                             root
//
//      currDepth [IN]      -- the depth of the parent node (initial value is
//                             negative one, set by the caller)
//
//      numNodes [IN/OUT]   -- a reference to an int that will contain the
//                             total number of nodes in the tree (initial value
//                             is zero, set by the caller)
//
// Output:
//      The length of the longest path from the root.
//
// ============================================================================

template    <typename  NodeType>
int     CBSTree<NodeType>::CountNodes(const CTreeNode<NodeType>* nodePtr
    , int  currDepth
    , int& numNodes) const {
    if (nodePtr == NULL) {
        return 0;
    }

    numNodes++;

    if (nodePtr->m_left == NULL && nodePtr->m_right == NULL) {
        return 0;
    } else {
        currDepth++;

        int left = CountNodes(nodePtr->m_left, currDepth, numNodes);
        int right = CountNodes(nodePtr->m_right, currDepth, numNodes);

        if (left > right) {
            left++;
            return left;
        } else {
            right++;
            return right;
        }
    }

}  // end of "CBSTree::CountNodes"



// ==== CBSTree::Delete =======================================================
//
// This function deletes a target node from the tree.  The function finds the
// correct location for the target node by calling itself recursively. It then
// returns the address of the (potentially new) root of the tree.
//
// Access: protected
//
// Input:
//      target [IN]         -- a const reference to a NodeType item that
//                             contains the target search key value
//
//      nodePtr [IN]        -- a pointer to a tree node (initially this is
//                             the root).
//
//      bItemDeleted [OUT]  -- a reference to a bool that will indicate if the
//                             target item was actually removed from the tree;
//                             if that's the case it will have a value of true,
//                             else it will have a value of false
//
// Output:
//      A pointer to the (potentially new) root of the tree.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>* CBSTree<NodeType>::Delete(const NodeType& target
    , CTreeNode<NodeType>* nodePtr
    , bool& bItemDeleted) {

    CTreeNode<NodeType>* temp = NULL;
    CTreeNode<NodeType>* child = NULL;

    if (nodePtr == NULL) {
        bItemDeleted = false;
        return NULL;
    }

    if (target < nodePtr->m_value) {
        nodePtr->m_left = Delete(target, nodePtr->m_left, bItemDeleted);
    } else if (target > nodePtr->m_value) {
        nodePtr->m_right = Delete(target, nodePtr->m_right, bItemDeleted);
    } else {
        if (nodePtr->m_left != NULL && nodePtr->m_right != NULL) {
            // Two children: replace this node's value with its inorder
            // successor (the minimum value in the right subtree), then
            // delete that successor node from the right subtree instead.
            temp = FindMinNode(nodePtr->m_right);
            nodePtr->m_value = temp->m_value;
            nodePtr->m_right = Delete(nodePtr->m_value, nodePtr->m_right, bItemDeleted);
        } else {
            // Zero or one child: splice out this node and return whichever
            // child exists (or NULL if it's a leaf).
            child = (nodePtr->m_left != NULL) ? nodePtr->m_left : nodePtr->m_right;
            delete nodePtr;
            bItemDeleted = true;
            return child;
        }

        bItemDeleted = true;
    }

    return nodePtr;

}  // end of "CBSTree<NodeType>::Delete"

// ==== CBSTree::DeleteItem ===================================================
//
// This function allows the caller to delete a target node from the tree.
//
// Access: public
//
// Input:
//      target [IN]      -- a const reference to a NodeType object
//
// Output:
//      A value of false if the target item is not in the tree, otherwise a
//      value of true is returned and the item is removed from the tree.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::DeleteItem(const NodeType& target) {
    bool bItemDeleted = false;
    m_root = Delete(target, m_root, bItemDeleted);
    return bItemDeleted;

}  // end of "CBSTree<NodeType>::DeleteItem"



// ==== CBSTree::DestroyNodes =================================================
//
// This function performs a recursive postorder descent down the tree,
// releasing all allocated memory.
//
// Access: protected
//
// Input:
//      nodePtr [IN/OUT]    -- a pointer to a tree node (initially the root)
//
// Output:
//      Nothing.
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::DestroyNodes(CTreeNode<NodeType>* const nodePtr) {
    if (nodePtr == NULL) {
        return;
    }

    // Postorder: release children before releasing this node, so every
    // node in the tree -- leaf or internal -- gets deleted exactly once.
    DestroyNodes(nodePtr->m_left);
    DestroyNodes(nodePtr->m_right);
    delete nodePtr;

}  // end of "CBSTree<ItemType>::DestroyNodes"



// ==== CBSTree::FindMinNode ==================================================
//
// This function finds the inorder successor of the node pointed to by the
// parameter, and returns a pointer to the inorder successor node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a tree node
//
// Output:
//      A pointer to the target node.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>* CBSTree<NodeType>::FindMinNode(
    CTreeNode<NodeType>* nodePtr) const {
    while (nodePtr->m_left != NULL) {
        nodePtr = nodePtr->m_left;
    }
    return nodePtr;

}  // end of "CBSTree<NodeType>::FindMinNode"



// ==== CBSTree::GetTreeInfo ==================================================
//
// This function allows the caller to get the current number of nodes and the
// height of the tree by calling the CBSTree::CountNodes member function.
//
// Access: public
//
// Input:
//      numNodes [OUT]  -- a reference to an int that will contain the total
//                         number of nodes currently in the tree
//
//      height [OUT]    -- a reference to an int that will contain the height
//                         of the tree; this is a zero-based value that
//                         represents the longest path from the root to a leaf
//                         (counting edges, not the nodes)
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::GetTreeInfo(int& numNodes, int& height) const {
    numNodes = 0;
    height = CountNodes(m_root, -1, numNodes);

}  // end of "CBSTree::GetTreeInfo"



// ==== CBSTree::InOrder ======================================================
//
// This function performs an in-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a tree node (this is a recursive
//                         function, initially this points to the root).
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a
//                         const reference to a NodeType object as input, and
//                         returns nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::InOrder(const CTreeNode<NodeType>* const nodePtr
    , void (*fPtr)(const NodeType&)) const {

    if (nodePtr == NULL) {
        return;
    }

    InOrder(nodePtr->m_left, fPtr);
    (*fPtr)(nodePtr->m_value);
    InOrder(nodePtr->m_right, fPtr);

}  // end of "CBSTree<NodeType>::InOrder"



// ==== CBSTree::InOrderTraverse ==============================================
//
// This function allows the caller to execute an in-order traversal through the
// tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const
//                     reference to a NodeType object as input, and returns
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::InOrderTraverse(void  (*fPtr)(const NodeType&)) const {
    InOrder(m_root, fPtr);

}  // end of "CBSTree<NodeType>::InOrderTraverse"



// ==== CBSTree::Insert =======================================================
//
// This function inserts a new node into the tree.  It finds the correct
// location for the new node by calling itself recursively. If the new item
// is unique, a copy is created and inserted into the tree. Then the address of
// the (potentially new) root of the tree is returned.
//
// If the root data member of this class is NULL upon entry, it is updated
// with the value of the nodePtr parameter.
//
// Access: protected
//
// Input:
//      newItem [IN]    -- a const reference to a NodeType object; it is
//                         assumed that the object is initialized and ready to
//                         be inserted
//
//      nodePtr [IN]    -- a pointer to a tree node (initially this is usually
//                         the root)
//
// Output:
//      A pointer to the (potentially new) root of the tree
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>* CBSTree<NodeType>::Insert(const NodeType& newItem
    , CTreeNode<NodeType>* nodePtr) {

    if (ItemInTree(newItem)) {
        return NULL;
    }
    if (nodePtr == NULL) {
        nodePtr = new CTreeNode<NodeType>;
        nodePtr->m_value = newItem;
        nodePtr->m_left = nullptr;
        nodePtr->m_right = nullptr;


        if (m_root == NULL) {
            m_root = nodePtr;
        }
    } else {
        if (newItem < nodePtr->m_value) {
            nodePtr->m_left = Insert(newItem, nodePtr->m_left);
        } else if (newItem > nodePtr->m_value) {
            nodePtr->m_right = Insert(newItem, nodePtr->m_right);
        }
    }

    return nodePtr;

}  // end of "CBSTree<NodeType>::Insert"



// ==== CBSTree::InsertItem ===================================================
//
// This function allows the caller to insert a new node into the tree.  The
// input parameter is a const reference to the item to insert.
//
// Access: public
//
// Input:
//      newItem [IN]    -- a const reference a NodeType object
//
// Output:
//      A value of true if the item was successfully inserted into the tree,
//      false otherwise.
//
// ============================================================================
template    <typename  NodeType>
bool    CBSTree<NodeType>::InsertItem(const NodeType& newItem) {
    if (Insert(newItem, m_root) == NULL) {
        return false;
    } else {
        return true;
    }

}  // end of "CBSTree<NodeType>::InsertItem"



// ==== CBSTree::ItemInTree ===================================================
//
// This function allows the caller to determine if a target item is in the
// tree. The input parameter is a const reference to the target tree node
// value, and this function calls CBSTree::Retrieve to determine if it's in the
// tree or not.
//
// Access: public
//
// Input:
//      target [IN]     -- a const reference to a NodeType object that contains
//                         the target key value to search for
//
// Output:
//      A value of true if the target item is found, false if not.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::ItemInTree(const NodeType& target) const {
    if (NULL == Retrieve(target, m_root)) {
        return false;
    } else {
        return true;
    }
}  // end of "CBSTree<NodeType>::ItemInTree"



// ==== CBSTree::PostOrder ====================================================
//
// This function performs a post-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a CTreeNode (this is a recursive
//                         function, initially this points to the root)
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a
//                         const reference to a NodeType object as input and
//                         returns nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PostOrder(const CTreeNode<NodeType>* const nodePtr
    , void (*fPtr)(const NodeType&)) const {

    if (nodePtr == NULL) {
        return;
    }

    PostOrder(nodePtr->m_left, fPtr);
    PostOrder(nodePtr->m_right, fPtr);
    (*fPtr)(nodePtr->m_value);

}  // end of "CBSTree<NodeType>::PostOrder"

// ==== CBSTree::PostOrderTraverse ============================================
//
// This function allows the caller to execute a post-order traversal through
// the tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const
//                     reference to a NodeType object as input, and returns
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PostOrderTraverse(void  (*fPtr)(const NodeType&)) const {
    PostOrder(m_root, fPtr);

}  // end of "CBSTree<NodeType>::PostOrderTraverse"



// ==== CBSTree::PreOrder =====================================================
//
// This function performs a pre-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a CTreeNode (this is a recursive
//                         function, initially this points to the root)
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a
//                         const reference NodeType object as input, and returns
//                         nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PreOrder(const CTreeNode<NodeType>* const nodePtr
    , void  (*fPtr)(const NodeType&)) const {
    if (nodePtr == NULL) {
        return;
    }

    (*fPtr)(nodePtr->m_value);
    PreOrder(nodePtr->m_left, fPtr);
    PreOrder(nodePtr->m_right, fPtr);

}  // end of "CBSTree<NodeType>::PreOrder"



// ==== CBSTree::PreOrderTraverse =============================================
//
// This function allows the caller to execute a pre-order traversal through the
// tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const
//                     reference to a NodeType object as input, and returns
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PreOrderTraverse(void (*fPtr)(const NodeType&)) const {
    PreOrder(m_root, fPtr);

}  // end of "CBSTree<NodeType>::PreOrderTraverse"

// ==== CBSTree::RebalanceTree ================================================
//
// This function rebalances the tree to an optimal height. It first derives the
// number of nodes in the tree, then allocates a dynamic array to store all of
// the tree's values. After that it saves the tree's values to the dynamic
// array by calling CBSTree::SaveToArray, then releases all of the nodes in the
// tree. Next, it calls the CBSTree::Repopulate function to repopulate the tree
// with the array before releasing the array back to the heap.
//
// Access: public
//
// Input:
//      Nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void        CBSTree<NodeType>::RebalanceTree() {
    // numNodes is a count, not a tree value, so it must be an int (not
    // NodeType) regardless of what type the tree stores.
    int numNodes = 0;
    CountNodes(m_root, -1, numNodes);

    NodeType* array = new NodeType[numNodes];

    int index = 0;
    SaveToArray(m_root, array, index);

    DestroyTree();
    Repopulate(array, 0, numNodes - 1);

    delete[] array;

}  // end of "CBSTree<NodeType>::RebalanceTree"



// ==== CBSTree::Repopulate ===================================================
//
// This function uses the contents of a sorted array to repopulate the tree.
// The array is processed using 'divide and conquer' recursion so that the
// middle element of the array is inserted into the tree, resulting in a
// balanced binary tree.
//
// Access: protected
//
// Input:
//      array [IN]      -- the base address of the array; it is assumed that
//                         the values in the array are in sorted ascending order
//
//      first [IN]      -- an index to the first element
//
//      last [IN]       -- an index to the last element
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void        CBSTree<NodeType>::Repopulate(const NodeType array[], int first
    , int last) {

    if (first > last) {
        return;
    }

    int mid = (first + last) / 2;
    Insert(array[mid], m_root);
    Repopulate(array, first, mid - 1);
    Repopulate(array, mid + 1, last);

}  // end of "CBSTree<NodeType>::Repopulate"



// ==== CBSTree::Retrieve =====================================================
//
// This function finds the node in the tree whose value equals that of the
// tree node reference parameter. The target node is located through recursive
// function calls. If the node does not exist in the tree, a value of NULL is
// returned.
//
// Access: public
//
// Input:
//      target [IN]     -- a reference to a NodeType object; it is assumed that
//                         the object is fully initialized so a search can be
//                         performed
//
//      nodePtr [IN]    -- a pointer to a tree node (initially this is usually
//                         the root)
//
// Output:
//      If a tree node is found that has the same value as the target
//      parameter, then a pointer to the node in the tree is returned.
//      Otherwise, a value of NULL is returned.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>* CBSTree<NodeType>::Retrieve(const NodeType& target
    , CTreeNode<NodeType>* nodePtr) const {

    if (nodePtr == NULL) {
        return NULL;
    }

    if (target < nodePtr->m_value) {
        nodePtr = Retrieve(target, nodePtr->m_left);
    } else if (target > nodePtr->m_value) {
        nodePtr = Retrieve(target, nodePtr->m_right);
    }

    return nodePtr;

}  // end of "CBSTree<NodeType>::Retrieve"



// ==== CBSTree::SaveToArray ==================================================
//
// This function performs an inorder traversal of the tree making recursive
// calls so that the values in the nodes can be written to the caller's array
// in sorted ascending order.
//
// Access: protected
//
// Input:
//      nodePtr[IN]     -- a ptr to a tree node, initially the root
//
//      array [IN]      -- the base address of the caller's array
//
//      index [IN/OUT]  -- a reference to the an int to be used for the index
//                         into the array; as node values are copied into the
//                         array, this index is incremented so the other
//                         recursive stack frames immediately have the updated
//                         index value to access the next element in the sorted
//                         array
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::SaveToArray(const CTreeNode<NodeType>* const nodePtr
    , NodeType array[]
    , int& index) {

    if (nodePtr != NULL) {
        SaveToArray(nodePtr->m_left, array, index);
        array[index] = nodePtr->m_value;
        index++;
        SaveToArray(nodePtr->m_right, array, index);
    }

}  // end of "CBSTree<NodeType>::SaveToArray"



// ==== CBSTree::operator= ====================================================
//
// This is the overloaded assignment operator for the CBSTree class. It first
// checks for assignment to self, then releases all of the nodes in the calling
// object. Then it uses the parameter as an argument to the CopyTree member
// function so that the parameter's tree is replicated in the calling object.
//
// Access: public
//
// Input:
//      rhs [IN]    -- a const reference to an existing CBSTree object
//
// Output:
//      A reference to the calling object.
//
// ============================================================================

template    <typename  NodeType>
CBSTree<NodeType>& CBSTree<NodeType>::operator=(const CBSTree<NodeType>& rhs) {
    
    if (this != &rhs) {
        DestroyTree();
        m_root = NULL;
        CopyTree(rhs.m_root);
    }
    return *this;

}  // end of "CBSTree<NodeType>::operator="



// ==== CBSTree::PrintNodes ====================================================
//
//  Recursively prints each node's value along with its left and right
//  children's values (or "NULL" if a child doesn't exist). Traverses the
//  tree in pre-order (node, then left subtree, then right subtree).
//
// Access: protected
//
// Input:
//      nodePtr [IN] -- pointer to the tree node to print, initially the root.
//                      Recursion terminates when nodePtr is nullptr.
//
// Output:
//      Nothing (writes directly to cout)
//
// ==============================================================================

template <typename NodeType>
void CBSTree<NodeType>::PrintNodes(const CTreeNode<NodeType>* nodePtr) const {
    if (nodePtr == nullptr)
        return;

    auto printChild = [](const char* label, const CTreeNode<NodeType>* child) {
        cout << label;
        if (child)
            cout << child->m_value;
        else
            cout << "NULL";
        cout << "\n";
    };

    cout << "Value: " << nodePtr->m_value << "\n";
    printChild("Left Node: ", nodePtr->m_left);
    printChild("Right Node: ", nodePtr->m_right);

    PrintNodes(nodePtr->m_left);
    PrintNodes(nodePtr->m_right);
}
