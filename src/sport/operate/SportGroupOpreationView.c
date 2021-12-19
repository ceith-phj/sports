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
#include <sport/SportGroupOpreationView.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <presenter/SportPresenter.h>
#include "core/display_metrics.h"
#include "packages/apps/sports/include/manager/sports_control_manager.h"
#include "packages/services/sport/misc/sport_misc.h"

#define TAG "groupOperate"
CONTAINER_FUNCTION_DEFINE();

#define ID_ACTION_PAUSE         0
#define ID_ACTION_FINISH        1
#define ID_ACTION_TAKE_BREAK    2

static const MenuContainerItemInfo items[] = {
    {ID_ACTION_PAUSE,       "id_sport_operate_pause"},
    {ID_ACTION_FINISH,      "id_sport_operate_finish"},
    {ID_ACTION_TAKE_BREAK,  "id_sport_operate_take_break"},
};

static SportGroupOpreationView *OpreationContainer = NULL;
static bool isBreakStarted = false;

static void initMenuContainer(SportGroupOpreationView *sportGroupOpreationView);
static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id);

static void initContainer(SportGroupOpreationView *sportGroupOpreationView, ScreenContext *screenContext)
{
    sportGroupOpreationView->screenContext = screenContext;
    sportGroupOpreationView->app = screenContext->_grAppContext;
    sportGroupOpreationView->initMenuContainer(sportGroupOpreationView);
    sportGroupOpreationView->path = "sportGroupOpreationLayer";
}

static void setVisiable(SportGroupOpreationView *sportGroupOpreationView, H_Int8 isVisiable)
{
    gre_set_layer_visible(sportGroupOpreationView->app, "SportScreen.sportGroupOpreationLayer", isVisiable);
    sportGroupOpreationView->menuContainer->updateAutoLockState(sportGroupOpreationView->menuContainer);
}

static bool getVisiable(SportGroupOpreationView *sportHalfOpreationView)
{
    int visiable = gre_get_layer_visible(sportHalfOpreationView->app, "SportScreen.sportGroupOpreationLayer");
    return visiable ? true : false;
}

static void initMenuContainer(SportGroupOpreationView *sportGroupOpreationView)
{
    sportGroupOpreationView->menuContainer = createMunuContainer(sportGroupOpreationView->app);
    addContainer(sportGroupOpreationView->screenContext, "sportGroupOpreationMenu", sportGroupOpreationView->menuContainer);
    sportGroupOpreationView->menuContainer->initContainer(sportGroupOpreationView->menuContainer, sportGroupOpreationView->screenContext);
    sportGroupOpreationView->menuContainer->initViews(sportGroupOpreationView->menuContainer,
                                                        "sportGroupOpreationLayer",
                                                        "sportOpreationStyleLayer.TitleLaterGroup",
                                                        (MenuContainerItemInfo *)(MenuContainerItemInfo *)items, sizeof(items) / sizeof(items[0]));
    sportGroupOpreationView->menuContainer->setOnItemClickListener(sportGroupOpreationView->menuContainer, (OnMenuContainerItemClickListener)sportGroupOpreationView->onMenuItemClick);
    sportGroupOpreationView->menuContainer->setOnItemLockListener(sportGroupOpreationView->menuContainer, (OnMenuContainerItemLockListener)sportGroupOpreationView->onMenuLockClick);
    sportGroupOpreationView->isBreakStarted = isBreakStarted;
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
        case ID_ACTION_PAUSE:
            getSportsScreenPresenter()->requestPause();
            HLOG_I(TAG, "%s : ID_ACTION_PAUSE clicked\r\n", __func__);
            break;
        case ID_ACTION_FINISH:
            OpreationContainer->isBreakStarted = false;
            getSportsScreenPresenter()->prepareSave();
            HLOG_I(TAG, "%s : ID_ACTION_FINISH clicked\r\n", __func__);
            break;
        case ID_ACTION_TAKE_BREAK:
            OpreationContainer->isBreakStarted = true;
            getSportsScreenPresenter()->requestLap();
            HLOG_I(TAG, "%s : ID_ACTION_TAKE_BREAK clicked\r\n", __func__);
            break;
        default:
            break;
    }
}

static void eventHandle(SportGroupOpreationView *sportPauseOpreationView, SportEventType event)
{

}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    SportGroupOpreationView *sportGroupOpreationView = (SportGroupOpreationView *)container;
    ScreenContainer *menuContainer = (ScreenContainer *)sportGroupOpreationView->menuContainer;
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
    SportGroupOpreationView *sportGroupOpreationView = (SportGroupOpreationView *)container;
    OpreationContainer = NULL;
    isBreakStarted = sportGroupOpreationView->isBreakStarted;
}

static bool isBreakStart(SportGroupOpreationView *sportGroupOpreationView)
{
    return sportGroupOpreationView->isBreakStarted;
}

SportGroupOpreationView *bindSportGroupOpreationContainer(gr_application_t *app)
{
    SportGroupOpreationView *sportGroupOpreationView = H_NULL;
    if (!sportGroupOpreationView)
    {
        createContainer(sportGroupOpreationView, SportGroupOpreationView, sportGroupOpreationView->container);
        sportGroupOpreationView->initContainer = initContainer;
        sportGroupOpreationView->setVisiable = setVisiable;
        sportGroupOpreationView->getVisiable = getVisiable;
        sportGroupOpreationView->initMenuContainer = initMenuContainer;
        sportGroupOpreationView->onMenuItemClick = onMenuItemClick;
        sportGroupOpreationView->eventHandle = eventHandle;
        sportGroupOpreationView->isBreakStart = isBreakStart;
        sportGroupOpreationView->onMenuLockClick = onMenuLockClick;
    }
    OpreationContainer = sportGroupOpreationView;
    return sportGroupOpreationView;
}