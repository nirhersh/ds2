#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "Player.h"
#include "wet2util.h"
#include <math.h>
#include "assert.h"
#include <iostream>
#include "Exception.h"
#include "AVLRankTree.h"
#include "List.h"

class Team;
class UnionFind{
public:
    class HashTable;
    //-------------- Node class declaration --------------
    class Node{
    public:
        
        Node(Player* player, const permutation_t& partialSpirit, Team* team, Node* parent, int gamesPlayed = 0);

        Node(const Node& other) = delete;

        Node& operator=(const Node& other) = delete;

        void set_parent(Node* parent);

        void set_team(Team* team);

        void update_partial_spirit(const permutation_t& newPerm);

        void update_games_played(int games);

        Player* get_player(){
            return m_player;
        }

    private:
        Node* m_parent;
        Team* m_team;
        Player* m_player;
        permutation_t m_partialSpirit;
        int m_gamesPlayed;
        friend class UnionFind;
        friend class UnionFind::HashTable;
    };

    //-------------- HashTable class declaration --------------
    class HashTable{
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
        List<UnionFind::Node>* m_nodesArray;
        int m_exponent;
        int m_currentSize;
        int m_arraySize;
        static constexpr double MAX_LOAD_FACTOR = 0.75;
        double calculate_load_factor() const;
        void rehash();

    };

    //-------------- UnionFind class declaration --------------

    UnionFind(AVLRankTree<Team, int>* teamsTree);

    ~UnionFind();

    UnionFind(const UnionFind& other) = delete;

    UnionFind& operator=(const UnionFind& other) = delete;

    void union_teams(Team* mainTeam, Team* secondTeam);
    
    Team* find(int playerId) const;

    void add_player(Player* player, int teamId);

    permutation_t get_partial_spirit(int playerId);

    int get_games_played(int PlayerId);

    HashTable* get_players_hash();

    bool exists(int PlayerId);
    
private:
    HashTable* m_players;
    AVLRankTree<Team, int>* m_teams;
};
#include "Team.h"
#endif //UNION_FIND_H