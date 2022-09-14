#include "Fighter.h"
#include "Constants.h"


Fighter::Fighter(const float& posx, const float& posy, int sizew, int sizeh) : Entity (posx, posy, sizew, sizeh) {
    velocity[0] = 0;
    velocity[1] = 0;
}

Fighter::~Fighter() = default;

//Sets Fighter velocity
void Fighter::setVelocity(int xVelocity, int yVelocity) {
    this->velocity[0] = xVelocity;
    this->velocity[1] = yVelocity;
}

//Reverses Fighter velocity
void Fighter::reverseVelocity(){
    this->velocity[0] *= -1;
    this->velocity[1] *= -1;
}

// Moves Fighter based on velocity
void Fighter::move(int deltaT){
    previousPosition[0] = position[0];
    previousPosition[1] = position[1];
    if(this->velocity[0] > 0) moveRight(this->velocity[0]*deltaT);
    if(this->velocity[0] < 0) moveLeft(this->velocity[0]*deltaT);
    if(this->velocity[1] > 0) moveDown(this->velocity[1]*deltaT);
    if(this->velocity[1] < 0) moveUp(this->velocity[1]*deltaT);

    //Limit player to bounds of map
    if (position[0] < 0) position[0] = 0;
    if (position[1] < 0) position[1] = 0;
    if (position[0] + size[0] > 960) position[0] = 960 - size[0];
    if (position[1] + size[1] > 720) position[1] = 720 - size[1];

    bounding_box.move(int(position[0])-int(previousPosition[0]), int(position[1])-int(previousPosition[1]));
}

void Fighter::moveUp(int amount) {
    position[1] += amount/FRAMERATE_DIVISOR;
}

void Fighter::moveDown(int amount) {
    position[1] += amount/FRAMERATE_DIVISOR;
}

void Fighter::moveLeft(int amount) {
    position[0] += amount/FRAMERATE_DIVISOR;
}

void Fighter::moveRight(int amount) {
    position[0] += amount/FRAMERATE_DIVISOR;
}

bool Fighter::collideWithEntity(const Entity& entity, int buffer){
    if(this->bounding_box.collideWithBox(entity.getBoundingBox(), buffer)){
        return true;
    }
    return false;
}

void Fighter::undoMove(){
    bounding_box.move(int(previousPosition[0])-int(position[0]), int(previousPosition[1])-int(position[1]));
    position[0] = previousPosition[0];
    position[1] = previousPosition[1];
    this->velocity[0] = 0;
    this->velocity[1] = 0;
}
