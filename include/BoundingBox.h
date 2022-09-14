#ifndef CSCI437_BOUNDINGBOX_H
#define CSCI437_BOUNDINGBOX_H

class BoundingBox{

public:
    BoundingBox(const float& posx,const float& posy,int Sizew, int Sizeh);
    ~BoundingBox();

    const int* getTLCorner() const{return TLCorner;};
    const int* getBRCorner() const{return BRCorner;};

    void move(int newX, int newY);
    bool collideWithBox(const BoundingBox& box, int buffer) const;

private:
    int TLCorner[2];
    int BRCorner[2];
};


#endif //CSCI437_BOUNDINGBOX_H
