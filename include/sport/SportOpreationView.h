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

#ifndef SPORTS_OPREATION_VIEW_H
#define SPORTS_OPREATION_VIEW_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <containers/MenuContainer.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportOpreationView SportOpreationView;

    struct _SportOpreationView
    {
        ScreenContainer container;

        void (*initContainer)(SportOpreationView *sportOpreationView, ScreenContext *screenContext);
        void (*setVisiable)(SportOpreationView *sportOpreationView, H_Int8 isVisiable);
        bool (*getVisiable)(SportOpreationView *sportOpreationView);
        void (*initMenuContainer)(SportOpreationView *sportOpreationView);
        void (*onMenuItemClick)(MenuContainer* menuContainer, H_Uint8 id);
        void (*onMenuLockClick)(MenuContainer* menuContainer, bool isLock);

        gr_application_t *app;
        ScreenContext *screenContext;
        char *screenName;
        char rootPath[120];
        MenuContainer* menuContainer;
        char* path;
    };

    SportOpreationView *bindSportOpreationView(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif