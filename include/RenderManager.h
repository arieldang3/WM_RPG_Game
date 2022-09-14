//
// Created by Omar Molina-Aguirre on 3/31/22.
//

#ifndef CSCI437_RENDERMANAGER_H
#define CSCI437_RENDERMANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include "Camera.h"
#include "Player.h"
#include "Entity.h"
#include "NPCManager.h"
#include "EntityManagerInterface.h"
#include "Timer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using std::string;
using std::vector;


class RenderManager {
public:
    //Constructor
    RenderManager(SDL_Renderer* renderer_ptr, shared_ptr<Camera>& camera,
                  shared_ptr<Player>& player,Timer& timer);

    //Functions
    void addManagerToCrawl(const shared_ptr<EntityManagerInterface>& manager);
    void addNpcManagerToCrawl(const shared_ptr<NPCManager>& manager);
    void render();
    void changeEnvironment(string& ground, string& canopy);
    void show_aux_map();
    void close_aux_map();




private:
    //Supporting Functions
    void renderBackground();
    void renderActiveElements();
    void renderAccessories();
    void renderGUI();
    void renderText(SDL_Renderer *r, const std::string& msg, int fontsize, SDL_Color c, vector<int> pos);
    void renderTimer(int penaltySeconds);
    void renderDialogBox();
    void renderNPCSpeech(SDL_Renderer *r, const std::string& msg, int fontsize, SDL_Color c, vector<int> pos);
    void renderShrinkedMap();
//    void set_npc_speech_string(const std::string& msg);

    //Error Messages
    void csci437_error(const std::string& msg);
    void csci437_img_error(const std::string& msg);
    void csci437_ttf_error(const std::string &msg);


    //Texture Methods
    void loadTextures();
    SDL_Texture* renderImage(SDL_Renderer *r,const string& fileName);

    //SDL Members
    SDL_Renderer* renderer;
    SDL_Rect      destRect;

    //Texture members
    //---Texture Path Atlas
    unordered_map<string, SDL_Texture*> textureID;

    //---Variable Textures
    string groundPath = "../resource/maps/test_map_ground.png";
    string canopyPath = "../resource/maps/test_map_canopy.png";

    //Other Members
    vector<shared_ptr<EntityManagerInterface>> managers;
    shared_ptr<Camera> camera;
    shared_ptr<Player> player;
    shared_ptr<Timer> timer;
    shared_ptr<NPCManager> npcManager;
//    std::map<string, NPC*>* current_npc_map_ptr;
    string npc_speech_string;
    // Button to show the map to player or not
    bool SHOWTHEMAP;

};


#endif //CSCI437_RENDERMANAGER_H
