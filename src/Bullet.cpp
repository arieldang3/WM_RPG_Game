#include "Entity.h"
#include "Bullet.h"

#define FRAMERATE_DIVISOR 9.0

Bullet::Bullet(const float& posx, const float& posy, int sizew, int sizeh) : Entity(posx, posy, sizew, sizeh){
    int i = 0;
}

/*
Parameters (8): float position x, y; int size w, h; float velocity x, y; int damage, bool isEnemyBullet
**/
Bullet::Bullet(const float& posx, const float& posy, int sizew, int sizeh, float velX, float velY, int damage, bool isEnemyBullet) : Entity(posx, posy, sizew, sizeh){
    this->velocity[0] = velX;
    this->velocity[1] = velY;
    this->damage = damage;
    this->isEnemyBullet = isEnemyBullet;
}

Bullet::~Bullet() = default;

void Bullet::setVelocity(float velocityX, float velocityY){
    this->velocity[0] = velocityX;
    this-> velocity[1] = velocityY;
}
void Bullet::setDamage(int damage) {this->damage = damage;}
void Bullet::setAffiliation(bool isEnemyBullet) {this->isEnemyBullet = isEnemyBullet;}

void Bullet::move(int deltaT){
    int prevPosition[2];
    prevPosition[0] = int(position[0]);
    prevPosition[1] = int(position[1]);
    position[0] += this->velocity[0]*deltaT/FRAMERATE_DIVISOR;
    position[1] += this->velocity[1]*deltaT/FRAMERATE_DIVISOR;
    bounding_box.move(int(position[0])-prevPosition[0], int(position[1])-prevPosition[1]);
}
