
#ifndef AVLTREE_H
#define AVLTREE_H

#include "wet2util.h"
#include "Exception.h"
#include <assert.h>
#include <iostream>

template <class T, class K>
class AVLTree{
public:
    /*
    C'tor of the AVL Tree
    */
    AVLTree();

    ~AVLTree();

    AVLTree(const AVLTree<T, K>& other) = delete;
    
    AVLTree<T, K>& operator=(const AVLTree<T, K>& other) = delete;

    /*
    Sub class of Nodes that hold the data in the tree
    */
    class Node;

    /*
    A function that searches for a value in the tree
    @param key: The key of the searched data
    @return: a pointer to the data
    */
    T* search(const K& key) const;

    /*
    A function that pushes a key-value pair into the tree
    @param key: The key of the pushed data
    @param item: the item to push into the tree
    @return: a pointer to the data
    */
    void push(T* item, const K& key);
    
    /*
    A function that removes for a value from the tree
    @param key: The key of the removed data
    */
    void remove(const K& key);

    /*
    A function that returns the amount of nodes in the tree
    @return size of the tree
    */
    int get_size()const;

    /*
    A function that returns the hieght of the tree
    @return: the height of the tree
    */
    int get_height()const;

    /*
    A function that returns the tree data in order
    @param array: The array to save the in order array to
    */
    void in_order(T** array) const;

    void in_order(Node* array[]) const;

    void print_tree();

    bool exists(const K& key) const;

    Node* getRoot() const;

    static void fix_tree_height(Node*);

    bool check_balance_factor(Node* node);
 
    static void unite_trees(AVLTree<T, K>& tree1, AVLTree<T, K>& tree2, AVLTree<T, K>& newTree);

    T* get_following_value(const K& key) const;

    T* get_preceding_value(const K& key) const;

    bool is_empty()const;

    T* binary_search_closest(K key);


private:
    Node* root;
    int m_size;
    void recurtion_dtor(Node* node);
    static void fix_height(Node* node1);
    void balance_tree(Node* newNode);
    Node* search_node(const K& key) const;
    Node* remove_node(Node* nodeToRemove);
    Node* get_following_node(Node* node)const;
    Node* get_preceding_node(Node* node)const;
    void switch_nodes(Node* node1,Node* node2);
    int in_order_recursion(T** array, Node* current, int index = 0) const;
    int in_order_recursion(Node* array[], Node* current, int index = 0) const;
    static void merge(Node* arrayTree1[], int na, Node* arrayTree2[], int nb, Node* newArrayTree[]);
    static Node* array_to_tree(Node** arrayTree, int start, int end);
    int calc_BF(Node* node);
    void LL_fix(Node* node);
    void RL_fix(Node* node);
    void LR_fix(Node* node);
    void RR_fix(Node* node);
    
};

template <class T, class K>
class AVLTree<T, K>::Node{
public:
    Node(const Node&) = delete;
    Node& operator=(const Node& other) = delete;
    K get_key();
    T* get_data(){
        return m_data;
    }
private:
    T* m_data;
    K m_key;
    int m_height; 
    Node* m_right;
    Node* m_left; 
    Node* m_parent;
    Node(T* data, const K& key);
    friend class AVLTree<T, K>;
};

template <class T, class K>
bool AVLTree<T, K>::is_empty()const{
    return m_size == 0 ? true : false;
}


//-------------------- Node Implemantation --------------------
template <class T, class K>
AVLTree<T, K>::Node::Node(T* data, const K& key) : m_data(data), m_key(key), m_height(0),
                         m_right(nullptr), m_left(nullptr), m_parent(nullptr){}

template <class T, class K>
K AVLTree<T, K>::Node::get_key()
{
    return m_key;
}

//-------------------- AVLTree Implemantation --------------------
template <class T, class K>
AVLTree<T, K>::AVLTree(): root(nullptr), m_size(0){}

template <class T, class K>
T* AVLTree<T, K>::search(const K& key) const{
    Node* nodeToReturn = search_node(key);
    return nodeToReturn->m_data;
}

template <class T, class K>
void AVLTree<T, K>::unite_trees(AVLTree<T, K>& tree1, AVLTree<T, K>& tree2, AVLTree<T, K>& newTree)
{

    int tree1Size = tree1.get_size();
    int tree2Size = tree2.get_size();
    typename AVLTree<T, K>::Node** arrayOfTree1 = nullptr;
    typename AVLTree<T, K>::Node** arrayOfTree2 = nullptr;
    if(tree1Size == 0 && tree2Size == 0){
        return;
    }
    if(tree1Size != 0){
        arrayOfTree1 = new typename AVLTree<T, K>::Node*[tree1Size];
        for(int i=0; i<tree1Size; i++){
            arrayOfTree1[i] = nullptr;
        }
    } 
    if(tree2Size != 0){
        arrayOfTree2 = new typename AVLTree<T, K>::Node*[tree2Size];
        for(int i=0; i<tree2Size; i++){
            arrayOfTree2[i] = nullptr;
        }
    } 
    tree1.in_order(arrayOfTree1);
    tree2.in_order(arrayOfTree2);
    typename AVLTree<T, K>::Node** newArrayTree = new typename AVLTree<T, K>::Node*[tree1Size + tree2Size];
    merge(arrayOfTree1, tree1Size, arrayOfTree2,  tree2Size, newArrayTree); 
    newTree.root = array_to_tree(newArrayTree, 0, tree1Size + tree2Size - 1);
    if(newTree.root)
    {
        if(newTree.root->m_left){
            newTree.root->m_left->m_parent = newTree.root;
        }
        if(newTree.root->m_right){
            newTree.root->m_right->m_parent = newTree.root;
        }
    }
    newTree.m_size = tree1Size + tree2Size;
    fix_tree_height(newTree.root);
    if(arrayOfTree1){
        delete[] arrayOfTree1;    
    }
    if(arrayOfTree2){
        delete[] arrayOfTree2;    
    }
    if(newArrayTree){
        delete[] newArrayTree;    
    }
}

template <class T, class K>
void AVLTree<T, K>::merge(typename AVLTree<T, K>::Node* arrayTree1[], int na, typename AVLTree<T, K>::Node* arrayTree2[], 
                                                      int nb, typename AVLTree<T, K>::Node* newArrayTree[])
{
    int ia, ib, ic;
    try{
            for(ia = ib = ic = 0; (ia < na) && (ib < nb); ic++){
                if(arrayTree1[ia]->m_key < arrayTree2[ib]->m_key){
                    newArrayTree[ic] = arrayTree1[ia];
                    ia++;
                }
                else {
                    newArrayTree[ic] = arrayTree2[ib];    
                    ib++;
                }
            }
        for(; ia < na; ia++, ic++) newArrayTree[ic] = arrayTree1[ia];
        for(; ib < nb; ib++, ic++) newArrayTree[ic] = arrayTree2[ib];
    }catch (KeyAlreadyExists& err){
            assert(1);
    }
}


template <class T, class K>
typename AVLTree<T, K>::Node* AVLTree<T, K>::array_to_tree(typename AVLTree<T, K>::Node** arrayTree, int start, int end)
{
    if(start > end){
        return nullptr;
    }
    int mid = (start + end) / 2;
    assert(arrayTree[mid] != nullptr);
    typename AVLTree<T, K>::Node* newNode (new typename AVLTree<T, K>::Node(arrayTree[mid]->m_data, arrayTree[mid]->m_key ));
    newNode->m_left = array_to_tree(arrayTree, start, mid - 1);
    if(newNode->m_left){
        newNode->m_left->m_parent = newNode;
    }

    newNode->m_right = array_to_tree(arrayTree, mid + 1, end);
    if(newNode->m_right){
        newNode->m_right->m_parent = newNode;
    }

    return newNode;
}

template <class T, class K>
void AVLTree<T, K>::fix_tree_height(Node* node)
{
    if(!node){
        return;
    }
    fix_tree_height(node->m_left);
    fix_tree_height(node->m_right);
    fix_height(node);
}

template <class T, class K>
bool AVLTree<T, K>::check_balance_factor(Node* node)
{
    if(!node)
    {
        return true;
    }
    int balanceFactor = calc_BF(node);
    bool valid = ((balanceFactor >= -1) && (balanceFactor <= 1)) ? true: false;
    if(!valid){
        return false;
    }
    return ((check_balance_factor(node->m_left) && check_balance_factor(node->m_right)))? true : false;
}

template <class T, class K>
void AVLTree<T, K>::push(T* item, const K& key){
    assert(item != nullptr);
    bool found = false;
    Node* currentNode = root;
    if(root == nullptr){
        Node* newNode(new AVLTree<T, K>::Node(item, key));
        root = newNode;
        found = true;
    }
    while (!found)
    {
        if(currentNode->m_key == key)
        {
            throw KeyAlreadyExists();
        }
        if (key > currentNode->m_key){
            if(currentNode->m_right == nullptr)
            {
                Node* newNode(new AVLTree<T, K>::Node(item, key));
                newNode->m_parent = currentNode;
                currentNode->m_right = newNode;
                balance_tree(newNode);
                found = true;
            } else {
                currentNode = currentNode->m_right;
            }
        }else{
            if((currentNode->m_left) == nullptr)
            {
                Node* newNode(new AVLTree<T, K>::Node(item, key));
                newNode->m_parent = currentNode;
                currentNode->m_left = newNode;
                balance_tree(newNode);
                found = true;
            } else {
                currentNode = ((currentNode)->m_left);
            }
        }
    }
    m_size++;
}

template<class T, class K>
void AVLTree<T, K>::balance_tree(Node* newNode)
{
    assert(newNode != nullptr);
    Node* tempNode = newNode;
    assert(tempNode->m_parent != nullptr);
    while(tempNode != root && tempNode->m_parent != nullptr)
    {
        assert(tempNode->m_parent);
        if(tempNode->m_parent->m_height >= tempNode->m_height + 1)
        {
            return;
        }
        tempNode->m_parent->m_height = tempNode->m_height + 1;
        int balance =calc_BF(tempNode->m_parent);
        if(balance == 2)
        {
            if(calc_BF(tempNode) > -1)
            {
                LL_fix(tempNode->m_parent);
            } else {
                LR_fix(tempNode->m_parent);
            }
            break;
        }
        else if(balance == -2){
            if(calc_BF(tempNode) < 1) {
                RR_fix(tempNode->m_parent);
            } else {
                RL_fix(tempNode->m_parent);
            }
            break;
        }
        tempNode = tempNode->m_parent;
    }

}

template<class T, class K>
void AVLTree<T, K>::LL_fix(Node* parent){
    Node* B = parent;
    Node* A = parent->m_left;
    Node* AR = A->m_right;
    assert(parent != nullptr || A != nullptr || B!= nullptr);
    A->m_parent = B->m_parent;
    A->m_right = B;
    B->m_left = AR;
    B->m_parent = A;
    if(A->m_parent)
    {
        if(A->m_parent->m_left == B){
            A->m_parent->m_left = A;
        }else{
            A->m_parent->m_right = A;
        }
    }
    if(AR != nullptr){
        AR->m_parent = B;
    }
    if(B == root){
        root = A;
    }

    
    fix_height(AR);
    fix_height(B);
    fix_height(A);
    //fix_tree_height(root);
}

template<class T, class K>
void AVLTree<T, K>::LR_fix(Node* parent)
{
    RR_fix(parent->m_left);
    LL_fix(parent);
}

template<class T, class K>
void AVLTree<T, K>::RL_fix(Node* parent){
    LL_fix(parent->m_right);
    RR_fix(parent);
}

template<class T, class K>
void AVLTree<T, K>::RR_fix(Node* parent)
{
    Node* B = parent;
    Node* BParent = parent->m_parent;
    Node* A = B->m_right;
    Node* AL = A->m_left;
    assert(parent != nullptr || A != nullptr || B!= nullptr);
    A->m_parent = BParent;
    A->m_left = B;
    B->m_right = AL;
    B->m_parent = A;
    if(AL != nullptr){
        AL->m_parent = B;
    }
    if(B == root){
        root = A;
    }
    if(A->m_parent)
    {
        if(A->m_parent->m_left == B){
            A->m_parent->m_left = A;
        }else{
            A->m_parent->m_right = A;
        }
    }
    
    fix_height(AL);
    fix_height(B);
    fix_height(A);
    //fix_tree_height(root);
}

template<class T, class K>
int AVLTree<T, K>::calc_BF(Node* node)
{
    if(node == nullptr){
        return -2;
    }
    int leftTreeH = node->m_left == nullptr ? -1 : node->m_left->m_height;
    int rightTreeH = node->m_right == nullptr ? -1 : node->m_right->m_height;
    return leftTreeH - rightTreeH;
}

template<class T, class K>
void AVLTree<T, K>::fix_height(Node* node1)
{
    if(!node1){
        return;
    }
    int leftTreeH = node1->m_left == nullptr ? -1 : node1->m_left->m_height;
    int rightTreeH = node1->m_right == nullptr ? -1 : node1->m_right->m_height;
    node1->m_height = leftTreeH > rightTreeH ? 1 + leftTreeH : 1 + rightTreeH;
}

template<class T, class K>
void AVLTree<T, K>::remove(const K& key)
{
    Node* nodeToRemove = search_node(key);
    Node* currentNode = remove_node(nodeToRemove); 
    if(currentNode == nullptr){
        m_size = 0;
        return;
    }
    Node* parent = currentNode->m_parent;
    do{
        int parentHeight = currentNode->m_parent? currentNode->m_parent->m_height: 0;
        fix_height(currentNode);
        balance_tree(currentNode);
        if(parent == nullptr || parentHeight == parent->m_height){
            break;
        }
        currentNode = currentNode->m_parent;
        parent = currentNode->m_parent;
    } while(currentNode != root);
    m_size--;


}

template<class T, class K>
typename AVLTree<T, K>::Node* AVLTree<T, K>::search_node(const K& key) const
{
    bool found = false;
    Node* currentNode = root;
    while (!found)
    {
        
        if(currentNode == nullptr)
        {
            throw KeyDoesntExists();
        }
        if(currentNode && currentNode->m_left){
            currentNode->m_left->m_parent = currentNode;
        }
        if(currentNode && currentNode->m_right){
            currentNode->m_right->m_parent = currentNode;
        }
        if (key > currentNode->m_key){
            currentNode = currentNode->m_right;
        }else if(key < currentNode->m_key){
            currentNode = currentNode->m_left;
        }else{
            found = true;
        }
    }
    return currentNode;
}

template<class T, class K>
typename AVLTree<T, K>::Node* AVLTree<T, K>::remove_node(Node* nodeToRemove){
    bool isRight = false;
    assert(nodeToRemove);
    if(nodeToRemove == nullptr){
        return nodeToRemove;
    }
    if(nodeToRemove->m_parent != nullptr && nodeToRemove->m_parent->m_right == nodeToRemove){
            isRight = true;
    }
    Node* nodeToReturn;
    if (nodeToRemove->m_right == nullptr && nodeToRemove->m_left == nullptr)

    {
        
        if(nodeToRemove != root && nodeToRemove->m_parent){
            if(!isRight){
                nodeToRemove->m_parent->m_left = NULL; 
            }
            else{
                nodeToRemove->m_parent->m_right = nullptr;
            }
        } else {
            root = nullptr;
        }
        nodeToReturn = nodeToRemove->m_parent;
        delete nodeToRemove; 
    }
    else if(nodeToRemove->m_right != nullptr && nodeToRemove->m_left == nullptr){
        if(nodeToRemove != root){
            if(isRight){
                nodeToRemove->m_parent->m_right = nodeToRemove->m_right;
            }
            else {
                nodeToRemove->m_parent->m_left = nodeToRemove->m_right;
            }
        }else{
            root = root->m_right;
        }
        nodeToRemove->m_right->m_parent = nodeToRemove->m_parent;
        nodeToReturn = nodeToRemove->m_right;
        delete nodeToRemove;
    }
    else if(nodeToRemove->m_right == nullptr && nodeToRemove->m_left != nullptr){
        if(nodeToRemove != root){
            if(isRight){
                nodeToRemove->m_parent->m_right = nodeToRemove->m_left;
            }
            else {
                nodeToRemove->m_parent->m_left = nodeToRemove->m_left;
            }
        }else{
            root = root->m_left;
        }
        nodeToRemove->m_left->m_parent = nodeToRemove->m_parent;
        nodeToReturn = nodeToRemove->m_left;
        delete nodeToRemove;
    }
    else{
        Node* followingNode = get_following_node(nodeToRemove);
        switch_nodes(nodeToRemove, followingNode);
        nodeToReturn = remove_node(followingNode);    
    }
    return nodeToReturn;
}

template<class T, class K>
void AVLTree<T, K>::switch_nodes(Node* node1, Node* node2){
    T* node1Data = node1->m_data;
    T* node2Data = node2->m_data;
    K key1 = node1->m_key;
    K key2 = node2->m_key;
    node1->m_data = node2Data;
    node1->m_key = key2;
    node2->m_data = node1Data;
    node2->m_key = key1;
}




template<class T, class K> 
typename AVLTree<T, K>::Node* AVLTree<T, K>::get_following_node(Node* node) const //if there is no right son return the parent
{
    if(node == nullptr){
        throw KeyDoesntExists();
    }
    Node* tempNode = node;
    Node* closest = nullptr;
    if(node->m_right){
        tempNode = node->m_right;
        while(tempNode->m_left){
            tempNode = tempNode->m_left;
        }
        closest = tempNode;
    }else{
        if(!tempNode->m_parent){
            return nullptr;
        }
        closest = nullptr;
        while(tempNode != nullptr){
            if(tempNode->get_key() > node->m_key)
            {
                if(closest == nullptr){
                    closest = tempNode;
                } else if(tempNode->get_key() < closest->get_key()){
                    closest = tempNode;
                }

            }
            tempNode = tempNode->m_parent;
        }
    }
    return closest; //if most right leaf returns nullptr
}


template<class T, class K>
int AVLTree<T, K>::get_size()const{
    return m_size;
}

template<class T, class K>
void AVLTree<T ,K>::in_order(T** array) const{
    if(array == nullptr){
        return;
    }
    in_order_recursion(array, root, 0);
}

template<class T, class K>
void AVLTree<T ,K>::in_order(Node* array[]) const{
    if(array == nullptr){
        return;
    }
    in_order_recursion(array, root, 0);
}


template<class T, class K>
int AVLTree<T, K>::in_order_recursion(T** array, Node* current, int index) const
{
    if(current == nullptr){
        return index;
    }

    index = in_order_recursion(array, current->m_left, index);
    if(index >= m_size){
        return index;
    }
    array[index++] = current->m_data;
    return in_order_recursion(array, current->m_right, index);
}

template<class T, class K>
int AVLTree<T, K>::in_order_recursion(Node* array[], Node* current, int index) const
{
    if(current == nullptr){
        return index;
    }
    index = in_order_recursion(array, current->m_left, index);
    if(index >= m_size){
        return index;
    }
    array[index++] = current;
    return in_order_recursion(array, current->m_right, index);
}


template<class T, class K>
bool AVLTree<T, K>::exists(const K& key) const
{
    try{
        search_node(key);
    } catch (KeyDoesntExists& var) {
        return false;
    }
    return true;
}

template<class T, class K>
T* AVLTree<T, K>::get_following_value(const K& key) const{
    if(is_empty()){
        throw EmptyTree();
    }
    Node* nodeToReturn = get_following_node(search_node(key));
    if(nodeToReturn == nullptr){
        return nullptr;
    }else{
        return nodeToReturn->m_data;
    }
}

template<class T, class K>
T* AVLTree<T, K>::get_preceding_value(const K& key) const{
    if(is_empty()){
        throw EmptyTree();
    }
    Node* nodeToReturn = get_preceding_node(search_node(key));
    if(nodeToReturn == nullptr){
        return nullptr;
    }else{
        return nodeToReturn->m_data;
    }
}

template<class T, class K> 
typename AVLTree<T, K>::Node* AVLTree<T, K>::get_preceding_node(Node* node) const
{
    if(node == nullptr){
        throw KeyDoesntExists();
    }
    Node* tempNode = node;
    Node* closest = nullptr;
    if(node->m_left){
        tempNode = node->m_left;
        while(tempNode->m_right){
            tempNode = tempNode->m_right;
        }
        closest = tempNode;
    }else{
        if(!tempNode->m_parent){
            return nullptr;
        }
        closest = nullptr;
        while(tempNode != nullptr){
            if(tempNode->get_key() < node->m_key)
            {
                if(closest == nullptr){
                    closest = tempNode;
                } else if(tempNode->get_key() > closest->get_key()){
                    closest = tempNode;
                }
            }
            tempNode = tempNode->m_parent;
        }
    }
    return closest; //if most right leaf returns nullptr
}

template<class T, class K> 
AVLTree<T, K>::~AVLTree()
{
    recurtion_dtor(root);
}

template<class T, class K> 
void AVLTree<T, K>::recurtion_dtor(Node* node)
{
    if(!node){
        return;
    }
    recurtion_dtor(node->m_left);
    recurtion_dtor(node->m_right);
    delete node;
}

template<class T, class K> 
int AVLTree<T, K>::get_height() const
{
    fix_tree_height(root);
    return root->m_height;
}

template<class T, class K> 
typename AVLTree<T, K>::Node* AVLTree<T, K>::getRoot() const
{
    return root;
}

template<class T, class K> 
T* AVLTree<T, K>::binary_search_closest(K key)
{
    if(is_empty()){
        return nullptr;
    }
    Node* closest = nullptr;
	Node* current = root;
	while(current)
	{
        if(key < current->m_key){
            if(closest == nullptr){
                closest = current;
            }else{
                if(current->m_key - key < closest->m_key - key){
                    closest = current;
                }
            }
            current = current->m_left;
        }else if(key > current->m_key){
            current = current->m_right;
        }else{
            closest = current;
            break;
        }
	}
    if(closest == nullptr){
        return nullptr;
    }else{
        return closest->m_data;
    }
}


#endif


