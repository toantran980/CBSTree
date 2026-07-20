# CBSTree Refactor Plan

## Goal
Refactor CBSTree into a safer, cleaner, and more correct C++ implementation while keeping the same core functionality:
- insert / delete / search
- traversals
- rebalance

## Current problems (from repo inspection)
- Template code appears split across `.hpp` and `.cpp`, and the build relies on including a `.cpp` file from `main.cpp`.
- Raw owning pointers (`new`/`delete`) increase risk of UB/leaks.
- Copy/delete logic looks inconsistent (risk of incorrect pointer handling).

## Refactor steps (suggested order)

### 1) Decide which implementation is canonical
- Choose a single canonical implementation for template behavior.
- Recommendation: make `cbstree.hpp` the single source of truth (header-only templates) by moving the V2 definitions into it.
- Remove `#include "cbstree_V2.cpp"` usage from `main.cpp`.
- Keep `cbstree.cpp` only if you want a reference/legacy implementation; it should not be part of the build.


### 2) Make template structure correct
- Move all `template<typename T> ...` function definitions into `cbstree.hpp` (recommended), OR
- convert to non-template implementation for a fixed type (not desired here), OR
- use explicit template instantiation correctly.
- Ensure `main.cpp` only includes headers (no `#include "*.cpp"`).

### 3) Fix node ownership and memory safety
- Replace raw pointers with RAII:
  - Use `std::unique_ptr<CTreeNode<T>>` for left/right.
  - Hold `m_root` as `std::unique_ptr<CTreeNode<T>>`.
- Remove/adjust manual `DestroyTree()` / `DestroyNodes()`.
- Ensure destructor behavior is trivial (unique_ptr handles deletion).

### 4) Correct copy/move semantics
- Implement:
  - copy constructor
  - copy assignment
  - move constructor
  - move assignment
- With `unique_ptr`, implement deep copy via a `clone()` helper on nodes.

### 5) Repair and validate BST operations
- Re-implement (or carefully correct):
  - `InsertItem`
  - `Retrieve`/`ItemInTree`
  - `DeleteItem`
- Provide strong invariants:
  - all nodes in left subtree < node value
  - all nodes in right subtree > node value
- Ensure delete handles:
  - leaf
  - single-child
  - two-children (successor replacement)

### 6) Rebalance improvements (performance + correctness)
- Current approach: destroy + repeated `Insert()` calls.
- Improve to O(n) rebuild:
  1. Inorder traverse to sorted `std::vector<T>`
  2. Build balanced tree from vector via recursion selecting mid.
- This avoids potential extra log factors and reduces complexity.

### 7) Modernize traversal API
- Replace function pointers `void (*fPtr)(const T&)` with:
  - templated callable: `template<class F> void inOrder(F&& f)`
- Keep the menu demo working by passing lambdas.

### 8) Add tests / demo verification
- Add a basic test harness (or compile-time/run-time checks) verifying:
  - insert/search
  - delete scenarios
  - traversal ordering (inorder is sorted)
  - rebalance reduces height (for worst-case inserts)

### 9) Update build instructions
- Ensure `README.md` includes a correct `g++` command for building.

## Completion checklist
- [x] Improve rebalance to O(n) rebuild via sorted vector (implemented in `cbstree_V2.cpp`)
- [x] Add refactor plan document (`TODO.md`)
- [x] No `#include "*.cpp"` from `main.cpp`
- [x] Template definitions are in the header
- [x] No raw owning pointers (use `unique_ptr`)
- [x] Copy/move operations are correct
- [x] Insert/search/delete pass validation cases
- [x] README updated with correct build + usage


