/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: wangxiaomeng@huami.com
 *  Maintainer: wangxiaomeng@huami.com
 *
 * =====================================================================================
 */

#include <stdbool.h>
#include <gre/gre.h>
#include <sb_screen_context.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_translation.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <system_locale.h>
#include "sport_utils.h"
#include <presenter/SportPresenter.h>
#include <BaseSportView.h>
#include "widget/alertDialog/alertDialog.h"
#include <core/power/hm_power_manager.h>
#include "manager/sports_control_manager.h"
#include "sport_auto_recog_service.h"
#include "packages/services/sport/voicebroad/sport_voice_service.h"
#ifdef HM_DISPLAY_IS_ROUND
#else
#include <hm_system_status_bar.h>
#endif


#define TAG "SportAutoRecogScreen"

#define SCREEN_NAME SportAutoRecog
#define SPORTTYPE_ICON_PATH "SportAutoRecogLayer.IconControl.sportType"
#define SPORTSTATE_ICON_PATH "SportAutoRecogLayer.statusControl.status"
#define SPORTBUTTON1_TEXT_PATH "SportAutoRecogLayer.ButttonControl1.button1Text"
#define SPORTBUTTON2_TEXT_PATH "SportAutoRecogLayer.ButttonControl2.button2Text"
#define SPORTBUTTON3_TEXT_PATH "SportAutoRecogLayer.ButttonControl3.button3Text"
#define SPORTBUTTON3_CONTROL_PATH "SportAutoRecogLayer.ButttonControl3"
#define SPORTBUTTON2_CONTROL_PATH "SportAutoRecogLayer.ButttonControl2"
#define SPORTBUTTON_ENTER_CONTROL_PATH "SportAutoRecogLayer.ButttonControl1"
#define SPORTBUTTON_IGNORE_CONTROL_PATH "SportAutoRecogLayer.ButttonControl2"
#define ID_SPORT_AUTO_RECOG_DIALOG "SportAutoRecogScreen.AlertDialog"
#define ENTER_SPORT_TIME    10  /* 10s */
#define REFRESH_TIME    1000   /* 1s */

static bool goto_sport = H_False;
static uint32_t enterTime = 0;

static void onScreenPreCreate(ScreenContext *screenContext, void *data);
static void onScreenPostCreate(ScreenContext *screenContext);
static void onScreenPreDestory(ScreenContext *screenContext);
static void onScreenPostDestory(ScreenContext *screenContext);
static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event);
static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId);
static bool handleKeyEvent(ScreenContext *screenContext,
                           const HmKeyEvent *info);
static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info);
static void onAnimationStop(ScreenContext *screenContext, const char *id);
static void onAnimationComplete(ScreenContext *screenContext, const char *id);
static void onDataSave(ScreenContext *screenContext);
static void onDataRestore(ScreenContext *screenContext, void *data);
static void onNewScreenData(ScreenContext *screenContext, void *data);



DEFINE_SB_DATA_START(sCustomData)
    .isHideSystemBar = H_True,
DEFINE_SB_DATA_END

INTERFACE_DEFINED_START(sScreenInterface)
    .onScreenPreCreate = onScreenPreCreate,
    .onScreenPostCreate = onScreenPostCreate,
    .onScreenPreDestory = onScreenPreDestory,
    .onScreenPostDestory = onScreenPostDestory,
    .onNewScreenData = onNewScreenData,
    .onDataRestore = onDataRestore,
    .onDataSave = onDataSave,
    .onAnimationComplete = onAnimationComplete,
    .onAnimationStop = onAnimationStop,
    .handleTouchEvent = handleTouchEvent,
    .handleKeyEvent = handleKeyEvent,
    .handleGestureEvent = handleGestureEvent,
    .handleEvent = handleEvent,
INTERFACE_DEFINED_END

SCREEN_DECLARE_CUSTOM_START(SCREEN_NAME)
    .priority = PRIORITY_SCREEN_LOW,
    .data = &sCustomData,
    .screenInterface = &sScreenInterface,
SCREEN_DECLARE_CUSTOM_END



typedef struct
{
    int16_t countDownTime;
    bool dialogIsShowing;
    void *timerHandle;
    AlertDialog *dialogHandle;
} AppData;

typedef struct
{
    /* data */
    bool dialogIsShowing;
} SaveData;


static void _refreshButtonTime(ScreenContext *screenContext, int16_t time)
{
    char text[256];
    snprintf(text, sizeof(text), System_getLocaleText("id_sport_autorecog_enter"),
             time);
    setSBStringData(screenContext->_grAppContext, SPORTBUTTON1_TEXT_PATH, text);
}

static H_Int32 _refreshScreen(struct gr_timer *timer, void *data, int dropped)
{
    ScreenContext *screenContext = (ScreenContext *)data;
    AppData *appData = getScreenData(screenContext, AppData);

    if (appData->countDownTime != 0)
    {
        appData->countDownTime--;
    }

    _refreshButtonTime(screenContext, appData->countDownTime);

    if (appData->countDownTime <= 0)
    {
        enterTime = 0;
        getSportUtils()->openSportByFlag(FLAG_SCREEN_REMOVE_CURRENT);
        sportVoice_broadSportControl(SPORT_CMD_START); /* 语音播报：开始运动*/
    }

    return 0;
}

static void _creatScreenTimer(ScreenContext *screenContext)
{
    AppData *appData = getScreenData(screenContext, AppData);

    if (appData->timerHandle)
    {
        return;
    }

    appData->timerHandle = gre_timer_create_timeout(screenContext->_grAppContext,
                           REFRESH_TIME, REFRESH_TIME, screenContext, _refreshScreen);

    if (appData->timerHandle == H_NULL)
    {
        HLOG_W(TAG, "initScreenTimer failed");
        return;
    }
}
static void _clickListener(ScreenContext *screenContext, AlertDialog *self,
                           AlertButtonType type)
{
    AppData *appData = getScreenData(screenContext, AppData);

    switch (type)
    {
        case BUTTON_LEFT_CLICK:
            HLOG_I(TAG, "%s auto-reg entered\r\n", __func__);
            self->dismissDialog(self);
            gre_timer_resume(screenContext->_grAppContext, appData->timerHandle);
            appData->dialogIsShowing = false;
            enterTime = 0;
            /* 持亮屏锁 */
            getPowerManager()->acquireWithTimeout("sport auto recog",
                                                  SCREEN_BRIGHT_WAKE_LOCK | ON_AFTER_RELEASE, appData->countDownTime);
            break;

        case BUTTON_RIGHT_CLICK:
            HLOG_I(TAG, "%s auto-reg ignored\r\n", __func__);
            enterTime = 0;
            /* 忽略自动识别 */
            SportService_getInstance()->getAutoRecogService()->ignoreSport();
            getScreenManager()->finishCurrentScreen();
            break;
    }
}

static void _creatDialog(ScreenContext *screenContext)
{
    AlertDialog *alert_dialog = NULL;
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    alert_dialog = creatAlertDialog(getScreenContext(
                                        currentScreenInstance), "SportAutoRecogScreen");
    addContainer(getScreenContext(currentScreenInstance),
                 ID_SPORT_AUTO_RECOG_DIALOG,
                 alert_dialog);
    alert_dialog->setTitleAndContent(alert_dialog, NULL,
                                     System_getLocaleText("id_sport_autorecog_dialog"));
    alert_dialog->setPositiveButton(alert_dialog, H_NULL);
    alert_dialog->setNegativeButton(alert_dialog, H_NULL);
    alert_dialog->setClickListener(alert_dialog, _clickListener);
    appData->dialogHandle = alert_dialog;
}

static H_Bool autoSportStart()
{
    HmTime time;
    hmTime_getTime(&time);
    uint8_t diff  = time.sec - enterTime;
    HLOG_I(TAG, "current:%d,enterTime:%d\r\n", time.sec,
           enterTime);

    if (diff > 10)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HmTime time;
    hmTime_getTime(&time);

    if (enterTime == 0)
    {
        enterTime = time.sec;
    }

    goto_sport = autoSportStart();

    if (goto_sport)
    {
        enterTime = 0;
        getSportUtils()->openSportByFlag(FLAG_SCREEN_REMOVE_CURRENT);
        sportVoice_broadSportControl(SPORT_CMD_START); /* 语音播报：开始运动*/
        return;
    }

    /* 初始化 app数据 */
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));

    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }
    else
    {
        H_ASSERT(0 && "malloc app data failed");
    }

    appData->countDownTime = ENTER_SPORT_TIME - (time.sec - enterTime);
    /* 修改运动类型 */
    sport_type_t sportType = getSportsScreenPresenter()->getSportType();
    //if (sportType_isSupported(sportType))
    {
        setSBStringData(screenContext->_grAppContext, SPORTTYPE_ICON_PATH,
                        sportType_getTypeIconSizeId(sportType, 160));
    }
    /* 修改状态图标 */
    setSBStringData(screenContext->_grAppContext, SPORTSTATE_ICON_PATH,
                    "images/sports/pause/icon_ar_start.png");
    /* 修改按钮个数和文案 */
    _refreshButtonTime(screenContext, appData->countDownTime);
    setSBStringData(screenContext->_grAppContext, SPORTBUTTON2_TEXT_PATH,
                    System_getLocaleText("id_sport_autorecog_ignore"));
    gre_set_control_visible(screenContext->_grAppContext, SPORTBUTTON3_CONTROL_PATH,
                            false);
    /* 超过三个按键要使能页面滑动 */
    //gre_set_layer_attr(screenContext->_grAppContext, "SportAutoRecogScreen.SportAutoRecogLayer", "scroll_enabled", true);
    /* 创建timer 刷新按钮时间 */
    _creatScreenTimer(screenContext);
    /* 创建对话框 */
    _creatDialog(screenContext);
    /* 持亮屏锁 */
    getPowerManager()->acquireWithTimeout("sport auto recog",
                                          SCREEN_BRIGHT_WAKE_LOCK | ON_AFTER_RELEASE, ENTER_SPORT_TIME);
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    if (goto_sport)
    {
        return;
    }

    //HLOG_I(TAG, "%s\r\n", __func__);
    if (data)
    {
        AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                         AppData);
        SaveData *saveData = (SaveData *)data;
        memcpy(&appData->dialogIsShowing, &saveData->dialogIsShowing, sizeof(bool));

        /* 还原dialog */
        if (appData->dialogIsShowing == true)
        {
            appData->dialogIsShowing = true;
            gre_timer_pause(screenContext->_grAppContext, appData->timerHandle);
            /* show dialog */
            appData->dialogHandle->showDialog(appData->dialogHandle);
        }
    }
    else
    {
        HLOG_W(TAG, "%s no app data\r\n", __func__);
    }
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    if (goto_sport)
    {
        return;
    }

    HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = getScreenData(screenContext, AppData);

    /* 震动 */
    if (appData->countDownTime == ENTER_SPORT_TIME)
    {
        getSportsScreenPresenter()
        ->playReminder(SPORT_REMIND_MODE_REMIND);
    }
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    if (goto_sport)
    {
        return;
    }

    AppData *appData = getScreenData(screenContext, AppData);
    HLOG_I(TAG, "%s\r\n", __func__);

    if (appData->timerHandle)
    {
        gre_timer_clear_timeout(screenContext->_grAppContext, &(appData->timerHandle));
        appData->timerHandle = H_NULL;
        HLOG_I(TAG, "destroyTimer");
    }
}

static void onDataSave(ScreenContext *screenContext)
{
    if (goto_sport)
    {
        return;
    }

    AppData *appData = getScreenData(screenContext, AppData);
    HLOG_I(TAG, "%s : countDownTime:%d", __func__, appData->countDownTime);
    SaveData *saveData = H_MALLOC(sizeof(SaveData));
    memcpy(&saveData->dialogIsShowing, &appData->dialogIsShowing, sizeof(bool));
    getScreenManager()->saveScreenData(TAG, saveData, sizeof(SaveData));
    H_FREE(saveData);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    if (goto_sport)
    {
        goto_sport = H_False;
        return;
    }

    HLOG_I(TAG, "%s\r\n", __func__);
    //free screen
    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId)
{
    /* 不支持右滑返回 */
    /* HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = getScreenData(screenContext, AppData);
    if (appData->dialogHandle != H_NULL && appData->dialogHandle->isShowing(appData->dialogHandle))
    {
        return true;
    }

    if (eventId == GESTURE_RIGHT)
    {
        getScreenManager()->finishCurrentScreen();
        return true;
    } */
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    HLOG_I(TAG, "%s :%s\r\n", __func__, event);
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    HLOG_I(TAG, "%s :%s\r\n", __func__, id);
    AppData *appData = getScreenData(screenContext, AppData);

    if (appData->dialogHandle != H_NULL
            && appData->dialogHandle->isShowing(appData->dialogHandle))
    {
        appData->dialogHandle->container.handleTouchEvent(screenContext,
                &appData->dialogHandle->container, id, eventId, info);
        return true;
    }

    if (STR_MATCH(SPORTBUTTON_ENTER_CONTROL_PATH, id))
    {
        enterTime = 0;
        getSportUtils()->openSportByFlag(FLAG_SCREEN_REMOVE_CURRENT);
        sportVoice_broadSportControl(SPORT_CMD_START); /* 语音播报：开始运动*/
        return true;
    }
    else if (STR_MATCH(SPORTBUTTON_IGNORE_CONTROL_PATH, id))
    {
        HLOG_I(TAG, "%s : ignore dialog open", __func__);

        // dial-state-dot should be unregistered when ignore-button pressed
        getSportsScreenPresenter()->requestDialState(NULL, 0);

        //
        if (appData->dialogHandle != NULL)
        {
            appData->dialogHandle->showDialog(appData->dialogHandle);
            getSportsScreenPresenter()
            ->playReminder(SPORT_REMIND_MODE_WARN);
            gre_timer_pause(screenContext->_grAppContext, appData->timerHandle);
            appData->dialogIsShowing = true;
            return true;
        }

        HLOG_W(TAG, "appData->dialogHandle is null");
    }

    return false;
}
