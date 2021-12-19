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

#include <stdbool.h>
#include <gre/gre.h>
#include <sb_screen_context.h>
#include <presenter/sports_list_presenter.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_translation.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <setting/AssistantSettingLayer.h>

#define TAG "SportList"

#define SCREEN_NAME SportSetting
#define ID_SPORT_SETTING_ASSISTANT "SportSettingAssistantContainer"

SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    BaseSportSettingLayer *settingContainer;
} AppData;

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }

    appData->settingContainer = createAssistantSettingContainer(screenContext->_grAppContext, "sportSettingMainLayer");
    addContainer(screenContext, ID_SPORT_SETTING_ASSISTANT, appData->settingContainer);
    appData->settingContainer->initContainer(appData->settingContainer, screenContext);
}

static void onNewScreenData(ScreenContext* screenContext, void* data) {
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);

    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    getScreenManager()->finishCurrentScreen();
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    ScreenContainer *settingList = findContainer(screenContext, ID_SPORT_SETTING_ASSISTANT);
    if (settingList)
    {
        return settingList->handleTouchEvent(screenContext, settingList, id, eventId, info);
    }
    return false;
}
