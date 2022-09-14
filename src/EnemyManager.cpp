#include "EnemyManager.h"
#include "Constants.h"

EnemyManager::EnemyManager() : EntityManagerInterface(){
}

void EnemyManager::removeAll(){
    list.clear();
}

void EnemyManager::updateLogic(int deltaT, Player& player){
    int toRemove[list.size()];
    int i = 0;
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto enemy(std::static_pointer_cast<Enemy>(it->second));
        //Move the enemy
        enemy->move(player, deltaT);

        //Intersect the enemy with the player
        if(player.collideWithEntity(*enemy, 0)){
           // player.reduceHealth(2); //THIS IS HARDCODED TOUCH DAMAGE FOR NOW, SHOULD CHANGE
            player.reverseVelocity();
            player.move(deltaT);
            player.setVelocity(0,0);
        }

        //If the enemy is dead, remove it from the list
        if(enemy->getHealth() <= 0){
            if (enemy->getType() == BOSS) player.setWon();
            enemy->kill();
            toRemove[i] = it->first;
            i++;
        }
    }
    //Remove enemies tagged for deletion
    for(int j = 0; j < i; j++){
        removeEntity(toRemove[j]);
    }

}

//void EnemyManager::intersectFighter(Fighter& fighter){
//    for ( auto it = list.begin(); it != list.end(); ++it ){
//        auto enemy(std::static_pointer_cast<Enemy>(it->second));
//        if(typeid(fighter) == typeid(Player)){// && bullet->getAffiliation() == true) || (typeid(fighter) == typeid(Enemy) && bullet->getAffiliation() == false)){
//            Player& player = (Player&) fighter;
//            //If collides, do damage to player
//            if(player.collideWithEntity(*enemy)){
//                player.reduceHealth(2); //THIS IS HARDCODED TOUCH DAMAGE FOR NOW, SHOULD CHANGE
//                player.reverseVelocity();
//                player.move(deltaTime);
//                player.setVelocity(0,0);
//            }
//        }
//    }
//}


EnemyManager::~EnemyManager() = default;
