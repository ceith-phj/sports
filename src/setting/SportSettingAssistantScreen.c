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

#include <BaseSportView.h>
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
#include <widget/unequalHeiList/unequalHeiList.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <setting/MainSettingLayer.h>
#include <setting/AssistantSettingScreenImpl.h>

#define TAG "SportSettingAssistScreen"

#define TABLE_LIST "ListLayer.ListTable"
#define ID_SPORT_SETTING_MAIN "sportSettingList.mainsetting"
#define SCREEN_SPORT_SETTING_NAME SportSettingAssistant1

SCREEN_DECLARE(SCREEN_SPORT_SETTING_NAME);

static BaseSportSettingScreen *settingScreen;

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    settingScreen = newAssistantSettingScreen();
    if(settingScreen)
    {
        settingScreen->onScreenPreCreate(settingScreen, screenContext, data);
    }
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
    settingScreen->onNewScreenData(settingScreen, screenContext, data);
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    settingScreen->onDataRestore(settingScreen, screenContext, data);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    settingScreen->onScreenPostCreate(settingScreen, screenContext);
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    settingScreen->onScreenPreDestory(settingScreen, screenContext);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    settingScreen->onDataSave(settingScreen, screenContext);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    settingScreen->onScreenPostDestory(settingScreen, screenContext);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    settingScreen->onAnimationComplete(settingScreen, screenContext, id);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    settingScreen->onAnimationStop(settingScreen, screenContext, id);
}

static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return settingScreen->handleKeyEvent(settingScreen, screenContext, info);
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return settingScreen->handleGestureEvent(settingScreen, screenContext, id, eventId);
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return settingScreen->handleEvent(settingScreen, screenContext, id, event);
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return settingScreen->handleTouchEvent(settingScreen, screenContext, id, eventId, info);
}
