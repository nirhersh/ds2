

#include "../AVLTree.h"
#include "../Player.h"
#include "../Team.h"
#include "../UnionFind.h"

#include "../wet2util.h"

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <functional>

void run_test(std::function<bool()> test, std::string test_name);
bool hash_rehash_test();
bool hash_add_find_test();

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
    UnionFind::Node** nodes = new UnionFind::Node*[100];
    for(int i=0; i<hashSize; i++){
        nodes[i] = new UnionFind::Node(new Player(i, i, permutation_t()), permutation_t(), nullptr, nullptr);
        hash1.add(i, nodes[i]);
    }
    for(int i=0; i<hashSize; i++){
        if(!(hash1.find(i)->get_player()->get_player_id() == i)){
            return false;
        }
    }
    for(int i=0; i<hashSize ;i++){
        delete nodes[i]->get_player();
        delete nodes[i];
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
    if(!(hash1.get_array_size() != (pow(2, 10) - 1))){
        return false;
    }

    for(int i=0; i<hashSize; i++){
        if(!(hash1.find(i)->get_player()->get_player_id() == i)){
            return false;
        }
    }
    for(int i=0; i<hashSize ;i++){
        delete nodes[i]->get_player();
        delete nodes[i];
    }
    delete[] nodes;
    return true;
}



int main()
{
    run_test(hash_add_find_test, "add find test");
    run_test(hash_rehash_test, "rehash test");
}