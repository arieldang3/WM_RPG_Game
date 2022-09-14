#include "BulletManager.h"
#include "Constants.h"

BulletManager::BulletManager() : EntityManagerInterface(){
//    BulletList = new std::unordered_map<int, Bullet>;
}

//void BulletManager::removeAll(){
//    list.clear();
//}

void BulletManager::updateLogic(int deltaT){
    int toRemove[list.size()];
    int i = 0;
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto bullet(std::static_pointer_cast<Bullet>(it->second));
        bullet->move(deltaT);
        //If the bullet has gone off-map, it's not coming back and should be deleted.
        if(it->second->getPosition()[0] < -10 || it->second->getPosition()[0] > SCREEN_WIDTH + 10 || it->second->getPosition()[1] < -10 || it->second->getPosition()[1] > SCREEN_HEIGHT + 10){
            toRemove[i] = it->first;
            i++;
        }
    }
    //Remove bullets tagged for deletion
    for(int j = 0; j < i; j++){
        removeEntity(toRemove[j]);
    }
}

void BulletManager::intersectFighter(Fighter& fighter){
    int toRemove[list.size()];
        int i = 0;
        for ( auto it = list.begin(); it != list.end(); ++it ){
            //No friendly fire, bullets won't interact with allies (allowed to pass through because spawned from center of fighter)
            auto bullet(std::static_pointer_cast<Bullet>(it->second));
            if((typeid(fighter) == typeid(Player) && bullet->getAffiliation() == true) || (typeid(fighter) == typeid(Enemy) && bullet->getAffiliation() == false)){
                //If collides, do damage and delete bullet
                if(fighter.collideWithEntity(*bullet, 0)){
                    toRemove[i] = it->first;
                    i++;
                    fighter.reduceHealth(bullet->getDamage());
                }
            }
        }
        //Remove bullets tagged for deletion
        for(int j = 0; j < i; j++){
            removeEntity(toRemove[j]);
        }
}

void BulletManager::intersectWall(Wall& wall){
    int toRemove[list.size()];
    int i = 0;
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto bullet(std::static_pointer_cast<Bullet>(it->second));
        if(wall.collideWithEntity(*bullet)){
            toRemove[i] = it->first;
            i++;
        }
    }
    //Remove bullets tagged for deletion
    for(int j = 0; j < i; j++){
        removeEntity(toRemove[j]);
    }
}

BulletManager::~BulletManager() = default;
