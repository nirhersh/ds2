#include "Team.h"

Team::Team(int teamId) : m_teamId(teamId), m_totalPoints(0), m_totalPlayersAbility(0),
                         m_gamesPlayed(0), m_goalkeeper(false){
    m_spiritStrength = permutation_t();
}

int Team::get_id() const
{
    return m_teamId;
}

int Team::get_total_points() const
{
    return m_totalPoints;
}

int Team::get_total_player_ability() const
{
    return m_totalPlayersAbility;
}

int Team::get_games_played() const
{
    return m_gamesPlayed;
}

const permutation_t& Team::get_spirit_strength() const
{
    return m_spiritStrength;
}

void Team::add_points(int points){
    m_totalPoints += points;
}

void Team::add_ability(int ability){
    m_totalPlayersAbility += ability;
}

void Team::add_games_played(int games){
    m_gamesPlayed += games;
}
void Team::update_spirit_strength(const permutation_t& newPerm){
    if(!m_spiritStrength.isvalid()){
        m_spiritStrength = newPerm;
    }else{
        m_spiritStrength = m_spiritStrength * newPerm; 
    }
}

void Team::has_goalkeeper(){
    m_goalkeeper = true;
}
