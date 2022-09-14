//
// Created by Omar Molina-Aguirre on 3/21/22.
//
/*
 * This Class records and manages the location of the screen camera. At render time, the render coordinates will be
 * Derived by finding the difference between the entities' absolute location and the camera's location, creating a delta
 * that, once adjusted by a quarter of the screen size so negative values do not draw out of the game window, represent
 * the correct on-screen renderable coordinates.
 */

#ifndef GAMEDESIGNTEAM1_CAMERA_H
#define GAMEDESIGNTEAM1_CAMERA_H

#include <memory>
#include <vector>
#include "Entity.h"
#include "Constants.h"

using std::shared_ptr;
using std::vector;

class Camera {
private:
    //Member Data
    vector<float> loc;  //{480, 360}
    vector<float> size; //{320, 240}
    Entity* target;

public:
    //Constructor
    Camera(Entity& target);

    //Functions
    void changeTarget(Entity& target);
    int transform(const float& coord, const int& axis);
    float transformF(const float& coord, const int& axis);
    void trackToTarget(const int& delta);
    void snapToTarget();
    void restrictToBounds(); //Magic numbers, will fix later
};


#endif //GAMEDESIGNTEAM1_CAMERA_H
