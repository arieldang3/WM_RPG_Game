#ifndef CSCI437_ENEMYMANAGER_H
#define CSCI437_ENEMYMANAGER_H

#include "EntityManagerInterface.h"
#include "Player.h"
#include "Enemy.h"
#include <unordered_map>
#include <SDL.h>


class EnemyManager : public EntityManagerInterface{
public:
    EnemyManager();
    ~EnemyManager();

    void removeAll();

    void intersectFighter(Fighter& fighter);

    void updateLogic(int deltaT, Player& player);
    void updateLogic(int deltaT){int i = 0;};

};

#endif //CSCI437_ENEMYMANAGER_H