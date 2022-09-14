#include "BoundingBox.h"

BoundingBox::BoundingBox(const float& posx,const float& posy,int Sizew, int Sizeh){
    TLCorner[0] = posx;
    TLCorner[1] = posy;
    BRCorner[0] = posx + Sizew;
    BRCorner[1] = posy + Sizeh;
}

/*
Updates the bounding box's location (should be called when player moves) according to the vector of movement
**/
void BoundingBox::move(int xDirection, int yDirection){
    BRCorner[0] += xDirection;
    BRCorner[1] += yDirection;
    TLCorner[0] += xDirection;
    TLCorner[1] += yDirection;
}

bool BoundingBox::collideWithBox(const BoundingBox& box, int buffer) const{
    if(this->TLCorner[0] <= box.getBRCorner()[0] + buffer && box.getTLCorner()[0] <= this->BRCorner[0] + buffer){
        if(this->TLCorner[1] <= box.getBRCorner()[1] + buffer && box.getTLCorner()[1] <= this->BRCorner[1] + buffer){
            return true;
        }
    }
    return false;
}

BoundingBox::~BoundingBox(){
}







