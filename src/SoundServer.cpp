#include "SoundServer.h"


#define CLICK_BYTE "../resource/sounds/clickEffect.wav"
#define STEP_BYTE "../resource/sounds/footstep.wav"
#define SHOT_BYTE "../resource/sounds/gunshot.wav"
#define BOLT_BYTE "../resource/sounds/magic_bolt.wav"
#define OVERWORLD_THEME "../resource/sounds/overworldThemeNew.mp3"
#define DUNGEON_THEME "../resource/sounds/dungeon.wav"
#define SPLASH_BYTE "../resource/sounds/splash.wav"
#define HIT_BYTE "../resource/sounds/takeHit.wav"
#define ENEMY_BYTE "../resource/sounds/squirrelHit.wav"
#define NPC_BYTE "../resource/sounds/NPC.wav"

SoundServer::SoundServer() {
    // music
    overworld = nullptr;
    dungeon = nullptr;
    titlescreen = nullptr;
    // effects
    takeHit = nullptr;
    npc = nullptr;
    walk = nullptr;
    click = nullptr;
    shot = nullptr;
    splash = nullptr;
    enemy = nullptr;
    bolt = nullptr;
}

SoundServer::~SoundServer() {}

// Sound Initialization
void SoundServer::init() {
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) csci437_error("Mixer could not be created!");
    //set volume
    Mix_Volume(-1, 25);
    Mix_VolumeMusic(20);
    //load music
    overworld = loadMusic( OVERWORLD_THEME );
    dungeon = loadMusic( DUNGEON_THEME );
    //load sound effects
    click = loadEffect( CLICK_BYTE );
    walk = loadEffect( STEP_BYTE );
    shot = loadEffect( SHOT_BYTE );
    bolt = loadEffect( BOLT_BYTE );
    splash = loadEffect( SPLASH_BYTE );
    takeHit = loadEffect( HIT_BYTE );
    enemy = loadEffect( ENEMY_BYTE );
    npc = loadEffect( NPC_BYTE );
}

// modifiers
void SoundServer::pauseMusic() {
    Mix_PauseMusic();
}

void SoundServer::resumeMusic() {
    Mix_ResumeMusic();
}

// music methods
void SoundServer::playOverworldMusic() {
    Mix_PlayMusic( overworld, -1 );
}

void SoundServer::playDungeonMusic() {
    Mix_PlayMusic( dungeon, -1);
}

// sound methods
void SoundServer::playClick() {
    Mix_PlayChannel( -1, click, 0);
}

void SoundServer::playShot() {
    Mix_PlayChannel( -1, shot, 0);
}

void SoundServer::playBolt() {
    Mix_PlayChannel( -1, bolt, 0);
}

void SoundServer::playSplash() {
    Mix_PlayChannel( -1, splash, 0);
}

void SoundServer::playEnemy() {
    Mix_PlayChannel( -1, enemy, 0);
}

void SoundServer::playNPC() {
    // use delay so sounds don't overlap
    if(this->lastTalk + 2000 < SDL_GetTicks()) {
        this->lastTalk = SDL_GetTicks();
        Mix_PlayChannel( -1, npc, 0);
    }
}

void SoundServer::playHit() {
    // use delay so sounds don't overlap
    if(this->lastHit + 200 < SDL_GetTicks()) {
        this->lastHit = SDL_GetTicks();
        Mix_PlayChannel( -1, takeHit, 0);
    }
}

void SoundServer::playWalk() {
    // use delay so sounds don't overlap
    if(this->lastWalk + 900 < SDL_GetTicks()) {
        this->lastWalk = SDL_GetTicks();
        Mix_PlayChannel( -1, walk, 0);
    }
}

// load sound effect
Mix_Chunk* SoundServer::loadEffect(const std::string& filename) {
    Mix_Chunk *sound = Mix_LoadWAV( filename.c_str() );
    if( sound == NULL ) csci437_error("Sound could not be created!");
    return sound;
}

//load music
Mix_Music* SoundServer::loadMusic(const std::string& filename) {
    Mix_Music *music = Mix_LoadMUS( filename.c_str() );
    if( music == NULL ) csci437_error("Music could not be loaded!");
    return music;
}

// error message
void SoundServer::csci437_error(const std::string& msg) {
    std::cerr << msg << " (" << SDL_GetError() << ")" << std::endl;
    exit(0);
}

// close Mix
void SoundServer::cleanUp() {
    //Free the sound effects
    Mix_FreeChunk( walk );
    Mix_FreeChunk( shot );
    Mix_FreeChunk( takeHit );
    Mix_FreeChunk( click );
    Mix_FreeChunk( splash );
    Mix_FreeChunk( enemy );
    Mix_FreeChunk( npc );
    Mix_FreeChunk( bolt );

    npc = NULL;
    takeHit = NULL;
    click = NULL;
    shot = NULL;
    walk = NULL;
    splash = NULL;
    enemy = NULL;
    bolt = NULL;
    
    //Free the music
    Mix_FreeMusic( overworld );
    Mix_FreeMusic( dungeon );
    //Mix_FreeMusic( titlescreen );

    //titlescreen = nullptr;
    overworld = NULL;
    dungeon = NULL;

    //Quit Mix
    Mix_Quit();
}