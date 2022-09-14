#ifndef CSCI437_SOUNDSERVER_H
#define CSCI437_SOUNDSERVER_H

#include <SDL_mixer.h>
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>

class SoundServer {
    public:
        SoundServer();
        ~SoundServer();

        // Message Report
        static void csci437_error(const std::string& msg);
    
        void init();
        void cleanUp();
        // music
        void playTitleMusic();
        void playOverworldMusic();
        void playDungeonMusic();
        // effects
        void playClick();
        void playWalk();
        void playShot();
        void playBolt();
        void playHit();
        void playEnemy();
        void playSplash();
        void playNPC();
        // modifiers
        void pauseMusic();
        void resumeMusic();

    private:
        Mix_Music* loadMusic(const std::string& filename);
        Mix_Chunk* loadEffect(const std::string& filename);
        // music
        Mix_Music *overworld;
        Mix_Music *titlescreen;
        Mix_Music *dungeon;
        // sound effects
        Mix_Chunk *walk;
        Mix_Chunk *takeHit;
        Mix_Chunk *click;
        Mix_Chunk *npc;
        Mix_Chunk *shot;
        Mix_Chunk *bolt;
        Mix_Chunk *splash;
        Mix_Chunk *enemy;
        // for proper delay
        int lastWalk = 0;
        int lastHit = 0;
        int lastTalk = 0;
        
};

#endif //CSCI437_SOUNDSERVER_H