//
// Created by Omar Molina-Aguirre on 4/22/22.
//

#include <iostream>
#include <string>
#include "XMLParser.h"
#include "tinyxml2.h"

using namespace tinyxml2;
using std::string;
using std::cerr;
using std::endl;
using std::stoi;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif


XMLParser::XMLParser(shared_ptr<NPCManager>&   npcManager,   shared_ptr<ItemManager>&   itemManager,
                     shared_ptr<EnemyManager>& enemyManager, shared_ptr<RenderManager>& renderManager,
                     shared_ptr<WarpGateManager>& warpGateManager, shared_ptr<WallManager> & wallManager) {

    this -> npcManager      = npcManager;
    this -> itemManager     = itemManager;
    this -> enemyManager    = enemyManager;
    this -> renderManager   = renderManager;
    this -> warpGateManager = warpGateManager;
    this -> wallManager = wallManager;
}

XMLError XMLParser::generateFrom(const string& path) {

    //Clear all entities from managers
    itemManager->removeAll();
    npcManager->removeAll();
    enemyManager->removeAll();
    warpGateManager->removeAll();
    wallManager->removeAll();


    //Read Document
    XMLError eResult = xmlDoc.LoadFile(path.c_str());
    XMLCheckResult(eResult);

    //Get root element
    XMLNode* pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;

    //Get Map element
    XMLElement* map = pRoot->FirstChildElement("Map");
    if (map == nullptr) return XML_ERROR_PARSING_ELEMENT;

    //Set environment textures
    string ground = map->Attribute("ground");
    string canopy = map->Attribute("canopy");
    renderManager->changeEnvironment(ground, canopy);




    //Commonly used variables for parsing
    int locX, locY, sizeX, sizeY;
    string textureID = "unset";

    // ///////////////////////////////////////////////THE BIG ONES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    Instantiate all NPC's ------------------------------------------------------------------------------------------------------------------
    for (XMLElement* e = map->FirstChildElement("NPC"); e != NULL; e = e->NextSiblingElement("NPC")){

//         Parse the most common attributes
         parseEntity(*e, locX, locY, sizeX, sizeY, textureID);

         NPC* npc = new NPC(locX,locY,sizeX, sizeY, e->Attribute("name"));

         //Parse NPC's dialogue tree
         for (XMLElement* f = e->FirstChildElement("npc_speech"); f != NULL; f = f->NextSiblingElement("npc_speech")){
                int stage_index = stoi(f->Attribute("stage_index"));
                int sentence_index = stoi(f->Attribute("sentence_index"));
                string dialogue = f->Attribute("dialogue");

                npc->add_npc_speech(stage_index, sentence_index, dialogue);
         }
         //Parse Player's response options
         for (XMLElement* f = e->FirstChildElement("player_options"); f != NULL; f = f->NextSiblingElement("player_options")){
                int option_index = stoi(f->Attribute("option_index"));
                int stage_index = stoi(f->Attribute("stage_index"));
                int jump_to_stage_index = stoi(f->Attribute("jump_to_stage_index"));
                bool passingCondition = false;
                if (f->Attribute("givesReward")) passingCondition = true;
                string prompt = f->Attribute("prompt");

        		npc->add_player_options(option_index,stage_index,jump_to_stage_index,prompt, passingCondition);
         }

         npc->setQuestRequirements(stoi(e->Attribute("itemDesired")), stoi(e->Attribute("itemReward")));

//        //Create the NPC
        int holdKey;
        holdKey = npcManager->attachEntity(npc);
        npcManager->getEntityAt(holdKey).setTextureID(textureID);
    }


    //Instantiate all Items ------------------------------------------------------------------------------------------------------------------
    for (XMLElement* e = map->FirstChildElement("Item"); e != NULL; e = e->NextSiblingElement("Item")){

        //Parse the most common attributes
        parseEntity(*e, locX, locY, sizeX, sizeY, textureID);

        //Get the item ID
        int itemID = stoi(e->Attribute("id"));

        if (!itemManager->previouslySpawned(itemID)) {
            //Create the Item
            int holdKey;
            holdKey = itemManager->attachEntity(new Item(itemID, locX, locY, sizeX, sizeY));
            itemManager->getEntityAt(holdKey).setTextureID(textureID);
        }
    }


    //Instantiate all Enemies ----------------------------------------------------------------------------------------------------------------
    for (XMLElement* e = map->FirstChildElement("Enemy"); e != NULL; e = e->NextSiblingElement("Enemy")){

        //Parse the most common attributes
        parseEntity(*e, locX, locY, sizeX, sizeY, textureID);

        std::string type = e->Attribute("type");
        EnemyType typeOfEnemy;

        if(type == "SHOOTING") typeOfEnemy = SHOOTING;
        else if(type == "CIRCUIT1") typeOfEnemy = CIRCUIT1;
        else if(type == "CIRCUIT2") typeOfEnemy = CIRCUIT2;
        else if(type == "CIRCUIT3") typeOfEnemy = CIRCUIT3;
        else if(type == "BOSS") typeOfEnemy = BOSS;
        else typeOfEnemy = CHASING;


        //Create the Enemies
        int holdKey;
        if (e->Attribute("speed")) holdKey = enemyManager->attachEntity(new Enemy(locX, locY, sizeX, sizeY, typeOfEnemy, stoi(e->Attribute("speed"))));
        else holdKey = enemyManager->attachEntity(new Enemy(locX, locY, sizeX, sizeY, typeOfEnemy));
        enemyManager->getEntityAt(holdKey).setTextureID(textureID);
    }



    //Instantiate all Warp Gates-------------------------------------------------------------------------------------------------------------
    for (XMLElement* e = map->FirstChildElement("Warp"); e != NULL; e = e->NextSiblingElement("Warp")){

        //Parse the most common attributes
        parseEntity(*e, locX, locY, sizeX, sizeY, textureID);

        //Get the destination location
        string dest_loc = e->Attribute("dest_loc");
        int destX, destY;
        if (dest_loc.substr(0, dest_loc.find(", ")) == "match")
            destX = -1;
        else
            destX = stoi(dest_loc.substr(0, dest_loc.find(", ")));

        if (dest_loc.substr(dest_loc.find(", ") + 2) == "match")
            destY = -1;
        else
            destY = stoi(dest_loc.substr(dest_loc.find(", ") + 2));

        //Get the destination data sheet
        string dest_map = e->Attribute("dest_map");

        //Check if it requires a key
        bool needsKey = false;
        if (e->Attribute("needsKey")) needsKey = true;

        //Create the Warp Gate
        warpGateManager->attachEntity(new WarpGate(dest_map, destX, destY, locX, locY, sizeX, sizeY, needsKey));
    }

    //Instantiate all WALLS-------------------------------------------------------------------------------------------------------------
     for (XMLElement* e = map->FirstChildElement("Wall"); e != NULL; e = e->NextSiblingElement("Wall")){
         //Parse the most common attributes
         parseEntity(*e, locX, locY, sizeX, sizeY, textureID);

         //Create the Wall
         wallManager->attachEntity(new Wall(locX, locY, sizeX, sizeY));
     }

    //---------------------------------------------------------------------------------------------------------------------------------------


    return XML_SUCCESS;
}

/**
 * Avoids repeat code or parsing the common attributes of the
 * @param e
 * @param posX
 * @param posY
 * @param sizeX
 * @param sizeY
 * @param tex
 */
void XMLParser::parseEntity(XMLElement &e, int &posX, int &posY, int &sizeX, int &sizeY, string& tex) {
    //Parse the location
    string location = e.Attribute("loc");
    posX = stoi(location.substr(0, location.find(", ")));
    posY = stoi(location.substr(location.find(", ") + 2));

    //Parse the size
    string size = e.Attribute("size");
    sizeX = stoi(size.substr(0, size.find(", ")));
    sizeY = stoi(size.substr(size.find(", ") + 2));

    //Get the entity texture
    if (e.Attribute("texture")) tex = e.Attribute("texture");
}