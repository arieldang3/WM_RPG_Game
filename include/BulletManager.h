#ifndef CSCI437_BULLETMANAGER_H
#define CSCI437_BULLETMANAGER_H

#include "Bullet.h"
#include "EntityManagerInterface.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include <unordered_map>
#include <SDL.h>


class BulletManager : public EntityManagerInterface{
public:
    BulletManager();
    ~BulletManager();

//    void removeAll();

    void intersectFighter(Fighter& fighter);
    void intersectWall(Wall& wall);


    void updateLogic(int deltaT);

    int totalNumBullets(){return list.size();};

};

#endif //CSCI437_BULLETMANAGER_H