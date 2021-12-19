#ifndef SPORTS_STATEBAR_H
#define SPORTS_STATEBAR_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include "sport_type_auto.h"

typedef struct _SportStateBarContainer SportStateBarContainer;

typedef struct {
    int16_t offsetY;
    bool timeIs24Hour;
    bool isLock;
    bool visiableState[3];
    uint8_t focus;
} SportStateBarStatus;

struct _SportStateBarContainer
{
    ScreenContainer container;

    void (*initContainer)(SportStateBarContainer *container, ScreenContext *screenContext);

    void (*setVisiable)(SportStateBarContainer *container, bool isVisiable);
    void (*setLock)(SportStateBarContainer *container, bool isLock);
    void (*setScheduleCurrent)(SportStateBarContainer *container, int current);
    H_Int16 (*isVisiable)(SportStateBarContainer *dataView);
    void (*secTickEvent)(SportStateBarContainer *container);
    void (*restoreContainer)(SportStateBarContainer *container);
    #ifndef HM_DISPLAY_IS_ROUND
    void (*setTitleText)(SportStateBarContainer *container,char *title);
    void (*setScheduleDefaultY)(SportStateBarContainer *container);
    void (*setScheduleDisappearY)(SportStateBarContainer *container);

    #endif
    gr_application_t *app;
    ScreenContext *screenContext;
    SportStateBarStatus status;
    gr_animation_handle_t *anim;
};

SportStateBarContainer *bindSportStateBarView(gr_application_t *app);

#endif