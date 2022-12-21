#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "wet2util.h"
#include "player.h"
#include "UnionFind.h"

class HashTable{
public:



private:
    UnionFind::Node** m_nodesArray;
    int m_exponent;
    int m_currentSize;


};

#endif HASH_TABLE_H