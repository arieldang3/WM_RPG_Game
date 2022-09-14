#include "Item.h"
#include "BoundingBox.h"
#include "Entity.h"
#include "Constants.h"
#include <cmath>

//DEPRECATING
Item::Item(int itemID, itemType type, int powerLevel, const float& posx,const float& posy,int sizew, int sizeh) : Entity(posx, posy, sizew, sizeh){
    this->itemID = itemID;
    this->type = type;
    this->powerLevel = powerLevel;
}

Item::Item(int itemID, itemType type, int powerLevel) : Entity(-8, -8, 0, 0){
    this->itemID = itemID;
    this->type = type;
    this->powerLevel = powerLevel;
}

//USE THESE
/*
Item creation for onscreen items, pass the id, position, and sizes;
**/
Item::Item(int ID, const float& posx,const float& posy,int sizew, int sizeh) : Entity(posx, posy, sizew, sizeh){
    this->itemID = ID;
    this->id = ID;
    switch(id){
        case 0:
            this->itemName = "Fist";
            this->type = Weapon;
            this->powerLevel = 0;
            break;
        case 1:
            this->itemName = "Note paper";
            this->type = Weapon;
            this->powerLevel = 1;
            break;
        case 2:
            this->itemName = "Rubber bands";
            this->type = Weapon;
            this->powerLevel = 1;
            break;
        case 3:
            this->itemName = "Bricks";
            this->type = Weapon;
            this->powerLevel = 2;
            break;
        case 4:
            this->itemName = "Rocks";
            this->type = Weapon;
            this->powerLevel = 2;
            break;
        case 5:
            this->itemName = "Slingshot";
            this->type = Weapon;
            this->powerLevel = 3;
            break;
        case 6:
            this->itemName = "Longbow";
            this->type = Weapon;
            this->powerLevel = 4;
            break;
        case 7:
            this->itemName = "Crossbow";
            this->type = Weapon;
            this->powerLevel = 4;
            break;
        case 8:
            this->itemName = "Rifle";
            this->type = Weapon;
            this->powerLevel = 5;
            break;
        case 9:
            this->itemName = "Blowgun";
            this->type = Weapon;
            this->powerLevel = 6;
            break;
        case 10:
            this->itemName = "Shirt";
            this->type = Armor;
            this->powerLevel = 0;
            break;
        case 11:
            this->itemName = "Hoopskirt";
            this->type = Armor;
            this->powerLevel = 2;
            break;
        case 12:
            this->itemName = "Textbook";
            this->type = Armor;
            this->powerLevel = 1;
            break;
        case 13:
            this->itemName = "Laboratory smock";
            this->type = Armor;
            this->powerLevel = 2;
            break;
        case 14:
            this->itemName = "Knight armor";
            this->type = Armor;
            this->powerLevel = 3;
            break;
        case 15:
            this->itemName = "Hair";
            this->type = Helmet;
            this->powerLevel = 0;
            break;
        case 16:
            this->itemName = "Wig";
            this->type = Helmet;
            this->powerLevel = 1;
            break;
        case 17:
            this->itemName = "Tricorn hat";
            this->type = Helmet;
            this->powerLevel = 2;
            break;
        case 18:
            this->itemName = "Knight helmet";
            this->type = Helmet;
            this->powerLevel = 3;
            break;
        default:
            this->itemName = "UNDEFINED";
            this->type = Other;
            this->powerLevel = 0;
    }
}

/*
Item creation for held items - sets position and size to ignorable values
**/
Item::Item(int ID) : Entity(-10, -10, 0, 0){
    this->itemID = ID;
    this->id = ID;
    switch(id){
        case 0:
            this->itemName = "Fist";
            this->type = Weapon;
            this->powerLevel = 0;
            break;
        case 1:
            this->itemName = "Note paper";
            this->type = Weapon;
            this->powerLevel = 1;
            break;
        case 2:
            this->itemName = "Rubber bands";
            this->type = Weapon;
            this->powerLevel = 1;
            break;
        case 3:
            this->itemName = "Bricks";
            this->type = Weapon;
            this->powerLevel = 2;
            break;
        case 4:
            this->itemName = "Rocks";
            this->type = Weapon;
            this->powerLevel = 2;
            break;
        case 5:
            this->itemName = "Slingshot";
            this->type = Weapon;
            this->powerLevel = 3;
            break;
        case 6:
            this->itemName = "Longbow";
            this->type = Weapon;
            this->powerLevel = 4;
            break;
        case 7:
            this->itemName = "Crossbow";
            this->type = Weapon;
            this->powerLevel = 5;
            break;
        case 8:
            this->itemName = "Rusty rifle";
            this->type = Weapon;
            this->powerLevel = 5;
            break;
        case 9:
            this->itemName = "Blowgun";
            this->type = Weapon;
            this->powerLevel = 6;
            break;
        case 10:
            this->itemName = "Shirt";
            this->type = Armor;
            this->powerLevel = 0;
            break;
        case 11:
            this->itemName = "Hoopskirt";
            this->type = Armor;
            this->powerLevel = 1;
            break;
        case 12:
            this->itemName = "Textbook";
            this->type = Armor;
            this->powerLevel = 2;
            break;
        case 13:
            this->itemName = "Laboratory smock";
            this->type = Armor;
            this->powerLevel = 2;
            break;
        case 14:
            this->itemName = "Knight armor";
            this->type = Armor;
            this->powerLevel = 3;
            break;
        case 15:
            this->itemName = "Hair";
            this->type = Helmet;
            this->powerLevel = 0;
            break;
        case 16:
            this->itemName = "Wig";
            this->type = Helmet;
            this->powerLevel = 1;
            break;
        case 17:
            this->itemName = "Tricorn hat";
            this->type = Helmet;
            this->powerLevel = 2;
            break;
        case 18:
            this->itemName = "Knight helmet";
            this->type = Helmet;
            this->powerLevel = 3;
            break;
        case 19:
            this->itemName = "Boots of speed";
            this->type = Other;
            this->powerLevel = 0;
            break;
        case 20:
            this->itemName = "Coffee";
            this->type = Other;
            this->powerLevel = 0;
            break;
        case 21:
            this->itemName = "Strong arm";
            this->type = Other;
            this->powerLevel = 0;
            break;
        case 22:
            this->itemName = "Key";
            this->type = Other;
            this->powerLevel = 0;
            break;
        case 23:
            this->itemName = "Fire power";
            this->type = Other;
            this->powerLevel = 0;
            break;
        case 24:
            this->itemName = "IOU Note";
            this->type = Other;
            this->powerLevel = 0;
            break;

        default:
            this->itemName = "UNDEFINED";
            this->type = Other;
            this->powerLevel = 0;
    }
}


Item::~Item() = default;



