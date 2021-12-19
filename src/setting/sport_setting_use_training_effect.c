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
#include "ui/Sports/include/setting/sport_setting_picker_screen.h"

#define SETTING_SCREEN              "SportSettingUseTrainingEffectScreen"       /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingUseTrainingEffectLayer"        /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_AEROBIC_TE,                                                           /**< 00' 有氧TE. */
    INDEX_ANAEROBIC_TE,                                                         /**< 01' 无氧TE. */
    INDEX_MAX,                                                                  /**< 02' ITEM数量. */
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
    case INDEX_AEROBIC_TE:
        switchPickerScreen(PICKER_ID_AEROBIC_TE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_ANAEROBIC_TE:
        switchPickerScreen(PICKER_ID_ANAEROBIC_TE, &ssc->rsi.set, ssc->change);
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 多选框事件
*
* @param  : index
* @return : None
******************************************************************************************************/
static void _radioSelectEvent(uint16_t index, SportTargetUseTrainingEffect *rawSet)
{
    switch(index)
    {
    case INDEX_AEROBIC_TE:
        rawSet->aerobicTE.onOff = true;
        break;

    case INDEX_ANAEROBIC_TE:
        rawSet->anaerobicTE.onOff = true;
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 取消选中事件
*
* @param  : index
* @return : None
******************************************************************************************************/
static void _radioDeselectEvent(uint16_t index, SportTargetUseTrainingEffect *rawSet)
{
    switch(index)
    {
    case INDEX_AEROBIC_TE:
        rawSet->aerobicTE.onOff = false;
        break;

    case INDEX_ANAEROBIC_TE:
        rawSet->anaerobicTE.onOff = false;
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 清除其他目标
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void disableOtherTarget(SportTarget *rawSet)
{
    rawSet->noGoals.onOff = false;
    rawSet->useWorkoutStatistics.distance.onOff = false;
    rawSet->useWorkoutStatistics.time.onOff = false;
    rawSet->useWorkoutStatistics.calories.onOff = false;
    rawSet->useWorkoutStatistics.count.onOff = false;
    rawSet->useWorkoutStatistics.trips.onOff = false;
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
    SportTargetUseTrainingEffect *rawSet = &ssc->rsi.set.assist.target.useTrainingEffect;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 文本点击事件 */
    if(event == LW_EVENT_TEXT) {
        _textEvent(ssc, imap[index]);
        return;
    }

    /* 选中事件 */
    if(lastIndex == -1)
        _radioSelectEvent(imap[index], rawSet);
    else {
        _radioSelectEvent(imap[index], rawSet);
        _radioDeselectEvent(imap[lastIndex], rawSet);
    }

    disableOtherTarget(&ssc->rsi.set.assist.target);
    ssc->change = true;
}

/*****************************************************************************************************
* @brief  : 设置单选文本
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_RadioText(lw_set_utils *lsu, uint16_t index, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet,
                                                                const char *text, char *val, int *select)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, "%.1f", rawSet->data);
        radioList_SetItem(item, text, val, true);

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
    char value[INDEX_MAX][SZ_SET_BUF] = {0};
    lw_item itemList[INDEX_MAX] = {0};

    /* 保存信息 */
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    SportTargetUseTrainingEffect *defSet = &ssc->rsi.def.assist.target.useTrainingEffect;
    SportTargetUseTrainingEffect *rawSet = &ssc->rsi.set.assist.target.useTrainingEffect;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->rlw = radioList_Create(SETTING_SCREEN, SETTING_LAYER, _rlwEvent);
    radioList_SetListAttr(context->rlw, LW_ATTR_TITLE, (void *)_ST(TK_TRAIN_EFFECT));
    radioList_SetListAttr(context->rlw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表 */
    setItem_RadioText(lsu, INDEX_AEROBIC_TE,   defSet->has_aerobicTE,   &defSet->aerobicTE,   &rawSet->aerobicTE,   _ST(TK_AEROBIC_TE),   value[i++], &select);
    setItem_RadioText(lsu, INDEX_ANAEROBIC_TE, defSet->has_anaerobicTE, &defSet->anaerobicTE, &rawSet->anaerobicTE, _ST(TK_ANAEROBIC_TE), value[i++], &select);

    (ssc->rsi.set.assist.target.noGoals.onOff == true) ? (select = -1) : (H_NULL);

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

const sport_setting_ops setting_ops_use_training_effect = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

