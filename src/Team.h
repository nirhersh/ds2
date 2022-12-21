#ifndef TEAM_H
#define TEAM_H

#include "wet2util.h"

class Team{
public: 
    Team(int teamId);

    int get_id() const;
    int get_total_points() const;
    int get_total_player_ability() const;
    int get_games_played() const;
    const permutation_t& get_spirit_strength() const;

    void add_points(int points);
    void add_ability(int ability);
    void add_games_played(int games);
    void update_spirit_strength(const permutation_t& newPerm);
    void has_goalkeeper();

    





private:
    int m_teamId;
    int m_totalPoints;
    int m_totalPlayersAbility;
    int m_gamesPlayed;
    permutation_t m_spiritStrength;
    bool m_goalkeeper;


};


#endif //TEAM_H
