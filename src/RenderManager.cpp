//
// Created by Omar Molina-Aguirre on 3/31/22.
//

/**This class offloads the core rendering tasks of the game from the main game loop. This does not the title, win, or lose
 * states. It crawls through each of the Managers polling for location and texture information and draws each element
 * in its rightful place on the screen.
 **/

#include "RenderManager.h"
#include "Camera.h"
#include "Constants.h"
#include "Entity.h"
#include "EntityManagerInterface.h"
#include "WMRPGame.h"
#include "SDL.h"
#include "SDL_image.h"
#include <typeinfo>

using std::string;
using std::unordered_map;

//Fixed Textures
#define TEXT_FONT_FILE "../resource/alagard.ttf"
#define VIGNETTE_IMAGE "../resource/accessory_maps/vignette.png"


//Constructor-----------------------------------------------------------------------------------------------------------

RenderManager::RenderManager(SDL_Renderer *renderer_ptr, shared_ptr<Camera>& camera,
                             shared_ptr<Player>& player, Timer& timer) {
    renderer     = renderer_ptr;
    this->camera = camera;
    this->player = player;
    this->timer  = shared_ptr<Timer> (&timer);
    SHOWTHEMAP = false;

    loadTextures();
}


//Functions-------------------------------------------------------------------------------------------------------------

/**
 * Lets the Render Manager know which lists to crawl for entities to render
 * @param manager Reference to the Manager to craw
 */
void RenderManager::addManagerToCrawl(const shared_ptr<EntityManagerInterface> &manager) {
    managers.push_back(manager);
}

/**
 * Calls all routines that execute the process of crawling through all elements and rendering them
 */
void RenderManager::render() {
    //Prepare for render
    SDL_RenderSetLogicalSize(renderer, INTERNAL_WIDTH, INTERNAL_HEIGHT);

    // Clear the last drawing
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);

    //Do the rendering in parts
    renderBackground();
    renderActiveElements();
    renderAccessories();
    renderGUI();

    renderDialogBox();

    //Present the newly rendered surface
    SDL_RenderPresent(renderer);
}

/**
 * Changes the textures of the ground and canopy by raw paths as there are too many to enumerate. Deletes the previous
 * texture pointer in the look-up table and creates a new texture in its place
 *
 * @param ground String with path to ground texture, relative to main.cpp
 * @param canopy String with path to canopy texture, relative to main.cpp
 */
void RenderManager::changeEnvironment(string& ground, string& canopy) {
    if (textureID["ground"] != nullptr) SDL_DestroyTexture(textureID["ground"]);
    textureID["ground"] = renderImage(renderer, ground);

    if (textureID["canopy"] != nullptr) SDL_DestroyTexture(textureID["canopy"]);
    textureID["canopy"] = renderImage(renderer, canopy);
}



//Supporting Functions--------------------------------------------------------------------------------------------------

/**
 * Renders the ground texture
 */
void RenderManager::renderBackground() {
    destRect = { camera->transform(0, 0),
                 camera->transform(0, 1), //Map is always at (0, 0)
                 MAP_WIDTH,MAP_HEIGHT};
    SDL_RenderCopy(renderer, textureID["ground"], nullptr, &destRect);
}



/**
 * v--------------------------------------------------------------------------------------------------------------------------------------- The Big One
 * Crawls through the added Entity Managers, extracts coordinate and size information and renders the requested texture
 * at that location
 */
void RenderManager::renderActiveElements() {
    //Render non-player Entities--------------------------------
    //Loop through all the managers
    for (shared_ptr<EntityManagerInterface> manager : managers) {
        for (auto it = manager->begin(); it != manager->end(); ++it) {
            destRect = { camera->transform(it->second->getPosition()[0], 0),
                         camera->transform(it->second->getPosition()[1], 1),
                         it->second->getSize()[0],it->second->getSize()[1]};
            //Sanitize texture, use debug render if invalid
            if (it->second->getTexture() == "unset" || textureID.count(it->second->getTexture()) == 0){
                SDL_RenderCopy(renderer, textureID["unset"], &destRect, &destRect);
            }
            else{
                //Get direction of enemy
                try{
                    auto squirrel(std::static_pointer_cast<Enemy>(it->second));
                    if(squirrel->isFacingLeft()) SDL_RenderCopy(renderer, textureID[it->second->getTexture()], nullptr, &destRect);
                    else SDL_RenderCopyEx(renderer, textureID[it->second->getTexture()], nullptr, &destRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
                }
                catch(const std::exception& e){
                    //SDL_RenderCopy(renderer, textureID[it->second->getTexture()], nullptr, &destRect);
                }

                //Special case to enlarge magic bolts for glow effect
                try{
                    shared_ptr<Bullet> bolt (std::static_pointer_cast<Bullet>(it->second));
                    //Avoid affecting Enemies, for whatever reason they pass the test
                    if ( (typeid(*bolt.get()) != typeid(Enemy)) && (bolt->getAffiliation()) ){
                        //render nothing, these bullets are handled in renderAccessories()
                    }
                }
                catch(const std::exception& e){
                    //Finally, the normal render
                    SDL_RenderCopy(renderer, textureID[it->second->getTexture()], nullptr, &destRect);
                }
            }
        }
        if (manager->get_to_remove()!=-1){
            SDL_Color textColor = {0, 0, 0};
            renderText(renderer,"ENTER to pickup",15,textColor,{10,INTERNAL_HEIGHT-50});
//            manager->removeEntity(manager->get_to_remove());
        }
    }
    //Render Player---------------------------------------------
    destRect = { camera->transform(player->getPosition()[0], 0),
                 camera->transform(player->getPosition()[1], 1),
                 player->getSize()[0],player->getSize()[1]};
    static bool facing = true;
    if (player->getPlayerDirection() == West) facing = true;
    if (player->getPlayerDirection() == East) facing = false;
    if (facing){
        SDL_RenderCopy(renderer, textureID["player_base"], nullptr, &destRect);
    }
    else{
        SDL_RenderCopyEx(renderer, textureID["player_base"], nullptr, &destRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }
}
/**
 * Renders aesthetic/non-essential layers to the foreground
 */
void RenderManager::renderAccessories() {
    //Canopy
    destRect = { camera->transform(0, 0),
                 camera->transform(0, 1), //Map is always at (0, 0)
                 MAP_WIDTH,MAP_HEIGHT};
    SDL_RenderCopy(renderer, textureID["canopy"], nullptr, &destRect);

    //Bullet Glow
    for (auto it = managers[0]->begin(); it != managers[0]->end(); ++it) {
        shared_ptr<Bullet> bolt (std::static_pointer_cast<Bullet>(it->second));
        if (bolt->getAffiliation()) {
            destRect = {camera->transform(it->second->getPosition()[0] - 42, 0),
                        camera->transform(it->second->getPosition()[1] - 42, 1),
                        it->second->getSize()[0] + 84, it->second->getSize()[1] + 84};
            SDL_RenderCopy(renderer, textureID[it->second->getTexture()], nullptr, &destRect);
        }
    }

    //Vignette
    destRect = { 0,0,320,240};
    SDL_RenderCopy(renderer, textureID["vignette"], nullptr, &destRect);
}

/**
 * Render User Interface related elements
 */
void RenderManager::renderGUI() {
    //Render health
    SDL_Color textColor = { 255, 255, 255 };
    textColor = {0, 0, 0};
    if(player->EnteredDungeon) textColor = {255,255,255};

    renderText(renderer,"Health: " + std::to_string(100*player->getHealth()/player->getMaxHealth()) + "%", 15,textColor,{50,10});

    //Render Time
    if(!player->EnteredDungeon) renderTimer(player->getTimePenalty());

    if (SHOWTHEMAP){
        renderShrinkedMap();
    }
}

/**
 * A short cut to draw text
 * @param r the render want to use
 * @param msg the text want to draw
 * @param fontsize the font size
 * @param c a color
 * @return a texture pointer
 */
void RenderManager::renderText(SDL_Renderer *r, const std::string& msg, int fontsize, SDL_Color c, vector<int> pos) {
    // init TTF
    TTF_Init();
    if( TTF_Init() < 0 ) csci437_ttf_error("Unable to initialize TTF!");
    TTF_Font* font    = TTF_OpenFont(TEXT_FONT_FILE, fontsize);
    SDL_Surface* text = TTF_RenderText_Solid(font, msg.c_str(), c );
    if ( text == nullptr ) csci437_ttf_error("Unable to render text!");

    // convert to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface( r, text );
    if(texture == nullptr) csci437_error("Could not create texture from surface!");
    SDL_Rect dst = { pos[0], pos[1], text->w, text->h};
    SDL_RenderCopyEx( r, texture, NULL, &dst, 0, NULL, SDL_FLIP_NONE );

    // clean up
    SDL_FreeSurface(text);
    text = nullptr;
    TTF_CloseFont(font);
    font = nullptr;
    SDL_DestroyTexture( texture );
    texture = nullptr;
}

/**
 * Render the Timer
 */
void RenderManager::renderTimer(int penaltySeconds) {
    int seconds = TIME_LIMITS - ((penaltySeconds + (int) timer->getTickPassed()) / 1000);
    std::stringstream clock;
    int minutes = (int) seconds / 60;
    seconds = (int) seconds % 60;
    clock << "Timer: ";
    if (minutes<10){
        clock << "0";
    }
    clock << minutes << " : ";
    if (seconds<10){
        clock << "0";
    }
    clock << seconds;
    SDL_Color textColor = {0, 0, 0};
    renderText(renderer,clock.str(), 15,textColor,{INTERNAL_WIDTH/2+15,10});
}


//Error Messages--------------------------------------------------------------------------------------------------------

void RenderManager::csci437_error(const std::string& msg)
{
    std::cerr << msg << " (" << SDL_GetError() << ")" << std::endl;
    exit(0);
}
void RenderManager::csci437_img_error(const std::string& msg)
{
    std::cerr << msg << " (" << IMG_GetError() << ")" << std::endl;
    exit(0);
}
void RenderManager::csci437_ttf_error(const std::string &msg) {
    std::cerr << msg << " (" << TTF_GetError() << ")" << std::endl;
    exit(0);
}


//Texture Methods-------------------------------------------------------------------------------------------------------

/**
 * Streamlines the process of loading textures
 * @param r Pointer to Renderer in use
 * @param fileName Named constant of path to texture
 * @return
 */
SDL_Texture* RenderManager::renderImage(SDL_Renderer *r,const std::string& fileName){
    // Load the image background
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if(tempSurface == nullptr) csci437_img_error("Could not image!");

    // convert to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface( r, tempSurface );
    if(texture == nullptr) csci437_error("Could not create texture from surface!");

    // free the surface (since has converted the surface to texture)
    SDL_FreeSurface( tempSurface );
    tempSurface = nullptr;
    return texture;
}
/**
 * add npcManager to renderManager
 * @param manager an npcManager
 */
void RenderManager::addNpcManagerToCrawl(const shared_ptr<NPCManager> &manager) {
    npcManager = manager;
    managers.push_back(npcManager);
}
/**
 * render the dialog box
 */
void RenderManager::renderDialogBox() {
    if (this->npcManager->NPC_is_speaking()){
        /* draw a dialog box */
        SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD );
        SDL_SetRenderDrawColor(renderer, 139, 139, 122, 100);
        destRect = {6, INTERNAL_HEIGHT/2+31, INTERNAL_WIDTH-10, INTERNAL_HEIGHT/2-34};
        SDL_RenderFillRect(renderer, &destRect);
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_RenderDrawRect(renderer, &destRect);

        /* draw npc's speech */
       SDL_Color textColor = {0, 0, 0};
       renderText(renderer,"ENTER to continue", 15,textColor,{10,INTERNAL_HEIGHT-18});
       npc_speech_string = *(npcManager->get_npc_speaking_string());
       renderNPCSpeech(renderer,npc_speech_string, 15,textColor,{10,INTERNAL_HEIGHT/2+36});

       /* draw player's options */
       if (npcManager->option1!= nullptr){
           SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD );
           SDL_SetRenderDrawColor(renderer, 130, 130, 120, 100);
           destRect = {INTERNAL_WIDTH/2-10-3,INTERNAL_HEIGHT/2+15-2, INTERNAL_WIDTH/2+10+3, 17};
           SDL_RenderFillRect(renderer, &destRect);
           renderText(renderer,*(npcManager->option1), 15,textColor,{INTERNAL_WIDTH/2-10,INTERNAL_HEIGHT/2+15});
       }
       if (npcManager->option2!= nullptr){
           SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD );
           SDL_SetRenderDrawColor(renderer, 130, 130, 120, 100);
           destRect = {INTERNAL_WIDTH/2-10-3,INTERNAL_HEIGHT/2-2, INTERNAL_WIDTH/2+10+3, 17};
           SDL_RenderFillRect(renderer, &destRect);
           renderText(renderer,*(npcManager->option2), 15,textColor,{INTERNAL_WIDTH/2-10,INTERNAL_HEIGHT/2});
       }
       if (npcManager->option3!= nullptr){
           SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD );
           SDL_SetRenderDrawColor(renderer, 130, 130, 120, 100);
           destRect = {INTERNAL_WIDTH/2-10-3,INTERNAL_HEIGHT/2-15-2, INTERNAL_WIDTH/2+10+3, 17};
           SDL_RenderFillRect(renderer, &destRect);
           renderText(renderer,*(npcManager->option3), 15,textColor,{INTERNAL_WIDTH/2-10,INTERNAL_HEIGHT/2-15});
       }
       if (npcManager->option4!= nullptr){
           SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD );
           SDL_SetRenderDrawColor(renderer, 130, 130, 120, 100);
           destRect = {INTERNAL_WIDTH/2-10-3,INTERNAL_HEIGHT/2-30-2, INTERNAL_WIDTH/2+10+3, 17};
           SDL_RenderFillRect(renderer, &destRect);
           renderText(renderer,*(npcManager->option4), 15,textColor,{INTERNAL_WIDTH/2-10,INTERNAL_HEIGHT/2-30});
       }

       /* draw "no item yet" if no item yet */
       if (npcManager->display_no_item_str()){
           SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD );
           SDL_SetRenderDrawColor(renderer, 130, 130, 120, 100);
           destRect = {INTERNAL_WIDTH/2-50-10,INTERNAL_HEIGHT/2-50,INTERNAL_WIDTH/2-15,17};
           SDL_RenderFillRect(renderer, &destRect);
           renderText(renderer,"No required item.", 15,textColor,{INTERNAL_WIDTH/2-30-20,INTERNAL_HEIGHT/2-50});
       }

   }
}
/**
 * auxiliary function to render the npc's speech (change line if necessary)
 * @param r renderer
 * @param msg text to render
 * @param fontsize
 * @param c color
 * @param pos position (of the first word in the speech)
 */
void RenderManager::renderNPCSpeech(SDL_Renderer *r, const std::string& msg, int fontsize, SDL_Color c, vector<int> pos) {
    SDL_Color textColor = {0, 0, 0};
    int len = msg.length();
    if (len<=41){
        renderText(r,msg, fontsize,c,pos);
    }
    else {
        int i = 0;
        std::stringstream str1;
        while (i < (len+40)) {
            int j = 0;
            if (msg[i+j]==' '){j++;}
            while (j < 40) {
                if ((i + j+1) == len) {
                    break;
                }
                str1 << msg[i + j];
                j++;
            }
            if ((i+j+1)==len){
                str1 << msg[i + j];
                renderText(r, str1.str(), fontsize, c, {pos[0], pos[1] + fontsize * (i/40)});
                break;
            }
            else if (msg[i + j-1] != ' ' && msg[i + j] != ' ') {
                str1 << '-';
            }
            renderText(r, str1.str(), fontsize, c, {pos[0], pos[1] + fontsize * (i/40)});
            i=i+40;
            str1.str("");
        }
    }
}
/**
 * Render an auxiliary map for player
 */
void RenderManager::renderShrinkedMap() {
    destRect = { shrinked_map_x,shrinked_map_y, shrinked_map_w,shrinked_map_h };
    SDL_RenderCopy(renderer, textureID["ground"], nullptr, &destRect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    float x = (player->getPosition()[0] / MAP_WIDTH)*shrinked_map_w  + shrinked_map_x;
    float y = (player->getPosition()[1] / MAP_HEIGHT)* shrinked_map_h + shrinked_map_y;
    destRect = {int(x), int(y), player_size_on_shrinked_map_w, player_size_on_shrinked_map_h};
    SDL_RenderFillRect(renderer, &destRect);
}
/**
 * auxiliary function to show the shrink map
 */
void RenderManager::show_aux_map() {
    SHOWTHEMAP = !SHOWTHEMAP;
}
/**
 * auxiliary function to close the shrink map
 */
void RenderManager::close_aux_map() {
    SHOWTHEMAP = false;
}
