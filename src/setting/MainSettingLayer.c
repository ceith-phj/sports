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
#include <setting/MainSettingLayer.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
typedef enum SPORTSETTING_ELEMENT_TYPE
{
    SPORTSETTING_ELEMENT_ASSISTANT = 0,
    SPORTSETTING_ELEMENT_TRAINING,
    SPORTSETTING_ELEMENT_MORE,
    NUM_OF_SPORTSETTING_ELEMENT
} SPORTSETTING_ELEMENT_TYPE;

static SportSettingItem settingItems[NUM_OF_SPORTSETTING_ELEMENT] = {
    {SPORTSETTING_ELEMENT_ASSISTANT, "translations.id_sport_setting_assistant", "images/sports/setting/sport_setting_icon_assistant.png", MAIN_ITEM},
    {SPORTSETTING_ELEMENT_TRAINING, "translations.id_sport_setting_remind", "images/sports/setting/sport_setting_icon_traning.png", MAIN_ITEM},
    {SPORTSETTING_ELEMENT_MORE, "translations.id_sport_setting_target", "images/sports/setting/sport_setting_icon_more.png", MAIN_ITEM}};

static bool isSettingItemEnable(SportSettingLayerImpl *child, H_Uint8 type)
{
    switch (type)
    {
    case SPORTSETTING_ELEMENT_ASSISTANT:
        return child->parent->setting->has_assist;
    case SPORTSETTING_ELEMENT_TRAINING:
        return child->parent->setting->has_training;
    case SPORTSETTING_ELEMENT_MORE:
        return child->parent->setting->has_more;
    default:
        return false;
    }
    return false;
}

static SportSettingItem* getDefaultSettingItem(void){
    return settingItems;
}

static H_Int8 getDefaultSettingItemSize(void) {
    return NUM_OF_SPORTSETTING_ELEMENT;
}

static void initData(SportSettingLayerImpl *child)
{
    HLOG_D("jude", "ChildSettingLayer1111111111111111 awesome!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
}

static char *getTitleId()
{
    return "translations.id_sport_setting_title_main";
}

static char *getContainerId()
{
    return "Setting.MainList";
}

static void onItemClick(SportSettingLayerImpl *child, H_Uint8 type, SportSettingItem *item)
{
    switch (type)
    {
    case SPORTSETTING_ELEMENT_ASSISTANT:
        getScreenManager()->startScreen("SportSettingAssistantScreen", H_NULL, 0);
        break;
    case SPORTSETTING_ELEMENT_TRAINING:
        getScreenManager()->startScreen("SportSettingTrainScreen", H_NULL, 0);
        break;
    case SPORTSETTING_ELEMENT_MORE:
        getScreenManager()->startScreen("SportSettingMoreScreen", H_NULL, 0);
        break;

    default:
        break;
    }
}

static void onItemUpdate(SportSettingLayerImpl *child, H_Uint8 type, SportSettingItem *item)
{

}

static void onItemCheckChanged(SportSettingLayerImpl *child, H_Uint8 type, bool isChecked, SportSettingItem *item)
{
}

static SportSettingLayerImpl childSettingLayer = {
    .initData = initData,
    .getTitleId = getTitleId,
    .getContainerId = getContainerId,
    .onItemClick = onItemClick,
    .onItemUpdate = onItemUpdate,
    .onItemCheckChanged = onItemCheckChanged,
    .isSettingItemEnable = isSettingItemEnable,
    .getDefaultSettingItem = getDefaultSettingItem,
    .getDefaultSettingItemSize = getDefaultSettingItemSize,
};

BaseSportSettingLayer *createMainSettingContainer(gr_application_t *app, char *layer)
{
    BaseSportSettingLayer *baseSettingLayer = createSettingContainer(app, layer);

    baseSettingLayer->setChild(baseSettingLayer, &childSettingLayer);
    (&childSettingLayer)->parent = baseSettingLayer;
    return baseSettingLayer;
}