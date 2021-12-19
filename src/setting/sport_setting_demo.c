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

#include <screen_manager.h>
#include <sb_screen_context.h>
#include "ui/Sports/include/setting/sport_setting_screen.h"

#define SETTING_SCREEN              "SportSettingReminderScreen"                /**< screen 名称. */
#define SETTING_LAYER               "SportSettingReminderLayer"                 /**< layer 名称. */
#define SETTING_ITEM_MAX            9                                           /**< 本业设置项最大数量. */

/* 文本 ID */
#define TEXT_ID_TITLE               "id_spset_exercise_assistance"              /**< 标题. */
#define TEXT_ID_GOAL                "id_spset_workout_goal"                     /**< 运动目标. */
#define TEXT_ID_REMINDER            "id_spset_workout_reminder"                 /**< 运动提醒. */
#define TEXT_ID_AUTO_ALP            "id_spset_auto_lap"                         /**< 自动分段. */
#define TEXT_ID_AUTO_PAUSE          "id_spset_auto_pause"                       /**< 自动暂停. */
#define TEXT_ID_VIRTUAL_PACER       "id_spset_virtual_pacer"                    /**< 虚拟兔子. */
#define TEXT_ID_CADENCE_ASSISTANT   "id_spset_cadence_assistant"                /**< 步频助手. */
#define TEXT_ID_PACE_STRATEGY       "id_spset_pace_strategy"                    /**< 配速策略. */
#define TEXT_ID_SPORT_COMPASS       "id_spset_compass"                          /**< 指南针. */
#define TEXT_ID_SPO2                "id_spset_spo2"                             /**< 血氧饱和度. */

typedef enum _list_item_index {
    INDEX_GOAL,                                                                 /**< 00' 运动目标. */
    INDEX_REMINDER,                                                             /**< 01' 提醒. */
    INDEX_AUTO_LAP,                                                             /**< 02' 自动分段. */
    INDEX_AUTO_PAUSE,                                                           /**< 03' 自动暂停. */
    INDEX_VIRTUAL_PACER,                                                        /**< 04' 虚拟兔子. */
    INDEX_CADENCE_ASSISTANT,                                                    /**< 05' 步频助手. */
    INDEX_PACE_STRATEGY,                                                        /**< 06' 配速策略. */
    INDEX_SPORT_COMPASS,                                                        /**< 07' 指南针. */
    INDEX_SPO2,                                                                 /**< 08' SPO2. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 列表控件的事件
*
* @param  : 屏幕
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void _lwEvent(list_widget *lw, uint16_t index, lw_event_type event, bool state)
{
#if 0
    /* 设置指针 */
    sport_setting_container *ssc = screen->_appData;
    list_setting_context *context = (list_setting_context *)&ssc->context;
    AssistMessage *rawSet = &ssc->ssm->assist;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 判断事件 */
    switch(imap[index])
    {
    case INDEX_GOAL:
  //      getScreenManager()->startScreen("SportSettingGoalScreen", H_NULL, 0);
        break;

    case INDEX_REMINDER:
  //      getScreenManager()->startScreen("SportSettingReminderScreen", H_NULL, 0);
        break;

    case INDEX_SPORT_COMPASS:
        rawSet->compass.onOff = state;
        ssc->change = true;
        break;

    case INDEX_SPO2:
        rawSet->spO2.onOff = state;
        ssc->change = true;
        break;
    }
#endif
}

/*****************************************************************************************************
* @brief  : 设置页面创建
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void _settingCreate(sport_setting_container *ssc)
{
    uint8_t i = 0;
    lw_item itemList[50];
    lw_item *item;

    /* 设置指针 */
    list_setting_context *context = (list_setting_context *)&ssc->context;

    printf("item size = %d, total = %d\n", sizeof(lw_item), sizeof(lw_item) * 50);

    /* 创建列表控件 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TEXT_ID_TITLE));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);

    for(i = 0 ; i < 4 ; i++)
    {
        uint8_t j = i * 12;
    
        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text no arrow", NULL, false);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text with arrow", NULL, true);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text double line no arrow", NULL, false);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text double line with arrw", NULL, true);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text third line with arrw word word word word", NULL, false);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text third line with arrw word word word word", NULL, true);
        listWidget_SetItemIcon(item, i, false);

        /* 双行 */
        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text no arrow", "Single text no arrow", false);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text with arrow", "Single text with arrow", true);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text double line no arrow", "Single text double line no arrow", false);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text double line with arrw", "Single text double line with arrw", true);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text third line with arrw word word word word", "Single text third line with arrw word word word word", false);
        listWidget_SetItemIcon(item, i, false);

        item = &itemList[j++];
        listWidget_SetItemText(item, "Single text third line with arrw word word word word", "Single text third line with arrw word word word word", true);
        listWidget_SetItemIcon(item, i, false);

    }

    /* 创建列表 */
    listWidget_SetList(context->lw, itemList, 47);
    addContainer(ssc->sc, SETTING_LAYER, context->lw);
}

/*****************************************************************************************************
* @brief  : 设置页面销毁
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void _settingDestory(sport_setting_container *ssc)
{
}

/*****************************************************************************************************
* @brief  : 设置页面事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static bool _settingEvent(sport_setting_container *ssc)
{
    list_setting_context *context = (list_setting_context *)&ssc->context;
    return listWidget_HandleEvent(context->lw, &ssc->event);
}

const sport_setting_ops setting_ops_demo = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

