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

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sport/SportPauseOpreationView.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <presenter/SportPresenter.h>
#include "core/display_metrics.h"
#include "packages/apps/sports/include/manager/sports_control_manager.h"
#include "packages/services/sport/misc/sport_misc.h"

#define TAG "pause"
CONTAINER_FUNCTION_DEFINE();

#define ID_ACTION_CONTINUE      0
#define ID_ACTION_FINISH        1
#define ID_ACTION_ASSIST        2
#define ID_ACTION_LATELY        3
#define ID_ACTION_MORE          4

static const MenuContainerItemInfo items[] = {
    {ID_ACTION_CONTINUE,    "id_sport_operate_continue"},
    {ID_ACTION_FINISH,      "id_sport_operate_finish"},
    {ID_ACTION_ASSIST,      "id_sport_operate_assist"},
    {ID_ACTION_LATELY,      "id_sport_operate_later_continue"},
    {ID_ACTION_MORE,        "id_sport_operate_more"},
};

static char value[80] = {0};
static char itemKey[80] = {0};

static uint32_t startPauseSecSave = 0;
static SportPauseOpreationView *OpreationContainer = NULL;

static void initMenuContainer(SportPauseOpreationView *sportPauseOpreationView);
static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id);
static void update(SportPauseOpreationView *sportPauseOpreationView);

static void initContainer(SportPauseOpreationView *sportPauseOpreationView, ScreenContext *screenContext)
{
    sportPauseOpreationView->screenContext = screenContext;
    sportPauseOpreationView->app = screenContext->_grAppContext;
    sportPauseOpreationView->initMenuContainer(sportPauseOpreationView);
    sportPauseOpreationView->path = "sportPauseOpreationLayer";
}

static void requstKeepPauseTimer(SportPauseOpreationView *sportPauseOpreationView)
{
    startPauseSecSave = sportPauseOpreationView->startPauseSec;
}

static void setVisiable(SportPauseOpreationView *sportPauseOpreationView, H_Int8 isVisiable)
{
    gre_set_layer_visible(sportPauseOpreationView->app, "SportScreen.sportPauseOpreationLayer", isVisiable);
    sportPauseOpreationView->menuContainer->updateAutoLockState(sportPauseOpreationView->menuContainer);
}

static bool getVisiable(SportPauseOpreationView *sportHalfOpreationView)
{
    int visiable = gre_get_layer_visible(sportHalfOpreationView->app, "SportScreen.sportPauseOpreationLayer");
    return visiable ? true : false;
}

static void update(SportPauseOpreationView *sportPauseOpreationView)
{
    if(!sportPauseOpreationView->startPauseSec)
        return;

    HmTime time;
    hmTime_getTime(&time);
    if(time.sec >= sportPauseOpreationView->startPauseSec)
    {
        hm_snprintf(itemKey, sizeof(itemKey), "%s.%s", "sportPauseOpreationLayer", "titlebarGroup.interval.text");
        uint32_t delta = time.sec - sportPauseOpreationView->startPauseSec;
        hm_snprintf(value, sizeof(value), "%02d:%02d:%02d", (int)delta / 3600, (int)(delta % 3600) / 60, (int)delta % 60);
        setSBStringData(sportPauseOpreationView->app, itemKey, value);
    }
}

static void secTickEvent(SportPauseOpreationView *container)
{
    update((SportPauseOpreationView *)container);
}

static void initMenuContainer(SportPauseOpreationView *sportPauseOpreationView)
{
    sportPauseOpreationView->menuContainer = createMunuContainer(sportPauseOpreationView->app);
    addContainer(sportPauseOpreationView->screenContext, "sportPauseOpreationMenu", sportPauseOpreationView->menuContainer);
    sportPauseOpreationView->menuContainer->initContainer(sportPauseOpreationView->menuContainer, sportPauseOpreationView->screenContext);
    sportPauseOpreationView->menuContainer->initViews(sportPauseOpreationView->menuContainer, "sportPauseOpreationLayer", "sportOpreationStyleLayer.PauseTitleGroup", (MenuContainerItemInfo *)items, sizeof(items) / sizeof(items[0]));
    sportPauseOpreationView->menuContainer->setOnItemClickListener(sportPauseOpreationView->menuContainer, (OnMenuContainerItemClickListener)sportPauseOpreationView->onMenuItemClick);
    sportPauseOpreationView->menuContainer->setOnItemLockListener(sportPauseOpreationView->menuContainer, (OnMenuContainerItemLockListener)sportPauseOpreationView->onMenuLockClick);

    #ifdef HM_DISPLAY_IS_ROUND
    char itemKey[128] = {0};
    hm_snprintf(itemKey, sizeof(itemKey), "%s.%s", "sportPauseOpreationLayer", "titlebarGroup.icon_pause.x");
    setSBIntData(sportPauseOpreationView->app, itemKey, getDimension(139));
    #endif
}

/**
 * @brief
 *
 * @param menuContainer
 * @param id
 */
static void onMenuLockClick(MenuContainer *menuContainer, bool isLock)
{
    if(!OpreationContainer->getVisiable(OpreationContainer))
        return;

    menuContainer->setOperateLock(menuContainer, isLock, true);
}

static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id)
{
    if(!OpreationContainer)
        return;

    switch (id)
    {
        case ID_ACTION_CONTINUE:
            startPauseSecSave = 0;
            getSportsScreenPresenter()->requestContinue();
            break;
        case ID_ACTION_FINISH:
            startPauseSecSave = 0;
            getSportsScreenPresenter()->prepareSave();
            break;
        case ID_ACTION_ASSIST:
            OpreationContainer->requstKeepPauseTimer(OpreationContainer);
            getSportsScreenPresenter()->requestOpenRemindPage();
            getScreenManager()->startScreen("SportSettingAssistantScreen", H_NULL, 0);
            break;
        case ID_ACTION_MORE:
            OpreationContainer->requstKeepPauseTimer(OpreationContainer);
            getSportsScreenPresenter()->requestOpenMorePage();
            getScreenManager()->startScreen("SportSettingMoreScreen", H_NULL, 0);
            break;
        case ID_ACTION_LATELY:
            // dial-state-dot should be registered when sport runs in the background
            getSportsScreenPresenter()->requestDialState("SportScreen", 1);
            OpreationContainer->requstKeepPauseTimer(OpreationContainer);
            getSportsScreenPresenter()->requestGoHome();
            HLOG_I(TAG, "%s : ID_ACTION_LATELY clicked\r\n", __func__);
        default:
            break;
    }
}

static void eventHandle(SportPauseOpreationView *sportPauseOpreationView, SportEventType event)
{
    HmTime time;
    hmTime_getTime(&time);
    switch (event)
    {
        case SPORT_EVENT_SUSPEND:
            sportPauseOpreationView->startPauseSec = time.sec;
            startPauseSecSave = sportPauseOpreationView->startPauseSec;
            update(sportPauseOpreationView);
            break;
        default:
            startPauseSecSave = 0;
            sportPauseOpreationView->startPauseSec = 0;
            break;
    }
}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    SportPauseOpreationView *sportPauseOpreationView = (SportPauseOpreationView *)container;
    ScreenContainer *menuContainer = (ScreenContainer *)sportPauseOpreationView->menuContainer;
    return menuContainer->handleTouchEvent(screenContext, menuContainer, id, eventId, info);
}

static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container,  const HmKeyEvent *info)
{
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const char *event)
{
    return false;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportPauseOpreationView *sportPauseOpreationView = (SportPauseOpreationView *)container;
    sportPauseOpreationView->startPauseSec = 0;
    OpreationContainer = NULL;
}

SportPauseOpreationView *bindSportPauseOpreationContainer(gr_application_t *app)
{
    SportPauseOpreationView *sportPauseOpreationView = H_NULL;
    if (!sportPauseOpreationView)
    {
        createContainer(sportPauseOpreationView, SportPauseOpreationView, sportPauseOpreationView->container);
        sportPauseOpreationView->initContainer = initContainer;
        sportPauseOpreationView->setVisiable = setVisiable;
        sportPauseOpreationView->getVisiable = getVisiable;
        sportPauseOpreationView->initMenuContainer = initMenuContainer;
        sportPauseOpreationView->onMenuItemClick = onMenuItemClick;
        sportPauseOpreationView->secTickEvent = secTickEvent;
        sportPauseOpreationView->requstKeepPauseTimer = requstKeepPauseTimer;
        sportPauseOpreationView->eventHandle = eventHandle;
        sportPauseOpreationView->onMenuLockClick = onMenuLockClick;

        if(startPauseSecSave)
        {
            sportPauseOpreationView->startPauseSec = startPauseSecSave;
            update(sportPauseOpreationView);
        }
    }
    OpreationContainer = sportPauseOpreationView;
    return sportPauseOpreationView;
}