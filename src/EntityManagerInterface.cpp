//
// Created by Omar Molina-Aguirre on 3/26/22.
//

#include "../include/EntityManagerInterface.h"
#include "NPC.h"
#include "SDL.h"
#include <stdlib.h>

//Initialization--------------------------------------------------------------------------------------
EntityManagerInterface::EntityManagerInterface() {}




//Modifiers------------------------------------------------------------------------------------------

/**
 * Add the newly allocated Entity to the hashtable
 * @param entity Entity to add, must be of form "new Entity()"
 * @return The assigned hashtable key
 */
int EntityManagerInterface::attachEntity(Entity* entity) {
    int tempID = rand();
    while(list.count(tempID) == 1) tempID = rand(); //Ensures no duplication
    entity->setID(tempID);
    list[tempID] = shared_ptr<Entity> (entity);
    return tempID;
}

/**
 * Remove entity from the hashmap
 * @param key ID of the Entity to remove
 */
void EntityManagerInterface::removeEntity(const int &key) {
    list.erase(key);
}

void EntityManagerInterface::removeAll(){
    list.clear();
}