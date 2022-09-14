#include "Timer.h"


Timer::Timer() : ticks_have_run(0), ticks_not_running(0), started(false), paused(false) {}

void Timer::start() {
    started = true;
    paused = false;
    ticks_not_running = SDL_GetTicks(); // A timer was not running before it is started
}

void Timer::stop() {
    started = false;
    paused = false;
    ticks_not_running = 0;
    ticks_have_run = 0;
}

void Timer::pause() {
    if (started && !paused){
        paused = true;
        ticks_have_run = SDL_GetTicks() - ticks_not_running;
    }
}

void Timer::unPause() {
    if (started && paused){
        paused = false;
        ticks_not_running = SDL_GetTicks() - ticks_have_run;
    }
}

void Timer::reStart() {
    started = true;
    paused = false;
    ticks_not_running = SDL_GetTicks();
    ticks_have_run = 0;
}

int Timer::getTickPassed() {
    if (started){
        if (paused)
            return int(ticks_have_run);
        else
            return int(SDL_GetTicks() - ticks_not_running);
    }
    return 0;
}

void Timer::addSeconds(int milliseconds) {
    ticks_not_running = ticks_not_running - (uint32_t) milliseconds;
}
