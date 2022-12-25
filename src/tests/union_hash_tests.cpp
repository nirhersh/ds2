

#include "../AVLRankTree.h"
#include "../Player.h"
#include "../Team.h"
#include "../UnionFind.h"

#include "../wet2util.h"

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <functional>
#include <algorithm>    // std::random_shuffle

void run_test(std::function<bool()> test, std::string test_name);
bool hash_rehash_test();
bool hash_add_find_test();
bool rankTree_test();
bool union_find_find_test();



int main()
{
    run_test(hash_add_find_test, "add find test");
    run_test(hash_rehash_test, "rehash test");
    run_test(rankTree_test, "rankTree test");
    run_test(union_find_find_test, "Union Find find() test");
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


bool hash_add_find_test(){
    UnionFind::HashTable hash1;
    int hashSize = 100;
    UnionFind::Node** nodes = new UnionFind::Node*[hashSize];
    for(int i=0; i<hashSize; i++){
        nodes[i] = new UnionFind::Node(new Player(i, i, permutation_t()), permutation_t(), nullptr, nullptr);
        hash1.add(i, nodes[i]);
    }
    for(int i=0; i<hashSize; i++){
        if(!(hash1.find(i)->get_player()->get_player_id() == i)){
            return false;
        }
    }
    delete[] nodes;
    return true;
}

bool hash_rehash_test(){
    UnionFind::HashTable hash1;
    int hashSize = 500;
    UnionFind::Node** nodes = new UnionFind::Node*[hashSize];
    for(int i=0; i<hashSize; i++){
        nodes[i] = new UnionFind::Node(new Player(i, i, permutation_t()), permutation_t(), nullptr, nullptr);
        hash1.add(i, nodes[i]);
    }
    if(hash1.get_array_size() != (pow(2, 10) - 1)){
        return false;
    }

    for(int i=0; i<hashSize; i++){
        if(!(hash1.find(i)->get_player()->get_player_id() == i)){
            return false;
        }
    }
    delete[] nodes;
    return true;
}


bool rankTree_test()
{
    int testLength = 100000;
    int* array = new int[testLength];
    for (int i = 0; i < testLength; i++)
    {
        array[i] = i;
    }
    std::random_shuffle(array, array + testLength - 1);
    // for (int i = 0; i < 1000; i++)
    // {
    //     std::cout << array[i] << ", ";
    // }
    

    AVLRankTree<int, int> avlTree1;
    for (int i = 0; i < testLength; i++)
    {
        try{
            avlTree1.push(&array[i], array[i]);
        } catch (KeyAlreadyExists& err){}
    }
    
    for (int i = 0; i < testLength; i++)
    {
        try{
            int x = *(avlTree1.get_item_by_index(i));
            //std::cout << "in index " << i + 1 << ", the value is " << x << std::endl;
            assert(x == i);
        } catch(InvalidArguments& err){
            assert(false);
        }
    }

    for (int i = 0; i < testLength; i += 2)
    {
        try{
            avlTree1.remove(i);
        } catch (KeyDoesntExists& err){
            assert(false);
        }
    }

    for (int i = 0; i < testLength / 2; i++)
    {
        try{
            int x = *(avlTree1.get_item_by_index(i));
            assert(x == i * 2 + 1);
        } catch(InvalidArguments& err){
            assert(false);
        }
    }
    delete[] array;
    return true;
    
}

bool union_find_find_test(){
    AVLRankTree<Team, int> teams;
    Team* team1 = new Team(1);
    teams.push(team1, 1);
    UnionFind* uf = new UnionFind(&teams);
    int testLength = 10000;
    int* array = new int[testLength];
    for (int i = 0; i < testLength; i++)
    {
        array[i] = i;
    }
    std::random_shuffle(array, array + testLength - 1);
    Player** players = new Player*[testLength];
    for(int i=0; i<testLength; i++){
        players[i] = new Player(i, 1, permutation_t::neutral());
    }
    for(int i=0; i<testLength; i++){
        uf->add_player(players[i], 1);
    }
    for(int i=0; i<testLength; i++){
        try{
            if(uf->find(i)->get_id() != 1){
                delete [] players;
                delete [] array;
                delete team1;
                return false;
            }
        }catch(IdDoesntExists& e){
            std::cout << e.what() << std::endl;
            delete [] players;
            delete [] array;
            delete team1;
            return false;
        }
    }

    Team* team2 = new Team(2);
    Team* team3 = new Team(3);
    teams.push(team2, 2);
    teams.push(team3, 3);
    Player* player2 = new Player(testLength+1, 2, permutation_t::neutral());
    Player* player3 = new Player(testLength+2, 3, permutation_t::neutral());
    uf->add_player(player2, 2);
    uf->add_player(player3, 3);
    for(int i=10001; i<10003; i++){
        try{
            if(uf->find(i)->get_id() != i-testLength +1){
                delete [] players;
                delete [] array;
                delete team1;
                delete team2;
                delete team3;
                return false;
            }
        }catch(IdDoesntExists& e){
            delete [] players;
            delete [] array;
            delete team1;
            delete team2;
            delete team3;
            std::cout << e.what() << std::endl;
            return false;
        }
    }
    try{
        uf->find(testLength + 5);
    }catch(IdDoesntExists& e){
        delete [] players;
        delete [] array;
        delete team1;
        delete team2;
        delete team3;
        delete uf;
        return true;
    }   
    delete [] players;
    delete [] array;
    delete team1;
    delete team2;
    delete team3;
    return false;
}

