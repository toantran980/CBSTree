// ============================================================================
// CBSTree V2 - declarations only
// Template declarations are provided here.
// Definitions are intentionally placed in cbstree_V2.cpp (explicit instantiation
// is required for each NodeType you want to use).
// ============================================================================

#ifndef CBSTREE_V2_HPP
#define CBSTREE_V2_HPP

#include <memory>
#include <vector>
#include <cstddef>

#include "ctreenodeV2.hpp"

template <typename NodeType>
class CBSTreeV2 {
public:
    CBSTreeV2();
    CBSTreeV2(const CBSTreeV2& other);
    CBSTreeV2& operator=(const CBSTreeV2& other);

    CBSTreeV2(CBSTreeV2&&) noexcept;
    CBSTreeV2& operator=(CBSTreeV2&&) noexcept;

    ~CBSTreeV2();

    bool IsTreeEmpty() const;

    bool InsertItem(const NodeType& newItem);
    bool ItemInTree(const NodeType& target) const;
    bool DeleteItem(const NodeType& target);

    void DestroyTree();
    void GetTreeInfo(int& numNodes, int& height) const;

    void RebalanceTree();

    void InOrderTraverse(void (*fPtr)(const NodeType&)) const;
    void PreOrderTraverse(void (*fPtr)(const NodeType&)) const;
    void PostOrderTraverse(void (*fPtr)(const NodeType&)) const;

    void DumpTree() const;

private:
    using NodePtr = std::unique_ptr<CTreeNode<NodeType>>;
    NodePtr m_root;

    // helpers
    static NodePtr Clone(const CTreeNode<NodeType>* src);
    static int CountNodes(const CTreeNode<NodeType>* nodePtr, int currDepth, int& numNodes);

    static bool Insert(NodePtr& nodeRef, const NodeType& newItem);
    static const CTreeNode<NodeType>* Retrieve(const NodeType& target, const CTreeNode<NodeType>* nodePtr);
    static NodePtr Delete(const NodeType& target, NodePtr nodeRef, bool& deleted);

    static void SaveToVector(const CTreeNode<NodeType>* nodePtr, std::vector<NodeType>& out);
    static NodePtr BuildFromSorted(const std::vector<NodeType>& arr, int first, int last);

    static void InOrder(const CTreeNode<NodeType>* nodePtr, void (*fPtr)(const NodeType&));
    static void PreOrder(const CTreeNode<NodeType>* nodePtr, void (*fPtr)(const NodeType&));
    static void PostOrder(const CTreeNode<NodeType>* nodePtr, void (*fPtr)(const NodeType&));

    static void PrintNodes(const CTreeNode<NodeType>* nodePtr);
};

#endif // CBSTREE_V2_HPP