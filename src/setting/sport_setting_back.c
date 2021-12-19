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
#include "sports/include/manager/sports_manager.h"

#define SETTING_SCREEN              "SportSettingBackScreen"                  /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingBackLayer"                   /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_TARGET_TIME,                                                          /**< 00' 时间. */
    INDEX_TARGET_DISTANCE,                                                      /**< 01' 距离. */
    INDEX_TARGET_CALORIES,                                                      /**< 02' 消耗. */
    INDEX_TARGET_COUNT,                                                         /**< 03' 计数. */
    INDEX_TARGET_TRIPS,                                                         /**< 04' 趟数. */
    INDEX_MAX,                                                                  /**< 05' 最大索引. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 文本事件
*
* @param  : index
* @return : None
******************************************************************************************************/
static void _textEvent(sport_setting_container *ssc, uint16_t index)
{
    switch(index)
    {
    case INDEX_TARGET_TIME:
        switchPickerScreen(PICKER_ID_REMINDER_BACK_TIME, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_TARGET_DISTANCE:
        switchPickerScreen(PICKER_ID_REMINDER_BACK_DISTANCE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_TARGET_CALORIES:
        break;

    case INDEX_TARGET_COUNT:
        break;

    case INDEX_TARGET_TRIPS:
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 文本事件
*
* @param  : index
* @return : None
******************************************************************************************************/
static void _radioBoxEvent(uint16_t index, bool state, SportSettingUseWorkoutStatistics *rawSet)
{
    switch(index)
    {
    case INDEX_TARGET_TIME:
        rawSet->time.onOff = state;
        break;        

    case INDEX_TARGET_DISTANCE:
        rawSet->distance.onOff = state;
        break;

    case INDEX_TARGET_CALORIES:
        rawSet->calories.onOff = state;
        break;

    case INDEX_TARGET_COUNT:
        rawSet->count.onOff = state;
        break;

    case INDEX_TARGET_TRIPS:
        rawSet->trips.onOff = state;
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 列表控件的事件
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void _rlwEvent(radio_list_widget *rlw, lw_event_type event, int index, int lastIndex)
{
    /* 设置指针 */
    sport_setting_container *ssc = rlw->param;
    list_setting_context *context = (list_setting_context *)&ssc->context;
    SportSettingUseWorkoutStatistics *rawSet = &ssc->rsi.set.assist.reminder.backTo.back;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 文本点击事件 */
    if(event == LW_EVENT_TEXT) {
        _textEvent(ssc, imap[index]);
        return;
    }

    /* 选中事件 */
    if(lastIndex == -1)
        _radioBoxEvent(imap[index], true, rawSet);
    else {
        _radioBoxEvent(imap[index], true, rawSet);
        _radioBoxEvent(imap[lastIndex], false, rawSet);
    }

    /* 设置选中值 */
    ssc->rsi.set.assist.reminder.backTo.choice = imap[index];
    ssc->change = true;
}

/*****************************************************************************************************
* @brief  : Picker类型
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_RadioText(lw_set_utils *lsu, uint16_t index, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet,
                                                const char *text1, const char *text2, int *select)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        radioList_SetItem(item, text1, text2, true);
        (rawSet->onOff) ? (*select = index) : (H_NULL);
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
    uint8_t i = 0;
    int select = -1;
    const char *unit;
    char value[INDEX_MAX][SZ_SET_BUF] = {0};
    lw_item itemList[INDEX_MAX] = {0};
    
    /* 保存信息 */
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    SportSettingUseWorkoutStatistics *defSet = &ssc->rsi.set.assist.reminder.backTo.back;
    SportSettingUseWorkoutStatistics *rawSet = &ssc->rsi.set.assist.reminder.backTo.back;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->rlw = radioList_Create(SETTING_SCREEN, SETTING_LAYER, _rlwEvent);
    radioList_SetListAttr(context->rlw, LW_ATTR_TITLE, (void *)_ST(TK_BACK));
    radioList_SetListAttr(context->rlw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置 - 用时 */
    snprintf(value[i], SZ_SET_BUF, _ST(TK_ARG_HOUR_MIN), (int)(rawSet->time.data)/60, (int)rawSet->time.data % 60);
    setItem_RadioText(lsu, INDEX_TARGET_TIME, defSet->has_time, &defSet->time, &rawSet->time, _ST(TK_BY_TIME), value[i++], &select);

    /* 设置 - 距离 */
    unit = choiceUnitText(_ST(TK_ARG_KM), _ST(TK_ARG_MILE));
    snprintf(value[i], SZ_SET_BUF, unit, (int32_t)rawSet->distance.data);
    setItem_RadioText(lsu, INDEX_TARGET_DISTANCE, defSet->has_distance, &defSet->distance, &rawSet->distance, _ST(TK_BY_DISTANCE), value[i++], &select);

    /* 设置 - 消耗 */
    snprintf(value[i], SZ_SET_BUF, _ST(TK_ARG_KCAL), rawSet->calories.data);
    setItem_RadioText(lsu, INDEX_TARGET_CALORIES, defSet->has_calories, &defSet->calories, &rawSet->calories, _ST(TK_BY_KCAL), value[i++], &select);

    /* 设置 - 计数 */
    snprintf(value[i], SZ_SET_BUF, _ST(TK_ARG_COUNT), (int32_t)rawSet->count.data);
    setItem_RadioText(lsu, INDEX_TARGET_COUNT, defSet->has_count, &defSet->count, &rawSet->count, _ST(TK_BY_COUNT), value[i++], &select);

    /* 设置 - 趟数 */
    snprintf(value[i], SZ_SET_BUF, _ST(TK_ARG_TRIPS), (int32_t)rawSet->trips.data);
    setItem_RadioText(lsu, INDEX_TARGET_TRIPS, defSet->has_trips, &defSet->trips, &rawSet->trips, _ST(TK_BY_TRIPS), value[i++], &select);

    /* 设置默认选中项 */
    select = ssc->rsi.set.assist.reminder.backTo.choice;

    /* 创建列表 */
    radioList_SetList(context->rlw, itemList, lwUtils_GetMapSize(lsu), lwUtils_MapIndex(lsu, select));
    addContainer(ssc->sc, SETTING_LAYER, context->rlw);
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
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    return radioList_HandleEvent(context->rlw, &ssc->event);
}

const sport_setting_ops setting_ops_back = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

