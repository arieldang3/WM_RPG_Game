#include "Entity.h"

using std::string;

Entity::Entity(const float& posx, const float& posy, int sizew, int sizeh) : bounding_box(posx, posy, sizew, sizeh), show_dialog_box(posx-5,posy-5,sizew+10,sizeh+10){
    position[0] = posx;
    position[1] = posy;
    size[0] = sizew;
    size[1] = sizeh;
    this->id = (int) SDL_GetTicks();
 }




//Accessors---------------------------------------------------------------
const float* Entity::getPosition() const{
    return position;
}

const int* Entity::getSize() const{
    return size;
}

const BoundingBox& Entity::getBoundingBox() const{
    return bounding_box;
}

const BoundingBox& Entity::getShowDialogBox() const{
    return show_dialog_box;
}

Entity::~Entity() {

}

/**
 * Gets the assigned hashtable key of Entity instance
 * @return Reference to Entity ID
 */
const int& Entity::getID() const{
    return id;
}

/**
 * Gets the key for the texture look up table
 * @return String with the name of the texture
 */
const string& Entity::getTexture() const {
    return textureID;
}




//Modifiers-------------------------------------------------------------------------------------------------------------

/**
 * Store the hashtable key of Entity instance
 * @param id Int Entity ID to assign
 */
void Entity::setID(const int& id) {
    this -> id = id;
}


void Entity::setTextureID(const string& texture) {
    this -> textureID = texture;
}

//Collision

bool Entity::collideWithEntity(const Entity& entity, int buffer){
    if(this->bounding_box.collideWithBox(entity.getBoundingBox(), buffer)){
        return true;
    }
    return false;
}





