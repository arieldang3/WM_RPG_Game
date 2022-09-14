#ifndef CSCI437_ENEMY_H
#define CSCI437_ENEMY_H

#include "Fighter.h"
#include "Player.h"

enum EnemyType {SHOOTING, CHASING, CIRCUIT1, CIRCUIT2, CIRCUIT3, BOSS};

class Enemy : public Fighter{
public:
    Enemy(const float& posx, const float& posy, int sizew, int sizeh, EnemyType type);
    Enemy(const float& posx, const float& posy, int sizew, int sizeh, EnemyType type, float speed);

    ~Enemy();

    void move(Player &player, int deltaT);
    bool getAlive() {return alive;}
    void kill() {alive = false;}
    Bullet shoot(Player &player);
    EnemyType getType() {return type;}
    void hit(Player &player);

    bool isFacingLeft(){return facingLeft;};


private:
    EnemyType type;
    bool alive;
    int lastHitTime = 0;
    int startX;
    int startY;
    int route;
    float speed = 1;
    bool facingLeft = true;
};

#endif //CSCI437_ENEMY_H