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

#include <setting/AssistantSettingScreenImpl.h>
#include <setting/AssistantSettingLayer.h>
#include <sb_screen_context.h>

#define SCREEN_SPORT_SETTING_NAME SportSettingAssistant

static BaseSportSettingLayer *getBaseSportLayer(ScreenContext *screenContext)
{
    return createAssistantSettingContainer(screenContext->_grAppContext, "sportSettingMainLayer");
}

static SportSettingScreenImpl childSettingLayer = {
    .getBaseSportLayer = getBaseSportLayer,
};

BaseSportSettingScreen *newAssistantSettingScreen()
{
    BaseSportSettingScreen *screen = newSettingScreen();
    if (screen)
    {
        screen->childScreen = &childSettingLayer;
    }
    return screen;
}