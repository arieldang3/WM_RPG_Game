#include "WallManager.h"

WallManager::WallManager() : EntityManagerInterface(){
    int i = 0;
}



//Intersect with player
//bool WallManager::intersectThing(Entity& fighter){
//    for ( auto it = list.begin(); it != list.end(); ++it ){
//        auto wall(std::static_pointer_cast<Wall>(it->second));
//        if(fighter.collideWithEntity(*wall)){
//            return true;
//        }
//    }
//    return false;
//}


WallManager::~WallManager() = default;
