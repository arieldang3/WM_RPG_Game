#include "WMRPGame.h"

#define TEXT_FONT_FILE "../resource/alagard.ttf"
#define TITLE_IMAGE "../resource/accessory_maps/title_screen2.png"
#define BACKGROUNDINSTRUCTIONS "../resource/Dark_Wren.png"
#define FIRE_IMAGE "../resource/Fire_below.png"


// Track the Game state
GameState WMRPGame::state = START;
// PAUSE the game
bool WMRPGame::PAUSED = false;
bool WMRPGame::INVENTORY_SHOWING = false;
/**
 * Constructor
 */
WMRPGame::WMRPGame() {
    gameWindow = nullptr;
    renderer = nullptr;
    EnteredDungeonState = false;
    titleImage = nullptr;
    backgroundInstructionsImage = nullptr;
    xmlParser = nullptr;
}
/**
 * Destructor
 */
WMRPGame::~WMRPGame() {
}
/**
 * Game Initialization
 */
void WMRPGame::init() {
    // Initialize SDL VIDEO and AUDIO
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) csci437_error("SDL could not initialize!");
    if (SDL_Init(SDL_INIT_AUDIO) < 0) csci437_error("SDL could not initialize!");

    // Create window
    gameWindow = SDL_CreateWindow( "CSCI-437 Team1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gameWindow == nullptr ) csci437_error("Window could not be created!");

    // Create renderer
    renderer = SDL_CreateRenderer( gameWindow, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) csci437_error("Unable to create renderer!");
    // prepare textures
    titleImage = renderImage(renderer, TITLE_IMAGE);
    backgroundInstructionsImage = renderImage(renderer, BACKGROUNDINSTRUCTIONS);

    //Set up pixel art rendering
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(renderer, INTERNAL_WIDTH, INTERNAL_HEIGHT);

    // initialize sounds
    soundServer.init();

    // Small delay to allow the system to create the window.
    SDL_Delay(100);

    state = START;

    /* Initialize game's entity managers */
    TestBulletManager = make_shared<BulletManager> (BulletManager());
    TestItemManager   = make_shared<ItemManager>   (ItemManager());
    npcManager        = make_shared<NPCManager>    (NPCManager());
    enemyManager      = make_shared<EnemyManager>  (EnemyManager());
    warpGateManager   = make_shared<WarpGateManager> (WarpGateManager());
    wallManager       = make_shared<WallManager> (WallManager());

    /* Need Player to exist before Render Manager */
    // Setup a player
    player = make_shared<Player>(Player(GAMESTART_X,GAMESTART_Y,16,32));
    //Setup Camera
    camera = make_shared<Camera>(Camera(*player));
    camera->snapToTarget();

    /* Add managers to RenderManager*/
    renderManager = make_shared<RenderManager>(RenderManager(renderer, camera, player, timer));
    renderManager -> addManagerToCrawl(TestBulletManager);
    renderManager -> addManagerToCrawl(TestItemManager);
    renderManager -> addManagerToCrawl(enemyManager);
    renderManager -> addNpcManagerToCrawl(npcManager);
    //renderManager -> addManagerToCrawl(warpGateManager);
    //renderManager -> addManagerToCrawl(wallManager);

    //setup parser
    xmlParser = new XMLParser(npcManager,TestItemManager,enemyManager, renderManager, warpGateManager, wallManager);

    // Update the view
    updateView();
}
/**
 * the Main Game Loop
 */
void WMRPGame::gameLoop() {
    bool running = true;
    SDL_Event e;
    /* adaptive updating time here */
    uint32_t currentTime;
    uint32_t lastTime;
    uint32_t diff;
    int lastBulletTimestamp = SDL_GetTicks();

    lastTime = SDL_GetTicks();
    // While application is running
    while( running ) {
        // Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            if(!handleEvents(e)){
                running = false;
                break;
            }
        }
        currentTime = SDL_GetTicks();
        diff = currentTime - lastTime;
        lastTime = currentTime;

        /* for PLAYING state */
        if (state == PLAYING) {
            if (!PAUSED){
                updateLogic(int(diff));
            }
            camera->trackToTarget(diff);

            // check the Time
            int timePassed = (int) timer.getTickPassed() / 1000;
            if (!EnteredDungeonState && timePassed + (player->getTimePenalty() / 1000) > TIME_LIMITS || player->getHealth() <= 0){
                timer.stop();
                state = LOSE;
            }
        }

        updateView();
    }
}
/**
 * Clean Up the Game
 */
void WMRPGame::cleanUP() {
    npcManager->cleanUp();
    // Destroy textures
    SDL_DestroyTexture(titleImage);
    SDL_DestroyTexture(backgroundInstructionsImage);
    // Destroy renderer
    SDL_DestroyRenderer( renderer );
    // Destroy window
    SDL_DestroyWindow( gameWindow );
    // quit sound server
    soundServer.cleanUp();
    // Quit SDL subsystems
    SDL_Quit();
    IMG_Quit();
}
/**
 * Report Messages
 * @param msg message
 */
void WMRPGame::csci437_error(const std::string& msg)
{
    std::cerr << msg << " (" << SDL_GetError() << ")" << std::endl;
    exit(0);
}
void WMRPGame::csci437_img_error(const std::string& msg)
{
    std::cerr << msg << " (" << IMG_GetError() << ")" << std::endl;
    exit(0);
}
void WMRPGame::csci437_ttf_error(const std::string &msg) {
    std::cerr << msg << " (" << TTF_GetError() << ")" << std::endl;
    exit(0);
}
/**
 * Update the Game Logic based on current state
 * @param deltaTime time passed (between frames)
 */
void WMRPGame::updateLogic(int deltaTime) {
    switch (state){
        case START:
            break;
        case PLAYING:
            updateMainMapLogic(deltaTime); // written in a separate file "MainMapLogic.cpp"
            break;
        case WIN:
            break;
        case LOSE:
            break;
        default:
            break;
    }
}
/**
 * Re-render the window based on current state
 */
void WMRPGame::updateView() {
    if (PAUSED){
        renderManager->close_aux_map();
        if(INVENTORY_SHOWING) renderInventoryView();
        else renderPAUSEDView();
        return;
    }
    //Don't bother re-rendering static pages
    if(state != PLAYING && state == prevState) return;

    switch (state){
        case START:
            EnteredDungeonState = false;
            renderSTARTView();
            prevState = START;
            break;
        case INSTRUCTIONS:
            renderInstructionsView();
            prevState = INSTRUCTIONS;
            break;
        case PLAYING:
            renderManager->render();
            break;
        case WIN:
            renderWinView();
            prevState = WIN;
            break;
        case LOSE:
            renderLoseView();
            prevState = LOSE;
            break;
        default:
            break;
    }
}
/**
 * Render the starting screen
 */
void WMRPGame::renderSTARTView() {
    SDL_RenderSetLogicalSize(renderer, INTERNAL_WIDTH, INTERNAL_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);
    //renderText(renderer,"Start Screen",50,{0,0,0},{SCREEN_WIDTH/2,SCREEN_HEIGHT/2});
    //Temp title screen - for demo v.01
    SDL_Rect destRect = { 0,0,320,240};
    SDL_RenderCopy(renderer, titleImage, nullptr, &destRect);
    SDL_RenderPresent(renderer);
}
/**
 * Render the instruction screen
 */
void WMRPGame::renderInstructionsView() {
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);

    SDL_Rect destRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundInstructionsImage, nullptr, &destRect);

    SDL_Color textColor = {255,255,255};
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    renderText(renderer,"It is your first day on this beautiful campus of William",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310});
    renderText(renderer,"and Mary, one of the only places of higher education",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 40});
    renderText(renderer,"in this New World. However, all is not well in this place",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 80});
    renderText(renderer,"- a great evil threatens your new home. Your OA said",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 120});
    renderText(renderer,"that a monster lurks in \"the deepest dungeon on",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 160});
    renderText(renderer,"campus,\" and warned you all to stay away. Also, the",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 200});
    renderText(renderer,"President of the College was supposed to give your",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 240});
    renderText(renderer,"convocation speech, but has gone missing. But you are",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 280});
    renderText(renderer,"not the kind of freshman to heed warnings. Collect",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 320});
    renderText(renderer,"items and investigate to defeat the evil before your",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 360});
    renderText(renderer,"first day on campus is over. That's in twenty minutes.",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 400});

    renderText(renderer,"ARROWS to walk                         M for map",45,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2+100 + 50});
    renderText(renderer,"ESC to pause      SPACE to shoot     Q to quit",45,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2+150 + 50});
    renderText(renderer,"ENTER to play!",90,textColor,{SCREEN_WIDTH/2-300,SCREEN_HEIGHT/2+250});

    SDL_RenderPresent(renderer);
}
/**
 * Render the winning screen
 */
void WMRPGame::renderWinView() {
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);

    if(player->checkPlayerHasItem(23)){
        SDL_Rect destRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, renderImage(renderer, FIRE_IMAGE), nullptr, &destRect);
        SDL_Color textColor = {0,0,0};
        renderText(renderer,"You fought valiantly, and have vanquished the evil",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310});
        renderText(renderer,"Squirrel of the Wren Crypt. Unfortunately, you ",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 40});
        renderText(renderer,"also burned down the building with your flaming",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 80});
        renderText(renderer,"projectiles. It turns out the squirrel was actually",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 120});
        renderText(renderer,"the President of the College, who had inadvertently",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 160});
        renderText(renderer,"crossed the Crim Dell Bridge at the same time as a",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 200});
        renderText(renderer,"squirrel, turning him into a squirrel forever until",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 240});
        renderText(renderer,"someone jumped off the bridge and broke the curse.",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 280});
        renderText(renderer,"For the next four years, you are the most popular",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 320});
        renderText(renderer,"person on campus, and your name will forever",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 360});
        renderText(renderer,"go down in campus legend as both the Squirrel-Slayer",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 400});
        renderText(renderer,"and the one who burned the oldest academic building",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 440});
        renderText(renderer,"on the continent to the ground.",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 480});
        renderText(renderer,"YOU WIN?",90,textColor,{SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2-310 + 550});
    }
    else{
        SDL_Rect destRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, renderImage(renderer, "../resource/accessory_maps/Happy_Wren.png"), nullptr, &destRect);
        SDL_Color textColor = {0,0,0};

         renderText(renderer,"You fought valiantly, and have vanquished the evil",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310});
         renderText(renderer,"Squirrel of the Wren Crypt. It turns out the massive",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 40});
         renderText(renderer,"squirrel was actually the President of the College,",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 80});
         renderText(renderer,"who had inadvertently crossed the Crim Dell Bridge",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 120});
         renderText(renderer,"at the same time as a squirrel, turning him into a",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 160});
         renderText(renderer,"squirrel forever until someone jumped off the bridge",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 200});
         renderText(renderer,"and broke the curse.",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 240});
         renderText(renderer,"For the next four years, you are the most popular",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 280});
         renderText(renderer,"person on campus, and your name will forever",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 320});
         renderText(renderer,"go down in campus legend as the Squirrel-Slayer.",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 360});
         renderText(renderer,"   ",35,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-310 + 400});
         renderText(renderer,"YOU WIN!",90,textColor,{SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2+200});
    }
//    renderText(renderer,"Win Screen",80,{0,0,0},{SCREEN_WIDTH/2-130,SCREEN_HEIGHT/2-30});
    SDL_RenderPresent(renderer);
}
/**
 * Render the lose screen
 */
void WMRPGame::renderLoseView() {
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);

    SDL_Rect destRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundInstructionsImage, nullptr, &destRect);
    SDL_Color textColor = {255,255,255};
    if(EnteredDungeonState){
        renderText(renderer,"You fought valiantly, but unfortunately",45,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-200});
        renderText(renderer,"were not able to prevail over the dungeon's",45,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-150});
        renderText(renderer,"evil. You will be remembered, but only as",45,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-100});
        renderText(renderer,"the tragic freshman who could not survive",45,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2-50});
        renderText(renderer,"even their first day on campus.",45,textColor,{SCREEN_WIDTH/2-450,SCREEN_HEIGHT/2});
        renderText(renderer,"GAME OVER",90,textColor,{SCREEN_WIDTH/2-250,SCREEN_HEIGHT/2+100});
    }
    else{
        renderText(renderer,"You were unable to enter the dungeon",45,textColor,{SCREEN_WIDTH/2-400,SCREEN_HEIGHT/2-200});
        renderText(renderer,"before sundown. Your hope for fame ",45,textColor,{SCREEN_WIDTH/2-400,SCREEN_HEIGHT/2-150});
        renderText(renderer,"and glory fades into the ether. Maybe ",45,textColor,{SCREEN_WIDTH/2-400,SCREEN_HEIGHT/2-100});
        renderText(renderer,"some future freshman will succeed ",45,textColor,{SCREEN_WIDTH/2-400,SCREEN_HEIGHT/2-50});
        renderText(renderer,"where you have failed.",45,textColor,{SCREEN_WIDTH/2-400,SCREEN_HEIGHT/2});
        renderText(renderer,"GAME OVER",90,textColor,{SCREEN_WIDTH/2-250,SCREEN_HEIGHT/2+100});
    }
    SDL_RenderPresent(renderer);
}
/**
 * A short cut to draw text
 * @param r the render want to use
 * @param msg the text want to draw
 * @param fontsize the font size
 * @param c a color
 * @return a texture pointer
 */
void WMRPGame::renderText(SDL_Renderer *r, const std::string& msg, int fontsize, SDL_Color c, vector<int> pos) {
    // init TTF
    TTF_Init();
    if( TTF_Init() < 0 ) csci437_ttf_error("Unable to initialize TTF!");
    TTF_Font* font = TTF_OpenFont(TEXT_FONT_FILE, fontsize);
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
 * A short cut to draw an image
 * @param r the render want to use
 * @param fileName the filename
 * @return a texture pointer
 */
SDL_Texture* WMRPGame::renderImage(SDL_Renderer *r,const std::string& fileName){
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
 * Render the Pause screen
 */
void WMRPGame::renderPAUSEDView() {
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 200);
    SDL_Rect rec = {SCREEN_WIDTH/2-150-40,SCREEN_HEIGHT/2-50-50-50-10,400,280};
    SDL_RenderFillRect(renderer, &rec);
    SDL_Color textColor = {255,255,255};
    renderText(renderer,"(U)npause",30,textColor,{SCREEN_WIDTH/2-100-50,SCREEN_HEIGHT/2-50-50-50+20});
    renderText(renderer,"(R)estart Game",30,textColor,{SCREEN_WIDTH/2-100-50,SCREEN_HEIGHT/2-50-50+20});
    renderText(renderer,"(will lose your progress!)",25,textColor,{SCREEN_WIDTH/2-100+40-50,SCREEN_HEIGHT/2-50-10+20});
    renderText(renderer,"(I)nventory",30,textColor,{SCREEN_WIDTH/2-100-50,SCREEN_HEIGHT/2-10+20});
    renderText(renderer,"(Q)uit",30,textColor,{SCREEN_WIDTH/2-100-50,SCREEN_HEIGHT/2-10+ 50 + 20});

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);

}

void WMRPGame::renderInventoryView(){
    //I'm going to draw inventory here
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
    SDL_Color textColor = {255,255,255};

    int boxTop = SCREEN_HEIGHT/2-50-50-150-10;
    int leftMargin = SCREEN_WIDTH/2-150-40;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rec = {leftMargin,boxTop,400,600};
    SDL_RenderFillRect(renderer, &rec);

    SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
    SDL_RenderFillRect(renderer, &rec);
    renderText(renderer,"Inventory", 40,textColor, {leftMargin + 120 - 10,boxTop + 40});
    renderText(renderer,"Main Items", 30,textColor, {leftMargin + 120,boxTop + 40 + 50});
    leftMargin += 25;
    renderText(renderer,"Weapon - " + player->getInventory().getStrongestWeapon().getItemName(), 30,textColor, {leftMargin,boxTop + 50 + 50 + 25});
    renderText(renderer,"Armor - " + player->getInventory().getStrongestArmor().getItemName(), 30,textColor, {leftMargin,boxTop + 90 + 50 + 25});
    renderText(renderer,"Helmet - " + player->getInventory().getStrongestHelmet().getItemName(), 30,textColor, {leftMargin,boxTop + 130 + 50 + 25});

    renderText(renderer,"Other Items", 30,textColor, {leftMargin + 110 - 10, boxTop + 220 + 40});

    int counter = 0;
    int lineNum = 0;
    int columnNum = -1;
    for ( auto it = player->getInventory().begin(); it != player->getInventory().end(); ++it ){
        auto item(std::static_pointer_cast<Item>(it->second));
        if(item->getType() == Other || (item->getPowerLevel() > 0 && item->getItemID() != player->getInventory().getStrongestWeapon().getItemID() && (item->getItemID() != player->getInventory().getStrongestArmor().getItemID()) && (item->getItemID()!= player->getInventory().getStrongestHelmet().getItemID()))){
            renderText(renderer,item->getItemName(), 20,textColor, {leftMargin + 100 + 100*columnNum,boxTop + 220 + 40 + 40 + lineNum*25});
            if(counter%2 == 1) lineNum ++;
            columnNum *= -1;
            counter ++;
        }
    }

    renderText(renderer,"Unpause - U", 25,textColor, {leftMargin + 10,boxTop + 570});

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
}


bool WMRPGame::handleEvents(SDL_Event e){

    // User requests quit
    if( e.type == SDL_QUIT ) return false;

    // User presses a key
    if( e.type == SDL_KEYDOWN )
    {
        //Q to quit, regardless of game state
        if( e.key.keysym.sym == SDLK_q && state == PAUSED) return false;

        //START menu buttons
        if(state == START) {
            if(e.key.keysym.sym == SDLK_RETURN) state = INSTRUCTIONS;
         }
        else if (state == INSTRUCTIONS){
            if (e.key.keysym.sym == SDLK_RETURN){
                xmlParser->generateFrom("../resource/map_data/overworld_13.xml");
                TestBulletManager->removeAll();
                state = PLAYING;
                PAUSED = false;
                renderManager->close_aux_map();
                //Set up pixel art rendering
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
                SDL_RenderSetLogicalSize(renderer, INTERNAL_WIDTH, INTERNAL_HEIGHT);
                timer.start();
                soundServer.playOverworldMusic();
                player->resetPlayer();
                TestItemManager->resetInventory();
                camera->snapToTarget();
                init_npc_status();
            }

        }
            //PLAYING buttons
        else if (state == PLAYING){
            //PAUSE BUTTON MENU BUTTONS
            if ( e.key.keysym.sym == SDLK_ESCAPE ) {
                PAUSED = true;
                INVENTORY_SHOWING = false;
                renderManager->close_aux_map();
                soundServer.pauseMusic();
                soundServer.playClick();
                timer.pause();
            }
            if ( e.key.keysym.sym == SDLK_i){
                PAUSED = true;
                INVENTORY_SHOWING = true;
                renderManager->close_aux_map();
                soundServer.pauseMusic();
                soundServer.playClick();
                timer.pause();
            }
            if(PAUSED == true){
                if (e.key.keysym.sym == SDLK_u){
                    PAUSED = false;
                    soundServer.resumeMusic();
                    soundServer.playClick();
                    timer.unPause();
                }
                if (e.key.keysym.sym == SDLK_r){
                    state = START;
                    PAUSED = false;
                    timer.stop();
                }
                if (e.key.keysym.sym == SDLK_i){
                    INVENTORY_SHOWING = true;
                }
                if( e.key.keysym.sym == SDLK_q) return false;
            }

                //UNPAUSED, regular gameplay buttons
            else{
                //PLAYER MOVEMENT
                if (e.key.keysym.sym == SDLK_m){
                    renderManager->show_aux_map();
                }
                if (e.key.keysym.sym == SDLK_DOWN){
                    soundServer.playWalk();
                    player->setVelocity(0,player->getSpeed());
                    player->setPlayerDirection(South);
                }
                else if (e.key.keysym.sym == SDLK_UP){
                    soundServer.playWalk();
                    player->setVelocity(0,-player->getSpeed());
                    player->setPlayerDirection(North);
                }
                else if (e.key.keysym.sym == SDLK_RIGHT){
                    soundServer.playWalk();
                    player->setVelocity(player->getSpeed(),0);
                    player->setPlayerDirection(East);
                }
                else if (e.key.keysym.sym == SDLK_LEFT){
                    soundServer.playWalk();
                    player->setVelocity(-player->getSpeed(),0);
                    player->setPlayerDirection(West);
                }
                else if(npcManager->collideWithPlayer_dialog_box(*player)){
                    // Dialog button
                    if (e.key.keysym.sym == SDLK_RETURN){
                        npcManager->updateSentence(0, *player,npcs_current_stage);
                    }
                    else if (e.key.keysym.sym == SDLK_1){
                        npcManager->updateSentence(1, *player,npcs_current_stage);
                    }
                    else if (e.key.keysym.sym == SDLK_2){
                        npcManager->updateSentence(2, *player, npcs_current_stage);
                    }
                    else if (e.key.keysym.sym == SDLK_3){
                        npcManager->updateSentence(3, *player, npcs_current_stage);
                    }
                    else if (e.key.keysym.sym == SDLK_4){
                        npcManager->updateSentence(4, *player, npcs_current_stage);
                    }
                }
                // To pick up items
                else if(TestItemManager->get_to_remove()!=-1){
                    if (e.key.keysym.sym == SDLK_RETURN){
                        player->pick_up_item_key_down = true;}
                }

                    //PLAYER FIRE BUTTON
                else if (e.key.keysym.sym == SDLK_SPACE){
                    Bullet bill = player->attack();
                    if(bill.getPosition()[0] > -10) {
                        soundServer.playShot();
                        int holdKey;
                        holdKey = TestBulletManager->attachEntity(
                                new Bullet(bill.getPosition()[0], bill.getPosition()[1], bill.getSize()[0],
                                           bill.getSize()[1], bill.getVelocity()[0], bill.getVelocity()[1],
                                           bill.getDamage(), bill.getAffiliation()));
                        TestBulletManager->getEntityAt(holdKey).setTextureID("bullet_ball");
                    }
                }

                //SHORTCUT TO WIN, LETS NOT KEEP THIS FOREVER
//                if (e.key.keysym.sym == SDLK_t){
//                    state = WIN;
//                    timer.stop();
//                }
            }
        }

            //BUTTONS FOR GAME OVER STATES
        else if(state == WIN || state == LOSE){
            if (e.key.keysym.sym == SDLK_RETURN) state = START;
        }
    }
    //Stop player if they let go of the key
    if(e.type == SDL_KEYUP && state == PLAYING){
        if( e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_LEFT) player->setVelocity(0,0);
    }

    //If didn't result in ending the game, return true
    return true;
}

void WMRPGame::init_npc_status() {
    npcs_current_stage.clear();
    for (std::string str:NPC_ARRAY){
        npcs_current_stage.insert(pair<string,int>(str,0));
    }
}







