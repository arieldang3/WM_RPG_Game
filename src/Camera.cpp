//
// Created by Omar Molina-Aguirre on 3/21/22.
/*
 * This Class records and manages the location of the screen camera. At render time, the render coordinates will be
 * Derived by finding the difference between the entities' absolute location and the camera's location, creating a delta
 * that, once adjusted by a quarter of the screen size so negative values do not draw out of the game window, represent
 * the correct on-screen renderable coordinates.
 */

#include <cmath>
#include <memory>
#include <vector>
#include "Camera.h"
#include "Constants.h"

using std::shared_ptr;
using std::vector;
using std::round;

//Constructor-----------------------------------------------------------------------------------------------------------
/**
 * Initialize Camera to follow a target
 * @param target Entity object reference for Camera to follow
 */
Camera::Camera(Entity& target){
    this->target = &target;
    loc =  {0.0, 0.0};
    size = {INTERNAL_WIDTH, INTERNAL_HEIGHT};
}


//Functions-------------------------------------------------------------------------------------------------------------
/**
 * Debug. Change the focus of the camera
 * @param target New Entity object for Camera to follow
 */
void Camera::changeTarget(Entity &target) {
    this->target = &target;
}

/**
 * Transform absolute world coordinates to screen-renderable coordinates
 * @param coordinate Absolute world coordinate
 * @param axis Camera axis on which to transform
 * @returns Int renderable coordinate
 */
int Camera::transform(const float& coord, const int& axis) {
    switch(axis){
        case 0:
            return (int) round(coord - loc[0] + (size[0] / 2));
            break;
        case 1:
            return (int) round(coord - loc[1] + (size[1] / 2));
            break;
        default:
            return 9999999;
    }
}

/**
 * Transform absolute world coordinates to screen-renderable FLOAT coordinates
 * @param coordinate Absolute world coordinate
 * @param axis Camera axis on which to transform
 * @returns float renderable coordinate
 */
float Camera::transformF(const float& coord, const int& axis) {
    switch(axis){
        case 0:
            return coord - loc[0] + (size[0] / 2);
            break;
        case 1:
            return coord - loc[1] + (size[1] / 2);
            break;
        default:
            return 9999999;
    }
}

/**
 * Move camera to follow target
 * @param delta Time elapsed to move
 */
void Camera::trackToTarget(const int& delta) {
    if (transformF(target->getPosition()[0], 0) < (size[0] * 1/3)){
        loc[0] -= delta / FRAMERATE_DIVISOR;
    }
    if (transformF(target->getPosition()[0], 0) > (size[0] * 2/3)){
        loc[0] += delta / FRAMERATE_DIVISOR;
    }
    //if (target->getPosition()[1] < loc[1] + (size[1] * 1/3)){
    if (transformF(target->getPosition()[1], 1) < (size[1] * 1/3)){
        loc[1] -= delta / FRAMERATE_DIVISOR;
    }
    if (transformF(target->getPosition()[1], 1) > (size[1] * 2/3)){
        loc[1] += delta / FRAMERATE_DIVISOR;
    }
    restrictToBounds();
}


/**
 * Instantly match the camera location to the target location
 */
void Camera::snapToTarget() {
    loc[0] = (int) round(target->getPosition()[0]);
    loc[1] = (int) round(target->getPosition()[1]);
    restrictToBounds();
}

void Camera::restrictToBounds() {
    if (loc[0] < (size[0] / 2)) loc[0] = (size[0] / 2);
    if (loc[1] < (size[1] / 2)) loc[1] = (size[1] / 2);
    if (loc[0] > 960 - (size[0] / 2)) loc[0] = 960 - (size[0] / 2);
    if (loc[1] > 720 - (size[1] / 2)) loc[1] = 720 - (size[1] / 2);
}