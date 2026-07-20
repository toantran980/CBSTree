// ============================================================================
// This file contains the definition of the CTreeNode class.
// ============================================================================

#ifndef CTREE_NODE_V2_HEADER
#define CTREE_NODE_V2_HEADER

#include <memory>

template <typename NodeValueType>
class CTreeNode {
public:
    using NodePtr = std::unique_ptr<CTreeNode<NodeValueType>>;

    CTreeNode() = default;
    explicit CTreeNode(const NodeValueType& newValue)
        : m_value(newValue), m_left(nullptr), m_right(nullptr) {}

    // Deep copy helper for unique_ptr
    NodePtr clone() const {
        auto n = std::make_unique<CTreeNode<NodeValueType>>(m_value);
        if (m_left) n->m_left = m_left->clone();
        if (m_right) n->m_right = m_right->clone();
        return n;
    }

    NodeValueType m_value{};
    NodePtr m_left;
    NodePtr m_right;
};

#endif  // CTREE_NODE_V2_HEADER