#include "UnionFind.h"


/*.......................UNION FIND IMPLEMENTATION............................*/

UnionFind::UnionFind(AVLRankTree<Team, int>* teamsTree){
    m_players = new HashTable();
    m_teams = teamsTree;
}

UnionFind::~UnionFind(){
    delete m_players;
}

Team* UnionFind::find(int playerId) const
{
    Node* currentPlayer = m_players->find(playerId);
    Node* tempToStart = currentPlayer;
    permutation_t permAccumulator = permutation_t::neutral(); // this is the identity element
    int gamesCounter = 0;
    while(currentPlayer->m_parent)
    {
        permAccumulator = currentPlayer->m_partialSpirit * permAccumulator;
        gamesCounter += currentPlayer->m_gamesPlayed;
        currentPlayer = currentPlayer->m_parent;
    }//now current player points to the root
    while(tempToStart != currentPlayer)
    {
        Node* next = tempToStart->m_parent;
        tempToStart->m_parent = currentPlayer;
        permAccumulator = permAccumulator * (tempToStart->m_partialSpirit.inv());
        gamesCounter -= tempToStart->m_gamesPlayed;
        tempToStart->m_gamesPlayed += gamesCounter;
        tempToStart->update_partial_spirit(permAccumulator);
        tempToStart = next;
    }
    return currentPlayer->m_team;
}

void UnionFind::union_teams(Team* mainTeam, Team* secondTeam)
{
    Node* root1 = mainTeam->get_team_root();
    Node* root2 = secondTeam->get_team_root();
    int team1Size = mainTeam->get_num_of_players();
    int team2Size = secondTeam->get_num_of_players();
    if(team1Size >= team2Size){
        root2->m_partialSpirit = (root1->m_partialSpirit.inv()) * mainTeam->get_spirit_strength() * root2->m_partialSpirit;
        root2->m_gamesPlayed -= root1->m_gamesPlayed;
    }else{
        root2->m_partialSpirit = mainTeam->get_spirit_strength() * root2->m_partialSpirit;
        root1->m_partialSpirit = root2->m_partialSpirit.inv() * root1->m_partialSpirit;
        root1->m_gamesPlayed -= root2->m_gamesPlayed;
        mainTeam->set_team_root(root2);
    }
    mainTeam->unite_team(secondTeam);
}

void UnionFind::add_player(Player* player, int teamId){
    Team* team = m_teams->search(teamId);
    Node* playerNode = nullptr;
    if(team->get_team_root() == nullptr){
        playerNode = new Node(player, player->get_spirit(), team, nullptr, 0);
        team->set_team_root(playerNode);
    }else{
        Node* teamRoot = team->get_team_root();
        permutation_t partialSpirit = (teamRoot->m_partialSpirit.inv()) * teamRoot->m_team->get_spirit_strength();
        playerNode = new Node(player, partialSpirit, team, teamRoot, -(teamRoot->m_gamesPlayed));
    }
    m_players->add(player->get_player_id(), playerNode);
}

permutation_t UnionFind::get_partial_spirit(Player* player){
    find(player->get_player_id());
    Node* current = m_players->find(player->get_player_id());
    permutation_t partialSpirit = current->m_partialSpirit * (current->m_partialSpirit.inv());
    while(current){
        partialSpirit = current->m_partialSpirit * partialSpirit;
    }
    return partialSpirit;
}

int UnionFind::get_games_played(Player* player)
{
    int gamesCounter = 0;
    Node* current = m_players->find(player->get_player_id());
    find(player->get_player_id());
    while(current)
    {
        gamesCounter += current->m_gamesPlayed;
    }
    return gamesCounter;
}

/*.......................NODE IMPLEMENTATION............................*/
UnionFind::Node::Node(Player* player, const permutation_t& partialSpirit, Team* team, Node* parent, int gamesPlayed) :  m_parent(parent), m_team(team), m_player(player),
    m_partialSpirit(partialSpirit), m_gamesPlayed(gamesPlayed){}

void UnionFind::Node::set_parent(Node* parent){
    m_parent = parent;
}

void UnionFind::Node::set_team(Team* team){
    m_team = team;
}

void UnionFind::Node::update_partial_spirit(const permutation_t& newPerm){
    m_partialSpirit = newPerm * m_partialSpirit;
}


/*.......................HASH TABLE IMPLEMENTATION............................*/
UnionFind::HashTable::HashTable() : m_exponent(8), m_currentSize(0) 
{
    m_arraySize = pow(2, m_exponent) - 1;
    m_nodesArray = new UnionFind::Node*[m_arraySize];
    for (int i = 0; i < m_arraySize; i++)
    {
        m_nodesArray[i] = nullptr;
    }
}

UnionFind::HashTable::~HashTable(){
    for(int i=0; i<m_arraySize; i++){
        if(m_nodesArray[i] != nullptr){
            delete m_nodesArray[i]->m_player;
            delete m_nodesArray[i];
        }
    }
    delete[] m_nodesArray;
}

UnionFind::Node* UnionFind::HashTable::find(int playerId) const
{
    int funcH = playerId % m_arraySize;
    int funcR = 1 + playerId % (((m_arraySize + 1) / 2) - 1); //the id, mod the last array size. have to be strangers
    for (int i = 0; i < m_arraySize; i++)
    {
        if(m_nodesArray[(funcH + i * funcR) % m_arraySize] == nullptr){
            throw IdDoesntExists(playerId);
            return nullptr;
        }
        if((m_nodesArray[(funcH + i * funcR) % m_arraySize]->m_player->get_player_id() == playerId)){
            return m_nodesArray[(funcH + i * funcR) % m_arraySize];
        }
    }
    throw IdDoesntExists(playerId);
    return nullptr;
}

double UnionFind::HashTable::calculate_load_factor() const{
    if(m_currentSize != 0 && (double)((double)m_currentSize / (double)m_arraySize) == 0){
        assert(false);
    }
    return (double)((double)m_currentSize / (double)m_arraySize);
}

void UnionFind::HashTable::add(int playerId, UnionFind::Node* node){
    if(calculate_load_factor() >= MAX_LOAD_FACTOR){
        rehash();
    }
    int funcH = playerId % m_arraySize;
    int funcR = 1 + playerId % (((m_arraySize + 1) / 2) - 1); //the id, mod the last array size. have to be strangers
    for(int i=0; i < m_arraySize; i++){
        if(m_nodesArray[(funcH + i * funcR) % m_arraySize] == nullptr){
            m_nodesArray[(funcH + i * funcR) % m_arraySize] = node;
            m_currentSize++;
            return;
        }   
    }
    assert(false);
}

void UnionFind::HashTable::rehash(){
    m_exponent++;
    int newSize = pow(2, m_exponent) - 1;
    Node** newNodesArray = new UnionFind::Node*[newSize];
    Node** tempArray = m_nodesArray;
    m_nodesArray = newNodesArray;
    for (int i = 0; i < newSize; i++)
    {
        m_nodesArray[i] = nullptr;
    }
    m_currentSize = 0;
    for(int i=0; i<m_arraySize; i++){
        if(tempArray[i] != nullptr){ 
            int playerId = tempArray[i]->m_player->get_player_id();
            add(playerId, tempArray[i]);
        }
    }
    delete[] tempArray;
    m_arraySize = newSize;
    m_nodesArray = newNodesArray;
}

