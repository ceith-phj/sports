#ifndef SPORT_GOAL_PAGE_H
#define SPORT_GOAL_PAGE_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <presenter/sport_data_presenter.h>
#include <ui/SportRecord/include/container/SportGoalContainer.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _SportGoalPage SportGoalPage;

    struct _SportGoalPage
    {
        ScreenContainer container;

        void (*update)(SportGoalPage *sportGoalPage);

        SportGoalContainer *sportGoalContainer;

        gr_application_t *app;
        ScreenContext *screenContext;
        char *layerId;
    };

    SportGoalPage *newSportGoalView(ScreenContext *screenContext, char *layerId);

#ifdef __cplusplus
}
#endif

#endif