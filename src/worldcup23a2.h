// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #2.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A2_H_
#define WORLDCUP23A2_H_

#include "wet2util.h"
#include "AVLRankTree.h"
#include "UnionFind.h"
#include "Player.h"
#include "Team.h"

class world_cup_t {
private:

	struct TeamKey{
	private:
		int m_teamId;
		int m_teamAbility;
	public:
		explicit TeamKey(int teamId, int teamAbility) : m_teamId(teamId), m_teamAbility(teamAbility){}

		~TeamKey() = default;

		friend bool operator>(const TeamKey& first, const TeamKey& second);

    	friend bool operator<(const TeamKey& first, const TeamKey& second);

		friend bool operator==(const TeamKey& first, const TeamKey& second);
	};
	UnionFind* m_playersTeamsUF;
	AVLRankTree<Team, int> m_UFTeams;
	AVLRankTree<Team, TeamKey> m_rankedTeams;
	static const int DISQUALIFIED_PLAYERS_TEAM_ID;
	static const int DRAW;
	static const int TEAM1_WON_WITH_ABILITY;
	static const int TEAM1_WON_WITH_TEAM_STRENGTH;
	static const int TEAM2_WON_WITH_ABILITY;
	static const int TEAM2_WON_WITH_TEAM_STRENGTH;

	friend bool operator>(const TeamKey& first, const TeamKey& second);

	friend bool operator<(const TeamKey& first, const TeamKey& second);

	friend bool operator==(const TeamKey& first, const TeamKey& second);
	
public:
	// <DO-NOT-MODIFY> {
	
	world_cup_t();
	virtual ~world_cup_t();
	
	StatusType add_team(int teamId);
	
	StatusType remove_team(int teamId);
	
	StatusType add_player(int playerId, int teamId,
	                      const permutation_t &spirit, int gamesPlayed,
	                      int ability, int cards, bool goalKeeper);
	
	output_t<int> play_match(int teamId1, int teamId2);
	
	output_t<int> num_played_games_for_player(int playerId);
	
	StatusType add_player_cards(int playerId, int cards);
	
	output_t<int> get_player_cards(int playerId);
	
	output_t<int> get_team_points(int teamId);
	
	output_t<int> get_ith_pointless_ability(int i);
	
	output_t<permutation_t> get_partial_spirit(int playerId);
	
	StatusType buy_team(int teamId1, int teamId2);
	
	// } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
