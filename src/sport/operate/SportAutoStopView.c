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
#include <sport/SportAutoStopView.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <presenter/SportPresenter.h>
#include "packages/services/sport/misc/sport_misc.h"
#include "ui/Sports/include/sport_type_utils.h"
#include "sport_auto_recog_service.h"

#define TAG "autoStop"
CONTAINER_FUNCTION_DEFINE();

#define ID_ACTION_CONTINUE  0
#define ID_ACTION_SAVE      1

static const MenuContainerItemInfo items[] =
{
    {ID_ACTION_SAVE, "id_sport_operate_finish"},
    {ID_ACTION_CONTINUE, "id_sport_operate_continue"},
};

#define AUTOSTOP_MAX_INTERVAL           (180)
#define AUTOSTOP_MAX_DISPLAY_INTERVAL   (60)

#define AUTOSTOP_SPORT_ICON_SIZE        (160)

static uint32_t autoStopStartSec = 0;
// static bool isAutoStopTrigged = false;

static void initMenuContainer(SportAutoStopView *sportAutoStopView);
static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id);
// static void autoStopViewResume(SportAutoStopView *sportAutoStopView);

static SportAutoStopView *autoStopView = NULL;

/**
 * @brief
 *
 * @param sportAutoStopView
 * @param screenContext
 */
static void initContainer(SportAutoStopView *sportAutoStopView,
                          ScreenContext *screenContext)
{
    sportAutoStopView->screenContext = screenContext;
    sportAutoStopView->app = screenContext->_grAppContext;
    sportAutoStopView->initMenuContainer(sportAutoStopView);
    sportAutoStopView->path = "sportAutoStopLayer";
}

/**
 * @brief Set the Visiable object
 *
 * @param sportAutoStopView
 * @param isVisiable
 */
static void setVisiable(SportAutoStopView *sportAutoStopView, H_Int8 isVisiable)
{
    gre_set_layer_visible(sportAutoStopView->app, "SportScreen.sportAutoStopLayer",
                          isVisiable);
    sportAutoStopView->menuContainer->updateAutoLockState(
        sportAutoStopView->menuContainer);
}

static bool getVisiable(SportAutoStopView *sportHalfOpreationView)
{
    int visiable = gre_get_layer_visible(sportHalfOpreationView->app,
                                         "SportScreen.sportAutoStopLayer");
    return visiable ? true : false;
}

/**
 * @brief
 *
 * @param sportAutoStopView
 */
static void initMenuContainer(SportAutoStopView *sportAutoStopView)
{
    sportAutoStopView->menuContainer = createMunuContainer(sportAutoStopView->app);
    addContainer(sportAutoStopView->screenContext, "sportAutoStopMenu",
                 sportAutoStopView->menuContainer);
    sportAutoStopView->menuContainer->initContainer(
        sportAutoStopView->menuContainer, sportAutoStopView->screenContext);
    sportAutoStopView->menuContainer->initViews(sportAutoStopView->menuContainer,
            "sportAutoStopLayer", "sportOpreationStyleLayer.AutoStopGroup",
            (MenuContainerItemInfo *)items, sizeof(items) / sizeof(items[0]));
    sportAutoStopView->menuContainer->setOnItemClickListener(
        sportAutoStopView->menuContainer,
        (OnMenuContainerItemClickListener)sportAutoStopView->onMenuItemClick);
    sportAutoStopView->menuContainer->setOnItemLockListener(
        sportAutoStopView->menuContainer,
        (OnMenuContainerItemLockListener)sportAutoStopView->onMenuLockClick);
    const char *img = sportType_getTypeIconSizeId(
                          SportContrlService_getOps()->getSportType(), AUTOSTOP_SPORT_ICON_SIZE);
    sportAutoStopView->menuContainer->updateItemImage(
        sportAutoStopView->menuContainer, (char *)img);
    // autoStopViewResume(sportAutoStopView);
}

/**
 * @brief
 *
 * @param sportAutoStopView
 */
// static void autoStopViewResume(SportAutoStopView *sportAutoStopView)
// {
//     sportAutoStopView->isAutoStop = isAutoStopTrigged;
//     HLOG_I(TAG, "auto stop : %s, autoStopStartSec : %d\r\n",
//            isAutoStopTrigged ? "true" : "false", autoStopStartSec);
// }
/**
 * @brief
 *
 * @param sportAutoStopView
 */
static void containerRecover(SportAutoStopView *sportAutoStopView)
{
    SportArStatus sportArStatus = {0};
    SportAutoRecogService_getOps()->getLastStatus(&sportArStatus);

    switch (sportArStatus.status)
    {
        case AUTO_RECOG_PAUSED:
            HLOG_I(TAG, "containerRecover AUTO_RECOG_PAUSED trigged\r\n");
            sportAutoStopView->isAutoStop = true;
            // getSportsScreenPresenter()->requestPause();
            autoStopStartSec = sportArStatus.timestamp;
            break;

        case AUTO_RECOG_RESUMED:
            HLOG_I(TAG, "containerRecover AUTO_RECOG_RESUMED trigged\r\n");
            sportAutoStopView->isAutoStop = false;
            // getSportsScreenPresenter()->requestContinue();
            autoStopStartSec = 0;
            break;

        case AUTO_RECOG_STOPPED:
            HLOG_I(TAG, "containerRecover AUTO_RECOG_STOPPED trigged\r\n");
            sportAutoStopView->isAutoStop = false;
            autoStopStartSec = 0;
            break;

        default:
            break;
    }
}


/**
 * @brief
 *
 * @param menuContainer
 * @param id
 */
static void onMenuLockClick(MenuContainer *menuContainer, bool isLock)
{
    if (!autoStopView->getVisiable(autoStopView))
    {
        return;
    }

    menuContainer->setOperateLock(menuContainer, isLock, true);
}

/**
 * @brief event pruducer0
 *
 * @param menuContainer
 * @param id
 */
static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id)
{
    switch (id)
    {
        case ID_ACTION_CONTINUE:
            autoStopView->isAutoStop = false;
            HLOG_I(TAG, "button ID_ACTION_CONTINUE pressed\r\n");
            getSportsScreenPresenter()->requestContinue();
            break;

        case ID_ACTION_SAVE:
            HLOG_I(TAG, "button ID_ACTION_SAVE pressed\r\n");
            getSportsScreenPresenter()->prepareSave();
            autoStopView->isAutoStop = false;
            break;

        default:
            break;
    }
}

/**
 * @brief event pruducer1
 *
 * @param menuContainer
 * @param id
 */
static void autoRecogEventHandle(SportAutoStopView *sportAutoStopView,
                                 SportAutoRecogStatus event)
{
    switch (event)
    {
        case AUTO_RECOG_PAUSED:
            HLOG_I(TAG, "event AUTO_RECOG_PAUSED trigged\r\n");
            sportAutoStopView->isAutoStop = true;
            getSportsScreenPresenter()->requestPause();
            break;

        case AUTO_RECOG_RESUMED:
            HLOG_I(TAG, "event AUTO_RECOG_RESUMED trigged\r\n");
            sportAutoStopView->isAutoStop = false;
            getSportsScreenPresenter()->requestContinue();
            break;

        default:
            break;
    }
}

/**
 * @brief event consumer
 *
 * @param sportAutoStopView
 * @param event
 */
static void eventHandle(SportAutoStopView *sportAutoStopView,
                        SportEventType event)
{
    switch (event)
    {
        case SPORT_EVENT_SUSPEND:
        {
            SportArStatus sportArStatus = {0};
            SportAutoRecogService_getOps()->getLastStatus(&sportArStatus);
            autoStopStartSec = sportArStatus.timestamp;
            break;
        }

        case SPORT_EVENT_CONTINUE:
            autoStopStartSec = 0;
            break;

        case SPORT_EVENT_SAVE:
            autoStopStartSec = 0;
            break;

        default:
            break;
    }
}

static void secTickEvent(SportAutoStopView *container)
{
    SportAutoStopView *sportAutoStopView = (SportAutoStopView *)container;

    if (!sportAutoStopView->isAutoStop)
    {
        return;
    }

    char value[24];
    HmTime time;
    hmTime_getTime(&time);
    HLOG_I(TAG, "currnet:%d  autoStopStartSec:%d \r\n", time.sec, autoStopStartSec);
    int32_t diff = AUTOSTOP_MAX_INTERVAL - (time.sec - autoStopStartSec);

    if (diff > 0)
    {
        if (diff <= AUTOSTOP_MAX_DISPLAY_INTERVAL)
        {
            hm_snprintf(value, sizeof(value), "(%d)", diff);
            sportAutoStopView->menuContainer->updateItemContent(
                sportAutoStopView->menuContainer, 0, value);
            HLOG_I(TAG, "count down : %d\r\n", diff);
        }
    }
    else if (autoStopStartSec != 0)
    {
        HLOG_I(TAG, "count down over, prepare to save\r\n");
        getSportsScreenPresenter()->prepareSave();
        sportAutoStopView->isAutoStop = false;
    }
}

/**
 * @brief
 *
 * @param sportAutoStopView
 * @return true
 * @return false
 */
// static bool checkIfAutoStopTrigged(SportAutoStopView *sportAutoStopView)
// {
//     return isAutoStopTrigged;
// }

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    SportAutoStopView *sportAutoStopView = (SportAutoStopView *)container;
    ScreenContainer *menuContainer = (ScreenContainer *)
                                     sportAutoStopView->menuContainer;
    return menuContainer->handleTouchEvent(screenContext, menuContainer, id,
                                           eventId, info);
}

static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container,  const HmKeyEvent *info)
{
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext,
                               ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext,
                        ScreenContainer *container, const char *id, const char *event)
{
    return false;
}

/**
 * @brief
 *
 * @param screenContext
 * @param container
 */
static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportAutoStopView *sportAutoStopView = (SportAutoStopView *)container;
    autoStopView = NULL;
    // isAutoStopTrigged = sportAutoStopView->isAutoStop;
    sportAutoStopView->isAutoStop = false;
    // HLOG_I(TAG, "auto stop : %s, autoStopStartSec : %d\r\n",
    //        isAutoStopTrigged ? "true" : "false", autoStopStartSec);
}

/**
 * @brief
 *
 * @param app
 * @return SportAutoStopView*
 */
SportAutoStopView *bindSportAutoStopView(gr_application_t *app)
{
    SportAutoStopView *sportAutoStopView = H_NULL;

    if (!sportAutoStopView)
    {
        createContainer(sportAutoStopView, SportAutoStopView,
                        sportAutoStopView->container);
        sportAutoStopView->app = app;
        sportAutoStopView->initContainer = initContainer;
        sportAutoStopView->setVisiable = setVisiable;
        sportAutoStopView->getVisiable = getVisiable;
        sportAutoStopView->initMenuContainer = initMenuContainer;
        sportAutoStopView->onMenuItemClick = onMenuItemClick;
        sportAutoStopView->autoRecogEventHandle = autoRecogEventHandle;
        sportAutoStopView->eventHandle = eventHandle;
        // sportAutoStopView->checkIfAutoStopTrigged = checkIfAutoStopTrigged;
        sportAutoStopView->secTickEvent = secTickEvent;
        sportAutoStopView->onMenuLockClick = onMenuLockClick;
        sportAutoStopView->containerRecover = containerRecover;
        autoStopView = sportAutoStopView;
    }

    return sportAutoStopView;
}