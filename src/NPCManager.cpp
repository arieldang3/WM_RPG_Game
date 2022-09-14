//
// Created by Omar Molina-Aguirre on 3/26/22.
//

#include "WMRPGame.h"
#include "EntityManagerInterface.h"
#include "NPCManager.h"
#include "Entity.h"
#include "NPC.h"
#include "Timer.h"

using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;

//Initialization---------------------------------------------------------------------

NPCManager::NPCManager() : EntityManagerInterface(){
    NPC_string_display = nullptr;
    current_NPC_collide_with_player_dialog_box = nullptr;
    display_do_not_have_item_yet_str = false;
}

bool NPCManager::collideWithPlayer(Player &player) {

    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto npc(std::static_pointer_cast<NPC>(it->second));
        if(player.collideWithEntity(*npc, 0)){
            return true;
        }
    }
    return false;

}


void NPCManager::cleanUp() {

    if (NPC_string_display!= nullptr){
        delete NPC_string_display;
        NPC_string_display = nullptr;
    }
    if (current_NPC_collide_with_player_dialog_box!= nullptr){
        delete current_NPC_collide_with_player_dialog_box;
        current_NPC_collide_with_player_dialog_box = nullptr;
    }
    if (option1!= nullptr){
        delete option1;
        option1 = nullptr;
    }
    if (option2!= nullptr){
        delete option2;
        option2 = nullptr;
    }
    if (option3!= nullptr){
        delete option3;
        option3 = nullptr;
    }
    if (option4!= nullptr){
        delete option4;
        option4 = nullptr;
    }
}

bool NPCManager::collideWithPlayer_dialog_box(Player &player) {
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto npc(std::static_pointer_cast<NPC>(it->second));
        if(player.collideWithEntityDialogBox(*npc.get())){
            current_NPC_collide_with_player_dialog_box = npc.get();
            NPC_string_display = current_NPC_collide_with_player_dialog_box->get_npc_sentence_speak();
            option1 = current_NPC_collide_with_player_dialog_box->option1_string_ptr;
            option2 = current_NPC_collide_with_player_dialog_box->option2_string_ptr;
            option3 = current_NPC_collide_with_player_dialog_box->option3_string_ptr;
            option4 = current_NPC_collide_with_player_dialog_box->option4_string_ptr;
            display_do_not_have_item_yet_str = current_NPC_collide_with_player_dialog_box->display_do_not_have_item_yet;
            return true;
        }
    }
    current_NPC_collide_with_player_dialog_box = nullptr;
    NPC_string_display = nullptr;
    display_do_not_have_item_yet_str = false;
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto npc(std::static_pointer_cast<NPC>(it->second));
        npc->display_do_not_have_item_yet = false;
    }

    return false;

}

bool NPCManager::NPC_is_speaking() {
    return NPC_string_display != nullptr;
}

string *NPCManager::get_npc_speaking_string() {
    return NPC_string_display;
}

void NPCManager::updateSentence(int case_index, Player& player, map<string, int> &npc_stage_index_map) {
    if (current_NPC_collide_with_player_dialog_box!= nullptr){
        current_NPC_collide_with_player_dialog_box->next_sentence(case_index, player, npc_stage_index_map);
    }
}

void NPCManager::update_npc_current_stage_index(map<string, int> &npc_stage_index_map) {
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto npc(std::static_pointer_cast<NPC>(it->second));
        auto iter = npc_stage_index_map.find(*npc.get()->get_name());
        if (iter!=npc_stage_index_map.end()){
            npc.get()->set_npc_stage(iter->second);
        }
    }
}

bool NPCManager::display_no_item_str() {
    return display_do_not_have_item_yet_str;
}



