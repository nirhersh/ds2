#include "UnionFind.h"


/*.......................UNION FIND IMPLEMENTATION............................*/





/*.......................NODE IMPLEMENTATION............................*/
UnionFind::Node::Node(Player* player, const permutation_t& partialSpirit, Team* team, Node* parent) : m_player(player),
    m_partialSpirit(partialSpirit), m_team(team), m_parent(parent){}

void UnionFind::Node::set_parent(Node* parent){
    m_parent = parent;
}

void UnionFind::Node::set_team(Team* team){
    m_team = team;
}

void UnionFind::Node::update_partial_spirit(const permutation_t& newPerm){
    m_partialSpirit = m_partialSpirit * newPerm;
}


/*.......................HASH TABLE IMPLEMENTATION............................*/
UnionFind::HashTable::HashTable() : m_exponent(8), m_currentSize(0) 
{
    m_arraySize = pow(2, m_exponent) - 1;
    m_nodesArray = new UnionFind::Node*[m_arraySize];
    for (int i = 0; i < m_arraySize; i++)
    {
        m_nodesArray[i] = nullptr;
    }
}

UnionFind::HashTable::~HashTable(){
    delete[] m_nodesArray;
}

UnionFind::Node* UnionFind::HashTable::find(int playerId) const
{
    int funcH = playerId % m_arraySize;
    int funcR = 1 + playerId % (((m_arraySize + 1) / 2) - 1); //the id, mod the last array size. have to be strangers
    for (int i = 0; i < m_arraySize; i++)
    {
        if(m_nodesArray[(funcH + i * funcR) % m_arraySize] == nullptr){
            //throw IdDoesntExists();
            return nullptr;
        }
        if((m_nodesArray[(funcH + i * funcR) % m_arraySize]->m_player->get_player_id() == playerId)){
            return m_nodesArray[(funcH + i * funcR) % m_arraySize];
        }
    }
    //throw IdDoesntExists();
    return nullptr;
}

double UnionFind::HashTable::calculate_load_factor() const{
    assert(m_arraySize != 0 && (double)((double)m_currentSize / (double)m_arraySize) != 0);
    return (double)((double)m_currentSize / (double)m_arraySize);
}

void UnionFind::HashTable::add(int playerId, UnionFind::Node* node){
    if(calculate_load_factor() > MAX_LOAD_FACTOR){
        rehash();
    }
    int funcH = playerId % m_arraySize;
    int funcR = 1 + playerId % (((m_arraySize + 1) / 2) - 1); //the id, mod the last array size. have to be strangers
    bool found = false;
    for(int i=0; i < m_arraySize; i++){
        if(m_nodesArray[(funcH + i * funcR) % m_arraySize] == nullptr){
            m_nodesArray[(funcH + i * funcR) % m_arraySize] = node;
            m_currentSize++;
            return;
        }   
    }
    assert(false);
}

void UnionFind::HashTable::rehash(){
    m_exponent++;
    int newSize = pow(2, m_exponent) - 1;
    Node** newNodesArray = new UnionFind::Node*[newSize];
    Node** tempArray = m_nodesArray;
    m_nodesArray = newNodesArray;
    for (int i = 0; i < newSize; i++)
    {
        m_nodesArray[i] = nullptr;
    }
    m_currentSize = 0;
    for(int i=0; i<m_arraySize; i++){
        if(tempArray[i] != nullptr){
            int playerId = tempArray[i]->m_player->get_player_id();
            add(playerId, tempArray[i]);
        }
    }
    delete[] tempArray;
    m_arraySize = newSize;
    m_nodesArray = newNodesArray;
}
