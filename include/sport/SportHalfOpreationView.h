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
#ifndef SPORTS_HALF_OPREATION_VIEW_H
#define SPORTS_HALF_OPREATION_VIEW_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <containers/MenuContainer.h>
#include "packages/apps/sports/include/presenter/SportPresenter.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportHalfOpreationContainer SportHalfOpreationView;

    struct _SportHalfOpreationContainer
    {
        ScreenContainer container;

        void (*initContainer)(SportHalfOpreationView *sportHalfOpreationView, ScreenContext *screenContext);
        void (*setVisiable)(SportHalfOpreationView *sportHalfOpreationView, H_Int8 isVisiable);
        bool (*getVisiable)(SportHalfOpreationView *sportHalfOpreationView);
        void (*initMenuContainer)(SportHalfOpreationView *sportHalfOpreationView);
        void (*onMenuItemClick)(MenuContainer* menuContainer, H_Uint8 id);
        void (*secTickEvent)(SportHalfOpreationView *container);
        void (*requstKeepPauseTimer)(SportHalfOpreationView *sportHalfOpreationView);
        void (*eventHandle)(SportHalfOpreationView *sportHalfOpreationView, SportEventType event);
        bool (*isSecondHalfStart)(SportHalfOpreationView *sportHalfOpreationView);
        void (*onMenuLockClick)(MenuContainer* menuContainer, bool isLock);

        gr_application_t *app;
        ScreenContext *screenContext;
        char *screenName;
        char rootPath[120];
        MenuContainer* menuContainer;
        char* path;
        uint32_t startPauseSec;
        bool lastVisiable;
        bool isNextHalfStarted;
    };

    SportHalfOpreationView *bindSportHalfOpreationContainer(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif