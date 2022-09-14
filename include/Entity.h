#ifndef CSCI437_ENTITY_H
#define CSCI437_ENTITY_H

#include "BoundingBox.h"
#include <SDL.h>
#include <string>

using std::string;

class Entity{
public:
    Entity(const float& posx, const float& posy, int sizew, int sizeh);
    ~Entity();

    //Accessors
    const float* getPosition() const;
    const int* getSize() const;
    const BoundingBox& getBoundingBox() const;
    const BoundingBox& getShowDialogBox() const;
    const int& getID() const;
    const string& getTexture() const;

    virtual bool collideWithEntity(const Entity& entity, int buffer = 0);//{return false;};

//    virtual bool collideWithEntity(const Entity& entity);//{return false;};

    //Modifiers
    void setID(const int& id);
    void setTextureID(const string& texture);

protected:
    // entity's position (Upper Left corner)
    float position[2]{};
    // entity's size
    int size[2]{};
    // entity's bounding box
    BoundingBox bounding_box;
    // If in this boc, show dialog
    BoundingBox show_dialog_box;

    //ID's
    int id;
    string textureID = "unset";


};


#endif //CSCI437_ENTITY_H
