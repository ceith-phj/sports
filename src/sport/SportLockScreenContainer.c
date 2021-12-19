#include <BaseSportView.h>
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sport/SportLockScreenContainer.h>
#include "hm_time.h"
#include "time/datetime_format.h"
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include "core/display_metrics.h"
#include "sb_screen_util.h"
#include "battery_manager.h"
#include "widget/hSchedule/hScedule.h"
#include "presenter/SportPresenter.h"
#include <sb_screen_context.h>

#define TAG "SportLockScreenContainer"
#define LOCK_LAYER_ID "SportScreen.SportLockLayer"
#define TIPS_TEXT_PATH "SportLockLayer.tips.text"
#define LOCK_ICON_PATH "SportLockLayer.lock.icon"

#define LOCK_IMAGE_PNG_PATH "images/sports/lock/icon_lock.png"
#define UNLOCK_IMAGE_PNG_PATH "images/sports/lock/icon_unlock.png"
#define TIMEOUT_SEC 800

CONTAINER_FUNCTION_DEFINE();

static H_Int32 _timerCallback(struct gr_timer *timer, void *data, int dropped)
{

    SportLockScreenContainer *container = (SportLockScreenContainer *)data;
    if (container == NULL || container->timerHandle == NULL)
    {
        return 0;
    }
    if(container->status.isShow)
    {
        container->status.isShow = false;
        gre_set_layer_attr(container->app, LOCK_LAYER_ID, "alpha", 0);
    }
    if (container->status.isLock == false)
    {
        container->setVisiable(container, false);
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_UNLOCK_SCREEN);
    }
    return 0;
}

static void _setTimer(SportLockScreenContainer *container)
{
    gre_timer_pause(container->app,container->timerHandle);
    gre_timer_set_repeat(container->app, container->timerHandle, TIMEOUT_SEC, 0);
}

static void _creatTimer(SportLockScreenContainer *container)
{
    if (container->timerHandle)
        return;
    container->timerHandle = gre_timer_create_timeout(container->app, TIMEOUT_SEC, 0, container, _timerCallback);
    if (container->timerHandle == H_NULL)
    {
        HLOG_W(TAG, "_creatTimer failed");
        return;
    }
}

static void _showLock(SportLockScreenContainer *container, bool isLock)
{

    if (isLock)
    {
        setSBStringData(container->app, LOCK_ICON_PATH, LOCK_IMAGE_PNG_PATH);
        setSBStringData(container->app, TIPS_TEXT_PATH, System_getLocaleText("id_sport_lock_tips"));
        gre_set_layer_attr(container->app, LOCK_LAYER_ID, "alpha", 255);
        container->status.isLock = true;
        container->status.isShow = true;
        container->setVisiable(container, true);
        _setTimer(container);
    }
    else
    {
        setSBStringData(container->app, LOCK_ICON_PATH, UNLOCK_IMAGE_PNG_PATH);
        setSBStringData(container->app, TIPS_TEXT_PATH, System_getLocaleText("id_sport_unlock_tips"));
        gre_set_layer_attr(container->app, LOCK_LAYER_ID, "alpha", 255);
        container->status.isLock = false;
        container->status.isShow = true;
        _setTimer(container);
    }
}

static void _setLock(SportLockScreenContainer *container, bool isLock)
{

    if (isLock)
    {
        setSBStringData(container->app, LOCK_ICON_PATH, LOCK_IMAGE_PNG_PATH);
        setSBStringData(container->app, TIPS_TEXT_PATH, System_getLocaleText("id_sport_lock_tips"));
        gre_set_layer_attr(container->app, LOCK_LAYER_ID, "alpha", 0);
        container->setVisiable(container, true);
        container->status.isLock = true;
    }
    else
    {
        setSBStringData(container->app, LOCK_ICON_PATH, UNLOCK_IMAGE_PNG_PATH);
        setSBStringData(container->app, TIPS_TEXT_PATH, System_getLocaleText("id_sport_unlock_tips"));
        gre_set_layer_attr(container->app, LOCK_LAYER_ID, "alpha", 0);
        container->setVisiable(container, false);
        container->status.isLock = false;
    }
}

static void _initView(SportLockScreenContainer *container)
{
    container->setLock(container, false);
    container->setVisiable(container, false);
}

static void _initData(SportLockScreenContainer *container)
{
    container->status.isLock = false;
}

static void _setVisiable(SportLockScreenContainer *container,bool isVisiable)
{
    gre_set_layer_visible(container->app, LOCK_LAYER_ID, isVisiable);
}

static bool _getVisiable(SportLockScreenContainer *container)
{
    return gre_get_layer_visible(container->app, LOCK_LAYER_ID);
}

static void _initContainer(SportLockScreenContainer *container, ScreenContext *screenContext)
{
    container->screenContext = screenContext;
    container->app = screenContext->_grAppContext;
    _initData(container);
    _initView(container);
    _creatTimer(container);
}

static void _restoreContainer(SportLockScreenContainer *container)
{
    if (container->status.isLock == true)
    {
        container->setLock(container, true);
    }
}

SportLockScreenContainer *bindSportLockScreenView(gr_application_t *app)
{
    SportLockScreenContainer *container = H_NULL;
    createContainer(container, SportLockScreenContainer, container->container);
    if (container)
    {
        container->initContainer = _initContainer;
        container->setVisiable = _setVisiable;
        container->setLock = _setLock;
        container->restoreContainer = _restoreContainer;
        container->getVisiable = _getVisiable;
    }
    return container;
}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    SportLockScreenContainer *lockContainer = (SportLockScreenContainer *)container;
    if (lockContainer->status.isLock == true)
    {
        _showLock(lockContainer, true);
    }

    return true;
}

static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container, const HmKeyEvent *info)
{
    SportLockScreenContainer *lockContainer = (SportLockScreenContainer *)container;
    if (info->code == HM_KEY_CODE_BACK ||
        info->code == HM_KEY_CODE_SELECT ||
        info->code == HM_KEY_CODE_UP ||
        info->code == HM_KEY_CODE_DOWN)
    {
        if (lockContainer->status.isLock == true)
        {
            _setLock(lockContainer, false);
            getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_UNLOCK_SCREEN);
        }
        return false;
    }

    if (info->code == HM_KEY_CODE_HOME ||
        info->code == HM_KEY_CODE_SHORTCUT)
    {
        if (lockContainer->status.isLock == true)
        {
            _showLock(lockContainer, false);
        }
        return true;
    }

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
    SportLockScreenContainer *LockScreenContainer = (SportLockScreenContainer *)container;
    LockScreenContainer->setVisiable(LockScreenContainer, false);

    if (LockScreenContainer->timerHandle)
    {
        gre_timer_clear_timeout(screenContext->_grAppContext, &(LockScreenContainer->timerHandle));
        LockScreenContainer->timerHandle = H_NULL;
    }
}