#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

struct Node{
    int value = 0;
    Node* left = NULL;
    Node* right = NULL;
};

class Tree{
private:
    Node* root;
    // Print functions
    void PrintPreOrderRecursive(Node* currentNode);
    void PrintInOrderRecursive(Node* currentNode);
    void PrintPostOrderRecursive(Node* currentNode);
    // Insert functions
    Node* FindInsertSpotRecursive(int inputNum, Node* currentNode);
    Node* FindInsertSpot(int inputNum);
    Node* FindParent(int inputNum);
    // Remove functions
    Node* FindLargestSmallerThanParent(Node* inputNode);
    void RemoveTwoChildNode(Node* nodeToRemove);
    // Rebalance functions
    int GetHeight(Node* heightRoot);
    int GetBalanceFactor(Node* heightRoot);
    void LeftRotation(Node* rotRoot);
    void RightRotation(Node* rotRoot);
    void LeftRightRotation(Node* rotRoot);
    void RightLeftRotation(Node* rotRoot);
    void RebalanceNode(Node* rebalanceRoot);
    void RebalanceTree(Node* updatedNode);
public:
    Tree();
    ~Tree();
    bool Empty();
    void Insert(int inputNum);
    void Remove(int inputNum);
    void Print(string order);
};

Tree::Tree(){
    root = NULL;
}

Tree::~Tree(){
}

// Returns if the tree is empty
bool Tree::Empty(){
    return root == NULL;
}

// Returns the height of a single node
int Tree::GetHeight(Node* heightRoot){
    if (heightRoot->left == NULL && heightRoot->right == NULL){
        return 0;
    } else if (heightRoot->left != NULL && heightRoot->right == NULL){
        return GetHeight(heightRoot->left) + 1;
    } else if (heightRoot->left == NULL && heightRoot->right != NULL){
        return GetHeight(heightRoot->right) + 1;
    } else {
        return max(GetHeight(heightRoot->left) + 1, GetHeight(heightRoot->right) + 1);
    }
    
    return 0;
}

// Returns the balance factor of a single node
int Tree::GetBalanceFactor(Node* balanceRoot){
    int leftHeight = 0;
    int rightHeight = 0;

    if (balanceRoot->left != NULL){
        // +1 to include the height of the left node itself
        leftHeight = GetHeight(balanceRoot->left) +1;
    }
    if (balanceRoot->right != NULL){
        // +1 to include the height of the right node itself
        rightHeight = GetHeight(balanceRoot->right) +1;
    }
    
    return leftHeight - rightHeight;
}

void Tree::LeftRotation(Node* rotRoot){
    Node* newRoot = new Node;
    newRoot->value = rotRoot->right->value;
    newRoot->right = rotRoot->right->right;
    newRoot->left = rotRoot;
    newRoot->left->right = rotRoot->right->left;
    Node* parentNode = FindParent(rotRoot->value);

    if (parentNode == NULL){
        // cout << "Left rotation of root node: " << root->value << endl;
        root = newRoot;
        return;
    } else if (parentNode->value > newRoot->value){
        // cout << "Left rotation insert left of: " << parentNode->value << endl;
        parentNode->left = newRoot;
    } else if (parentNode->value < newRoot->value){
        // cout << "Left rotation insert right of: " << parentNode->value << endl;
        parentNode->right = newRoot;
    }
    
    return;
}

void Tree::RightRotation(Node* rotRoot){
    Node* newRoot = new Node;
    newRoot->value = rotRoot->left->value;
    newRoot->left = rotRoot->left->left;
    newRoot->right = rotRoot;
    newRoot->right->left = rotRoot->left->right;
    Node* parentNode = FindParent(rotRoot->value);

    if (parentNode == NULL){
        // cout << "Right rotation of root node: " << root->value << endl;
        root = newRoot;
        return;
    } else if (parentNode->value > newRoot->value){
        // cout << "Right rotation insert left of: " << parentNode->value << endl;
        parentNode->left = newRoot;
    } else if (parentNode->value < newRoot->value){
        // cout << "Right rotation insert right of: " << parentNode->value << endl;
        parentNode->right = newRoot;
    }
    
    return;
}

void Tree::LeftRightRotation(Node* rotRoot){
    Node* newRoot = new Node;
    Node* temp = rotRoot->left->right->right;
    newRoot->value = rotRoot->left->right->value;
    newRoot->left = rotRoot->left;
    newRoot->left->right = rotRoot->left->right->left;
    newRoot->right = rotRoot;
    newRoot->right->left = temp;

    Node* parentNode = FindParent(rotRoot->value);
    if (parentNode == NULL){
        // cout << "Left-right rotation of root node: " << root->value << endl;
        root = newRoot;
        return;
    } else if (parentNode->value > newRoot->value){
        // cout << "Left-right rotation insert left of: " << parentNode->value << endl;
        parentNode->left = newRoot;
    } else if (parentNode->value < newRoot->value){
        // cout << "Left-right rotation insert right of: " << parentNode->value << endl;
        parentNode->right = newRoot;
    }

    return;
}

void Tree::RightLeftRotation(Node* rotRoot){
    Node* newRoot = new Node;
    Node* temp = rotRoot->right->left->left;
    newRoot->value = rotRoot->right->left->value;
    newRoot->right = rotRoot->right;
    newRoot->right->left = rotRoot->right->left->right;
    newRoot->left = rotRoot;
    newRoot->left->right = temp;

    Node* parentNode = FindParent(rotRoot->value);
    if (parentNode == NULL){
        // cout << "Right-left rotation of root node: " << root->value << endl;
        root = newRoot;
        return;
    } else if (parentNode->value > newRoot->value){
        // cout << "Right-left rotation insert left of: " << parentNode->value << endl;
        parentNode->left = newRoot;
    } else if (parentNode->value < newRoot->value){
        // cout << "Right-left rotation insert right of: " << parentNode->value << endl;
        parentNode->right = newRoot;
    }

    return;
}

// Rebalances a single node by considering its balance
// factor
void Tree::RebalanceNode(Node* rebalanceRoot){
    int balanceFactor = GetBalanceFactor(rebalanceRoot);

    if (balanceFactor > 1){
        if (rebalanceRoot->left != NULL && GetBalanceFactor(rebalanceRoot->left) >= 0){
            RightRotation(rebalanceRoot);
        } else {
            LeftRightRotation(rebalanceRoot);
        }
        
    } else if (balanceFactor < -1){
        if (rebalanceRoot->right != NULL && GetBalanceFactor(rebalanceRoot->right) <= 0){
            LeftRotation(rebalanceRoot);
        } else {
            RightLeftRotation(rebalanceRoot);
        }
    }

    return;
}

// Checks the balance factor from the given node, all the
// way to the root, rebalancing and checking the balance
// factor of each node along the way
void Tree::RebalanceTree(Node* updatedNode){
    stack<Node*> stack;
    Node* currentNode = root;

    while (currentNode != NULL){
        stack.push(currentNode);
        if (currentNode->value > updatedNode->value){
            currentNode = currentNode->left;
        } else if (currentNode->value < updatedNode->value){
            currentNode = currentNode->right;
        } else {
            currentNode = NULL;
        }   
    }

    while (!stack.empty()){
        RebalanceNode(stack.top());
        stack.pop();
    }

    return;
}

// Helper function for finding where to insert a node
Node* Tree::FindInsertSpotRecursive(int inputNum, Node* currentNode){
    if (currentNode->value == inputNum){
        return NULL;

    } else if (currentNode->left == NULL && currentNode->right == NULL){
        return currentNode;

    } else if (currentNode->value > inputNum){
        if (currentNode->left == NULL){
            return currentNode;
        }
        return FindInsertSpotRecursive(inputNum, currentNode->left);

    } else if (currentNode->value < inputNum){
        if (currentNode->right == NULL){
            return currentNode;
        }
        return FindInsertSpotRecursive(inputNum, currentNode->right);
    }
        
    return NULL;
}

// Returns the node which the value must be inserted as
// a child node in the tree
Node* Tree::FindInsertSpot(int inputNum){    
    return FindInsertSpotRecursive(inputNum, root);
}

// Inserts a number into the tree, if it already exists,
// does nothing
void Tree::Insert(int inputNum){
    // cout << "Inserting: " << inputNum << endl;
    Node* inputNode = new Node;
    inputNode->value = inputNum;
    
    if (Empty()){
        // cout << "Tree empty, inserting: " << inputNum << " at root" << endl;
        root = inputNode;
        // cout << "Root balance: " << GetBalanceFactor(root) << endl;
        return;
    }

    // Returns NULL if the input number is already in the tree
    // When inserting numbers, if one is already in the tree
    // the insert call returns and does nothing
    Node* parentNode = FindInsertSpot(inputNum);

    if (parentNode == NULL){
        // cout << "Already in tree!" << endl;
        return;
    } else if (parentNode->value > inputNum){
        // cout << "Inserting left of: " << parentNode->value << endl;
        parentNode->left = inputNode;
    } else if (parentNode->value < inputNum){
        // cout << "Inserting right of: " << parentNode->value << endl;
        parentNode->right = inputNode;
    }
    
    RebalanceTree(parentNode);

    return;
}

// Returns the parent of the node value given, or NULL
// if no node with that value exists
Node* Tree::FindParent(int inputNum){
    queue<Node*> q;
    q.push(root);

    while (!q.empty()){
        Node* currentNode = q.front();
        q.pop();

        if (currentNode->value > inputNum && currentNode->left != NULL){
            if (currentNode->left->value == inputNum){
                return currentNode;
            }
            q.push(currentNode->left);
        } else if (currentNode->value < inputNum && currentNode->right != NULL){
            if (currentNode->right->value == inputNum){
                return currentNode;
            }
            q.push(currentNode->right);
        }   
    }
    
    return NULL;
}

// Returns the parent of the node
// that is the largest value smaller than the value given
Node* Tree::FindLargestSmallerThanParent(Node* inputNode){
    Node* parentNode = inputNode;
    Node* currentNode = inputNode->left;
    while (currentNode->right != NULL){
        parentNode = currentNode;
        currentNode = currentNode->right;
    }

    return parentNode;
}

// If the node has 2 children, swap the value of the node
// to remove with the value of the largest node
// smaller than it, and delete the swap node instead
// This effectively deletes the node to be removed
void Tree::RemoveTwoChildNode(Node* nodeToRemove){
    Node* swapNodeParent = FindLargestSmallerThanParent(nodeToRemove);
    Node* swapNode = NULL;
    char swapNodeParentDir = '0';

    if (swapNodeParent->left != NULL && swapNodeParent->right == NULL){
        swapNode = swapNodeParent->left;
        swapNodeParentDir = 'L';
    } else if (swapNodeParent->left == NULL && swapNodeParent->right != NULL){
        swapNode = swapNodeParent->right;
        swapNodeParentDir = 'R';
    } else if (swapNodeParent->left != NULL && swapNodeParent->right != NULL){
        if (swapNodeParent->right->value > nodeToRemove->value){
            swapNode = swapNodeParent->left;
            swapNodeParentDir = 'L';
        } else {
            swapNode = swapNodeParent->right;
            swapNodeParentDir = 'R';
        }
    } else {
        // cout << "Swap node parent has no children!" << endl;
    }
    
    // cout << "2 child node, largest smaller than node: " << swapNode->value << endl;
    int removeValue = nodeToRemove->value;
    nodeToRemove->value = swapNode->value;
    swapNode->value = removeValue;

    // At this point, the swap node is only ever going to have
    // 1 child to the left of it, or none
    // Because if it did have a right child, the right
    // child would become the new largest smaller than node
    // So, since we only have 1 node to the left
    // or no child nodes at all, that is assumed in the
    // swap node removal code below:

    // No children for swap node
    if (swapNode->left == NULL){
        delete swapNode;
        if (swapNodeParentDir == 'L'){
            swapNodeParent->left = NULL;
        } else {
            swapNodeParent->right = NULL;
        }
        
    // 1 child to the left of swap node
    } else {
        Node* temp = swapNode->left;
        delete swapNode;
        if (swapNodeParentDir == 'L'){
            swapNodeParent->left = temp;
        } else {
            swapNodeParent->right = temp;
        }
    }

    RebalanceTree(swapNodeParent);

    return;
}

// Removes a value from the tree, if it isn't in the tree,
// does nothing
void Tree::Remove(int inputNum){
    // cout << "Removing: " << inputNum << endl;

    if (Empty()){
        // cout << "Tree empty!" << endl;
        return;
    // Removing the root node (has no parent so is a bit simpler) 
    } else if (root->value == inputNum){
        // cout << "Removing root node" << endl;

        if (root->left == NULL && root->right == NULL){
            delete root;
            root = NULL;

        } else if (root->left == NULL && root->right != NULL){
            Node* temp = root->right;
            delete root;
            root = temp;

        } else if (root->left != NULL && root->right == NULL){
            Node* temp = root->left;
            delete root;
            root = temp;
        } else {
            RemoveTwoChildNode(root);
        }

        RebalanceTree(root);

        return;
    }
    
    Node* parentNode = FindParent(inputNum);
    Node* nodeToRemove = NULL;
    char parentDir = '0';

    // If parentNode is NULL, it isn't in the tree so don't
    // do anything
    if (parentNode == NULL){
        // cout << "Node not in tree!" << endl;
        return;
    }
    
    // Finding the actual node to remove
    if (parentNode->value > inputNum){
        nodeToRemove = parentNode->left;
        parentDir = 'L';
    } else {
        nodeToRemove = parentNode->right;
        parentDir = 'R';
    }
    
    // cout << "Parent node is: " << parentNode->value << endl;

    if (nodeToRemove->left == NULL && nodeToRemove->right == NULL){
        delete nodeToRemove;
        if (parentDir == 'L'){
            parentNode->left = NULL;
        } else {
            parentNode->right = NULL;
        }
        RebalanceTree(parentNode);
        
    } else if (nodeToRemove->left != NULL && nodeToRemove->right == NULL){
        Node* temp = nodeToRemove->left;
        delete nodeToRemove;
        if (parentDir == 'L'){
            parentNode->left = temp;
        } else {
            parentNode->right = temp;
        }
        RebalanceTree(parentNode);
        
    } else if (nodeToRemove->left == NULL && nodeToRemove->right != NULL){
        Node* temp = nodeToRemove->right;
        delete nodeToRemove;
        if (parentDir == 'L'){
            parentNode->left = temp;
        } else {
            parentNode->right = temp;
        }
        RebalanceTree(parentNode);

    } else {
        RemoveTwoChildNode(nodeToRemove);
    }

    return;
}

// Helper function for printing the values of the tree
// in pre-order
void Tree::PrintPreOrderRecursive(Node* currentNode){
    cout << currentNode->value << " ";

    if (currentNode->left != NULL){
        PrintPreOrderRecursive(currentNode->left);
    }

    if (currentNode->right != NULL){
        PrintPreOrderRecursive(currentNode->right);
    }
    
    return;
}

// Helper function for printing the values of the tree
// in-order
void Tree::PrintInOrderRecursive(Node* currentNode){
    if (currentNode->left != NULL){
        PrintInOrderRecursive(currentNode->left);
    }

    cout << currentNode->value << " ";

    if (currentNode->right != NULL){
        PrintInOrderRecursive(currentNode->right);
    }
    
    return;
}

// Helper function for printing the values of the tree
// in post-order
void Tree::PrintPostOrderRecursive(Node* currentNode){
    if (currentNode->left != NULL){
        PrintPostOrderRecursive(currentNode->left);
    }

    if (currentNode->right != NULL){
        PrintPostOrderRecursive(currentNode->right);
    }

    cout << currentNode->value << " ";
    
    return;
}

// Prints to console the values of the tree in the specified
// order, or "EMPTY" if the tree doesn't contain values
void Tree::Print(string order){
    // cout << "Printing " << order << " order" << endl;

    if (Empty()){
        cout << "EMPTY" << endl;
        return;
    }

    if (order == "PRE"){
        PrintPreOrderRecursive(root);
    } else if (order == "IN"){
        PrintInOrderRecursive(root);
    } else if (order == "POST"){
        PrintPostOrderRecursive(root);
    }

    return;
}

int main(void){
    Tree tree;

    // Processing input
    string input_str;
    getline(cin, input_str);  
    stringstream stream_object(input_str);  //stream object initialized with input_str
    string individual_word;

    while (stream_object >> individual_word) {
        if (!isdigit(individual_word.at(1))){
            tree.Print(individual_word);
        } else {
            // Converting input string into a character command and number
            char action = individual_word.at(0);
            int number = 0;
            
            for (int i = 1; i < (int)individual_word.size(); i++){
                number *= 10;
                number += individual_word.at(i) - '0';
            }
            // Processing character command
            if (action == 'A'){
                tree.Insert(number);
            } else {
                tree.Remove(number);
            }
        }
    }
    cout << endl;
    
    return 0;
}
