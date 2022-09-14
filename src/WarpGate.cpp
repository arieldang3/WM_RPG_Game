#include "WarpGate.h"
#include "BoundingBox.h"
#include "Entity.h"
#include "Constants.h"
#include <cmath>

WarpGate::WarpGate(string newMapXML, const int& destX, const int& destY, const float& posx,const float& posy,int sizew, int sizeh, bool needsKey) : Entity(posx, posy, sizew, sizeh){
    this->newMapXML = newMapXML;
    this->destinationX     = destX;
    this->destinationY     = destY;
    this->needsKey = needsKey;
}

const int &WarpGate::destX() const {
    return destinationX;
}

const int &WarpGate::destY() const {
    return destinationY;
}

WarpGate::~WarpGate() = default;



