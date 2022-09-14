#ifndef CSCI437_Wall_H
#define CSCI437_Wall_H

#include "Entity.h"

class Wall : public Entity{
public:
    Wall(const float& posx,const float& posy,int sizew, int sizeh): Entity(posx, posy, sizew, sizeh){};
    ~Wall();

};

#endif //CSCI437_Wall_H

