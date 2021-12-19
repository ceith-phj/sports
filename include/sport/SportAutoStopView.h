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

#ifndef SPORTS_AUTOSTOP_VIEW_H
#define SPORTS_AUTOSTOP_VIEW_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <containers/MenuContainer.h>
#include "packages/services/sport/include/sport_service_message.h"
#include "packages/apps/sports/include/presenter/SportPresenter.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _SportAutoStopView SportAutoStopView;

struct _SportAutoStopView
{
    ScreenContainer container;

    void (*initContainer)(SportAutoStopView *sportAutoStopView,
                          ScreenContext *screenContext);
    void (*setVisiable)(SportAutoStopView *sportAutoStopView, H_Int8 isVisiable);
    bool (*getVisiable)(SportAutoStopView *sportAutoStopView);
    void (*initMenuContainer)(SportAutoStopView *sportAutoStopView);
    void (*onMenuItemClick)(MenuContainer *menuContainer, H_Uint8 id);
    void (*eventHandle)(SportAutoStopView *sportAutoStopView, SportEventType event);
    void (*autoRecogEventHandle)(SportAutoStopView *sportAutoStopView,
                                 SportAutoRecogStatus event);
    bool (*checkIfAutoStopTrigged)(SportAutoStopView *sportAutoStopView);
    void (*secTickEvent)(SportAutoStopView *container);
    void (*onMenuLockClick)(MenuContainer *menuContainer, bool isLock);
    void (*containerRecover)(SportAutoStopView *container);

    gr_application_t *app;
    ScreenContext *screenContext;
    char *screenName;
    char rootPath[120];
    MenuContainer *menuContainer;
    char *path;
    // void *timerHandle;
    bool isAutoStop;
};

SportAutoStopView *bindSportAutoStopView(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif