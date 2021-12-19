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

#include <setting/BaseSportSettingLayer.h>
#include <setting/AssistantSettingLayer.h>
#include <screen_manager.h>
typedef enum SPORTSETTING_ELEMENT_TYPE
{
    SPORTSETTING_ELEMENT_GOAL = 0,
    SPORTSETTING_ELEMENT_REMINDER,
    SPORTSETTING_ELEMENT_SEGMENT,
    SPORTSETTING_ELEMENT_AUTO_PAUSE,
    SPORTSETTING_ELEMENT_VIRTUAL_RABBIT,
    SPORTSETTING_ELEMENT_CADENCE,
    SPORTSETTING_ELEMENT_PACE,
    SPORTSETTING_ELEMENT_COMPASS,
    SPORTSETTING_ELEMENT_SAO2,
    NUM_OF_SPORTSETTING_ELEMENT
} SPORTSETTING_ELEMENT_TYPE;

static SportSettingItem defaultSettingItems[NUM_OF_SPORTSETTING_ELEMENT] = {
    {SPORTSETTING_ELEMENT_GOAL, "translations.id_sport_setting_workout_goal", "images/sports/setting/sport_setting_icon_assistant.png", SINGLE_LINE},
    {SPORTSETTING_ELEMENT_REMINDER, "translations.id_sport_setting_workout_reminder", "images/sports/setting/sport_setting_icon_traning.png", SINGLE_LINE},
    {SPORTSETTING_ELEMENT_SEGMENT, "translations.id_sport_setting_auto_lap", "images/sports/setting/sport_setting_icon_more.png", SINGLE_LINE},
    {SPORTSETTING_ELEMENT_AUTO_PAUSE, "translations.id_sport_setting_auto_pause", "images/sports/setting/sport_setting_icon_assistant.png", SINGLE_LINE},
    {SPORTSETTING_ELEMENT_VIRTUAL_RABBIT, "translations.id_sport_setting_virtual_pacer", "images/sports/setting/sport_setting_icon_traning.png", SINGLE_LINE},
    {SPORTSETTING_ELEMENT_CADENCE, "translations.id_sport_setting_cadence_assistant", "images/sports/setting/sport_setting_icon_more.png", SINGLE_LINE},
    {SPORTSETTING_ELEMENT_PACE, "translations.id_sport_setting_pace_strategy", "images/sports/setting/sport_setting_icon_assistant.png", SINGLE_LINE},
    {SPORTSETTING_ELEMENT_COMPASS, "translations.id_sport_setting_compass", "images/sports/setting/sport_setting_icon_traning.png", SWITCH_SINGLE_LINE},
    {SPORTSETTING_ELEMENT_SAO2, "translations.id_sport_setting_spo2", "images/sports/setting/sport_setting_icon_traning.png", SWITCH_SINGLE_LINE},
};

static SportSettingItem *getDefaultSettingItem(void)
{
    return defaultSettingItems;
}

static H_Int8 getDefaultSettingItemSize(void)
{
    return NUM_OF_SPORTSETTING_ELEMENT;
}

static bool isSettingItemEnable(SportSettingLayerImpl *child, H_Uint8 type)
{
    bool isEnable = false;
    AssistMessage *thisSet = &child->parent->setting->assist;

    switch (type)
    {
    case SPORTSETTING_ELEMENT_GOAL:
        isEnable = thisSet->has_target;
        break;

    case SPORTSETTING_ELEMENT_REMINDER:
        isEnable = thisSet->has_reminder;
        break;

    case SPORTSETTING_ELEMENT_SEGMENT:
        isEnable = thisSet->has_autoLap;
        break;

    case SPORTSETTING_ELEMENT_AUTO_PAUSE:
        isEnable = thisSet->has_autoPause;
        break;

    case SPORTSETTING_ELEMENT_VIRTUAL_RABBIT:
        isEnable = thisSet->has_virtualPacer;
        break;

    case SPORTSETTING_ELEMENT_CADENCE:
        isEnable = thisSet->has_cadenceAssistant;
        break;

    case SPORTSETTING_ELEMENT_PACE:
        isEnable = thisSet->has_paceStrategy;
        break;

    case SPORTSETTING_ELEMENT_COMPASS:
        isEnable = thisSet->has_compass;
        break;

    case SPORTSETTING_ELEMENT_SAO2:
        isEnable = thisSet->has_spO2;
        break;

    default:
        isEnable = false;
        break;
    }
    return isEnable;
}

static void initData(SportSettingLayerImpl *child)
{
}

static char *getTitleId()
{
    return "translations.id_sport_setting_title_assistant_setting";
}

static char *getContainerId()
{
    return "Setting.Assistant";
}

static void onItemClick(SportSettingLayerImpl *child, H_Uint8 type, SportSettingItem *item)
{
    HLOG_W("jude", "onSettingItemClick type:%d\r\n", type);
    switch (type)
    {
    case SPORTSETTING_ELEMENT_GOAL:
        getScreenManager()->startScreen("SportSettingGoalScreen", H_NULL, 0);
        break;

    case SPORTSETTING_ELEMENT_REMINDER:
        getScreenManager()->startScreen("SportSettingReminderScreen", H_NULL, 0);
        break;

    case SPORTSETTING_ELEMENT_SEGMENT:
        getScreenManager()->startScreen("SportSettingAutoLapScreen", H_NULL, 0);
        break;

    case SPORTSETTING_ELEMENT_AUTO_PAUSE:
        getScreenManager()->startScreen("SportSettingAutoPauseScreen", H_NULL, 0);
        break;

    case SPORTSETTING_ELEMENT_VIRTUAL_RABBIT:
        getScreenManager()->startScreen("SportSettingVirtualPacerScreen", H_NULL, 0);
        break;

    case SPORTSETTING_ELEMENT_CADENCE:
        getScreenManager()->startScreen("SportSettingCadenceAssistantScreen", H_NULL, 0);
        break;

    case SPORTSETTING_ELEMENT_PACE:
        getScreenManager()->startScreen("SportSettingPaceStrategyScreen", H_NULL, 0);
        break;

    default:
        break;
    }
}

static void onItemUpdate(SportSettingLayerImpl *child, H_Uint8 type, SportSettingItem *item)
{
    HLOG_W("jude", "onSettingItemUpdate type:%d\r\n", type);

    AssistMessage *thisSet = &child->parent->setting->assist;
    switch (type)
    {
    case SPORTSETTING_ELEMENT_COMPASS:
        item->checkBox->setChecked(item->checkBox, thisSet->compass.onOff);
        break;

    case SPORTSETTING_ELEMENT_SAO2:
        item->checkBox->setChecked(item->checkBox, thisSet->compass.onOff);
        break;

    default:
        break;
    }
}

static void onItemCheckChanged(SportSettingLayerImpl *child, H_Uint8 type, bool isChecked, SportSettingItem *item)
{
    AssistMessage *thisSet = &child->parent->setting->assist;

    switch (type)
    {
    case SPORTSETTING_ELEMENT_COMPASS:
        thisSet->compass.onOff = isChecked;
        break;

    case SPORTSETTING_ELEMENT_SAO2:
        thisSet->spO2.onOff = isChecked;
        break;

    default:
        break;
    }
}

static SportSettingLayerImpl childSettingLayer = {
    .getTitleId = getTitleId,
    .initData = initData,
    .getContainerId = getContainerId,
    .onItemClick = onItemClick,
    .onItemUpdate = onItemUpdate,
    .onItemCheckChanged = onItemCheckChanged,
    .getDefaultSettingItem = getDefaultSettingItem,
    .getDefaultSettingItemSize = getDefaultSettingItemSize,
    .isSettingItemEnable = isSettingItemEnable,
};

BaseSportSettingLayer *createAssistantSettingContainer(gr_application_t *app, char *layer)
{
    BaseSportSettingLayer *baseSettingLayer = createSettingContainer(app, layer);

    baseSettingLayer->setChild(baseSettingLayer, &childSettingLayer);
    return baseSettingLayer;
}