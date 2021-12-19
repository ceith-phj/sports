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
#ifndef SPORTS_GROUP_OPREATION_VIEW_H
#define SPORTS_GROUP_OPREATION_VIEW_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <containers/MenuContainer.h>
#include "packages/apps/sports/include/presenter/SportPresenter.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportGroupOpreationContainer SportGroupOpreationView;

    struct _SportGroupOpreationContainer
    {
        ScreenContainer container;

        void (*initContainer)(SportGroupOpreationView *sportGroupOpreationView, ScreenContext *screenContext);
        void (*setVisiable)(SportGroupOpreationView *sportGroupOpreationView, H_Int8 isVisiable);
        bool (*getVisiable)(SportGroupOpreationView *sportGroupOpreationView);
        void (*initMenuContainer)(SportGroupOpreationView *sportGroupOpreationView);
        void (*onMenuItemClick)(MenuContainer* menuContainer, H_Uint8 id);
        void (*eventHandle)(SportGroupOpreationView *sportGroupOpreationView, SportEventType event);
        bool (*isBreakStart)(SportGroupOpreationView *sportGroupOpreationView);
        void (*onMenuLockClick)(MenuContainer* menuContainer, bool isLock);

        gr_application_t *app;
        ScreenContext *screenContext;
        char *screenName;
        char rootPath[120];
        MenuContainer* menuContainer;
        char* path;
        bool isBreakStarted;
    };

    SportGroupOpreationView *bindSportGroupOpreationContainer(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif