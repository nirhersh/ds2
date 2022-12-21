#include "Player.h"

Player::Player(int playerId, int teamId, const permutation_t& spirit, int gamesPlayed,int ability, 
        int cards,  bool goalKeeper) : m_playerId(playerId), m_teamId(teamId), m_spirit(spirit),
        m_gamesPlayed(gamesPlayed), m_ability(ability), m_cards(cards), m_goalkeeper(goalKeeper){}

int Player::getPlayerId() const{
    return m_playerId;
}

int Player::getTeamId() const{
    return m_teamId;
}

int Player::getCards() const{
    return m_cards;
}

int Player::getGamesPlayed() const{
    return m_gamesPlayed;
}

const permutation_t& Player::getSpirit() const{
    return m_spirit;
}

bool Player::isGoalkeeper() const{
    return m_goalkeeper;
}

void Player::addCards(int cards){
    m_cards += cards;
}

void Player::addGames(int games){
    m_gamesPlayed += games;
}
