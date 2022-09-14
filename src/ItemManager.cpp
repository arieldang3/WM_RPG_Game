#include "ItemManager.h"
#include "Constants.h"

ItemManager::ItemManager() : EntityManagerInterface(){
    strongestWeapon = new Item(0);
    strongestArmor = new Item(10);
    strongestHelmet = new Item(15);
    checked_item.clear();
}

/*
Adds a new item to the list
**/
int ItemManager::attachEntity(Entity* entity){
    Item* item = (Item*) entity;
    shared_ptr<Entity> tempPtr (item);
    tempPtr->setID(item->getItemID());
    list[item->getItemID()] = tempPtr;

    // add the item to checked_item_map;
    auto iter = checked_item.find(item->getItemID());
    if (iter==checked_item.end()){
        checked_item.insert(std::pair<int,int>(item->getItemID(),0));
    }

    //Check to see whether this new item is better than something else you have
    //ONLY MATTERS FOR INVENTORY
    switch(item->getType()){
        case Weapon:
            if(item->getPowerLevel() > strongestWeapon->getPowerLevel()) strongestWeapon = item;
            break;
        case Armor:
            if(item->getPowerLevel() > strongestArmor->getPowerLevel()) strongestArmor = item;
            break;
        case Helmet:
            if(item->getPowerLevel() > strongestHelmet->getPowerLevel()) strongestHelmet = item;
            break;
    }
    return item->getItemID();
}

void ItemManager::removeEntity(const int &key) {

    //If it's player inventory do fancy check to update strongest items
    if(hasItem(0)){

        auto removedItem(std::static_pointer_cast<Item>(list[key]));
        if(removedItem->getType() == Weapon && removedItem->getPowerLevel() == strongestWeapon->getPowerLevel()){
            list.erase(key);
            auto stronk(std::static_pointer_cast<Item>(list[0]));
            strongestWeapon = stronk.get();
            for ( auto it = list.begin(); it != list.end(); ++it ){
                auto item(std::static_pointer_cast<Item>(it->second));
//                if(item->getType() == Weapon){
//                    if(item->getPowerLevel() >= strongestWeapon->getPowerLevel()){
//                        strongestWeapon = item.get();
//                    }
//                }
                if(item->getType() == Weapon && item->getPowerLevel() > strongestWeapon->getPowerLevel()) strongestWeapon = item.get();
            }
        }
        if(removedItem->getType() == Armor && removedItem->getPowerLevel() == strongestArmor->getPowerLevel()){
            list.erase(key);
            auto stronk(std::static_pointer_cast<Item>(list[10]));
            strongestArmor = stronk.get();
            for ( auto it = list.begin(); it != list.end(); ++it ){
                auto item(std::static_pointer_cast<Item>(it->second));
                if(item->getType() == Armor && item->getPowerLevel() > strongestArmor->getPowerLevel()) strongestArmor = item.get();
            }
        }
        if(removedItem->getType() == Helmet && removedItem->getPowerLevel() == strongestHelmet->getPowerLevel()){
            list.erase(key);
            auto stronk(std::static_pointer_cast<Item>(list[15]));
            strongestHelmet = stronk.get();
            for ( auto it = list.begin(); it != list.end(); ++it ){
                auto item(std::static_pointer_cast<Item>(it->second));
                if(item->getType() ==  Helmet && item->getPowerLevel() > strongestHelmet->getPowerLevel()) strongestHelmet = item.get();
            }
        }
    }

    else list.erase(key);
}

void ItemManager::resetInventory(){
    removeAll();
    strongestWeapon = new Item(0);
    strongestArmor = new Item(10);
    strongestHelmet = new Item(15);
    checked_item.clear();
    attachEntity(new Item(0));
    attachEntity(new Item(10));
    attachEntity(new Item(15));
}

void ItemManager::updateLogic(int deltaT){
    int i = 0;
}

//ITEMS ON MAP
void ItemManager::intersectFighter(Fighter& player){
//    int toRemove;
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto item(std::static_pointer_cast<Item>(it->second));
        if(player.collideWithEntity(*item, 0)){
            //item is added to player inventory within Player code
            to_remove = it->first;
//            return;
            if(player.pick_up_item_key_down){
                removeEntity(to_remove);
                auto iterr = checked_item.find(to_remove);
                if (iterr!=checked_item.end()){
                    iterr->second = 1; // marked it as HAS picked up
                }
                player.pick_up_item_key_down = false;
            }
            return;
        }
    }
    to_remove = -1;
//    if(toRemove) removeEntity(toRemove);
}

bool ItemManager::hasItem(int itemID){
    if(list.count(itemID) > 0) return true;
    return false;
}

bool ItemManager::previouslySpawned(const int &itemID) {
    if (checked_item.count(itemID) > 0 && checked_item[itemID] != 0) return true;
    else return false;
}

void ItemManager::remove_picked_up_items() {
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto item(std::static_pointer_cast<Item>(it->second));
        auto iter = checked_item.find(item->getItemID());
        if (iter!=checked_item.end()){
            if (iter->second==1){
                removeEntity(item->getItemID());
            }
        }
    }
}

ItemManager::~ItemManager() = default;
