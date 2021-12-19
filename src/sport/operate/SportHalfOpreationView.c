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
#include <sport/SportHalfOpreationView.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <presenter/SportPresenter.h>
#include "core/display_metrics.h"
#include "packages/apps/sports/include/manager/sports_control_manager.h"
#include "packages/services/sport/misc/sport_misc.h"

#define TAG "half"
CONTAINER_FUNCTION_DEFINE();

#define ID_ACTION_CONTINUE      0
#define ID_ACTION_NEXT_HALF     1
#define ID_ACTION_FINISH        2
#define ID_ACTION_ASSIST        3
#define ID_ACTION_LATELY        4
#define ID_ACTION_MORE          5

static const MenuContainerItemInfo items[] = {
    {ID_ACTION_CONTINUE,    "id_sport_operate_continue"},
    {ID_ACTION_NEXT_HALF,   "id_sport_operate_next_half"},
    {ID_ACTION_FINISH,      "id_sport_operate_finish"},
    {ID_ACTION_ASSIST,      "id_sport_operate_assist"},
    {ID_ACTION_LATELY,      "id_sport_operate_later_continue"},
    {ID_ACTION_MORE,        "id_sport_operate_more"},
};

static char value[80] = {0};
static char itemKey[80] = {0};

static uint32_t startPauseSecSave = 0;
static SportHalfOpreationView *OpreationContainer = NULL;
static bool isNextHalfStarted = false;

static void initMenuContainer(SportHalfOpreationView *sportHalfOpreationView);
static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id);
static void update(SportHalfOpreationView *sportHalfOpreationView);

static void initContainer(SportHalfOpreationView *sportHalfOpreationView, ScreenContext *screenContext)
{
    sportHalfOpreationView->screenContext = screenContext;
    sportHalfOpreationView->app = screenContext->_grAppContext;
    sportHalfOpreationView->initMenuContainer(sportHalfOpreationView);
    sportHalfOpreationView->path = "sportHalfOpreationLayer";
}

static void requstKeepPauseTimer(SportHalfOpreationView *sportPauseOpreationView)
{
    startPauseSecSave = sportPauseOpreationView->startPauseSec;
}

static void setVisiable(SportHalfOpreationView *sportHalfOpreationView, H_Int8 isVisiable)
{
    gre_set_layer_visible(sportHalfOpreationView->app, "SportScreen.sportHalfOpreationLayer", isVisiable);
    sportHalfOpreationView->menuContainer->updateAutoLockState(sportHalfOpreationView->menuContainer);
}

static bool getVisiable(SportHalfOpreationView *sportHalfOpreationView)
{
    int visiable = gre_get_layer_visible(sportHalfOpreationView->app, "SportScreen.sportHalfOpreationLayer");
    return visiable ? true : false;
}

static void update(SportHalfOpreationView *sportHalfOpreationView)
{
    if(!sportHalfOpreationView->startPauseSec)
        return;

    HmTime time;
    hmTime_getTime(&time);
    if(time.sec >= sportHalfOpreationView->startPauseSec)
    {
        hm_snprintf(itemKey, sizeof(itemKey), "%s.%s", "sportHalfOpreationLayer", "titlebarGroup.interval.text");
        uint32_t delta = time.sec - sportHalfOpreationView->startPauseSec;
        hm_snprintf(value, sizeof(value), "%02d:%02d:%02d", (int)delta / 3600, (int)(delta % 3600) / 60, (int)delta % 60);
        setSBStringData(sportHalfOpreationView->app, itemKey, value);
    }
}

static void secTickEvent(SportHalfOpreationView *container)
{
    update((SportHalfOpreationView *)container);
}

static void initMenuContainer(SportHalfOpreationView *sportHalfOpreationView)
{
    sportHalfOpreationView->menuContainer = createMunuContainer(sportHalfOpreationView->app);
    addContainer(sportHalfOpreationView->screenContext, "sportHalfOpreationMenu", sportHalfOpreationView->menuContainer);
    sportHalfOpreationView->menuContainer->initContainer(sportHalfOpreationView->menuContainer, sportHalfOpreationView->screenContext);
    sportHalfOpreationView->menuContainer->initViews(sportHalfOpreationView->menuContainer, "sportHalfOpreationLayer", "sportOpreationStyleLayer.PauseTitleGroup", (MenuContainerItemInfo *)items, sizeof(items) / sizeof(items[0]));
    sportHalfOpreationView->menuContainer->setOnItemClickListener(sportHalfOpreationView->menuContainer, (OnMenuContainerItemClickListener)sportHalfOpreationView->onMenuItemClick);
    sportHalfOpreationView->menuContainer->setOnItemLockListener(sportHalfOpreationView->menuContainer, (OnMenuContainerItemLockListener)sportHalfOpreationView->onMenuLockClick);
    sportHalfOpreationView->isNextHalfStarted = isNextHalfStarted;
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
            HLOG_I(TAG, "%s : ID_ACTION_CONTINUE clicked\r\n", __func__);
            break;
        case ID_ACTION_FINISH:
            startPauseSecSave = 0;
            OpreationContainer->isNextHalfStarted = false;
            getSportsScreenPresenter()->prepareSave();
            HLOG_I(TAG, "%s : ID_ACTION_FINISH clicked\r\n", __func__);
            break;
        case ID_ACTION_ASSIST:
            OpreationContainer->requstKeepPauseTimer(OpreationContainer);
            getSportsScreenPresenter()->requestOpenRemindPage();
            getScreenManager()->startScreen("SportSettingReminderScreen", H_NULL, 0);
            HLOG_I(TAG, "%s : ID_ACTION_ASSIST clicked\r\n", __func__);
            break;
        case ID_ACTION_MORE:
            OpreationContainer->requstKeepPauseTimer(OpreationContainer);
            getSportsScreenPresenter()->requestOpenMorePage();
            getScreenManager()->startScreen("SportSettingMoreScreen", H_NULL, 0);
            HLOG_I(TAG, "%s : ID_ACTION_MORE clicked\r\n", __func__);
            break;
        case ID_ACTION_NEXT_HALF:
            startPauseSecSave = 0;
            OpreationContainer->isNextHalfStarted = true;
            getSportsScreenPresenter()->requestSecondHalf();
            getSportsScreenPresenter()->requestContinue();
            HLOG_I(TAG, "%s : ID_ACTION_NEXT_HALF clicked\r\n", __func__);
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

static void eventHandle(SportHalfOpreationView *sportPauseOpreationView, SportEventType event)
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
    SportHalfOpreationView *sportHalfOpreationView = (SportHalfOpreationView *)container;
    ScreenContainer *menuContainer = (ScreenContainer *)sportHalfOpreationView->menuContainer;
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
    SportHalfOpreationView *sportHalfOpreationView = (SportHalfOpreationView *)container;
    sportHalfOpreationView->startPauseSec = 0;
    OpreationContainer = NULL;
    isNextHalfStarted = sportHalfOpreationView->isNextHalfStarted;
}

static bool isSecondHalfStart(SportHalfOpreationView *sportHalfOpreationView)
{
    return sportHalfOpreationView->isNextHalfStarted;
}

SportHalfOpreationView *bindSportHalfOpreationContainer(gr_application_t *app)
{
    SportHalfOpreationView *sportHalfOpreationView = H_NULL;
    if (!sportHalfOpreationView)
    {
        createContainer(sportHalfOpreationView, SportHalfOpreationView, sportHalfOpreationView->container);
        sportHalfOpreationView->initContainer = initContainer;
        sportHalfOpreationView->setVisiable = setVisiable;
        sportHalfOpreationView->getVisiable = getVisiable;

        sportHalfOpreationView->initMenuContainer = initMenuContainer;
        sportHalfOpreationView->onMenuItemClick = onMenuItemClick;
        sportHalfOpreationView->secTickEvent = secTickEvent;
        sportHalfOpreationView->requstKeepPauseTimer = requstKeepPauseTimer;
        sportHalfOpreationView->eventHandle = eventHandle;
        sportHalfOpreationView->isSecondHalfStart = isSecondHalfStart;
        sportHalfOpreationView->onMenuLockClick = onMenuLockClick;

        if(startPauseSecSave)
        {
            sportHalfOpreationView->startPauseSec = startPauseSecSave;
            update(sportHalfOpreationView);
        }
    }
    OpreationContainer = sportHalfOpreationView;
    return sportHalfOpreationView;
}