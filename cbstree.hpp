// ============================================================================
// This header file contains the declaration of the CBSTree class. It uses the
// template parameter "NodeType" for the type of values that are stored in the
// tree.
// ============================================================================

#ifndef CBIN_SEARCH_TREE_HEADER
#define CBIN_SEARCH_TREE_HEADER

#include    "ctreenode.hpp"

// class declaration
template    <typename  NodeType>
class   CBSTree
{
public:
    // constructors and destructor
    CBSTree() : m_root(NULL) {}
    CBSTree(const CBSTree& other);
    virtual ~CBSTree() { DestroyTree(); }

    // member functions
    bool DeleteItem(const NodeType& target);
    void DestroyTree() { DestroyNodes(m_root); m_root = NULL; }
    void GetTreeInfo(int& numNodes, int& height) const;
    void InOrderTraverse(void  (*fPtr)(const NodeType&)) const;
    bool InsertItem(const NodeType& newItem);
    bool IsTreeEmpty() { return (NULL == m_root); }
    bool ItemInTree(const NodeType& target) const;
    void PostOrderTraverse(void (*fPtr)(const NodeType&)) const;
    void PreOrderTraverse(void (*fPtr)(const NodeType&)) const;
    void RebalanceTree();
    void DumpTree() { PrintNodes(m_root); }

    // operators
    CBSTree<NodeType>& operator=(const CBSTree<NodeType>& rhs);

protected:
    // member functions
    int CountNodes(const CTreeNode<NodeType>* nodePtr, 
                    int  currDepth, 
                    int& numNodes) const;
    CTreeNode<NodeType>* Delete(const NodeType& target, 
                                CTreeNode<NodeType>* nodePtr, 
                                bool& bItemDeleted);
    void DestroyNodes(CTreeNode<NodeType>* const nodePtr);
    CTreeNode<NodeType>* FindMinNode(CTreeNode<NodeType>* nodePtr) const;
    void InOrder(const CTreeNode<NodeType>* const nodePtr,
                    void (*fPtr)(const NodeType&)) const;
    CTreeNode<NodeType>* Insert(const NodeType& newItem, 
                                CTreeNode<NodeType>* nodePtr);
    void PostOrder(const CTreeNode<NodeType>* const nodePtr, 
                    void (*fPtr)(const NodeType&)) const;
    void PreOrder(const CTreeNode<NodeType>* const nodePtr, 
                    void (*fPtr)(const NodeType&)) const;
    void Repopulate(const NodeType array[], int first, int last);
    CTreeNode<NodeType>* Retrieve(const NodeType& target, 
                                    CTreeNode<NodeType>* nodePtr) const;
    void SaveToArray(const CTreeNode<NodeType>* const nodePtr, 
                        NodeType array[], 
                        int& index);
    void  PrintNodes(const CTreeNode<NodeType>* nodePtr) const;

private:
    // member functions
    CTreeNode<NodeType>* CopyTree(const CTreeNode<NodeType>* sourcePtr);

    // data members
    CTreeNode<NodeType>* m_root;
};

//#include    "cbstree.cpp"
#endif  // CBIN_SEARCH_TREE_HEADER