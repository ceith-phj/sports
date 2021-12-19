#include <BaseSportView.h>
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <packages/apps/sports/include/presenter/SportPresenter.h>
#include <sport/SportGpsNoticeContainer.h>
#include <core/power/hm_power_manager.h>
#include "packages/services/sport/misc/sport_misc.h"
#include "hm_time.h"

#define TAG "gpsNotice"

CONTAINER_FUNCTION_DEFINE();

// ##
#define GPS_NOTICE_ID   "SportGpsNoticeLayer.gpsNotice"
#define BTN_WAIT_ID     "SportGpsNoticeLayer.btnWait"
#define BTN_START_ID    "SportGpsNoticeLayer.btnStart"
#define LAYER_NOTICE_ID "SportScreen.SportGpsNoticeLayer"

#define START_COUNT_DOWN_SEC    (10)

// ##
static uint32_t _getTimeStamp(void);
static void _initViews(SportGpsNoticeContainer *container);
static void _initData(SportGpsNoticeContainer *container);
static void _secTickEvent(SportGpsNoticeContainer *container);

// ##
static void initContainer(SportGpsNoticeContainer *container, ScreenContext *context)
{
    container->context = context;
    container->app = context->_grAppContext;
    container->setVisiable(container, false);
    _initData(container);
    _initViews(container);
}

static void _initViews(SportGpsNoticeContainer *container)
{
    char btnWaitID[48] = {0};
    char btnStartID[48] = {0};
    char btnNoticeID[128] = {0};

    hm_snprintf(btnNoticeID, sizeof(btnNoticeID), "%s.text", GPS_NOTICE_ID);
    setSBStringData(container->app, btnNoticeID, System_getLocaleText("id_sport_gps_notice"));

    hm_snprintf(btnWaitID, sizeof(btnWaitID), "%s.text", BTN_WAIT_ID);
    setSBStringData(container->app, btnWaitID, System_getLocaleText("id_sport_gps_operate_wait"));

    char startText[24] = {0};
    hm_snprintf(startText, sizeof(startText), "GO(%d)", START_COUNT_DOWN_SEC);
    hm_snprintf(btnStartID, sizeof(btnStartID), "%s.text", BTN_START_ID);
    setSBStringData(container->app, btnStartID, startText);
    gre_redraw(container->app);
    _secTickEvent(container);
}

static void _initData(SportGpsNoticeContainer *container)
{
    container->startTime = 0;
}

static void setVisiable(SportGpsNoticeContainer *container, int isVisiable)
{
    if (isVisiable)
    {
        _initViews(container);
    }
    gre_set_layer_visible(container->app, LAYER_NOTICE_ID, isVisiable);
    container->startTime = 0;
}

static bool getVisiable(SportGpsNoticeContainer *container)
{
    int visiable = gre_get_layer_visible(container->app, LAYER_NOTICE_ID);
    return visiable ? true : false;
}

static uint32_t _getTimeStamp(void)
{
    HmTime time;
    hmTime_getTime(&time);
    return time.sec;
}

static void _secTickEvent(SportGpsNoticeContainer *container)
{
    if (!container)
    {
        return;
    }

    if (!container->getVisiable(container))
    {
        return;
    }

    if(!container->startTime)
    {
        container->startTime = _getTimeStamp();
    }
    else if(_getTimeStamp() > container->startTime + START_COUNT_DOWN_SEC)
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_START_SPORT);
    }
    else
    {
        char startText[24] = {0};
        char btnStartID[48] = {0};
        int32_t diff = START_COUNT_DOWN_SEC + container->startTime - _getTimeStamp();
        hm_snprintf(startText, sizeof(startText), "GO(%d)", diff);
        hm_snprintf(btnStartID, sizeof(btnStartID), "%s.text", BTN_START_ID);
        setSBStringData(container->app, btnStartID, startText);
        HLOG_I(TAG, "count down : %d\r\n", diff);
    }
}

static void _restoreContainer(SportGpsNoticeContainer *container)
{
    getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_HIDE_STATUS_BAR);
    getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_GPS_NOTICE);
}

SportGpsNoticeContainer *bindSportGpsNoticeView(gr_application_t *app)
{
    SportGpsNoticeContainer *container = H_NULL;
    createContainer(container, SportGpsNoticeContainer, container->container);
    container->initContainer = initContainer;
    container->setVisiable = setVisiable;
    container->getVisiable = getVisiable;
    container->secTickEvent = _secTickEvent;
    container->restoreContainer = _restoreContainer;

    return container;
}

static bool handleTouchEvent(ScreenContext *context,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    SportGpsNoticeContainer *gpsContainer = (SportGpsNoticeContainer *)container;

    if (!gpsContainer->getVisiable(gpsContainer))
    {
        return false;
    }

    if (eventId == TOUCH_SELECT)
    {
        if (STR_MATCH(BTN_WAIT_ID, id))
        {
            getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
            getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_PRESTART);
            return true;
        }
        else if (STR_MATCH(BTN_START_ID, id))
        {
            gpsContainer->startTime = 0;
            getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_START_SPORT);
            return true;
        }
    }

    return false;
}

static bool handleKeyEvent(ScreenContext *context, ScreenContainer *container, const HmKeyEvent *info)
{
    return false;
}

static bool handleGestureEvent(ScreenContext *context, ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *context, ScreenContainer *container, const char *id, const char *event)
{
    return false;
}

static void destroy(ScreenContext *context, ScreenContainer *container)
{
    SportGpsNoticeContainer *gpsContainer = (SportGpsNoticeContainer *)container;
    gpsContainer->startTime = 0;
}