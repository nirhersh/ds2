#include "Player.h"

Player::Player(int playerId, int teamId, const permutation_t& spirit, int gamesPlayed,int ability, 
        int cards,  bool goalKeeper) : m_playerId(playerId), m_teamId(teamId), m_gamesPlayed(gamesPlayed),
         m_ability(ability), m_cards(cards), m_goalkeeper(goalKeeper), m_spirit(spirit){}

int Player::get_player_id() const{
    return m_playerId;
}

int Player::get_team_id() const{
    return m_teamId;
}

int Player::get_cards() const{
    return m_cards;
}

int Player::get_games_played() const{
    return m_gamesPlayed;
}

const permutation_t& Player::get_spirit() const{
    return m_spirit;
}

bool Player::is_goalkeeper() const{
    return m_goalkeeper;
}

void Player::add_cards(int cards){
    m_cards += cards;
}

void Player::add_games(int games){
    m_gamesPlayed += games;
}
