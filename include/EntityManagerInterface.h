//
// Created by Omar Molina-Aguirre on 3/26/22.
//

#ifndef GAMEDESIGNTEAM1_ENTITYMANAGERINTERFACE_H
#define GAMEDESIGNTEAM1_ENTITYMANAGERINTERFACE_H

#include<memory>
#include <unordered_map>
#include <set>
#include"Entity.h"
#include "Fighter.h"


using std::iterator;
using std::set;
using std::unordered_map;
using std::shared_ptr;


class EntityManagerInterface {
public:

    //Initialization
    EntityManagerInterface();
//    ~EntityManagerInterface();

    //Modifiers
    virtual void updateLogic(int deltaT) = 0;
    virtual int attachEntity(Entity* entity);
    virtual void removeEntity(const int& key);
    void intersectFighter(Fighter& fighter);
    void removeAll();
    int get_to_remove(){return to_remove;};

    //Inspectors
    Entity& getEntityAt(const int& index){return *list[index];};
    unordered_map<int, shared_ptr<Entity>>::const_iterator begin() const{return list.begin();};
    unordered_map<int, shared_ptr<Entity>>::const_iterator end() const{return list.end();};


protected:
    //Members
    unordered_map<int, shared_ptr<Entity>> list;
    int to_remove = -1;
};


#endif //GAMEDESIGNTEAM1_ENTITYMANAGERINTERFACE_H

/*
 * A middle man is good because it can just hold everything and give it out when it needs
 */
