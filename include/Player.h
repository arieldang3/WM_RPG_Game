#ifndef CSCI437_TEMPFORTESTINGPLAYER_H
#define CSCI437_TEMPFORTESTINGPLAYER_H

#include "Fighter.h"
#include "ItemManager.h"
#include "Item.h"
#include "Bullet.h"
#include <cmath>

#define MAP_NUM 2

enum PlayerPosition { Normal, LeftUpCorner, LeftDownCorner, RightUpCorner, RightDownCorner };

enum PlayerFacing {North, South, East, West};

class Player : public Fighter{
public:
    Player(const float& posx,const float& posy,int sizew, int sizeh);
    ~Player();
    //void cleanUpPlayer(); // Clean up

    bool collideWithEntity(const Entity& entity, int buffer = 0);
    bool collideWithEntityDialogBox(Entity &entity);

    PlayerFacing getPlayerDirection(){return playerFacingDirection;};
    void setPlayerDirection(PlayerFacing direction){playerFacingDirection = direction;};
    int getSpeed(){return playerSpeed;};
    int getMaxHealth();

    void resetPlayer();

    void addItemToInventory(Item* item);
    bool checkPlayerHasItem(int itemID){return inventory.hasItem(itemID);};
    void removeItemFromInventory(int itemID){inventory.removeEntity(itemID);};
    const ItemManager& getInventory()const {return inventory;};

    void addTimePenalty(int penaltyMilliSeconds){this->timePenalty += penaltyMilliSeconds;};
    int getTimePenalty(){return this->timePenalty;};

    Bullet attack();

    //Methods for warp gates to move player
    void snapPlayerXTo(const int& coord);
    void snapPlayerYTo(const int& coord);

    void setWon() {won = true;};
    bool getWon() {return won;};

    bool EnteredDungeon = false;


private:

    PlayerFacing playerFacingDirection;
    int playerSpeed;
    int timePenalty = 0;
    bool won = false;

    ItemManager inventory;

};


#endif //CSCI437_TEMPFORTESTINGPLAYER_H
