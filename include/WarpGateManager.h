#ifndef CSCI437_WarpGateManager_H
#define CSCI437_WarpGateManager_H

#include "EntityManagerInterface.h"
#include "Player.h"
#include "WarpGate.h"
#include <unordered_map>
#include <SDL.h>


class WarpGateManager : public EntityManagerInterface{
public:
    WarpGateManager();
    ~WarpGateManager();


//    void updateLogic(int deltaT, Player& player);
    void updateLogic(int deltaT){int i = 0;};

    shared_ptr<WarpGate> getPlayerSteppedOn(Player& player);


};

#endif //CSCI437_WarpGateManager_H