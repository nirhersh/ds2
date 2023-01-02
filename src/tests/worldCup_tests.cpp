

#include "../AVLRankTree.h"
#include "../Player.h"
#include "../Team.h"
#include "../UnionFind.h"
#include "../worldcup23a2.h"

#include "../wet2util.h"

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <functional>
#include <algorithm>    // std::random_shuffle

bool add_remove_team_test();
bool add_player_test();
bool play_match_test();
bool get_num_played_games_for_player_test();
bool add_get_player_cards_test();
bool get_team_points();
void run_test(std::function<bool()> test, std::string test_name);

#include "../worldcup23a2.h"

int main()
{
    run_test(add_remove_team_test, "add and remove team test");
    run_test(add_player_test, "add player test");
    run_test(play_match_test, "play match test");
    run_test(get_num_played_games_for_player_test, "get num played games for player test");
    run_test(add_get_player_cards_test, "add get player cards test");
    //run_test(get_team_points, "get team points");
}

void run_test(std::function<bool()> test, std::string test_name)
{
    if(!test()){
        std::cout<< test_name <<" FAILED."<<std::endl;
        std::cout << std::endl;
        return;
    }
    std::cout<<test_name<<" SUCCEEDED."<<std::endl;
    std::cout << std::endl;

}

bool add_remove_team_test()
{
    int testLength = 10000;
    int seed = 493204;
    world_cup_t* wct1 = new world_cup_t();
    assert(wct1->add_team(0) == StatusType::INVALID_INPUT);
    assert(wct1->add_team(-1) == StatusType::INVALID_INPUT);
    assert(wct1->remove_team(0) == StatusType::INVALID_INPUT);
    assert(wct1->remove_team(-1) == StatusType::INVALID_INPUT);
    int arr[10000] = {0};
    for (int i = 0; i < testLength; i++)
    {
        srand(seed + i);
        int x = rand();
        assert(wct1->add_team(x) == StatusType::SUCCESS);
        assert(wct1->add_team(x) == StatusType::FAILURE);
        arr[i] = x;
    }
    for (int i = 0; i < testLength; i++)
    {
        int x = arr[i];
        assert(wct1->remove_team(x) == StatusType::SUCCESS);
        assert(wct1->remove_team(x) == StatusType::FAILURE);
    }
    return true;    
}

bool add_player_test()
{
    
    int testLength = 1000;
    int seed = 493204;
    world_cup_t* wct1 = new world_cup_t();
    // add teams
    for (int i = 0; i < 100; i++)
    {
        assert(wct1->add_team(i + 1) == StatusType::SUCCESS);
    }

    for (int i = 0; i < testLength; i++)
    {
        srand(seed + i);
        assert(wct1->add_player(rand() + 1, rand() % 100 + 1, permutation_t::neutral(),
                rand() % 20, rand() % 10, rand() % 3, !(rand() % 11)) == StatusType::SUCCESS);
        assert(wct1->add_player(rand(), rand() % 100 + 200, permutation_t::neutral(),
                rand() % 20, rand() % 10, rand() % 3, !(rand() % 11)) == StatusType::FAILURE);
    }
    return true;
}

bool play_match_test()
{
    int seed = 493204;
    world_cup_t* wct1 = new world_cup_t();
    // add teams
    for (int i = 0; i < 100; i++)
    {
        assert(wct1->add_team(i + 1) == StatusType::SUCCESS);
    }

    assert(wct1->play_match(0, 9).status() == StatusType::INVALID_INPUT);
    assert(wct1->play_match(8, 0).status() == StatusType::INVALID_INPUT);
    assert(wct1->play_match(8, 8).status() == StatusType::INVALID_INPUT);
    
    //check first won by ability
    srand(seed);
    assert(wct1->add_player(rand(), 2, permutation_t::neutral(),
                rand() % 20, 100000, rand() % 3, true) == StatusType::SUCCESS);
    assert(wct1->add_player(rand(), 3, permutation_t::neutral(),
                rand() % 20, 0, rand() % 3, true) == StatusType::SUCCESS);
    assert(wct1->play_match(2, 3).ans() == 1);

    //check second won by ability
    assert(wct1->play_match(3, 2).ans() == 3);

    //check first won by team spirit
    srand(seed);
    int perm1[5] = {4, 3, 2, 1, 0};
    int perm2[5] = {0, 1, 2, 3, 4};
    int x = rand();
    assert(wct1->add_player(rand() + 1, 4, perm1,
                x % 20, 0, x % 3, true) == StatusType::SUCCESS);
    assert(wct1->add_player(rand(), 5, perm2,
                x % 20, 0, x % 3, true) == StatusType::SUCCESS);
    assert(wct1->play_match(4, 5).ans() == 4);

    //check second won by team spirit
    std::cout << "dsfsfds" << wct1->play_match(5, 4).ans() << std::endl;
    assert(wct1->add_player(rand() + 1, 90, perm1,
                x % 20, 0, x % 3, true) == StatusType::SUCCESS);
    assert(wct1->add_player(rand(), 91, perm2,
                x % 20, 0, x % 3, true) == StatusType::SUCCESS);
    assert(wct1->play_match(91, 90).ans() == 2);

    //check tie
    srand(seed);
    assert(wct1->add_player(rand(), 6, permutation_t(perm2),
                rand() % 20, 0, rand() % 3, true) == StatusType::SUCCESS);
    assert(wct1->add_player(rand(), 7, permutation_t(perm2),
                rand() % 20, 0, rand() % 3, true) == StatusType::SUCCESS);
    assert(wct1->play_match(6, 7).ans() == 0);
    assert(wct1->play_match(7, 6).ans() == 0);

    return true;
}

bool get_num_played_games_for_player_test()
{
    int testLength = 10000;
    int seed = 90856;
    world_cup_t* wct1 = new world_cup_t();
    // add teams
    for (int i = 1; i < 101; i++)
    {
        srand(seed + 10 * i);
        assert(wct1->add_team(i) == StatusType::SUCCESS);
        assert(wct1->add_player(i, i, permutation_t::neutral(),
                0, rand() % 10, rand() % 3, true) == StatusType::SUCCESS);
        assert(wct1->num_played_games_for_player(i).ans() == 0);
    }

    //check input
    assert(wct1->num_played_games_for_player(-1).status() == StatusType::INVALID_INPUT);
    assert(wct1->num_played_games_for_player(10000).status() == StatusType::FAILURE);
    
    //total check

    for (int i = 0; i < testLength; i++)
    {
        srand(seed + i);
        assert(wct1->add_player(rand(), rand() % 100 + 1, permutation_t::neutral(),
                0, rand() % 10, rand() % 3, !(rand() % 11)) == StatusType::SUCCESS);
    }
    wct1->play_match(1, 2);
    assert(wct1->num_played_games_for_player(1).ans() == 1);
    assert(wct1->num_played_games_for_player(2).ans() == 1);
    int histogram [101] = {0};
    histogram[1]++;
    histogram[2]++;
    for (int i = 0; i < testLength; i++)
    {
        srand(seed + i);
        int firstTeam = rand() % 100 + 1;
        int secondTeam = rand() % 100 + 1;
        if(firstTeam == secondTeam){
            if(firstTeam == 1){
                secondTeam = 2 + rand() % 90;
            }else{
                secondTeam = firstTeam - 1;
            }
        }
        wct1->play_match(firstTeam, secondTeam);
        histogram[firstTeam]++;
        histogram[secondTeam]++;
        assert(wct1->num_played_games_for_player(firstTeam).ans() == histogram[firstTeam]);
        assert(wct1->num_played_games_for_player(secondTeam).ans() == histogram[secondTeam]);
    }
    //check non-active player
    wct1->remove_team(1);
    assert(wct1->num_played_games_for_player(1).ans() == histogram[1]);
    wct1->remove_team(2);
    assert(wct1->num_played_games_for_player(2).ans() == histogram[2]);
    wct1->remove_team(3);
    assert(wct1->num_played_games_for_player(3).ans() == histogram[3]);
    wct1->remove_team(4);
    assert(wct1->num_played_games_for_player(4).ans() == histogram[4]);

    return true;
}

bool add_get_player_cards_test()
{
    int testLength = 10000;
    int seed = 76134;
    world_cup_t* wct1 = new world_cup_t();
    // add teams
    for (int i = 1; i < 103; i++)
    {
        srand(seed + 10 * i);
        assert(wct1->add_team(i) == StatusType::SUCCESS);
        assert(wct1->add_player(i, i, permutation_t::neutral(),
                rand() % 20, rand() % 10, 0, true) == StatusType::SUCCESS);
        assert(wct1->get_player_cards(i).ans() == 0);
    }

    //check input for add
    assert(wct1->add_player_cards(-1, 1000) == StatusType::INVALID_INPUT);
    assert(wct1->add_player_cards(0, 1000) == StatusType::INVALID_INPUT);
    assert(wct1->add_player_cards(1000, 10) == StatusType::FAILURE);
    assert(wct1->remove_team(102) == StatusType::SUCCESS);
    assert(wct1->add_player_cards(102, 10) == StatusType::FAILURE);

    //check input for get
    assert(wct1->get_player_cards(0).status() == StatusType::INVALID_INPUT);
    assert(wct1->get_player_cards(-1).status() == StatusType::INVALID_INPUT);
    assert(wct1->get_player_cards(10000).status() == StatusType::FAILURE);
    assert(wct1->get_player_cards(102).ans() == 0);
    
    

    //total check
    for (int i = 0; i < testLength; i++)
    {
        srand(seed + i);
        assert(wct1->add_player(rand(), rand() % 100 + 1, permutation_t::neutral(),
                rand() % 20, rand() % 10, rand() % 3, !(rand() % 11)) == StatusType::SUCCESS);
    }
    int histogram [101]= {0};
    for (int i = 0; i < testLength; i++)
    {
        srand(seed + i);
        int playerToGetCards = rand() % 100 + 1;
        int numOfcards = rand() % 5;
        wct1->add_player_cards(playerToGetCards, numOfcards);
        histogram[playerToGetCards] += numOfcards;
        assert(wct1->get_player_cards(playerToGetCards).ans() == histogram[playerToGetCards]);
    }
    return true;
}

bool get_team_points()
{
    int testLength = 10000;
    int seed = 90856;
    world_cup_t* wct1 = new world_cup_t();
    // add teams
    for (int i = 1; i < 101; i++)
    {
        srand(seed + i);
        assert(wct1->add_team(i) == StatusType::SUCCESS);
    }

    //check input
    assert(wct1->get_team_points(0).status() == StatusType::INVALID_INPUT);
    assert(wct1->get_team_points(-1).status() == StatusType::INVALID_INPUT);
    assert(wct1->num_played_games_for_player(10000).status() == StatusType::FAILURE);
    
    //total check

    for (int i = 0; i < testLength; i++)
    {
        srand(seed + i);
        assert(wct1->add_player(rand(), rand() % 100, permutation_t::neutral(),
                0, rand() % 10, rand() % 3, !(rand() % 11)) == StatusType::SUCCESS);
        assert(wct1->get_team_points(i).ans() == 0);
    }   
    return true; 
}
















