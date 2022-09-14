#include "WarpGateManager.h"

WarpGateManager::WarpGateManager() : EntityManagerInterface(){
    int i = 0;
}

//void WarpGateManager::updateLogic(int deltaT, Player& player){
//    for ( auto it = list.begin(); it != list.end(); ++it ){
//        //Intersect the warp gate with the player
//        if(player.collideWithEntity(*(it->second))){
//            //UPDATE THE XML SHEET
//        }
//    }
//}

//Intersect with player
shared_ptr<WarpGate> WarpGateManager::getPlayerSteppedOn(Player& player){
    for ( auto it = list.begin(); it != list.end(); ++it ){
        auto gate(std::static_pointer_cast<WarpGate>(it->second));
        if(player.collideWithEntity(*gate)){
            if(gate->gateNeedsKey()){
                if(player.checkPlayerHasItem(22)) return gate;
            }
            else return gate;
        }
    }
    return nullptr;
}


WarpGateManager::~WarpGateManager() = default;
