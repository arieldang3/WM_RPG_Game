#ifndef CSCI437_BULLET_H
#define CSCI437_BULLET_H

#include "Entity.h"

class Bullet : public Entity{
public:
    Bullet(const float& posx, const float& posy, int sizew, int sizeh);
//    Bullet(Bullet& bullet) ;
    Bullet(const float& posx, const float& posy, int sizew, int sizeh, float velX, float velY, int damage, bool isEnemyBullet);

    ~Bullet();

//    void checkPlayerLocation();
    void setVelocity(float velocityX, float velocityY);
    float* getVelocity(){return velocity;};
    void setDamage(int damage);
    int getDamage(){return damage;};
    void setAffiliation(bool isEnemyBullet);
    bool getAffiliation(){return isEnemyBullet;};
    void move(int deltaT);
//    void clearAll();

private:
    float velocity[2];
    int damage;
    bool isEnemyBullet;
};

#endif //CSCI437_BULLET_H