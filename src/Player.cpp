#include "Fighter.h"
#include "Player.h"
#include "BoundingBox.h"
#include "typeinfo"
#include "Bullet.h"

#define ONE_MOVE_STEP_SIZE 1
#define WINDOW_SIZE_W 1024
#define WINDOW_SIZE_H 768

Player::Player(const float& posx, const float& posy, int sizew, int sizeh) : Fighter (posx, posy, sizew, sizeh) {
    //map_Index_i = 0;
    this->health = 10;
    this->playerSpeed = 1;
    this->damageDealt = 0;
    playerFacingDirection = West;
    pick_up_item_key_down = false;
    inventory.attachEntity(new Item(0));
    inventory.attachEntity(new Item(10));
    inventory.attachEntity(new Item(15));
}

void Player::resetPlayer(){
    this->health = 10;
    this->playerSpeed = 1;
    this->fireRate = 1000;
    this->damageDealt = 0;
    this->inventory.resetInventory();
    this->setVelocity(0,0);
    this->EnteredDungeon = false;
    this->won = false;
    snapPlayerXTo(GAMESTART_X);
    snapPlayerYTo(GAMESTART_Y);
    playerFacingDirection = West;
}

Player::~Player() = default;

int Player::getMaxHealth() {
    return 10 + 15*(inventory.getStrongestArmor().getPowerLevel() + inventory.getStrongestHelmet().getPowerLevel());
}

void Player::addItemToInventory(Item* item){
    if(checkPlayerHasItem(item->getItemID())) return;
    inventory.attachEntity(item);
    //picking up an item restores all your health (emergent gameplay, this was for armor item purposes originally)
    this->health = getMaxHealth();
    //Some items update player stats directly
    if(item->getType() == Other){
        //Boots of speed
        if(item->getItemID() == 19){
            this->playerSpeed *= 2;
        }
        //Coffee
        if(item->getItemID() == 20){
            this->fireRate /= 2;
        }
        //Muscles and firepower
        if(item->getItemID() == 21 || item->getItemID() == 23){
            this->damageDealt ++;
        }
    }
}

bool Player::collideWithEntity(const Entity& entity, int buffer){
    if(this->bounding_box.collideWithBox(entity.getBoundingBox(), buffer)){
        if(typeid(entity) == typeid(Item)){
            if (pick_up_item_key_down){
                inventory.attachEntity(new Item(entity.getID()));
                health = getMaxHealth();
            }
        }
        return true;
    }
    return false;
}

bool Player::collideWithEntityDialogBox(Entity& entity){
    if(this->bounding_box.collideWithBox(entity.getShowDialogBox(), 0)){
        return true;
    }
    return false;
}

Bullet Player::attack(){
    if(lastBulletTimestamp + fireRate < SDL_GetTicks()){
        lastBulletTimestamp = SDL_GetTicks();

        Bullet bill(this->position[0] + this->size[0]/2 - 2, this->position[1] + this->size[1]/2 - 2, 5, 5);
        switch(playerFacingDirection){
            case North:
                bill.setVelocity(0, -BULLET_SPEED);
                break;
            case East:
                bill.setVelocity(BULLET_SPEED, 0);
                break;
            case South:
                bill.setVelocity(0, BULLET_SPEED);
                break;
            case West:
                bill.setVelocity(-BULLET_SPEED, 0);
            }
        bill.setDamage(inventory.getStrongestWeapon().getPowerLevel() + this->damageDealt);
        bill.setAffiliation(false);
        return bill;
    }
    return Bullet(-15,-15,0,0);
}

void Player::snapPlayerXTo(const int &coord) {
    this->bounding_box.move(coord-int(position[0]), 0);
    previousPosition[0] = coord;
    position[0] = coord;
}

void Player::snapPlayerYTo(const int &coord) {
    this->bounding_box.move(0, coord - int(position[1]));
    previousPosition[1] = coord;
    position[1] = coord;
}

