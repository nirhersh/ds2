#include "worldcup23a2.h"

world_cup_t::world_cup_t()
{
	Team* disQualifiedPlayers = new Team(DISQUALIFIED_PLAYERS_TEAM_ID);
	m_UFTeams.push(disQualifiedPlayers, DISQUALIFIED_PLAYERS_TEAM_ID);
	m_playersTeamsUF = new UnionFind(&m_UFTeams);
}

world_cup_t::~world_cup_t()
{
	// delete all teams
	Team** teams = new Team*[m_rankedTeams.get_size()];
	m_rankedTeams.in_order(teams);
	for(int i=0; i<m_rankedTeams.get_size(); i++){
		delete teams[i];
	}
	delete[] teams;
	// delete team of disqualified players
	m_UFTeams.remove(DISQUALIFIED_PLAYERS_TEAM_ID);
}

StatusType world_cup_t::add_team(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	Team* newTeam = nullptr;
	try{
		newTeam = new Team(teamId);
		m_rankedTeams.push(newTeam, TeamKey(teamId, 0));
		m_UFTeams.push(newTeam, teamId);
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}catch(KeyAlreadyExists& e){
		delete newTeam;
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if(teamId <= 0){
		return StatusType::FAILURE;
	}
	try{
		Team* teamToRemove = m_UFTeams.search(teamId);
		Team* disqualifiedPlayersTeam = m_UFTeams.search(DISQUALIFIED_PLAYERS_TEAM_ID);
		m_playersTeamsUF->union_teams(disqualifiedPlayersTeam, teamToRemove);
		m_rankedTeams.remove(TeamKey(teamId, teamToRemove->get_team_ability()));
		m_UFTeams.remove(teamId);
		delete teamToRemove;
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
	if(playerId <= 0 || teamId <= 0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0){
		return StatusType::INVALID_INPUT;
	}
	Player* newPlayer = nullptr;
	try{
		newPlayer = new Player(playerId, teamId, spirit, gamesPlayed, ability, cards, goalKeeper);
		m_playersTeamsUF->add_player(newPlayer, teamId);
		Team* playersTeam = m_UFTeams.search(teamId);
		m_rankedTeams.remove(TeamKey(teamId, playersTeam->get_team_ability()));
		playersTeam->add_ability(ability);
		playersTeam->update_spirit_strength(spirit);
		m_rankedTeams.push(playersTeam, TeamKey(teamId, playersTeam->get_team_ability()));
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}catch(IdDoesntExists& e){
		delete newPlayer;
		return StatusType::FAILURE;
	}catch(KeyDoesntExists& e){
		delete newPlayer;
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
		return StatusType::INVALID_INPUT;
	}
	try{
		Team* team1 = m_UFTeams.search(teamId1);
		Team* team2 = m_UFTeams.search(teamId2);
		int team1Ability = team1->get_total_points() + team1->get_team_ability();
		int team2Ability = team2->get_total_points() + team2->get_team_ability();
		if(team1Ability > team2Ability){
			team1->add_points(3);
			return TEAM1_WON_WITH_ABILITY;
		}else if(team1Ability < team2Ability){
			team2->add_points(3);
			return TEAM2_WON_WITH_ABILITY;
		}else{
			int team1Strength = team1->get_spirit_strength().strength();
			int team2Strength = team2->get_spirit_strength().strength();
			if(team1Strength > team2Strength){
				team1->add_points(3);
				return TEAM1_WON_WITH_TEAM_STRENGTH;
			}else if(team1Strength < team2Strength){
				team2->add_points(3);
				return TEAM2_WON_WITH_TEAM_STRENGTH;
			}else{
				team1->add_points(1);
				team2->add_points(1);
				return DRAW;
			}
		}
		team1->add_games_played(1);
		team2->add_games_played(1);
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}
	try{
		return(m_playersTeamsUF->get_games_played(playerId));
	} catch(IdDoesntExists& e){
		return StatusType::FAILURE;
	}
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}
	Player* playerToUpdate;
	UnionFind::HashTable* playersHash = m_playersTeamsUF->get_players_hash();
	try{
		playerToUpdate = playersHash->find(playerId)->get_player();
	} catch(IdDoesntExists& e){
		return StatusType::FAILURE;
	}
	Team* TeamOfPlayer = m_playersTeamsUF->find(playerId);
	if(TeamOfPlayer->get_id() == DISQUALIFIED_PLAYERS_TEAM_ID){
		return StatusType::FAILURE;
	}
	playerToUpdate->add_cards(cards);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}
	Player* player;
	UnionFind::HashTable* playersHash = m_playersTeamsUF->get_players_hash();
	try{
		player = playersHash->find(playerId)->get_player();
	} catch(IdDoesntExists& e){
		return StatusType::FAILURE;
	}
	return player->get_cards();
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	Team* currentTeam;
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	try{
		currentTeam = m_UFTeams.search(teamId);
	} catch (KeyDoesntExists& err){
		return StatusType::FAILURE;
	}
	return currentTeam->get_total_points();
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
	Team* ithTeam;
	try{
		ithTeam = m_rankedTeams.get_item_by_index(i);
	}catch(KeyDoesntExists& err){
		return StatusType::FAILURE;
	}
	return ithTeam->get_id();
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}
	permutation_t permForPlayer = m_playersTeamsUF->get_partial_spirit(playerId);
	return permForPlayer;
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

bool operator>(const world_cup_t::TeamKey& first, const world_cup_t::TeamKey& second){
	if (first.m_teamAbility > second.m_teamAbility){
		return true;
	}else if(first.m_teamAbility < second.m_teamAbility){
		return false;
	}else{
		return first.m_teamId > second.m_teamId;
	}
}

bool operator<(const world_cup_t::TeamKey& first, const world_cup_t::TeamKey& second){
	if (first.m_teamAbility < second.m_teamAbility){
		return true;
	}else if(first.m_teamAbility > second.m_teamAbility){
		return false;
	}else{
		return first.m_teamId < second.m_teamId;
	}
}