#ifndef CSCI437_CONSTANTS_H
#define CSCI437_CONSTANTS_H

#include <SDL.h>
#include <iostream>
#include <string>

const int TIME_LIMITS = 1200; //unit: seconds
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;
// Internal resolution
const int INTERNAL_WIDTH = 320;
const int INTERNAL_HEIGHT = 240;
//Map Dimensions
const int MAP_WIDTH =  960;
const int MAP_HEIGHT = 720;
const int GAMESTART_X = 572;
const int GAMESTART_Y = 425;
// Shrinked map data:
const int shrinked_map_x = 10;
const int shrinked_map_y = 30;
const int shrinked_map_w = 96;
const int shrinked_map_h = 72;
const int player_size_on_shrinked_map_w = 2;
const int player_size_on_shrinked_map_h = 2;

//Bullet speed
const int BULLET_SPEED = 3;

//Item list yay
const std::string ITEM_ARRAY[25] = {
    "Fist", "Note paper", "Rubber bands", "Bricks", "Rocks", //0-4 (Weapons)
    "Slingshot", "Longbow", "Crossbow", "Rifle", "Blowgun", //5-9 (Weapons)
    "Shirt", "Hoopskirt", "Textbook", "Laboratory smock", "Knight armor", //10-14 (Armor)
    "Hair", "Wig", "Tricorn hat", "Knight helmet", //15-18 (Helmet)
    "Boots of Speed", "Coffee", "Muscles", "Key", "Fire", "IOU" //19-24 (Misc)
    };

//npc list
const std::string NPC_ARRAY[12] = {
        "music_student", "stressed_student", "Anthropology_Professor", "George_Washington", "Brick_loving_Senior", //0-4
        "blacksmith", "librarian", "front_desk_person", "jitters", "strong_man", //5-9
        "lovestruck_student", "ignominy_student" //10-11
};

//Player List
#define FRAMERATE_DIVISOR 16.66666666666 //originally 9.0, stick to 1/monitor refresh rate when above 9


#endif //CSCI437_CONSTANTS_H
