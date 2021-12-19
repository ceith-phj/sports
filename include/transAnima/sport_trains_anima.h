#ifndef SPORT_TRAINS_ANIMA_H
#define SPORT_TRAINS_ANIMA_H
//===========================
#include <widget/base/sb_container.h>
//===========================
typedef struct _SportTransAnima SportTransAnima;
//---------------------------
struct _SportTransAnima
{
    ScreenContext *screenContext;
    void *timerHandle;
    bool showTransAnima;
    void (*start)(SportTransAnima *anima);
    void (*pause)(SportTransAnima *anima, bool withAnima);
    void (*destory)(SportTransAnima *anima);
    void (*finish)(SportTransAnima *anima);
    void (*animaTimerOutCallBack)(void);
    bool (*isPause)(SportTransAnima *anima);
};
//---------------------------
SportTransAnima *createSportTransAnima(ScreenContext *screenContext);
//===========================
//===========================
#endif
