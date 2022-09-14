#ifndef CSCI437_FIGHTER_H
#define CSCI437_FIGHTER_H

#include "Entity.h"

#define MAP_NUM 2

class Fighter : public Entity {
public:
    Fighter(const float& posx, const float& posy, int sizew, int sizeh);
    ~Fighter();
    void cleanUpFighter();

    void attack();
    int getFireRate(){return fireRate;};
    void reduceHealth(int damage){health -= damage;};
    void increaseHealth();
    int getHealth() {return this->health;};
    int getMaxHealth();
    bool pick_up_item_key_down = true;
    void die();

    void setVelocity(int xVelocity, int yVelocity);
    void reverseVelocity();
    virtual void move(int deltaT); //calls the appropriate move function below
    void moveUp(int amount);
    void moveDown(int amount);
    void moveLeft(int amount);
    void moveRight(int amount);
    void undoMove();
    bool isFacingLeft() {return velocity[0] <= 0;};

    virtual bool collideWithEntity(const Entity& entity, int buffer = 0);

protected:
    int velocity[2];
    int health;
    int damageDealt;
    int fireRate = 1000;
    int lastBulletTimestamp = 0;

    float previousPosition[2];
};

#endif //CSCI437_FIGHTER_H