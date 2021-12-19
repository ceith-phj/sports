#ifndef SPORT_REMINDER_H
#define SPORT_REMINDER_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include "packages/apps/sports/include/presenter/sport_reminder_presenter.h"

typedef struct _SportReminderContainer SportReminderContainer;
typedef void (*SportReminderMesgListener)(void);

struct _SportReminderContainer
{
    ScreenContainer container;
    gr_application_t *app;
    ScreenContext *screenContext;
    void *timerHandle;
    SportReminderPresenter *presenter;
    bool isShowing;
    bool isFullScreen;
};
//-----------------------------
SportReminderContainer *createReminderContainer(ScreenContext *screenContext);
//-----------------------------
#endif
