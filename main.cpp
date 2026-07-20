// =====================================================================================
// This is a test driver to exercise the CBSTree (compressed binary search tree)
//template class.An instance is instantiated for type int, and various member functions 
// are called to store random or sequential sets of integers in the tree.
// =====================================================================================

//#pragma warning(disable: 4996)
#include    <iostream>
#include    <cstdlib>

#include    "cbstree.hpp"
#include    "cbstree_V2.cpp"

using namespace std;

// define to show duplicate insertions
#define SHOW_INSERT

// defined constants
const   int         BUFLEN = 256;

// function prototypes
void    AddRandomInts(CBSTree<int>& tree);
void    AddSequentialInts(CBSTree<int>& tree);
void    BalanceTree(CBSTree<int>& tree);
void    DisplayMenu();
void    DisplayTree(const CBSTree<int>& tree);
void    PrintInt(const int& intRef);


// ==== main ==================================================================
//
// ============================================================================

int     main() {
    bool                bLoop = true;
    CBSTree<int>        myIntTree;
    char                buf[BUFLEN];
    int                 height;
    int                 numNodes;

    // loop and let the user manipulate the tree
    do {
        // display the menu and get a user selection
        DisplayMenu();
        cout << "Please enter a selection: ";
        cin.getline(buf, BUFLEN);
        switch (toupper(*buf)) {
            // add random values to the tree
        case 'A':
            AddRandomInts(myIntTree);
            break;

        case 'B':
            BalanceTree(myIntTree);
            break;

            // add sequential values to the tree
        case 'I':
            AddSequentialInts(myIntTree);
            break;

            // display the tree
        case 'D':
            myIntTree.GetTreeInfo(numNodes, height);
            if (0 == numNodes) {
                cout << "The tree is currently empty..." << endl;
            } else {
                DisplayTree(myIntTree);
            }
            break;

            // display tree statistics
        case 'S':
            myIntTree.GetTreeInfo(numNodes, height);
            cout << "The tree has "
                << numNodes
                << ((1 == numNodes) ? " node" : " nodes")
                << " and a height of " << height << endl;
            break;

            // display nodes, left and right tree
        case 'M':
            myIntTree.DumpTree();
            break;

            // remove all values from the tree
        case 'Q':
        case 'R':
            myIntTree.GetTreeInfo(numNodes, height);
            myIntTree.DestroyTree();
            cout << numNodes
                << ((1 == numNodes) ? " node" : " nodes")
                << " released"
                << endl;

            if ('Q' == toupper(*buf)) {
                cout << "Bye!" << endl;
                bLoop = false;
            }
            break;
            
        default:
            cout << "Invalid selection, try again..." << endl;
            break;

        }  // end of switch

    } while (bLoop);

    return 0;

}  // end of "main"

// ==== AddRandomInts =========================================================
//
// This function allows the user to add a random integer values to the tree
// parameter. The user is prompted to enter the number of ints to be inserted,
// as well as the upper bound. If the random number to be inserted already
// exists in the tree, a message is written to stdout indicating as such.
//
// Input:
//      tree [IN/OUT]   -- a reference to a CBSTree object instantiated for
//                         an int
//
// Output:
//      Nothing
//
// ============================================================================

void    AddRandomInts(CBSTree<int>& tree) {
    int         intVal;
    int         numInts;
    int         maxVal;
    char        buf[BUFLEN];

    // set the random number generator
    srand(unsigned(time(NULL)));

    // get some information from the user...
    cout << "How many random integers would you like to insert? ";
    cin.getline(buf, BUFLEN); // may cause memory leaks
    if (1 != sscanf(buf, "%d", &numInts)) {
        cout << "Sorry, bogus input..." << endl;
        return;
    }

    cout << "Enter the upper bound for values: ";
    cin.getline(buf, BUFLEN);
    if (1 != sscanf(buf, "%d", &maxVal)) {
        cout << "Sorry, bogus input..." << endl;
        return;
    }

    // populate the tree with random values
    for (; numInts > 0; --numInts) {
        intVal = rand() % (maxVal + 1);
        if (false == tree.InsertItem(intVal)) {
#ifdef  SHOW_INSERT
            cout << "  " << intVal << " is already in the tree..." << endl;
#endif  // SHOW_INSERT
        }
    }

}  // end of "AddRandomInts"



// ==== AddSequentialInts =====================================================
//
// This function allows the user to add a sequence of integer values to the
// tree parameter. The user is prompted to enter the lower and upper bounds of
// the number sequence. If the number to be inserted already exists in the tree,
// a message is written to stdout indicating as such.
//
// Input:
//      tree [IN/OUT]   -- a reference to a CBSTree object instantiated for
//                         an int
//
// Output:
//      Nothing
//
// ============================================================================

void    AddSequentialInts(CBSTree<int>& tree) {
    int         lower;
    int         upper;
    char        buf[BUFLEN];

    cout << "Enter the lower bound of the sequence: ";
    cin.getline(buf, BUFLEN);
    if (1 != sscanf(buf, "%d", &lower)) {
        cout << "Sorry, bogus input..." << endl;
        return;
    }

    cout << "Enter the upper bound of the sequence: ";
    cin.getline(buf, BUFLEN);
    if (1 != sscanf(buf, "%d", &upper)) {
        cout << "Sorry, bogus input..." << endl;
        return;
    }


    // populate the tree with random values
    for (; lower <= upper; ++lower) {
        if (false == tree.InsertItem(lower)) {
#ifdef  SHOW_INSERT
            cout << "  " << lower << " is already in the tree..." << endl;
#endif  // SHOW_INSERT
        }
    }

}  // end of "AddSequentialInts"

// ==== BalanceTree ===========================================================
//
// This function displays the list of menu options available to the user.
//
// Input:  nothing
// Output: nothing
//
// ============================================================================

void    BalanceTree(CBSTree<int>& tree) {
    cout << "BalanceTree called..." << endl;
    tree.RebalanceTree();

}  // end of "BalanceTree"



// ==== DisplayMenu ===========================================================
//
// This function displays the list of menu options available to the user.
//
// Input:  nothing
// Output: nothing
//
// ============================================================================

void    DisplayMenu() {
    cout << "\nD)isplay the tree\n";
    cout << "A)dd random values to the tree\n";
    cout << "B)alance the tree\n";
    cout << "I)nsert sequential values to the tree\n";
    cout << "R)elease all tree nodes\n";
    cout << "S)how tree statistics\n";
    cout << "Q)uit\n";

}  // end of "DisplayMenu"



// ==== DisplayTree ===========================================================
//
// This function will display the contents of the tree parameter to stdout. The
// user is first prompted to specify if the values are to be displayed using a
// preorder, inorder or postorder traversal.
//
//
// Input:
//      tree [IN]   -- a reference to a CBSTree object instantiated for an int
//
// Output:
//      Nothing
//
// ============================================================================
void    DisplayTree(const CBSTree<int>& tree)
{
    char        buf[BUFLEN];

    cout << " 1) Preorder\n 2) Inorder\n 3) Postorder\n";
    cout << "Please enter the traversal type: ";
    cin.getline(buf, BUFLEN);

    switch (*buf - '0') {
    case  1:
        tree.PreOrderTraverse(PrintInt);
        break;

    case  2:
        tree.InOrderTraverse(PrintInt);
        break;

    case  3:
        tree.PostOrderTraverse(PrintInt);
        break;

    default:
        cout << "Sorry, invalid selection..." << endl;
        break;
    }

    cout << endl;

}  // end of "DisplayTree"



// ==== PrintInt ==============================================================
//
// This is a function that can be used as an argument to the CBSTree traversal
// functions. It just displays an integer argument to stdout.
//
// Input:
//      intRef [IN]     -- a const reference to an int
//
// Output:
//      Nothing
//
// ============================================================================

void    PrintInt(const int& intRef) {
    cout << intRef << '\t';

}  // end of "PrintInt"
