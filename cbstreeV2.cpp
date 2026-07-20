// ============================================================================
// CBSTree V2 implementation.
// Rewritten to lean fully on std::unique_ptr ownership semantics:
//  - Insert takes a NodePtr& and grows the tree in place.
//  - Delete takes ownership of a subtree (NodePtr by value) and returns the
//    (possibly different) NodePtr that should replace it -- no raw new/delete,
//    no get()/release()/reset() juggling.
//  - RebalanceTree collects values into a std::vector and rebuilds directly
//    into unique_ptrs rather than re-inserting one at a time.
// ============================================================================

#include <iostream>
#include <utility>
#include "cbstreeV2.hpp"

using namespace std;

// Ctors / Dtor / Assignment 
template <typename NodeType>
CBSTreeV2<NodeType>::CBSTreeV2() : m_root(nullptr) {}

template <typename NodeType>
CBSTreeV2<NodeType>::CBSTreeV2(const CBSTreeV2& other)
    : m_root(other.m_root ? Clone(other.m_root.get()) : nullptr) {}

template <typename NodeType>
CBSTreeV2<NodeType>& CBSTreeV2<NodeType>::operator=(const CBSTreeV2& other) {
    if (this == &other) return *this;
    m_root = other.m_root ? Clone(other.m_root.get()) : nullptr;
    return *this;
}

template <typename NodeType>
CBSTreeV2<NodeType>::CBSTreeV2(CBSTreeV2&& other) noexcept
    : m_root(std::move(other.m_root)) {}

template <typename NodeType>
CBSTreeV2<NodeType>& CBSTreeV2<NodeType>::operator=(CBSTreeV2&& other) noexcept {
    if (this != &other) {
        m_root = std::move(other.m_root);
    }
    return *this;
}

template <typename NodeType>
CBSTreeV2<NodeType>::~CBSTreeV2() = default;  // unique_ptr chain unwinds recursively

// Clone helper

template <typename NodeType>
typename CBSTreeV2<NodeType>::NodePtr
CBSTreeV2<NodeType>::Clone(const CTreeNode<NodeType>* src) {
    if (!src) return nullptr;
    return src->clone();
}

// IsTreeEmpty / DestroyTree

template <typename NodeType>
bool CBSTreeV2<NodeType>::IsTreeEmpty() const {
    return m_root == nullptr;
}

template <typename NodeType>
void CBSTreeV2<NodeType>::DestroyTree() {
    m_root.reset();
}

// Insert 

template <typename NodeType>
bool CBSTreeV2<NodeType>::Insert(NodePtr& nodeRef, const NodeType& newItem) {
    if (!nodeRef) {
        nodeRef = std::make_unique<CTreeNode<NodeType>>(newItem);
        return true;
    }
    if (newItem < nodeRef->m_value) {
        return Insert(nodeRef->m_left, newItem);
    }
    if (newItem > nodeRef->m_value) {
        return Insert(nodeRef->m_right, newItem);
    }
    return false;  // duplicate key
}

template <typename NodeType>
bool CBSTreeV2<NodeType>::InsertItem(const NodeType& newItem) {
    return Insert(m_root, newItem);
}

// Retrieve / ItemInTree

template <typename NodeType>
const CTreeNode<NodeType>* CBSTreeV2<NodeType>::Retrieve(
    const NodeType& target, const CTreeNode<NodeType>* nodePtr) {

    if (nodePtr == nullptr) return nullptr;
    if (target < nodePtr->m_value) return Retrieve(target, nodePtr->m_left.get());
    if (target > nodePtr->m_value) return Retrieve(target, nodePtr->m_right.get());
    return nodePtr;
}

template <typename NodeType>
bool CBSTreeV2<NodeType>::ItemInTree(const NodeType& target) const {
    return Retrieve(target, m_root.get()) != nullptr;
}

// Delete 
// Takes ownership of the subtree, returns the NodePtr that should replace it.
// No manual new/delete: the local `nodeRef` owns the node until it is either
// returned (ownership passed on) or falls out of scope (destroyed).

template <typename NodeType>
typename CBSTreeV2<NodeType>::NodePtr CBSTreeV2<NodeType>::Delete(
    const NodeType& target, NodePtr nodeRef, bool& deleted) {

    if (!nodeRef) {
        deleted = false;
        return nullptr;
    }

    if (target < nodeRef->m_value) {
        nodeRef->m_left = Delete(target, std::move(nodeRef->m_left), deleted);
        return nodeRef;
    }
    if (target > nodeRef->m_value) {
        nodeRef->m_right = Delete(target, std::move(nodeRef->m_right), deleted);
        return nodeRef;
    }

    // Found the target node.
    deleted = true;

    // 0 children: returning nullptr drops the last reference to nodeRef,
    // which destroys it automatically when this frame unwinds.
    if (!nodeRef->m_left && !nodeRef->m_right) {
        return nullptr;
    }

    // 1 child: hand ownership of the surviving subtree up to the caller.
    if (!nodeRef->m_left) {
        return std::move(nodeRef->m_right);
    }
    if (!nodeRef->m_right) {
        return std::move(nodeRef->m_left);
    }

    // 2 children: copy in-order successor's value, then delete it from
    // the right subtree.
    const CTreeNode<NodeType>* succ = nodeRef->m_right.get();
    while (succ->m_left) succ = succ->m_left.get();
    nodeRef->m_value = succ->m_value;

    bool dummy = false;
    nodeRef->m_right = Delete(succ->m_value, std::move(nodeRef->m_right), dummy);
    return nodeRef;
}

template <typename NodeType>
bool CBSTreeV2<NodeType>::DeleteItem(const NodeType& target) {
    bool deleted = false;
    m_root = Delete(target, std::move(m_root), deleted);
    return deleted;
}

// CountNodes / GetTreeInfo

template <typename NodeType>
int CBSTreeV2<NodeType>::CountNodes(const CTreeNode<NodeType>* nodePtr,
                                     int currDepth,
                                     int& numNodes) {
    if (nodePtr == nullptr) return 0;

    numNodes++;

    if (nodePtr->m_left == nullptr && nodePtr->m_right == nullptr) {
        return 0;
    }

    currDepth++;
    int left = CountNodes(nodePtr->m_left.get(), currDepth, numNodes);
    int right = CountNodes(nodePtr->m_right.get(), currDepth, numNodes);

    if (left > right) {
        return left + 1;
    }
    return right + 1;
}

template <typename NodeType>
void CBSTreeV2<NodeType>::GetTreeInfo(int& numNodes, int& height) const {
    numNodes = 0;
    height = CountNodes(m_root.get(), -1, numNodes);
}

// RebalanceTree 

template <typename NodeType>
void CBSTreeV2<NodeType>::SaveToVector(const CTreeNode<NodeType>* nodePtr,
                                        std::vector<NodeType>& out) {
    if (nodePtr == nullptr) return;
    SaveToVector(nodePtr->m_left.get(), out);
    out.push_back(nodePtr->m_value);
    SaveToVector(nodePtr->m_right.get(), out);
}

template <typename NodeType>
typename CBSTreeV2<NodeType>::NodePtr CBSTreeV2<NodeType>::BuildFromSorted(
    const std::vector<NodeType>& arr, int first, int last) {

    if (first > last) return nullptr;
    int mid = first + (last - first) / 2;

    auto node = std::make_unique<CTreeNode<NodeType>>(arr[mid]);
    node->m_left = BuildFromSorted(arr, first, mid - 1);
    node->m_right = BuildFromSorted(arr, mid + 1, last);
    return node;
}

template <typename NodeType>
void CBSTreeV2<NodeType>::RebalanceTree() {
    std::vector<NodeType> values;
    SaveToVector(m_root.get(), values);

    if (values.size() <= 1) return;

    m_root = BuildFromSorted(values, 0, static_cast<int>(values.size()) - 1);
}

// Traversals

template <typename NodeType>
void CBSTreeV2<NodeType>::InOrder(const CTreeNode<NodeType>* nodePtr,
                                   void (*fPtr)(const NodeType&)) {
    if (nodePtr == nullptr) return;
    InOrder(nodePtr->m_left.get(), fPtr);
    fPtr(nodePtr->m_value);
    InOrder(nodePtr->m_right.get(), fPtr);
}

template <typename NodeType>
void CBSTreeV2<NodeType>::PreOrder(const CTreeNode<NodeType>* nodePtr,
                                    void (*fPtr)(const NodeType&)) {
    if (nodePtr == nullptr) return;
    fPtr(nodePtr->m_value);
    PreOrder(nodePtr->m_left.get(), fPtr);
    PreOrder(nodePtr->m_right.get(), fPtr);
}

template <typename NodeType>
void CBSTreeV2<NodeType>::PostOrder(const CTreeNode<NodeType>* nodePtr,
                                     void (*fPtr)(const NodeType&)) {
    if (nodePtr == nullptr) return;
    PostOrder(nodePtr->m_left.get(), fPtr);
    PostOrder(nodePtr->m_right.get(), fPtr);
    fPtr(nodePtr->m_value);
}

template <typename NodeType>
void CBSTreeV2<NodeType>::InOrderTraverse(void (*fPtr)(const NodeType&)) const {
    InOrder(m_root.get(), fPtr);
}

template <typename NodeType>
void CBSTreeV2<NodeType>::PreOrderTraverse(void (*fPtr)(const NodeType&)) const {
    PreOrder(m_root.get(), fPtr);
}

template <typename NodeType>
void CBSTreeV2<NodeType>::PostOrderTraverse(void (*fPtr)(const NodeType&)) const {
    PostOrder(m_root.get(), fPtr);
}

// DumpTree / PrintNodes

template <typename NodeType>
void CBSTreeV2<NodeType>::PrintNodes(const CTreeNode<NodeType>* nodePtr) {
    if (nodePtr == nullptr) return;

    cout << "Value: " << nodePtr->m_value << "\n";

    if (nodePtr->m_left) {
        cout << "Left Node: " << nodePtr->m_left->m_value << "\n";
    } else {
        cout << "Left Node: NULL\n";
    }

    if (nodePtr->m_right) {
        cout << "Right Node: " << nodePtr->m_right->m_value << "\n";
    } else {
        cout << "Right Node: NULL\n";
    }

    PrintNodes(nodePtr->m_left.get());
    PrintNodes(nodePtr->m_right.get());
}

template <typename NodeType>
void CBSTreeV2<NodeType>::DumpTree() const {
    PrintNodes(m_root.get());
}

// ============================================================================
// Explicit instantiations
// Add one line per NodeType you actually use, e.g.:
//
//   template class CBSTreeV2<int>;
//   template class CBSTreeV2<std::string>;
//   template class CBSTreeV2<double>;
//
// ============================================================================

// template class CBSTreeV2<int>;