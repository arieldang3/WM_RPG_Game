//
// Created by lordpotato on 4/22/22.
//

#ifndef CSCI437_XMLPARSER_H
#define CSCI437_XMLPARSER_H

#include <string>
#include <vector>
#include "NPCManager.h"
#include "ItemManager.h"
#include "EnemyManager.h"
#include "RenderManager.h"
#include "WarpGateManager.h"
#include "WallManager.h"
#include "tinyxml2.h"

using namespace tinyxml2;
using std::string;

class XMLParser {
public:
    XMLParser(shared_ptr<NPCManager>&   npcManager,   shared_ptr<ItemManager>&   itemManager,
              shared_ptr<EnemyManager>& enemyManager, shared_ptr<RenderManager>& renderManager,
              shared_ptr<WarpGateManager>& warpGateManager, shared_ptr<WallManager>& wallManager);
    XMLError generateFrom(const string& path);

private:

    void parseEntity(XMLElement& e, int& posX, int& posY, int& sizeX, int& sizeY, string& tex);

    XMLDocument xmlDoc;

    shared_ptr<NPCManager>      npcManager;
    shared_ptr<ItemManager>     itemManager;
    shared_ptr<EnemyManager>    enemyManager;
    shared_ptr<RenderManager>   renderManager;
    shared_ptr<WarpGateManager> warpGateManager;
    shared_ptr<WallManager> wallManager;
};


#endif //CSCI437_XMLPARSER_H
