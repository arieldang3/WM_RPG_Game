#ifndef CSCI437_WallManager_H
#define CSCI437_WallManager_H

#include "EntityManagerInterface.h"
#include "Player.h"
#include "Wall.h"
#include <unordered_map>
#include <SDL.h>


class WallManager : public EntityManagerInterface{
public:
    WallManager();
    ~WallManager();

    void updateLogic(int deltaT, Player& player);
    void updateLogic(int deltaT){int i = 0;};
//    void intersectFighter(Fighter& fighter);

};

#endif //CSCI437_WallManager_H