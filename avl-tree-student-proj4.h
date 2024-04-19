#ifndef AVL_TREE_STUDENT_PROJ4
#define AVL_TREE_STUDENT_PROJ4

#include "avl-tree-prof-proj4.h"
#include <queue>

/* ~AVLNode()
 * Destructor for AVLNode, recursively deletes left and right children
 */
template <typename T>
AVLNode<T>::~AVLNode(){
    const AVLNode<T> *leftChild = this->getLeft();
    const AVLNode<T> *rightChild = this->getRight();
    if (leftChild) {
        delete leftChild;
        leftChild = nullptr;
    }
    if (rightChild) {
        delete rightChild;
        rightChild = nullptr;
    }
    this->left = nullptr;
    this->right = nullptr;
    return;
}

/* minNode() const
 * Returns a pointer to the node with the minimum value of the given node
 *  parameters:
 *
 *  return value:
 *  Pointer to the node with the minimum value
 */
template <typename T>
const AVLNode<T>* AVLNode<T>::minNode() const{
    if (!this->left){
        return this;
    }
    AVLNode* that = this->left;
    while (that->left){
        that = that->left;
    }
    return that;
}

/* maxNode() const
 * Returns a pointer to the node with the maximum value of the given node
 *  parameters:
 *
 *  return value:
 *  Pointer to the node with the maximum value
 */
template <typename T>
const AVLNode<T>* AVLNode<T>::maxNode() const{
    if (!this->right){
        return this;
    }
    AVLNode* that = this->right;
    while (that->right){
        that = that->right;
    }
    return that;
}

/* singleRotateLeft()
 * Performs a single rotation to the left on the AVL Node
 * parameters:
 *
 * return value:
 *   Pointer to the node that becomes the new "root" after rotation
 */
template <typename T>
AVLNode<T>* AVLNode<T>::singleRotateLeft(){
    if (this->right){
        AVLNode* temp = this->right;
        AVLNode* hold = nullptr;
        if (temp->left){
            hold = temp->left;
            temp->left = nullptr;
            temp->updateHeight();
        }
        this->right = nullptr;
        this->updateHeight();
        temp->left = this;
        this->right = hold;
        this->updateHeight();
        if (temp->left){
            temp->left->updateHeight();
        }
        if (temp->right){
            temp->right->updateHeight();
        }
        temp->updateHeight();
        return temp;
    }
    return this;
}

/* singleRotateRight()
 * Performs a single rotation to the right on the AVL Node
 * parameters:
 *
 * return value:
 *   Pointer to the node that becomes the new "root" after rotation
 */
template <typename T>
AVLNode<T>* AVLNode<T>::singleRotateRight(){
    if (this->left){
        AVLNode* temp = this->left;
        AVLNode* hold = nullptr;
        if (temp->right){
            hold = temp->right;
            temp->right = nullptr;
            temp->updateHeight();
        }
        this->left = nullptr;
        this->updateHeight();
        temp->right = this;
        this->left = hold;
        this->updateHeight();
        if (temp->left){
            temp->left->updateHeight();
        }
        if (temp->right){
            temp->right->updateHeight();
        }
        temp->updateHeight();
        return temp;
    }
    return this;
}

/* doubleRotateLeftRight()
 * Performs a double rotation (left-right) on the AVL Node, left rotation on
 * its left child, and right rotation on "root" location
 * parameters:
 *
 * return value:
 *   pointer to the node that becomes the new "root" after rotation
 */
template <typename T>
AVLNode<T>* AVLNode<T>::doubleRotateLeftRight(){
    this->left = this->left->singleRotateLeft();
    return this->singleRotateRight();
}

/* doubleRotateRightLeft()
 * Performs a double rotation (right-left) on the AVL Node, right rotation on
 * its right child, and left rotation on "root" location
 * parameters:
 *
 * return value:
 *   pointer to the node that becomes the new "root" after rotation
 */
template <typename T>
AVLNode<T>* AVLNode<T>::doubleRotateRightLeft(){
    this->right = this->right->singleRotateRight();
    return this->singleRotateLeft();
}

/* insert(const T&)
 * Inserts a new node with the given item into the AVL Tree
 *  parameters:
 *  item, value to be inserted into the AVL Tree
 *
 *  return value:
 *
 */
template <typename T>
void AVLTree<T>::insert(const T &item){
    if (!this->root){
        this->root = new AVLNode<T> (item);
        return;
    }
    vector<AVLNode<T>*> path;
    AVLNode<T>* temp = this->root;
    while(true) {
        path.push_back(temp);
        if (!(item < temp->data) && !(temp->data < item)){
            return;
        }
        else if (item < temp->data){
            if (temp->left){
                temp = temp->left;
            }
            else {
                temp->left = new AVLNode<T>(item);
                path.push_back(temp->left);
                break;
            }
        }
        else {
            if(temp->right){
                temp = temp->right;
            }
            else {
                temp->right = new AVLNode<T>(item);
                path.push_back(temp->right);
                break;
            }
        }
    }
    for (int i = path.size() - 1; i >= 0; i--){
        if (path.at(i)->right && path.at(i)->left) {
            path.at(i)->updateHeight();
        }
        else if (path.at(i)->right){
            path.at(i)->height = path.at(i)->right->height + 1;
        }
        else if (path.at(i)->left){
            path.at(i)->height = path.at(i)->left->height + 1;
        }
        else {
            path.at(i)->height = 0;
        }
    }
    this->rebalancePathToRoot(path);
}

/* rebalancePathToRoot(const vector<AVLNode<T>*>&)
 * Rebalances the AVL Tree along the path from the newly inserted node to the root
 * parameters:
 *   path, vector of pointers to nodes along the path from the newly inserted node to the root
 * return value:
 *
 */

const int HEIGHTMAX = 2;
template <typename T>
void AVLTree<T>::rebalancePathToRoot(vector<AVLNode<T>*> const &path){
    if (!this->root){
        return;
    }
    if (this->root->height < HEIGHTMAX){
        return;
    }
    //i is set to this size because we assume it is always being balanced, so the
    //last 2 nodes for the path don't need to be checked? could be wrong
    AVLNode<T>* temp = nullptr;
    AVLNode<T>* parent = nullptr;
    for (int i = path.size() - 1; i >= 0; i--){
        temp = path.at(i);
        if (i > 0){
            parent = path.at(i - 1);
        }
        else {
            if (!path.empty()){
                parent = path.at(0);
            }
        }
        if (temp->height >= HEIGHTMAX){
            if (temp->right && temp->left){
                if (abs(temp->right->height - temp->left->height) > 1){
                    if (temp->right->height > temp->left->height){
                        if (temp->right->right && temp->right->left){
                            if (temp->right->right->height >= temp->right->left->height){
                                if (parent->right == temp){
                                    parent->right = temp->singleRotateLeft();
                                }
                                else if (parent->left == temp){
                                    parent->left = temp->singleRotateLeft();
                                }
                                else {
                                    this->root = temp->singleRotateLeft();
                                }
                            }
                            else {
                                if (parent->right == temp){
                                    parent->right = temp->doubleRotateRightLeft();
                                }
                                else if (parent->left == temp){
                                    parent->left = temp->doubleRotateRightLeft();
                                }
                                else {
                                    this->root = temp->doubleRotateRightLeft();
                                }
                            }
                        }
                        else if (temp->right->right){
                            if (parent->right == temp){
                                parent->right = temp->singleRotateLeft();
                            }
                            else if (parent->left == temp){
                                parent->left = temp->singleRotateLeft();
                            }
                            else {
                                this->root = temp->singleRotateLeft();
                            }
                        }
                        else if (temp->right->left){
                            if (parent->right == temp){
                                parent->right = temp->doubleRotateRightLeft();
                            }
                            else if (parent->left == temp){
                                parent->left = temp->doubleRotateRightLeft();
                            }
                            else {
                                this->root = temp->doubleRotateRightLeft();
                            }
                        }
                    }
                    else {
                        if (temp->left->right && temp->left->left){
                            if (temp->left->left->height >= temp->left->right->height){
                                if (parent->right == temp){
                                    parent->right = temp->singleRotateRight();
                                }
                                else if (parent->left == temp){
                                    parent->left = temp->singleRotateRight();
                                }
                                else {
                                    this->root = temp->singleRotateRight();
                                }
                            }
                            else {
                                if (parent->right == temp){
                                    parent->right = temp->doubleRotateLeftRight();
                                }
                                else if (parent->left == temp){
                                    parent->left = temp->doubleRotateLeftRight();
                                }
                                else {
                                    this->root = temp->doubleRotateLeftRight();
                                }
                            }
                        }
                        else if(temp->left->left){
                            if (parent->right == temp){
                                parent->right = temp->singleRotateRight();
                            }
                            else if (parent->left == temp){
                                parent->left = temp->singleRotateRight();
                            }
                            else {
                                this->root = temp->singleRotateRight();
                            }
                        }
                        else if (temp->left->right){
                            if (parent->right == temp){
                                parent->right = temp->doubleRotateLeftRight();
                            }
                            else if (parent->left == temp){
                                parent->left = temp->doubleRotateLeftRight();
                            }
                            else {
                                this->root = temp->doubleRotateLeftRight();
                            }
                        }
                    }
                }
            }
            else if (temp->right){
                if (temp->right->right && temp->right->left){
                    if (temp->right->right->height >= temp->right->left->height){
                        if (parent->right == temp){
                            parent->right = temp->singleRotateLeft();
                        }
                        else if (parent->left == temp){
                            parent->left = temp->singleRotateLeft();
                        }
                        else {
                            this->root = temp->singleRotateLeft();
                        }
                    }
                    else {
                        if (parent->right == temp){
                            parent->right = temp->doubleRotateRightLeft();
                        }
                        else if (parent->left == temp){
                            parent->left = temp->doubleRotateRightLeft();
                        }
                        else {
                            this->root = temp->doubleRotateRightLeft();
                        }
                    }
                }
                else if (temp->right->right){
                    if (parent->right == temp){
                        parent->right = temp->singleRotateLeft();
                    }
                    else if (parent->left == temp){
                        parent->left = temp->singleRotateLeft();
                    }
                    else {
                        this->root = temp->singleRotateLeft();
                    }
                }
                else if (temp->right->left){
                    if (parent->right == temp){
                        parent->right = temp->doubleRotateRightLeft();
                    }
                    else if (parent->left == temp){
                        parent->left = temp->doubleRotateRightLeft();
                    }
                    else {
                        this->root = temp->doubleRotateRightLeft();
                    }
                }
            }
            else if (temp->left){
                if (temp->left->right && temp->left->left){
                    if (temp->left->left->height >= temp->left->right->height){
                        if (parent->right == temp){
                            parent->right = temp->singleRotateRight();
                        }
                        else if (parent->left == temp){
                            parent->left = temp->singleRotateRight();
                        }
                        else {
                            this->root = temp->singleRotateRight();
                        }
                    }
                    else {
                        if (parent->right == temp){
                            parent->right = temp->doubleRotateLeftRight();
                        }
                        else if (parent->left == temp){
                            parent->left = temp->doubleRotateLeftRight();
                        }
                        else {
                            this->root = temp->doubleRotateLeftRight();
                        }
                    }
                }
                else if(temp->left->left){
                    if (parent->right == temp){
                        parent->right = temp->singleRotateRight();
                    }
                    else if (parent->left == temp){
                        parent->left = temp->singleRotateRight();
                    }
                    else {
                        this->root = temp->singleRotateRight();
                    }
                }
                else if (temp->left->right){
                    if (parent->right == temp){
                        parent->right = temp->doubleRotateLeftRight();
                    }
                    else if (parent->left == temp){
                        parent->left = temp->doubleRotateLeftRight();
                    }
                    else {
                        this->root = temp->doubleRotateLeftRight();
                    }
                }
            }
            for (int j = i; j >= 0; j--){
                if (path.at(j)->right && path.at(j)->left) {
                    path.at(j)->updateHeight();
                }
                else if (path.at(j)->right){
                    path.at(j)->height = path.at(j)->right->height + 1;
                }
                else if (path.at(j)->left){
                    path.at(j)->height = path.at(j)->left->height + 1;
                }
                else {
                    path.at(j)->height = 0;
                }
            }
        }
    }
}

/* remove(const T&)
 * Removes a node with the given item from the AVL Tree
 *  parameters:
 *  item, value to be removed from the AVL Tree
 *
 *  return value:
 *
 */
template <typename T>
void AVLTree<T>::remove(const T &item){
    if(!this->root){
        return;
    }
    AVLNode<T>* toRemove = this->root;
    AVLNode<T>* parent = nullptr;
    vector<AVLNode<T>*> path;
    while((toRemove->data < item || item < toRemove->data)
          && (toRemove->right || toRemove->left)){
        path.push_back(toRemove);
        parent = toRemove;
        if (item < toRemove->data){
            if (toRemove->left){
                toRemove = toRemove->left;
            }
            else {
                return;
            }
        }
        else if (toRemove->data < item){
            if (toRemove->right){
                toRemove = toRemove->right;
            }
            else {
                return;
            }
        }
    }
    if (toRemove == nullptr) {
        return;
    }
    int ndx = path.size();
    bool check = false;
    if (!(toRemove->data < item) && !(item < toRemove->data)){
        AVLNode<T>* child = nullptr;
        if (toRemove->left && toRemove->right){
            AVLNode<T>* toNull = nullptr;
            child = toRemove->right;
            path.push_back(child);
            while(child->left){
                toNull = child;
                child = child->left;
                path.push_back(child);
                check = true;
            }
            if (check){
                path.pop_back();
                if (ndx != path.size()){
                    path.insert(path.begin() + ndx, child);
                }
            }
            if (toNull){
                if (child->right){
                    toNull->left = child->right;
                }
                else {
                    toNull->left = nullptr;
                }
            }
            child->left = toRemove->left;
            if (child != toRemove->right){
                child->right = toRemove->right;
            }
        }
        else if (toRemove->left){
            child = toRemove->left;
            path.push_back(child);
        }
        else if (toRemove->right){
            child = toRemove->right;
            path.push_back(child);
        }
        toRemove->left = nullptr;
        toRemove->right = nullptr;
        if (parent == nullptr){
            this->root = child;
        }
        else if (toRemove == parent->right){
            parent->right = child;
        }
        else if (toRemove == parent->left){
            parent->left = child;
        }
        else {
            this->root = child;
        }
        delete toRemove;
        for (int j = path.size() - 1; j >= 0; j--) {
            if (path.at(j)->right && path.at(j)->left) {
                path.at(j)->updateHeight();
            }
            else if (path.at(j)->right){
                path.at(j)->height = path.at(j)->right->height + 1;
            }
            else if (path.at(j)->left){
                path.at(j)->height = path.at(j)->left->height + 1;
            }
            else {
                path.at(j)->height = 0;
            }
        }
    }
    else {
        return;
    }
    this->rebalancePathToRoot(path);
}



/* printLevelOrder(ostream&, string) const
 * Prints the data of the AVLNode and its children in level order traversal, uses BFS
 *  parameters:
 *  os, ostream reference for output
 *
 *  return value:
 *
 */
const int countMAX = 19;
template <typename T>
void AVLTree<T>::printLevelOrder(ostream &os) const{
    int count = 0, count2 = 0;
    queue<AVLNode<T>*> queue, queue2;
    if (!this->root){
        os << "NULL" << endl;
        return;
    }
    AVLNode<T>* temp = this->root;
    queue.push(temp);
    queue2.push(temp);
    while (!queue.empty()){
        if (temp->left){
            queue.push(temp->left);
            queue2.push(temp->left);
        }
        else {
            queue.push(nullptr);
            queue2.push(nullptr);
            count2++;
        }
        if (temp->right) {
            queue.push(temp->right);
            queue2.push(temp->right);
        }
        else {
            queue.push(nullptr);
            queue2.push(nullptr);
            count2++;
        }
        queue.pop();
        if (!queue.empty()){
            temp = queue.front();
            while (!temp){
                queue.pop();
                temp = queue.front();
                if (queue.empty()){
                    break;
                }
            }
        }
    }
    while (!queue2.empty() && count2 > 0){
        temp = queue2.front();
        if (temp){
            os << queue2.front()->getData();
            if (count < countMAX){
                os << " ";
            }
        }
        else {
            os << "NULL";
            count2--;
            if (count2 > 0 && count < countMAX){
                os << " ";
            }
        }
        count++;
        if (count > countMAX){
            os << endl;
            count = 0;
        }
        queue2.pop();
    }
    os << endl;
    return;
}


/* printPreorder(ostream&, string) const
 * Prints the data of the BSTNode and its children in preorder traversal
 *  parameters:
 *  os, ostream reference for output
 *  indent, string for indentation in the output
 *
 *  return value:
 *
 */
template <typename T>
void AVLNode<T>::printPreorder(ostream &os, string indent) const{
    os << indent << this->data << endl;
    indent += "  ";
    if (this->left){
        this->left->printPreorder(os, indent);
    }
    else {
        os << indent << "NULL" << endl;
    }
    if (this->right){
        this->right->printPreorder(os, indent);
    }
    else {
        os << indent << "NULL" << endl;
    }
    return;
}

/* encrypt(const T&) const
 * Encrypts the given item and returns its code path
 *  parameters:
 *      item - value to be encrypted
 *
 *  return value:
 *  Encrypted code path as a string
 *  Returns '?' if item is not in tree
 */
template <typename T>
string EncryptionTree<T>::encrypt(const T &item) const{
    if (!this->root){
        return "?";
    }
    string code;
    const AVLNode<T>* temp = this->root;
    while (true){
        if (!(item < temp->getData()) && !(temp->getData() < item)){
            if (code.empty()){
                code += 'r';
            }
            return code;
        }
        else if (item < temp->getData()){
            if (code.empty()){
                code += 'r';
            }
            if (temp->getLeft()){
                code += '0';
                temp = temp->getLeft();
            }
            else {
                return "?";
            }
        }
        else {
            if (code.empty()){
                code += 'r';
            }
            if (temp->getRight()){
                code += '1';
                temp = temp->getRight();
            }
            else {
                return "?";
            }
        }
    }
}

/* decrypt(const string&) const
 * Decrypts the code path and returns the corresponding item
 *  parameters:
 *  path, code path to be decrypted
 *
 *  return value:
 *  Pointer to the decrypted item
 *  Nullptr if path is invalid
 */
template <typename T>
const T* EncryptionTree<T>::decrypt(const string &path) const{
    if (!this->root){
        return nullptr;
    }
    const AVLNode<T>* temp = this->root;
    for (int i = 0; i < path.length(); i++){
        if(path.at(0) != 'r'){
            return nullptr;
        }
        else if (path.at(i) == '0'){
            if (temp->getLeft()){
                temp = temp->getLeft();
            }
            else {
                return nullptr;
            }
        }
        else if (path.at(i) == '1'){
            if (temp->getRight()){
                temp = temp->getRight();
            }
            else {
                return nullptr;
            }
        }
    }
    return &temp->getData();
}

#endif
