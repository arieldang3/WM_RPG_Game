#ifndef CSCI437_TIMER_H
#define CSCI437_TIMER_H

#include "Constants.h"

class Timer{
public:
    Timer();

    void start();
    void stop();
    void pause();
    void unPause();
    void reStart();
    void addSeconds(int milliseconds);

    int getTickPassed();


private:
    bool started, paused;
    uint32_t ticks_have_run, ticks_not_running;
};

#endif //CSCI437_TIMER_H
