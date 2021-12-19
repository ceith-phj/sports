#ifndef SPORTS_LOCKSCREEN_H
#define SPORTS_LOCKSCREEN_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include "sport_type_auto.h"

typedef struct _SportLockScreenContainer SportLockScreenContainer;

typedef struct {
    bool isLock;
    bool isShow;
} SportLockScreenStatus;

struct _SportLockScreenContainer
{
    ScreenContainer container;

    void (*initContainer)(SportLockScreenContainer *container, ScreenContext *screenContext);

    void (*setVisiable)(SportLockScreenContainer *container, bool isVisiable);
    void (*setLock)(SportLockScreenContainer *container, bool isLock);
    void (*setScheduleCurrent)(SportLockScreenContainer *container, int current);
    H_Int16 (*isVisiable)(SportLockScreenContainer *dataView);
    void (*secTickEvent)(SportLockScreenContainer *container);
    void (*restoreContainer)(SportLockScreenContainer *container);
    bool (*getVisiable)(SportLockScreenContainer *container);
    gr_application_t *app;
    ScreenContext *screenContext;
    SportLockScreenStatus status;
    //gr_animation_handle_t *anim;
    void *timerHandle;
};

SportLockScreenContainer *bindSportLockScreenView(gr_application_t *app);

#endif