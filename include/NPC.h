#ifndef CSCI437_NPC_H
#define CSCI437_NPC_H

#include "Entity.h"
#include "Player.h"
#include <string>
#include <map>

//enum NPCState{ NOTTALKED = 0, TALKING=1, HasTALKED=2 };


class NPC : public Entity{
public:
    NPC(const float& posx,const float& posy,int sizew, int sizeh, string npc_name);
    ~NPC();

    string* get_npc_sentence_speak();
    string* option1_string_ptr;
    string* option2_string_ptr;
    string* option3_string_ptr;
    string* option4_string_ptr;
    bool display_do_not_have_item_yet;
    string* get_name();
//    void reset();


//    bool show_option1;
//    bool show_option2;
//    bool show_option3;
//    bool show_option4;
    void add_npc_speech(int stage_index, int sentence_index, string text);
    void add_player_options(int option_index, int stage_index, int jump_to_stage_index, string text, bool passingCondition = false);
//    void set_conversation_separate_indices(int sep1, int sep2, int sep3);

    void next_sentence(int event_index, Player& player, std::map<string, int> &npc_stage_index_map);

    void setQuestRequirements(int requiredItemID, int rewardItemID);
    bool checkQuestCompleted(Player& player);
    int get_npc_stage();
    void set_npc_stage(int stage_index);
    void completeQuest(Player& player);

    void switch_NPC_display();

    void cleanUp();


private:
    int Index_current_sentence;

    bool display;
//    NPCState npc_state;
//    int conversation_separate_indices[3];
    int current_stage;

    std::map<int, string> speech; // int = stage_index *10 + sentence_index
    std::map<int, std::pair<string,std::pair<int, bool>>> option1; // old_stage_index ; new_stage_number
    std::map<int, std::pair<string,std::pair<int, bool>>> option2;
    std::map<int, std::pair<string,std::pair<int, bool>>> option3;
    std::map<int, std::pair<string,std::pair<int, bool>>> option4;

    int requiredItemID;
    int rewardItemID;
    string Name;


};

#endif //CSCI437_NPC_H