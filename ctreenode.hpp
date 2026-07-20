// ============================================================================
// This file contains the definition of the CTreeNode class.  It uses the
// "NodeValueType" template parameter to store a copy of a value.
// ============================================================================

#ifndef CTREE_NODE_HEADER
#define CTREE_NODE_HEADER

#include    <iostream>
using namespace std;

template    <typename NodeValueType>
class   CTreeNode {
public:
    // constructor
    CTreeNode() : m_left(NULL), m_right(NULL) {}
    CTreeNode(const NodeValueType& newValue) : m_value(newValue), m_left(NULL)
        , m_right(NULL) {}
    ~CTreeNode() { m_left = m_right = NULL; }

    // data members
    NodeValueType       m_value;
    CTreeNode* m_left;
    CTreeNode* m_right;
};

#endif  // CTREE_NODE_HEADER
