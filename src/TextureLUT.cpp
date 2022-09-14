//
// Created by Omar Molina-Aguirre on 4/12/22.
//

#include "RenderManager.h"

/**
 * Creates SDL2 pointers of all textures. They're small enough so might as well
 * get them out of the way. It's not the 1980's
 */
void RenderManager::loadTextures() {
    //Testing
    textureID["npc_test"]       = renderImage(renderer, "../resource/mario.png");
    textureID["title_test"]     = renderImage(renderer, "../resource/accessory_maps/title_screen.png");
    textureID["enemy_test"]     = renderImage(renderer, "../resource/darth.png");

    //Items
    textureID["bullet_ball"]    = renderImage(renderer, "../resource/items/bullet.png");
    textureID["item_notes"]     = renderImage(renderer, "../resource/items/notes.png");
    textureID["item_slingshot"] = renderImage(renderer, "../resource/items/slingshot.png");
    textureID["item_rocks"]     = renderImage(renderer, "../resource/items/rocks.png");
    textureID["key"]            = renderImage(renderer, "../resource/items/bronze_key.png");
    textureID["bolt"]           = renderImage(renderer, "../resource/items/bolt.png");
    textureID["textbook"]       = renderImage(renderer, "../resource/items/textbook.png");

    //Characters
    textureID["player_base"]     = renderImage(renderer, "../resource/characters/player.png");
    textureID["squirrel"]        = renderImage(renderer, "../resource/evil_squirrel.png");
    textureID["npc_student_0"]   = renderImage(renderer, "../resource/characters/student_0.png");
    textureID["npc_student_1"]   = renderImage(renderer, "../resource/characters/student_1.png");
    textureID["npc_student_2"]   = renderImage(renderer, "../resource/characters/student_2.png");
    textureID["npc_student_3"]   = renderImage(renderer, "../resource/characters/student_3.png");
    textureID["npc_stressed"]    = renderImage(renderer, "../resource/characters/stressed.png");
    textureID["npc_jitters"]     = renderImage(renderer, "../resource/characters/jitters.png");
    textureID["npc_strong_man"]  = renderImage(renderer, "../resource/characters/strongman.png");
    textureID["npc_washington"]  = renderImage(renderer, "../resource/characters/washington.png");
    textureID["npc_professor_0"] = renderImage(renderer, "../resource/characters/professor_0.png");
    textureID["npc_professor_1"] = renderImage(renderer, "../resource/characters/professor_1.png");
    textureID["npc_professor_2"] = renderImage(renderer, "../resource/characters/professor_2.png");
    textureID["npc_professor_3"] = renderImage(renderer, "../resource/characters/professor_3.png");

    //Enemies
    textureID["enemy_chaser"] = renderImage(renderer, "../resource/characters/chaser.png");
    textureID["enemy_shooter"] = renderImage(renderer, "../resource/characters/shooter.png");
    textureID["enemy_circuit"] = renderImage(renderer, "../resource/characters/circuit.png");

    //Misc
    textureID["unset"]    = renderImage(renderer, "../resource/debug.png");
    textureID["invisible"]    = renderImage(renderer, "../resource/maps/empty_canopy.png");
    textureID["ground"]   = renderImage(renderer, groundPath);
    textureID["canopy"]   = renderImage(renderer, canopyPath);
    textureID["vignette"] = renderImage(renderer, "../resource/accessory_maps/vignette.png");
}