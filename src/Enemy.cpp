#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const float& posx, const float& posy, int sizew, int sizeh, EnemyType type) : Fighter (posx, posy, sizew, sizeh) {
    // set initial values
    this->type = type;
    this->damageDealt = 2;
    if (type == SHOOTING) {
        this->health = 8*2;
    } else if (type == CHASING) {
        this->health = 4*2;
    } else if (type == BOSS) {
        this->health = 30*2;
    } else {
        this->health = 6*2;
        this->startX = posx;
        this->startY = posy;
        this->route = rand() % 2;
    }
    this->alive = true;
}

Enemy::Enemy(const float& posx, const float& posy, int sizew, int sizeh, EnemyType type, float speed) : Fighter (posx, posy, sizew, sizeh) {
    // enemy with different speed
    this->type = type;
    this->speed = speed;
    this->damageDealt = 2;
    if (type == SHOOTING) {
        this->health = 8*2;
    } else if (type == CHASING) {
        this->health = 4*2;
    } else if (type == BOSS) {
        this->health = 30*2;
    } else {
        this->health = 6*2;
        this->startX = posx;
        this->startY = posy;
        this->route = rand() % 2;
    }
    this->alive = true;
}

Enemy::~Enemy() = default;

// move enemy based on players position
void Enemy::move(Player &player, int deltaT) {
    previousPosition[0] = position[0];
    previousPosition[1] = position[1];
    // delay if just hit player
    if (this->lastHitTime + fireRate < SDL_GetTicks()) {
        // switch movements based on enemy type
        // follow player
        if (type == CHASING || type == BOSS) {
                const float* moveTo = player.getPosition();
                const int* playerSize = player.getSize();
                if (moveTo[0] + playerSize[0] < position[0]) {
                    int amount = position[0] - (moveTo[0] + playerSize[0]);
                    if (amount > .5*deltaT) amount = .5*deltaT;
                    moveLeft(-amount*this->speed);
                }
                if (moveTo[0] > position[0] + size[0]) {
                    int amount = moveTo[0] - (position[0] + size[0]);
                    if (amount > .5*deltaT) amount = .5*deltaT;
                    moveRight(amount*this->speed);
                }
                if (moveTo[1] + playerSize[1] < position[1]) {
                    int amount = position[1] - (moveTo[1] + playerSize[1]);
                    if (amount > .5*deltaT) amount = .5*deltaT;
                    moveUp(-amount*this->speed);
                }
                if (moveTo[1] > position[1] + size[1]) {
                    int amount = moveTo[1] - (position[1] + size[1]);
                    if (amount > .5*deltaT) amount = .5*deltaT;
                    moveDown(amount*this->speed);
                }
            // move in rectangular path
            } else if (type == CIRCUIT1) {
                if (previousPosition[0] <= startX) moveUp(-.5*deltaT*this->speed);
                if (previousPosition[1] <= startY - 50) moveRight(.5*deltaT*this->speed);
                if (previousPosition[0] >= startX + 100) moveDown(.5*deltaT*this->speed);
                if (previousPosition[1] >= startY + 50) moveLeft(-.5*deltaT*this->speed);
            // move along vertical line
            } else if (type == CIRCUIT2) {
                // randomly start up or down
                if (route == 0) {
                    if (previousPosition[0] <= startX) moveUp(-.5*deltaT*this->speed);
                    if (previousPosition[1] <= startY - 100) moveRight(2*this->speed);
                    if (previousPosition[0] > startX) moveDown(.5*deltaT*this->speed);
                    if (previousPosition[1] >= startY + 100) moveLeft(-2*this->speed);
                } else {
                    if (previousPosition[0] <= startX) moveDown(.5*deltaT*this->speed);
                    if (previousPosition[1] >= startY + 100) moveRight(2*this->speed);
                    if (previousPosition[0] > startX) moveUp(-.5*deltaT*this->speed);
                    if (previousPosition[1] <= startY - 100) moveLeft(-2*this->speed);
                }
            // move along horizontal line
            } else if (type == CIRCUIT3) {
                // randomly start left or right
                if (route == 0) {
                    if (previousPosition[1] <= startY) moveRight(.5*deltaT*this->speed);
                    if (previousPosition[0] >= startX + 100) moveDown(2*this->speed);
                    if (previousPosition[1] > startY) moveLeft(-.5*deltaT*this->speed);
                    if (previousPosition[0] <= startX - 100) moveUp(-2*this->speed);
                } else {
                    if (previousPosition[1] <= startY) moveLeft(-.5*deltaT*this->speed);
                    if (previousPosition[0] <= startX - 100) moveDown(2*this->speed);
                    if (previousPosition[1] > startY) moveRight(.5*deltaT*this->speed);
                    if (previousPosition[0] >= startX + 100) moveUp(-2*this->speed);
                }
            }
        bounding_box.move(int(position[0])-int(previousPosition[0]), int(position[1])-int(previousPosition[1]));
        if(position[0] < previousPosition[0]) facingLeft = true;
        else facingLeft = false;
    }
} 

// close range combat with delay
void Enemy::hit(Player &player) {
    if(this->lastHitTime + fireRate*2 < SDL_GetTicks()) {
        this->lastHitTime = SDL_GetTicks();
        player.reduceHealth(2*damageDealt);
    }
}

// shoot bullet towards player every two seconds
Bullet Enemy::shoot(Player &player) {
    if(this->lastBulletTimestamp + fireRate < SDL_GetTicks()){
        this->lastBulletTimestamp = SDL_GetTicks();
        Bullet bill(position[0] + size[0]/2 - 2, position[1] + size[1]/2 - 2, 5, 5);
        const float* shootAt = player.getPosition();
        const float* origin = bill.getPosition();
        float x = shootAt[0] + player.getSize()[0]/2-2 - origin[0];
        float y = shootAt[1] + player.getSize()[1]/2-2 - origin[1];
        float denom = sqrt(x*x+y*y);
        x = x/denom;
        y = y/denom;
        bill.setVelocity(x, y);
        bill.setDamage(1*damageDealt);
        bill.setAffiliation(true);
        return bill;
    }
    return Bullet(-15,-15,0,0);
}