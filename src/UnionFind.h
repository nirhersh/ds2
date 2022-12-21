#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "Team.h"
#include "Player.h"
#include "wet2util.h"

#include <math.h>
#include "assert.h"

//#include "Exception.h"

class UnionFind{
public:
    class Node;
    class HashTable;
    
    
private:
    




};

class UnionFind::Node{
public:
    
    Node(Player* player, const permutation_t& partialSpirit, Team* team, Node* parent);

    Node(const Node& other) = delete;

    Node& operator=(const Node& other) = delete;

    void set_parent(Node* parent);

    void set_team(Team* team);

    void update_partial_spirit(const permutation_t& newPerm);


    Player* get_player(){
        return m_player;
    }

private:
    Node* m_parent;
    Team* m_team;
    Player* m_player;
    permutation_t m_partialSpirit;
    friend class UnionFind;
    friend class UnionFind::HashTable;
};

class UnionFind::HashTable{
public:
    HashTable();

    ~HashTable();

    HashTable(const HashTable& other) = delete;

    HashTable& operator=(const HashTable& other) = delete;

    void add(int playerId, Node* node);

    Node* find(int playerId) const;


    int get_array_size(){
        return m_arraySize;
    }


private:
    UnionFind::Node** m_nodesArray;
    int m_exponent;
    int m_currentSize;
    int m_arraySize;
    static const int MAX_LOAD_FACTOR = 0.75;
    double calculate_load_factor() const;
    void rehash();

};

#endif //UUNION_FIND_H