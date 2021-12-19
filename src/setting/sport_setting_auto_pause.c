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
#include "sports/include/manager/sports_control_manager.h"

#define SETTING_SCREEN              "SportSettingAutoPauseScreen"               /**< screen 名称. */
#define SETTING_LAYER               "SportSettingAutoPauseLayer"                /**< layer 名. */

typedef enum _list_item_index {
    INDEX_STANDING,                                                             /**< 00' 静止. */
    INDEX_PACE_SPEED,                                                           /**< 01' 按配速/速度. */
    INDEX_CLOSE,                                                                /**< 02' 关闭. */
    INDEX_MAX,                                                                  /**< 03' ITEM数量. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 设置配速和速度
*
* @param  : index
* @return : None
******************************************************************************************************/
static void setPaceSpeed(SportAssistAutoPause *rawSet, bool state)
{
    if(groupIsMatch(KEY_SGG_PACE))
        rawSet->byPace.onOff = state;
    else
        rawSet->bySpeed.onOff = state;
}

/*****************************************************************************************************
* @brief  : 切换到速度或配速的选择器
*
* @param  : index
* @return : None
******************************************************************************************************/
static void switchPaceSpeedPicker(sport_setting_container *ssc)
{
    if(groupIsMatch(KEY_SGG_PACE))
        switchPickerScreen(PICKER_ID_PAUSE_PACE, &ssc->rsi.set, ssc->change);
    else
        switchPickerScreen(PICKER_ID_PAUSE_SPEED, &ssc->rsi.set, ssc->change);
}

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
    case INDEX_PACE_SPEED:
        switchPaceSpeedPicker(ssc);
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
static void _radioSelectEvent(uint16_t index, SportAssistAutoPause *rawSet)
{
    switch(index)
    {
    case INDEX_STANDING:
        rawSet->standing.onOff = true;
        break;

    case INDEX_PACE_SPEED:
        setPaceSpeed(rawSet, true);
        break;

    case INDEX_CLOSE:
        rawSet->onOff.onOff = true;
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
static void _radioDeselectEvent(uint16_t index, SportAssistAutoPause *rawSet)
{
    switch(index)
    {
    case INDEX_STANDING:
        rawSet->standing.onOff = false;
        break;

    case INDEX_PACE_SPEED:
        setPaceSpeed(rawSet, false);
        break;

    case INDEX_CLOSE:
        rawSet->onOff.onOff = false;
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
    SportAssistAutoPause *rawSet = &ssc->rsi.set.assist.autoPause;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 文本点击事件 */
    if(event == LW_EVENT_TEXT) {
        _textEvent(ssc, imap[index]);
        return;
    }

    /* 切换事件 */
    _radioSelectEvent(imap[index], rawSet);
    _radioDeselectEvent(imap[lastIndex], rawSet);
    ssc->change = true;
}

/*****************************************************************************************************
* @brief  : 设置ITEM - 静止
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Standing(lw_set_utils *lsu, bool exist, SportSettingTypeOnOff *defSet, SportSettingTypeOnOff *rawSet, int *select)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, INDEX_STANDING, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        radioList_SetItem(item, _ST(TK_STANDING), NULL, false);
        (rawSet->onOff) ? (*select = INDEX_STANDING) : (H_NULL);
    }
}

/*****************************************************************************************************
* @brief  : 设置配速/速度
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_PaceSpeed(lw_set_utils *lsu, SportAssistAutoPause *defSet, SportAssistAutoPause *rawSet, char *val, int *select)
{
    lw_item *item;
    uint16_t raw, hour, min;

    /* 按配速 */
    if(groupIsMatch(KEY_SGG_PACE))
    {
        item = lwUtils_InitItem(lsu, INDEX_PACE_SPEED, defSet->has_byPace, defSet->byPace.has_onlyPreStart & defSet->byPace.onlyPreStart);
        if(item != NULL) {
            raw = rawSet->byPace.data;
            hour = raw / 60;
            min =  raw % 60;
            snprintf(val, SZ_SET_BUF, _ST(TK_ARG_BELOW_PACE), hour, min);
            radioList_SetItem(item, _ST(TK_BY_PACE), val, true);
            (rawSet->byPace.onOff) ? (*select = INDEX_PACE_SPEED) : (H_NULL);
        }
    }
    else
    {
        item = lwUtils_InitItem(lsu, INDEX_PACE_SPEED, defSet->has_bySpeed, defSet->bySpeed.has_onlyPreStart & defSet->bySpeed.onlyPreStart);
        if(item != NULL) {
            snprintf(val, SZ_SET_BUF, choiceUnitText(_ST(TK_ARG_BELOW_SPEED), _ST(TK_ARG_BELOW_SPEED_MILE)), (int32_t)rawSet->bySpeed.data);
            radioList_SetItem(item, _ST(TK_BY_SPEED), val, true);
            (rawSet->bySpeed.onOff) ? (*select = INDEX_PACE_SPEED) : (H_NULL);
        }
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - 关闭
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Close(lw_set_utils *lsu, bool exist, SportSettingTypeOnOff *defSet, SportSettingTypeOnOff *rawSet, int *select)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, INDEX_CLOSE, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        radioList_SetItem(item, _ST(TK_CLOSE), NULL, false);
        (rawSet->onOff) ? (*select = INDEX_CLOSE) : (H_NULL);
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
    int select = -1;
    char value[SZ_SET_BUF] = {0};
    lw_item itemList[INDEX_MAX] = {0};

    /* 保存信息 */
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    SportAssistAutoPause *defSet = &ssc->rsi.def.assist.autoPause;
    SportAssistAutoPause *rawSet = &ssc->rsi.set.assist.autoPause;
    lw_set_utils *lsu = &context->lsu;


    /* 创建列表控件 */
    context->rlw = radioList_Create(SETTING_SCREEN, SETTING_LAYER, _rlwEvent);
    radioList_SetListAttr(context->rlw, LW_ATTR_TITLE, (void *)_ST(TK_AUTO_PAUSE));
    radioList_SetListAttr(context->rlw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表 */
    setItem_Standing(lsu, defSet->has_standing, &defSet->standing, &rawSet->standing, &select);
    setItem_PaceSpeed(lsu, defSet, rawSet, value, &select);
    setItem_Close(lsu, defSet->has_onOff, &defSet->onOff, &rawSet->onOff, &select);

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
    SportContrlService_getOps()->notifySettingChange(SPT_SETTING_AUTO_PAUSE);
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

const sport_setting_ops setting_ops_auto_pause = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

