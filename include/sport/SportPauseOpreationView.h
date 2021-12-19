/*
* =====================================================================================
*
*  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
*  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
*  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
*  All Rights Reserved.
*
*  Author: liubingyan@zepp.com
*
* =====================================================================================
*/
#ifndef SPORTS_PAUSE_OPREATION_VIEW_H
#define SPORTS_PAUSE_OPREATION_VIEW_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <containers/MenuContainer.h>
#include "packages/apps/sports/include/presenter/SportPresenter.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportPauseOpreationContainer SportPauseOpreationView;

    struct _SportPauseOpreationContainer
    {
        ScreenContainer container;

        void (*initContainer)(SportPauseOpreationView *sportPauseOpreationView, ScreenContext *screenContext);
        void (*setVisiable)(SportPauseOpreationView *sportPauseOpreationView, H_Int8 isVisiable);
        bool (*getVisiable)(SportPauseOpreationView *sportPauseOpreationView);
        void (*initMenuContainer)(SportPauseOpreationView *sportPauseOpreationView);
        void (*onMenuItemClick)(MenuContainer* menuContainer, H_Uint8 id);
        void (*secTickEvent)(SportPauseOpreationView *container);
        void (*requstKeepPauseTimer)(SportPauseOpreationView *sportPauseOpreationView);
        void (*eventHandle)(SportPauseOpreationView *sportPauseOpreationView, SportEventType event);
        void (*onMenuLockClick)(MenuContainer* menuContainer, bool isLock);

        gr_application_t *app;
        ScreenContext *screenContext;
        char *screenName;
        char rootPath[120];
        MenuContainer* menuContainer;
        char* path;
        uint32_t startPauseSec;
        bool lastVisiable;
    };

    SportPauseOpreationView *bindSportPauseOpreationContainer(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif