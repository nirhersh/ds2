#ifndef PLAYER_H
#define PLAYER_H

#include "wet2util.h"

class Player{
public:
    /*
    Player C'tor
    */
    Player(int playerId, int teamId, const permutation_t& spirit, int gamesPlayed =0,
            int ability=0, int cards=0,  bool goalKeeper=false);

    int get_player_id() const;

    int get_team_id() const;

    int get_games_played() const;

    int get_cards() const;

    bool is_goalkeeper() const;

    const permutation_t& get_spirit() const;

    void add_cards(int cards);

    void add_games(int games);
    
private:
    int m_playerId;
    int m_teamId;
    int m_gamesPlayed;
    int m_ability;
    int m_cards;
    bool m_goalkeeper;
    permutation_t m_spirit;
};

#endif //PLAYER_H