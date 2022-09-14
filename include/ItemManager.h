#ifndef CSCI437_ITEMMANAGER_H
#define CSCI437_ITEMMANAGER_H

#include "Item.h"
#include "EntityManagerInterface.h"
#include "Fighter.h"
#include <unordered_map>
#include <SDL.h>
#include <map>
#include "Constants.h"

class ItemManager : public EntityManagerInterface{
public:
    ItemManager();
    ~ItemManager();

    int attachEntity(Entity* item) override;
    void removeEntity(const int &key) override;
//    void removeAll();

    //Inventory purposes
    const Item& getStrongestWeapon()const{return *strongestWeapon;};
    const Item& getStrongestArmor()const{return *strongestArmor;};
    const Item& getStrongestHelmet()const{return *strongestHelmet;};

    void intersectFighter(Fighter& player);
    void resetInventory();

    bool hasItem(int itemID);
    bool previouslySpawned(const int& itemID);

    void updateLogic(int deltaT);

    void remove_picked_up_items();

private:
//    std::unordered_map<int, const Item> *ItemList;
    Item* strongestWeapon;
    Item* strongestArmor;
    Item* strongestHelmet;
    std::map<int, int> checked_item;
};

#endif //CSCI437_ITEMMANAGER_H