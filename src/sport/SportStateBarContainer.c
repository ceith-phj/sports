#include <BaseSportView.h>
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sport/SportStateBarContainer.h>
#include "hm_time.h"
#include "time/datetime_format.h"
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include "core/display_metrics.h"
#include "sb_screen_util.h"
#include "battery_manager.h"
#include "widget/hSchedule/hScedule.h"
#include "presenter/SportPresenter.h"
#include <sb_screen_context.h>

#define TAG "SportStateBarContainer"

#define TIME_CONTROL_PATH       "sportStateBarLayer.time"
#define BATTERT_CONTROL_PATH    "sportStateBarLayer.battery"
#define LOCK_CONTROL_PATH       "sportStateBarLayer.lock"
#define BG_PATH                 "sportStateBarLayer.pauseBackground"
#define STATUS_BAR_LAYER_PATH   "sportStateBarLayer"

#define ID_SCHEDULE             "SportScreen.sportStateBarLayer"

#define BATTERT_PATH            "sportStateBarLayer.battery.batteryIcon"
#define BATTERT_X               "sportStateBarLayer.battery.x"

#define TIME_PATH               "sportStateBarLayer.time.time"
#define TIME_X                  "sportStateBarLayer.time.x"

#define TIME_FORMAT_PATH        "sportStateBarLayer.ampm.text"
#define TIME_FORMAT_X           "sportStateBarLayer.ampm.x"

#define BG_COLOR_PATH           "sportStateBarLayer.pauseBackground.color"
#define LOCK_ICON_X_PATH        "sportStateBarLayer.lock.x"

#define LOCK_IMAGE_PNG_PATH     "images/sports/stateBar/lock.png"

// ##
#define SLIDEANIMATION_TIME 250
#define SLIDEANIMATION_FPS  50
#define SLIDEOFFSETY        getDimension(-75)

#ifndef HM_FEATURE_BLED_UI
#define SCHEDULE_DAFUALT    getDimension(60)
#else
#define SCHEDULE_DAFUALT    gre_get_control_h(getAppInstance(), "sportStateBarLayer.pauseBackground")
#endif

#define SCHEDULE_DISAPPEAR  getDimension(-100)

#define LOCK_BACKGROUND_COLOR 0xD3374F

static const char *batteryIcon[] = {
    "images/sports/stateBar/percent_1.png",
    "images/sports/stateBar/percent_2.png",
    "images/sports/stateBar/percent_3.png",
    "images/sports/stateBar/percent_4.png",
    "images/sports/stateBar/percent_5.png",
    "images/sports/stateBar/percent_6.png",
    "images/sports/stateBar/percent_7.png",
    "images/sports/stateBar/percent_8.png",
    "images/sports/stateBar/percent_9.png",
    "images/sports/stateBar/percent_10.png",
    "images/sports/stateBar/percent_10.png",
    "images/sports/stateBar/recharge.png",
};

CONTAINER_FUNCTION_DEFINE();

static void _updateTime(SportStateBarContainer *container)
{
    HmTime timestamp = {0};
    HmTimeDate timeDate = {0};
    char time[20] = {0};

    hmTime_getTime(&timestamp);
    hmTime_getLocalTime(&timestamp, &timeDate);
    if(datetime_formatTime(&timestamp, time, sizeof(time)))
    {
        setSBStringData(container->app, TIME_PATH, time);
    }
    if (container->status.timeIs24Hour)
    {
        setSBStringData(container->app, TIME_FORMAT_PATH, "");
    }
    else
    {
        setSBIntData(container->app, TIME_FORMAT_X, getDimension(279));
        if (timeDate.hour < 12)
        {
            setSBStringData(container->app, TIME_FORMAT_PATH, "AM");
        }
        else
        {
            setSBStringData(container->app, TIME_FORMAT_PATH, "PM");
        }
    }
}


static void _updateBattery(SportStateBarContainer *container)
{
    BatteryStatus batteryStatus = {0};
    Battery_getStatus(&batteryStatus);
    if (batteryStatus.charge_status == BATTERY_NOT_CHARGING)
    {
        setSBStringData(container->app, BATTERT_PATH, batteryIcon[batteryStatus.battery_level / 10]);
    }
    else
    {
        setSBStringData(container->app, BATTERT_PATH, batteryIcon[11]);
    }
}

static void _setLock(SportStateBarContainer *container, bool isLock)
{

    if (isLock)
    {
        #ifdef HM_DISPLAY_IS_ROUND
        if (container->status.timeIs24Hour)
        {
            setSBIntData(container->app, LOCK_ICON_X_PATH, getDimension(274));
        }
        else
        {
            setSBIntData(container->app, LOCK_ICON_X_PATH, getDimension(310));
        }
        #else
            setSBIntData(container->app, LOCK_ICON_X_PATH, getDimension(3));
        #endif
        gre_set_control_visible(container->app, LOCK_CONTROL_PATH, true);
        //setSBStringData(container->app, BATTERT_PATH, LOCK_IMAGE_PNG_PATH);
        setSBIntData(container->app, BG_COLOR_PATH, LOCK_BACKGROUND_COLOR);
    }
    else
    {
        gre_set_control_visible(container->app, LOCK_CONTROL_PATH, false);
        setSBIntData(container->app, BG_COLOR_PATH, 0);
    }
}

#ifndef HM_DISPLAY_IS_ROUND
static void _setTitleText(SportStateBarContainer *container,char* title)
{
    if(NULL == title)
        return;

    setSBStringData(container->app,"sportStateBarLayer.titleControl.title", System_getLocaleText(title));

}

static void _initTitleView(SportStateBarContainer *container)
{
    _setTitleText(container,"id_sa_sport");
}

static void _setScheduleDefaultY(SportStateBarContainer *container)
{
    hSchedule *_hScedule = findContainer(container->screenContext, ID_SCHEDULE);
    if (_hScedule)
    {
        setScheduleY(_hScedule, SCHEDULE_DAFUALT);
    }

}
void _setScheduleDisappearY(SportStateBarContainer *container)
{
    hSchedule *_hScedule = findContainer(container->screenContext, ID_SCHEDULE);
    if (_hScedule)
    {
        setScheduleY(_hScedule, SCHEDULE_DISAPPEAR);
    }
}
#endif

static void _setScheduleCurrent(SportStateBarContainer *container,int current)
{
    hSchedule *_hScedule = findContainer(container->screenContext, ID_SCHEDULE);
    container->status.focus = current;
    if (_hScedule)
        _hScedule->setScheduleCurrent(_hScedule, current);
    container->setVisiable(container, container->status.visiableState[container->status.focus]);
}


static void _initHSceduleView(SportStateBarContainer *container)
{
    hSchedule *_hScedule = createHSchedulerContainer(container->app);
    if (_hScedule)
    {
        addContainer(container->screenContext, ID_SCHEDULE, _hScedule);
        _hScedule->create(container->app, _hScedule, "sportStateBarLayer", 3, 10);
        _hScedule->initShedule(_hScedule);
        _hScedule->setSchedulePostiton(_hScedule, SCREEN_ROUND);
        _hScedule->setScheduleCurrent(_hScedule, 0);
    }
    #ifndef HM_DISPLAY_IS_ROUND
    _setScheduleDefaultY(container);
    #endif
}

static void _initLockView(SportStateBarContainer *container)
{
    _setLock(container, false);
}

static void _initView(SportStateBarContainer *container)
{
    _initHSceduleView(container);
    _updateBattery(container);
    _updateTime(container);
    _initLockView(container);
    #ifndef HM_DISPLAY_IS_ROUND
    _initTitleView(container);
    #endif
}

static void _initData(SportStateBarContainer *container)
{
    container->status.isLock = false;       //need resume status
    //container->status.isVisiable = true;
    container->status.offsetY = 0;
    container->status.timeIs24Hour = (datetime_getTimeFormat() == TIME_FORMAT_24H); //need update time format
    container->status.focus = 0;
    container->status.visiableState[0] = true;
    container->status.visiableState[1] = true;
    container->status.visiableState[2] = true;
}

static void slideAnimationCallback(gr_application_t *app, void *handle, void *param)
{
    HLOG_D(TAG, "slideAnimationCallback\r\n");
    SportStateBarContainer *container = param;
    container->anim = NULL;
}

static void _startSlideAnimation(SportStateBarContainer *container, bool isVisiable)
{
    if (container->anim != NULL)
    {
        gre_animation_stop(container->app, container->anim);
    }

    container->anim = gre_animation_create(getAppInstance(),
                                           SLIDEANIMATION_FPS, 1, slideAnimationCallback, container);

    gr_animation_data_t data[4] = {0};
    char time[8] = {0};
    char to[8] = {0};
    data[0].key = "rate";
    data[0].val = "easeout";

    data[1].key = "key";
    data[1].val = "SportScreen.sportStateBarLayer.grd_yoffset";

    data[2].key = "to";
    int16_t toVal = isVisiable ? 0 : SLIDEOFFSETY;
    snprintf(to, sizeof(to), "%d", toVal);
    data[2].val = to;

    uint16_t time_ms = SLIDEANIMATION_TIME;
    snprintf(time, sizeof(time), "%d", time_ms);
    data[3].key = "duration";
    data[3].val = time;

    gre_animation_add_step(getAppInstance(), container->anim, data, sizeof(data) / sizeof(data[0]));

    gre_animation_trigger(getAppInstance(), container->anim);
}

static void _setVisiable(SportStateBarContainer *container,bool isVisiable)
{
    /* if (container->status.visiableState[container->status.focus] == isVisiable)
    {
        return;
    } */
    //container->status.isVisiable = isVisiable;
    container->status.visiableState[container->status.focus] = isVisiable;
    _startSlideAnimation(container, isVisiable);
}

static void _initContainer(SportStateBarContainer *container, ScreenContext *screenContext)
{
    container->screenContext = screenContext;
    container->app = screenContext->_grAppContext;
    _initData(container);
    _initView(container);
    //container->createTimer(container);
}


static void _secTickEvent(SportStateBarContainer *container)
{
    /* if (container->status.isVisiable)
        return; */
    _updateBattery(container);
    _updateTime(container);
}

static void _restoreContainer(SportStateBarContainer *container)
{
    if (container->anim != NULL)
    {
        gre_animation_stop(container->app, container->anim);
    }
    if (container->status.visiableState[container->status.focus] == true)
    {
        setSBIntData(container->app, "SportScreen.sportStateBarLayer.grd_yoffset", 0);
    }
    else
    {
        setSBIntData(container->app, "SportScreen.sportStateBarLayer.grd_yoffset", SLIDEOFFSETY);
    }
}

SportStateBarContainer *bindSportStateBarView(gr_application_t *app)
{
    SportStateBarContainer *container = H_NULL;
    createContainer(container, SportStateBarContainer, container->container);
    if (container)
    {
        container->initContainer = _initContainer;
        container->setVisiable = _setVisiable;
        container->setLock = _setLock;
        container->setScheduleCurrent = _setScheduleCurrent;
        container->secTickEvent = _secTickEvent;
        container->restoreContainer = _restoreContainer;
        #ifndef HM_DISPLAY_IS_ROUND
        container->setTitleText = _setTitleText;
        container->setScheduleDefaultY = _setScheduleDefaultY;
        container->setScheduleDisappearY = _setScheduleDisappearY;

        #endif
    }
    return container;
}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container, const HmKeyEvent *info)
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
    SportStateBarContainer *StateBarContainer = (SportStateBarContainer *)container;
    // reset offsetY
    StateBarContainer->setLock(StateBarContainer, false);
    setSBIntData(StateBarContainer->app, "SportScreen.sportStateBarLayer.grd_yoffset", 0);
    if (StateBarContainer->anim != NULL)
    {
        gre_animation_stop(StateBarContainer->app, StateBarContainer->anim);
    }
}