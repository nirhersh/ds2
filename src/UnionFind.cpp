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
    if(root1 != nullptr || root2 != nullptr){
        if(team1Size >= team2Size){
            if(root2 != nullptr){
                root2->m_partialSpirit = (root1->m_partialSpirit.inv()) * mainTeam->get_spirit_strength() * root2->m_partialSpirit;
                root2->m_gamesPlayed -= root1->m_gamesPlayed;
                root2->m_team = nullptr;
                assert(root2 != root1);
                root2->m_parent = root1;
            }
        }else{
            root2->m_partialSpirit = mainTeam->get_spirit_strength() * root2->m_partialSpirit;
            if(root1 != nullptr){
                root1->m_partialSpirit = root2->m_partialSpirit.inv() * root1->m_partialSpirit;
                root1->m_gamesPlayed -= root2->m_gamesPlayed;   
                root1->m_parent = root2;
                root1->m_team = nullptr;
                assert(root2->m_parent == nullptr);
            }
            mainTeam->set_team_root(root2);
            root2->m_team = mainTeam;
        }
    }
    mainTeam->unite_team(secondTeam);
    secondTeam->set_team_root(nullptr);
}

void UnionFind::add_player(Player* player, int teamId){
    Team* team = m_teams->search(teamId);
    Node* playerNode = nullptr;
    if(team->get_team_root() == nullptr){
        playerNode = new Node(player, player->get_spirit(), team, nullptr, player->get_games_played());
        team->set_team_root(playerNode);
        assert(team->get_team_root()->m_parent == nullptr);
    }else{
        Node* teamRoot = team->get_team_root();
        
        permutation_t partialSpirit = (teamRoot->m_partialSpirit.inv()) * teamRoot->m_team->get_spirit_strength() * player->get_spirit();
        playerNode = new Node(player, partialSpirit, team, teamRoot, player->get_games_played()-(teamRoot->m_gamesPlayed));


        assert(teamRoot->m_parent == nullptr);
    }
    m_players->add(player->get_player_id(), playerNode);
}

permutation_t UnionFind::get_partial_spirit(int playerId){
    find(playerId);
    Node* current = m_players->find(playerId);
    if(current->m_parent)
    {
        return current->m_parent->m_partialSpirit * current->m_partialSpirit;
    }
    return current->m_partialSpirit;
}

int UnionFind::get_games_played(int playerId)
{
    int gamesCounter = 0;
    Node* current = m_players->find(playerId);
    find(playerId); 
    while(current)
    {
        gamesCounter += current->m_gamesPlayed;
        current = current->m_parent;
    }
    return gamesCounter;
}

UnionFind::HashTable* UnionFind::get_players_hash()
{
    return m_players;
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

void UnionFind::Node::update_games_played(int games){
    m_gamesPlayed += games;
}


/*.......................HASH TABLE IMPLEMENTATION............................*/
UnionFind::HashTable::HashTable() : m_exponent(8), m_currentSize(0) 
{
    m_arraySize = pow(2, m_exponent) - 1;
    m_nodesArray = new List<UnionFind::Node>[m_arraySize];
}

UnionFind::HashTable::~HashTable(){
    for(int i=0; i<m_arraySize; i++){
        List<Node>::Node* current = m_nodesArray[i].get_first();
        while(current != nullptr){
            List<Node>::Node* temp = current;
            current = current->get_next();
            delete temp->get_data()->m_player;
            delete temp->get_data();
        }
    }
    delete[] m_nodesArray;
}

UnionFind::Node* UnionFind::HashTable::find(int playerId) const
{
    int listIndex= playerId % m_arraySize;
    List<Node>::Node* current = m_nodesArray[listIndex].get_first();
    while(current != nullptr){
        if(current->get_data()->get_player()->get_player_id() == playerId){
            return current->get_data();
        }else{
            current = current->get_next();
        }
    }
    throw IdDoesntExists(playerId);
    return nullptr;
}

bool UnionFind::exists(int PlayerId)
{
    try{
        m_players->find(PlayerId);
    }catch(IdDoesntExists& err){
        return false;
    }
    return true;
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
    int listIndex = playerId % m_arraySize;
    m_nodesArray[listIndex].append(node);
}

void UnionFind::HashTable::rehash(){
    m_exponent++;
    int newSize = pow(2, m_exponent) - 1;
    List<UnionFind::Node>* newNodesArray = new List<UnionFind::Node>[newSize];
    List<UnionFind::Node>* tempArray = m_nodesArray;
    m_nodesArray = newNodesArray;
    m_currentSize = 0;
    int oldSize = m_arraySize;
    m_arraySize = newSize;
    for(int i=0; i<oldSize; i++){
        List<Node>::Node* current = tempArray[i].get_first();
        if(current != nullptr){
            int playerId = current->get_data()->get_player()->get_player_id();
            add(playerId, current->get_data());
            current = current->get_next();
        }
    }
    delete[] tempArray;
}



