#include "Item.h"
#include "BoundingBox.h"
#include "NPC.h"
#include <cmath>
#include <utility>

NPC::~NPC() = default;

/*
 * Constructor. give the address of an array that stores all possible words. */
NPC::NPC(const float &posx, const float &posy, int sizew, int sizeh, string npc_name) : Entity(posx, posy, sizew, sizeh) {
    Index_current_sentence = 0;
    current_stage = 0;
    display = false;
//    npc_state = NOTTALKED;
    Name = std::move(npc_name);
    option1_string_ptr = nullptr;
    option2_string_ptr = nullptr;
    option3_string_ptr = nullptr;
    option4_string_ptr = nullptr;
    display_do_not_have_item_yet = false;
}

void NPC::switch_NPC_display() {
    display = !display;
}

/*
 * return the NPC's string, not the player options (player responds) */
string *NPC::get_npc_sentence_speak() {
    auto iter = speech.find(current_stage*10+ Index_current_sentence);
    if (iter!=speech.end()){
        return &iter->second;
    }
    return nullptr;
}

//void NPC::set_conversation_separate_indices(int sep1, int sep2, int sep3) {
//    conversation_separate_indices[0] = sep1;
//    conversation_separate_indices[1] = sep2;
//    conversation_separate_indices[2] = sep3;
//}

void NPC::add_npc_speech(int stage_index, int sentence_index, string text) {
    speech.insert(std::pair<int, string>(stage_index*10+sentence_index, text));
}
/*
 * events:
 * 0. "next sentence" key: should play next sentence until reach to the end of this stage.
 * 1. player choose option 1.
 * 2. player choose option 2. */
void NPC::next_sentence(int event_index, Player& player, std::map<string, int> &npc_stage_index_map) {
    switch (event_index) {
        case 0:
            display_do_not_have_item_yet = false;
            if (speech.find(current_stage*10+Index_current_sentence+1)!=speech.end()){
                Index_current_sentence = Index_current_sentence+1;
                return;
            }
            else {
                auto iter = option1.find(current_stage);
                if (iter!=option1.end()){
                    option1_string_ptr = &(iter->second.first);
                }
                auto iter2 = option2.find(current_stage);
                if (iter2!=option2.end()){
                    option2_string_ptr = &(iter2->second.first);
                }
                auto iter3 = option3.find(current_stage);
                if (iter3!=option3.end()){
                    option3_string_ptr = &(iter3->second.first);
                }
                auto iter4 = option4.find(current_stage);
                if (iter4!=option4.end()){
                    option4_string_ptr = &(iter4->second.first);
                }
                return;
            }
        case 1:
            display_do_not_have_item_yet = false;
            if (option1_string_ptr!= nullptr){
                //Check if this was supposed to complete a quest
                if(option1.find(current_stage)->second.second.second){
                    if(checkQuestCompleted(player)){
                        //Carry on as you should
                        option1_string_ptr = nullptr;
                        option2_string_ptr = nullptr;
                        option3_string_ptr = nullptr;
                        option4_string_ptr = nullptr;
                        Index_current_sentence = 0;
                        current_stage = option1.find(current_stage)->second.second.first;
                        auto iter = npc_stage_index_map.find(Name);
                        iter->second = current_stage;
                    }
                        //Do nothing (for now)
                    else{
                        display_do_not_have_item_yet = true;
                    }

                }
                //Regular non-important response
                else{
                    option1_string_ptr = nullptr;
                    option2_string_ptr = nullptr;
                    option3_string_ptr = nullptr;
                    option4_string_ptr = nullptr;
                    Index_current_sentence = 0;
                    current_stage = option1.find(current_stage)->second.second.first;
                    auto iter = npc_stage_index_map.find(Name);
                    iter->second = current_stage;
                }
            }
            return;
        case 2:
            display_do_not_have_item_yet = false;
            if (option2_string_ptr!= nullptr){
                //Check if this was supposed to complete a quest
                if(option2.find(current_stage)->second.second.second){
                    if(checkQuestCompleted(player)){
                        //Carry on as you should
                        option1_string_ptr = nullptr;
                        option2_string_ptr = nullptr;
                        option3_string_ptr = nullptr;
                        option4_string_ptr = nullptr;
                        Index_current_sentence = 0;
                        current_stage = option2.find(current_stage)->second.second.first;
                        auto iter = npc_stage_index_map.find(Name);
                        iter->second = current_stage;
                    }
                    //Do nothing (for now)
                    else{
                        display_do_not_have_item_yet = true;
                    }
                }
                else{
                    option1_string_ptr = nullptr;
                    option2_string_ptr = nullptr;
                    option3_string_ptr = nullptr;
                    option4_string_ptr = nullptr;
                    Index_current_sentence = 0;
                    current_stage = option2.find(current_stage)->second.second.first;
                    auto iter = npc_stage_index_map.find(Name);
                    iter->second = current_stage;
                }
            }
            return;
        case 3:
            display_do_not_have_item_yet = false;
            if (option3_string_ptr!= nullptr){
                //Check if this was supposed to complete a quest
                if(option3.find(current_stage)->second.second.second){
                    if(checkQuestCompleted(player)){
                        //Carry on as you should
                        option1_string_ptr = nullptr;
                        option2_string_ptr = nullptr;
                        option3_string_ptr = nullptr;
                        option4_string_ptr = nullptr;
                        Index_current_sentence = 0;
                        current_stage = option3.find(current_stage)->second.second.first;
                        auto iter = npc_stage_index_map.find(Name);
                        iter->second = current_stage;
                    }
                    //Do nothing (for now)
                    else{
                        display_do_not_have_item_yet = true;
                    }
                }
                else{
                    option1_string_ptr = nullptr;
                    option2_string_ptr = nullptr;
                    option3_string_ptr = nullptr;
                    option4_string_ptr = nullptr;
                    Index_current_sentence = 0;
                    current_stage = option3.find(current_stage)->second.second.first;
                    auto iter = npc_stage_index_map.find(Name);
                    iter->second = current_stage;
                }
            }
            return;
        case 4:
            display_do_not_have_item_yet = false;
            if (option4_string_ptr!= nullptr){
                //Check if this was supposed to complete a quest
                if(option4.find(current_stage)->second.second.second){
                    if(checkQuestCompleted(player)){
                        //Carry on as you should
                        option1_string_ptr = nullptr;
                        option2_string_ptr = nullptr;
                        option3_string_ptr = nullptr;
                        option4_string_ptr = nullptr;
                        Index_current_sentence = 0;
                        current_stage = option4.find(current_stage)->second.second.first;
                        auto iter = npc_stage_index_map.find(Name);
                        iter->second = current_stage;
                    }
                    //Do nothing (for now)
                    else{
                        display_do_not_have_item_yet = true;
                    }
                }
                else{
                    option1_string_ptr = nullptr;
                    option2_string_ptr = nullptr;
                    option3_string_ptr = nullptr;
                    option4_string_ptr = nullptr;
                    Index_current_sentence = 0;
                    current_stage = option4.find(current_stage)->second.second.first;
                    auto iter = npc_stage_index_map.find(Name);
                    iter->second = current_stage;
                }
            }
            return;
    }
}

void NPC::add_player_options(int option_index, int stage_index, int jump_to_stage_index, string text, bool passingCondition) {
    switch (option_index) {
        case 1:
            option1.insert(std::pair<int, std::pair<string,std::pair<int, bool>>>(stage_index,std::pair<string,std::pair<int, bool>>(text,std::pair<int, bool>(jump_to_stage_index, passingCondition))));
            break;
        case 2:
            option2.insert(std::pair<int, std::pair<string,std::pair<int, bool>>>(stage_index,std::pair<string,std::pair<int, bool>>(text,std::pair<int, bool>(jump_to_stage_index, passingCondition))));
//            option2.insert(std::pair<int, std::pair<string,int>>(stage_index,std::pair<string,int>(text,jump_to_stage_index)));
            break;
        case 3:
            option3.insert(std::pair<int, std::pair<string,std::pair<int, bool>>>(stage_index,std::pair<string,std::pair<int, bool>>(text,std::pair<int, bool>(jump_to_stage_index, passingCondition))));
//            option3.insert(std::pair<int, std::pair<string,int>>(stage_index,std::pair<string,int>(text,jump_to_stage_index)));
            break;
        case 4:
            option4.insert(std::pair<int, std::pair<string,std::pair<int, bool>>>(stage_index,std::pair<string,std::pair<int, bool>>(text,std::pair<int, bool>(jump_to_stage_index, passingCondition))));
//            option4.insert(std::pair<int, std::pair<string,int>>(stage_index,std::pair<string,int>(text,jump_to_stage_index)));
            break;
    }
}

void NPC::cleanUp() {
    if (option1_string_ptr!= nullptr){
        delete option1_string_ptr;
        option1_string_ptr = nullptr;
    }
    if (option2_string_ptr!= nullptr){
        delete option2_string_ptr;
        option2_string_ptr = nullptr;
    }
    if (option3_string_ptr!= nullptr){
        delete option3_string_ptr;
        option3_string_ptr = nullptr;
    }
    if (option4_string_ptr!= nullptr){
        delete option4_string_ptr;
        option4_string_ptr = nullptr;
    }
}


//For giving the NPC quest information - requiring an item and dispensing an item and thanks
void NPC::setQuestRequirements(int requiredItemID, int rewardItemID){
     this->requiredItemID = requiredItemID;
     this->rewardItemID = rewardItemID;
 }

bool NPC::checkQuestCompleted(Player& player){
    //Check for whether an item was required for completion
    if(requiredItemID > 0 && player.checkPlayerHasItem(requiredItemID)){
        player.removeItemFromInventory(requiredItemID);
        player.addItemToInventory(new Item(rewardItemID));
        return true;
    }
    //Check for whether there's a time penalty for completion
    else if(requiredItemID > 1000){
        player.addTimePenalty(requiredItemID);
        player.addItemToInventory(new Item(rewardItemID));
        return true;
    }
    //If the response guarantees completion
    else if(requiredItemID < 0) {
        player.addItemToInventory(new Item(rewardItemID));
        return true;
    }
    return false;
}

void NPC::completeQuest(Player& player){
    player.addItemToInventory(new Item(rewardItemID));
}

int NPC::get_npc_stage() {
    return current_stage;
}

void NPC::set_npc_stage(int stage_index) {
    current_stage = stage_index;
}

string *NPC::get_name() {
    return &Name;
}

//void NPC::reset() {
//
//}




