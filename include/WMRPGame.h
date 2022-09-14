#ifndef GAMEDESIGNTEAM1_WMRPGAME_H
#define GAMEDESIGNTEAM1_WMRPGAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
//#include "Constants.h"
#include "Camera.h"
#include "Player.h"
#include "NPC.h"
#include "Timer.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "ItemManager.h"
#include "Item.h"
#include "NPCManager.h"
#include "SoundServer.h"
#include "Enemy.h"
#include "RenderManager.h"
#include "EnemyManager.h"
#include "XMLParser.h"
#include "WarpGateManager.h"
#include "Wall.h"
#include "WallManager.h"
using namespace std;

enum GameState{ START, INSTRUCTIONS, PLAYING, WIN, LOSE };

class WMRPGame {
public:
    WMRPGame();
    ~WMRPGame();

    void init();
    void gameLoop();
    void cleanUP();

    // Message Report
    static void csci437_error(const std::string& msg);
    static void csci437_img_error(const std::string& msg);
    static void csci437_ttf_error(const std::string& msg);

    // Some Short Cuts of SDL2
    static void renderText(SDL_Renderer *r, const string &msg, int fontsize, SDL_Color c, vector<int> pos);
    static SDL_Texture* renderImage(SDL_Renderer *r, const std::string& fileName);

private:
    // Track the Game state
    static GameState state;
    GameState prevState = WIN;
    bool EnteredDungeonState;
    // PAUSE the game
    static bool PAUSED;
    static bool INVENTORY_SHOWING;

    // sdl2 render tools
    SDL_Window *gameWindow;
    SDL_Renderer *renderer;

    SDL_Texture *titleImage;
    SDL_Texture *backgroundInstructionsImage;

    // Game Views
    void updateView();
    //void renderMainMapView();
    void renderSTARTView();
    void renderInstructionsView();
    void renderLoseView();
    void renderWinView();

    void renderPAUSEDView();
    void renderInventoryView();

    // Game Logic
    void updateLogic(int deltaTime);
    void updateMainMapLogic(int deltaTime);
    bool handleEvents(SDL_Event e);
    void init_npc_status();

    // a Logic Player
    shared_ptr<Player> player;
    //Parser
    XMLParser* xmlParser;
    //Rendering Camera
    shared_ptr<Camera> camera;
    // Timer in game
    Timer timer;

    //Logic Members
    shared_ptr<NPCManager>    npcManager;
    shared_ptr<BulletManager> TestBulletManager;
    shared_ptr<ItemManager>   TestItemManager;
    shared_ptr<EnemyManager>  enemyManager;
    shared_ptr<RenderManager> renderManager;
    shared_ptr<WarpGateManager> warpGateManager;
    shared_ptr<WallManager> wallManager;

    // class with sounds
    SoundServer soundServer;

    std::map<string, int> npcs_current_stage;

};

#endif //GAMEDESIGNTEAM1_WMRPGAME_H
