//
// Created by Omar Molina-Aguirre on 3/26/22.
//

#ifndef GAMEDESIGNTEAM1_LOGICNPCMANAGER_H
#define GAMEDESIGNTEAM1_LOGICNPCMANAGER_H

#include <memory>
#include "EntityManagerInterface.h"
#include "NPC.h"
#include "Player.h"
#include <map>

using std::shared_ptr;

class NPCManager : public EntityManagerInterface{
public:
    NPCManager();

    void init();

    bool collideWithPlayer(Player &player);
    bool collideWithPlayer_dialog_box(Player &player);
//    void attachEntity(int map_index, std::map<string, NPC*>* map);

//    void setNPCMap(int map_index);

//    std::map<string, NPC*>* get_current_map_of_NPCs();
    string* get_npc_speaking_string();
    string* option1;
    string* option2;
    string* option3;
    string* option4;
    bool NPC_is_speaking();

    void updateLogic(int deltaT){int i = 0;};
    void updateSentence(int case_index, Player& player, std::map<string, int> &npc_stage_index_map);
    bool display_no_item_str();

    void cleanUp();

    void update_npc_current_stage_index(std::map<string,int> &npc_stage_index_map);


private:
    void switch_status(int map_index);
    std::map<int,  std::map<string, NPC*>*> all_npcs;
    int current_map;

    NPC* current_NPC_collide_with_player_dialog_box;

    string* NPC_string_display;

    bool display_do_not_have_item_yet_str;


};


#endif //GAMEDESIGNTEAM1_LOGICNPCMANAGER_H
