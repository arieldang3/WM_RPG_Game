#ifndef CSCI437_ITEM_H
#define CSCI437_ITEM_H

#include "Entity.h"
#include "Constants.h"
#include <string>

enum itemType { Armor, Helmet, Weapon, Other };

class Item : public Entity{
public:
    //Don't use these ones, they'll deprecate
    Item(int itemID, itemType type, int powerLevel, const float& posx,const float& posy,int sizew, int sizeh);
    Item(int itemID, itemType type, int powerLevel);

    //Use this, way easier. Check Item id in Constants.h, or the desired behavior in these methods
    Item(int itemID, const float& posx,const float& posy,int sizew, int sizeh);
    Item(int itemID);
    ~Item();

    const std::string getItemName() const {return itemName;};//ITEM_ARRAY[itemID];};
    const int getItemID() const {return itemID;};
    const itemType getType() const {return type;};
    const int getPowerLevel() const {return powerLevel;};
//    void checkPlayerLocation();
    void clearUp();
    int getAttributes();
    void setAttributes(int shield, int attack, int speed);

private:
    int shieldAmount;
    int attackDamage;
    int speedChange;
    std::string itemName;
    itemType type;
    int powerLevel;
    int itemID;
};

#endif //CSCI437_ITEM_H

