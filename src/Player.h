#ifndef TEAM_H
#define TEAM_H

#include "wet2util.h"

class Player{
public:
    /*
    Player C'tor
    */
    Player(int playerId, int teamId, const permutation_t& spirit, int gamesPlayed,
            int ability, int cards,  bool goalKeeper);

    int getPlayerId() const;

    int getTeamId() const;

    int getGamesPlayed() const;

    int getCards() const;

    bool isGoalkeeper() const;

    const permutation_t& getSpirit() const;

    void addCards(int cards);

    void addGames(int games);
    
private:
    int m_playerId;
    int m_teamId;
    int m_gamesPlayed;
    int m_ability;
    int m_cards;
    bool m_goalkeeper;
    permutation_t m_spirit;
};

#endif TEAM_H