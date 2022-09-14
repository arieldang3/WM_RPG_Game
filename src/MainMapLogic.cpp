#include "WMRPGame.h"


void WMRPGame::updateMainMapLogic(int deltaTime) {
    //check if player won
    if (player->getWon()) state = WIN;

    player->move(deltaTime);

    //Let's NPC-manage
    if(npcManager->collideWithPlayer(*player)){
        player->undoMove();
        soundServer.playNPC();
    }
    npcManager->collideWithPlayer_dialog_box(*player);

    //Let's enemy-manage
    enemyManager->updateLogic(deltaTime, *player);

    //Manage enemies + bullets yay
    for ( auto it = enemyManager->begin(); it != enemyManager->end(); ++it ){
        auto enemy(std::static_pointer_cast<Enemy>(it->second));
        //Check whether the enemy was hit by any already-existing bullets
        int hitCheck = enemy->getHealth();
        TestBulletManager->intersectFighter(*enemy);
        if (enemy->getHealth() < hitCheck) {
            soundServer.playEnemy();
        }
        //Create new bullet if this is a shooting enemy
        if(enemy->getType() == SHOOTING || enemy->getType() == BOSS) {
            Bullet bobby = enemy->shoot(*player);
            if(bobby.getPosition()[0] > -10) {
                int holdKey;
                holdKey = TestBulletManager->attachEntity(new Bullet(bobby.getPosition()[0], bobby.getPosition()[1], bobby.getSize()[0], bobby.getSize()[1], bobby.getVelocity()[0], bobby.getVelocity()[1], bobby.getDamage(), bobby.getAffiliation()));
                TestBulletManager->getEntityAt(holdKey).setTextureID("bolt");
                soundServer.playBolt();
            }
        } else if (enemy->getType() != SHOOTING) {
           if (enemy->collideWithEntity(*player, 6)) {
               enemy->hit(*player);
               soundServer.playHit();
           }
        }
    }

    int hitCheck = player->getHealth();
    //Let's bullet-manage
    TestBulletManager->updateLogic(deltaTime);
    TestBulletManager->intersectFighter(*player);
    //play damage sound if hit by bullet
    if (player->getHealth() < hitCheck) {
        soundServer.playHit();
    }
    //Let's item-manage
    TestItemManager->intersectFighter(*player);

    //Check for wall collisions
    for ( auto it = wallManager->begin(); it != wallManager->end(); ++it ){
        auto wall(std::static_pointer_cast<Wall>(it->second));
        //Enemies reverse their last step
        for ( auto it = enemyManager->begin(); it != enemyManager->end(); ++it ){
            auto enemy(std::static_pointer_cast<Enemy>(it->second));
            //Affects chasers only because don't program the circuits to go through walls, and if you want a shooter in the wall, your choice
            if(enemy->getType() == CHASING){
                if(enemy->collideWithEntity(*wall)){
                    enemy->undoMove();
                }
            }
        }
        //Bullets silently delete
        TestBulletManager->intersectWall(*wall);
        //Player reverses last step
        if(player->collideWithEntity(*wall)){
            player->undoMove();
        }
    }

    //Let's check the warp gates
    //If the player is intersecting a warp gate, it will return the xml file path
    //Use XMLParser::generateFrom(const string& path) to generate the new map
    // = warpGateManager->getPlayerSteppedOn(*player);
    shared_ptr<WarpGate> activatedGate = warpGateManager->getPlayerSteppedOn(*player);

    if (activatedGate != nullptr){
        int destX = activatedGate->destX();
        int destY = activatedGate->destY();
        std::string newMap = activatedGate->getNewMap();
        xmlParser->generateFrom(newMap);
        if (destX >= 0) player->snapPlayerXTo(destX);
        if (destY >= 0) player->snapPlayerYTo(destY);
        camera->snapToTarget();

        /*
        //Check to see if you ported into a wall. If so, keep going until you're not colliding any more
        for ( auto it = wallManager->begin(); it != wallManager->end(); ++it ){
            auto wall(std::static_pointer_cast<Wall>(it->second));
            while(player->collideWithEntity(*wall)){
                player->move(1);
            }
        }
         */

        //Check to see if entered the dungeon, on an xml map starting with "Dungeon"
        if(newMap.find("Dungeon_map_01") != std::string::npos) {
            this->EnteredDungeonState = true;
            player->EnteredDungeon = true;
            soundServer.playDungeonMusic();
        }
        if(newMap.find("25") != std::string::npos) soundServer.playSplash();

        // re-write the stage of npc
        npcManager->update_npc_current_stage_index(npcs_current_stage);

        // remove HAS picked up items
        //TestItemManager->remove_picked_up_items();

    }

}



