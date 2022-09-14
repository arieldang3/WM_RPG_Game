#ifndef CSCI437_WARPGATE_H
#define CSCI437_WARPGATE_H

#include "Entity.h"
#include <string>

class WarpGate : public Entity{
public:
    WarpGate(std::string newMap, const int& destX, const int& destY, const float& posx,const float& posy,int sizew, int sizeh, bool needsKey = false);
    ~WarpGate();

    const int getWarpGateID() const {return WarpGateID;};

    //Negative destination values equate to matching the previous values (leaving them alone)
    const int& destX() const;
    const int& destY() const;

    bool gateNeedsKey() {return this->needsKey;};

    std::string getNewMap() const {return newMapXML;};

private:
    std::string newMapXML;
    int WarpGateID;
    int destinationX;
    int destinationY;
    bool needsKey;
};

#endif //CSCI437_WarpGate_H

