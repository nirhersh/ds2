#ifndef TEAM_H
#define TEAM_H

#include "wet2util.h"
#include "UnionFind.h"

class Team{
public: 
    Team(int teamId);

    int get_id() const;
    int get_total_points() const;
    int get_team_ability() const;
    int get_games_played() const;
    int get_num_of_players() const;
    const permutation_t& get_spirit_strength() const;
    UnionFind::Node* get_team_root() const;

    void add_points(int points);
    void add_ability(int ability);
    void add_games_played(int games);
    void add_cards(int cards);
    void update_spirit_strength(const permutation_t& newPerm);
    void has_goalkeeper();
    void set_team_root(UnionFind::Node* root);
    void add_player(Player* newPlayer);
    void unite_team(Team* newTeam);
    





private:
    int m_teamId;
    int m_totalPoints;
    int m_totalPlayersAbility;
    int m_gamesPlayed;
    int m_numOfPlayers;
    permutation_t m_spiritStrength;
    bool m_goalkeeper;
    UnionFind::Node* m_teamRoot;

};


#endif //TEAM_H
