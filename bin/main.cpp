#include "WMRPGame.h"

// Run the Game
int main(int argc, char** argv) {
    WMRPGame wmrpGame;
    wmrpGame.init();
    wmrpGame.gameLoop();
    wmrpGame.cleanUP();
    return 0;
}