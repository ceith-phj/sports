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

#define SETTING_SCREEN              "SportSettingAutoLapScreen"                 /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingAutoLapLayer"                  /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_AUTO_LAP,                                                             /**< 00' 自动分段. */
    INDEX_AUTO_LAP_DISTANCE,                                                    /**< 01' 自动分段距离. */
    INDEX_MAX,                                                                  /**< 02' ITEM数量. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 切换分段选择器
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void switchLapPicker(sport_setting_container *ssc)
{
    picker_id pickerID;
    sport_type_t sport = sportManager()->ctrlManager()->getType();
    sport_group_t group = sportGroup_get(sport);

    if(groupIsMatch(KSY_SGG_RUN_WALK_OPEN_WATER))
        pickerID = PICKER_ID_AUTO_LAP_RUN;
    else if(group == SPORT_GROUP_POOL_SWIMMING)
        pickerID = PICKER_ID_AUTO_LAP_SWIM;
    else
        pickerID = PICKER_ID_AUTO_LAP_OTHER;

    switchPickerScreen(pickerID, &ssc->rsi.set, ssc->change);
}

/*****************************************************************************************************
* @brief  : 文本事件
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void _textEvent(sport_setting_container *ssc, uint16_t index)
{
    switch(index)
    {
    case INDEX_AUTO_LAP_DISTANCE:
        switchLapPicker(ssc);
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 设置自动分段事件
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void setAutoLapMemberState(list_widget *lw, bool state)
{
    listWidget_SetItemAttr(lw, INDEX_AUTO_LAP_DISTANCE, LW_ITEM_ATTR_ITEM_STATE, (void *)state);

    /* 更新状态文本 */
    char *str = (state) ? TK_COM_ENABLE : TK_COM_DISABLE;
    listWidget_SetItemAttr(lw, INDEX_AUTO_LAP, LW_ITEM_ATTR_UPDATE_TEXT2, (void *)_ST(str));
}

/*****************************************************************************************************
* @brief  : 开关事件
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void _switchEvent(list_widget *lw, uint16_t index, SportAssistAutoLap *rawSet, bool state)
{
    switch(index)
    {
    case INDEX_AUTO_LAP:
        rawSet->onOff.onOff = state;
        setAutoLapMemberState(lw, state);
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
static void _lwEvent(list_widget *lw, uint16_t index, lw_event_type event, bool state)
{
    /* 设置指针 */
    sport_setting_container *ssc = lw->attr.param;
    list_setting_context *context = (list_setting_context *)&ssc->context;
    SportAssistAutoLap *rawSet = &ssc->rsi.set.assist.autoLap;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    if(event == LW_EVENT_TEXT)
        _textEvent(ssc, imap[index]);
    else {
        _switchEvent(lw, index, rawSet, state);
        ssc->change = true;
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - 分段
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_AutoLap(lw_set_utils *lsu, bool exist, SportSettingTypeOnOff *defSet, SportSettingTypeOnOff *rawSet, const char *text)
{
    char *str;
    lw_item *item = lwUtils_InitItem(lsu, INDEX_AUTO_LAP, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);

    if(item != NULL) {
        str = (rawSet->onOff) ? TK_COM_ENABLE : TK_COM_DISABLE;
        listWidget_SetItemText(item, text, str, false);
        listWidget_SetItemIcon(item, LW_ITEM_SWITCH_TEXT, rawSet->onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - 距离
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Distance(lw_set_utils *lsu, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet, const char *text, char *val)
{
    lw_item *item = lwUtils_InitItem(lsu, INDEX_AUTO_LAP_DISTANCE, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, choiceUnitText(_ST(TK_ARG_KM1), _ST(TK_ARG_MILE1)), rawSet->data);
        listWidget_SetItemText(item, text, val, true);
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - 趟数
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Trips(lw_set_utils *lsu, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet, const char *text, char *val)
{
    lw_item *item = lwUtils_InitItem(lsu, INDEX_AUTO_LAP_DISTANCE, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, _ST(TK_ARG_PER_TRIP), (int32_t)rawSet->data);
        listWidget_SetItemText(item, text, val, true);
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
    char value[SZ_SET_BUF] = {0};
    lw_item itemList[INDEX_MAX] = {0};

    /* 保存信息 */
    list_setting_context *context = (list_setting_context *)&ssc->context;
    SportAssistAutoLap *defSet = &ssc->rsi.def.assist.autoLap;
    SportAssistAutoLap *rawSet = &ssc->rsi.set.assist.autoLap;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TK_AUTO_LAP));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置 - 分段 */
    setItem_AutoLap(lsu,  rawSet->has_onOff, &defSet->onOff, &rawSet->onOff, _ST(TK_COM_STATE));

    /* 设置趟数/距离 */
    if(isSportGroup(SPORT_GROUP_POOL_SWIMMING))
        setItem_Trips(lsu, rawSet->has_distance, &defSet->distance, &rawSet->distance, _ST(TK_AUTO_LAP_TRIPS), value);
    else
        setItem_Distance(lsu, rawSet->has_distance, &defSet->distance, &rawSet->distance, _ST(TK_AUTO_LAP_DISTANCE), value);

    /* 创建列表 */
    listWidget_SetList(context->lw, itemList, lwUtils_GetMapSize(lsu));

    /* 设置是否置灰 */
    setAutoLapMemberState(context->lw, rawSet->onOff.onOff);
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

const sport_setting_ops setting_ops_auto_lap = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

