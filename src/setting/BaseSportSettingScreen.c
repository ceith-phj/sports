/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: lizheng1@huami.com
 *  Maintainer: lizheng1@huami.com
 *
 * =====================================================================================
 */

#include <setting/BaseSportSettingScreen.h>
#include <hlog.h>
#include <sb_screen_context.h>
#include <setting/BaseSportSettingLayer.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <manager/sports_setting_manager.h>

#define TAG "BaseSportSettingScreen"

typedef struct
{
    bool isStart;
    int *types;
    int type_len;
    int select_sport_type;
    BaseSportSettingLayer *settingLayer;
} AppData;

static AppData *getAppData(BaseSportSettingScreen *baseScreen, ScreenContext *screenContext)
{
    AppData *data = getScreenData(screenContext, AppData);
    return data;
}

static void initSetting(BaseSportSettingScreen *screen, ScreenContext *screenContext)
{
    BaseSportSettingLayer *settingLayer = getAppData(screen, screenContext)->settingLayer;
    settingLayer->setting = H_MALLOC(sizeof(SportSettingsMessage));
    memset(getAppData(screen, screenContext)->settingLayer->setting, 0, sizeof(SportSettingsMessage));
    settingLayer->sportType = getSportsControlManager()->getType();
    sportSetting_getSettingManager()->getSetting(settingLayer->sportType, settingLayer->setting);
}

static void saveSetting(BaseSportSettingScreen *screen, ScreenContext *screenContext)
{
    BaseSportSettingLayer *settingLayer = getAppData(screen, screenContext)->settingLayer;
    sportSetting_getSettingManager()->setSetting(settingLayer->sportType, settingLayer->setting);

    if (getAppData(screen, screenContext)->settingLayer->setting)
    {
        H_FREE(getAppData(screen, screenContext)->settingLayer->setting);
    }
}

static void onScreenPreCreate(BaseSportSettingScreen *screen, ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }
    getAppData(screen, screenContext)->settingLayer = screen->childScreen->getBaseSportLayer(screenContext);
    addContainer(screenContext, screen->childScreen->getBaseSportLayer(screenContext)->childLayer->getContainerId(), getAppData(screen, screenContext)->settingLayer);
    getAppData(screen, screenContext)->settingLayer->initContainer(getAppData(screen, screenContext)->settingLayer, screenContext);

    initSetting(screen, screenContext);
    screen->gestureCount = 0;
}

static void onNewScreenData(BaseSportSettingScreen *screen, ScreenContext *screenContext, void *data)
{
}

static void onDataRestore(BaseSportSettingScreen *screen, ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostCreate(BaseSportSettingScreen *screen, ScreenContext *screenContext)
{

    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPreDestory(BaseSportSettingScreen *screen, ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onDataSave(BaseSportSettingScreen *screen, ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostDestory(BaseSportSettingScreen *screen, ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    saveSetting(screen, screenContext);
    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

static void onAnimationComplete(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onAnimationStop(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static bool handleKeyEvent(BaseSportSettingScreen *screen, ScreenContext *screenContext, const HmKeyEvent *info)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return false;
}

static bool handleGestureEvent(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    if (screen->gestureCount == 0 && eventId == GESTURE_RIGHT)
    {
        getScreenManager()->finishCurrentScreen();
        screen->gestureCount++;
        return true;
    }
    return false;
}

static bool handleEvent(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id, const char *event)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return false;
}

static bool handleTouchEvent(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    ScreenContainer *settingList = findContainer(screenContext, screen->childScreen->getBaseSportLayer(screenContext)->childLayer->getContainerId());
    if (settingList)
    {
        return settingList->handleTouchEvent(screenContext, settingList, id, eventId, info);
    }
    return false;
}

BaseSportSettingScreen *newSettingScreen()
{

    BaseSportSettingScreen *screen = H_NULL;
    screen = H_MALLOC(sizeof(BaseSportSettingScreen));
    if (screen)
    {
        screen->onScreenPreCreate = onScreenPreCreate;
        screen->onNewScreenData = onNewScreenData;
        screen->onDataRestore = onDataRestore;
        screen->onScreenPostCreate = onScreenPostCreate;
        screen->onScreenPreDestory = onScreenPreDestory;
        screen->onDataSave = onDataSave;
        screen->onScreenPostDestory = onScreenPostDestory;
        screen->onAnimationComplete = onAnimationComplete;
        screen->onAnimationStop = onAnimationStop;
        screen->handleTouchEvent = handleTouchEvent;
        screen->handleKeyEvent = handleKeyEvent;
        screen->handleGestureEvent = handleGestureEvent;
        screen->handleEvent = handleEvent;
    }
    return screen;
}