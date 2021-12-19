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
#include <widget/list/list_widget.h>
#include "ui/Sports/include/setting/sport_setting_screen.h"

#define SETTING_SCREEN              "SportSettingGoalScreen"                    /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingGoaLayer"                      /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_TRAIN_EFFECT,                                                         /**< 00' 运动目标. */
    INDEX_STATISTICS,                                                           /**< 01' 提醒. */
    INDEX_NO_GOALS,                                                             /**< 02' 自动分段. */
    INDEX_MAX,                                                                  /**< 03' ITEM数量. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 清除其他目标
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void noGoalEvent(sport_setting_container *ssc, SportTarget *rawSet)
{

    rawSet->noGoals.onOff = true;

    rawSet->useTrainingEffect.aerobicTE.onOff = false;
    rawSet->useTrainingEffect.anaerobicTE.onOff = false;

    rawSet->useWorkoutStatistics.distance.onOff = false;
    rawSet->useWorkoutStatistics.time.onOff = false;
    rawSet->useWorkoutStatistics.calories.onOff = false;
    rawSet->useWorkoutStatistics.count.onOff = false;
    rawSet->useWorkoutStatistics.trips.onOff = false;

    ssc->change = true;
    switchSettingScreen(NULL, &ssc->rsi.set, ssc->change);
}

/*****************************************************************************************************
* @brief  : 列表控件的事件
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void _lwEvent(list_widget *lw, uint16_t index, lw_event_type event, bool state)
{
    /* 初始化使用指针 */
    sport_setting_container *ssc = lw->attr.param;
    list_setting_context *context = (list_setting_context *)&ssc->context;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);
    SportTarget *rawSet = &ssc->rsi.set.assist.target;

    /* 判断事件 */
    switch(imap[index])
    {
    case INDEX_TRAIN_EFFECT:
        switchSettingScreen("SportSettingUseTrainingEffectScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_STATISTICS:
        switchSettingScreen("SportSettingUseWorkoutStatisticScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_NO_GOALS:
        noGoalEvent(ssc, rawSet);
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 设置Assistant文本项
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Text(lw_set_utils *lsu, uint16_t index, bool exist, bool preStartOnly, const char *text, bool arrow)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, preStartOnly);
    if(item != NULL) {
        listWidget_SetItemText(item, text, NULL, arrow);
    }
}

/*****************************************************************************************************
* @brief  : 设置页面创建
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void _settingCreate(sport_setting_container *ssc)
{
    lw_item itemList[INDEX_MAX] = {0};

    /* 初始化使用指针 */
    list_setting_context *context = (list_setting_context *)&ssc->context;
    SportTarget *defSet = &ssc->rsi.def.assist.target;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TK_GOAL));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表项, 是否运动前独有统一按不使用目标处理 */
    setItem_Text(lsu, INDEX_TRAIN_EFFECT, defSet->has_useTrainingEffect,    defSet->noGoals.has_onlyPreStart, _ST(TK_TRAIN_EFFECT), true);
    setItem_Text(lsu, INDEX_STATISTICS,   defSet->has_useWorkoutStatistics, defSet->noGoals.has_onlyPreStart, _ST(TK_STATISTICS),   true);
    setItem_Text(lsu, INDEX_NO_GOALS,     defSet->has_noGoals,              defSet->noGoals.has_onlyPreStart, _ST(TK_NO_GOALS),     false);

    /* 创建列表 */
    listWidget_SetList(context->lw, itemList, lwUtils_GetMapSize(lsu));
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

const sport_setting_ops setting_ops_goal = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

