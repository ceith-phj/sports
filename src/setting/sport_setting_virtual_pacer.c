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

#define SETTING_SCREEN              "SportSettingVirtualPacerScreen"            /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingVirtualPacerLayer"             /**< 设置所在layer. */
typedef enum _list_item_index {
    INDEX_VP_STATE,                                                             /**< 00' 状态. */
    INDEX_VP_PACER,                                                             /**< 01' 配速. */
    INDEX_MAX,                                                                  /**< 03' ITEM数量. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 设置状态ITEM的状态(打开/关闭)
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void setStateItemState(list_widget *lw, bool state)
{
    listWidget_SetItemAttr(lw, INDEX_VP_PACER, LW_ITEM_ATTR_ITEM_STATE, (void *)state);

    /* 更新状态文本 */
    char *str = (state) ? TK_COM_ENABLE : TK_COM_DISABLE;
    listWidget_SetItemAttr(lw, INDEX_VP_STATE, LW_ITEM_ATTR_UPDATE_TEXT2, (void *)_ST(str));
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
    case INDEX_VP_PACER:
        switchPickerScreen(PICKER_ID_VIRTUAL_PACER, &ssc->rsi.set, ssc->change);
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 开关事件
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void _switchEvent(list_widget *lw, uint16_t index, SportSettingTypePicker *rawSet, bool state)
{
    switch(index)
    {
    case INDEX_VP_STATE:
        rawSet->onOff = state;
        setStateItemState(lw, state);
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
    SportSettingTypePicker *rawSet = &ssc->rsi.set.assist.virtualPacer;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    if(event == LW_EVENT_TEXT)
        _textEvent(ssc, imap[index]);
    else {
        _switchEvent(lw, imap[index], rawSet, state);
        ssc->change = true;
    }
}

/*****************************************************************************************************
* @brief  : 设置开关类的ITEM
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_State(lw_set_utils *lsu, uint16_t index, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet, const char *text)
{
    lw_item *item;
    char *str;

    item = lwUtils_InitItem(lsu, index, true, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        str = (rawSet->onOff) ? TK_COM_ENABLE : TK_COM_DISABLE;
        listWidget_SetItemText(item, text, _ST(str), true);
        listWidget_SetItemIcon(item, LW_ITEM_SWITCH_TEXT, rawSet->onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置we类的ITEM
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Pacer(lw_set_utils *lsu, uint16_t index, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet, const char *text, char *val)
{
    lw_item *item;
    uint16_t raw, min, sec;

    item = lwUtils_InitItem(lsu, index, true, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        raw = rawSet->data;
        min = raw / 60;
        sec = raw % 60;
        snprintf(val, SZ_SET_BUF, _ST(TK_ARG_MIN_SEC), min, sec);
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
    SportSettingTypePicker *defSet = &ssc->rsi.def.assist.virtualPacer;
    SportSettingTypePicker *rawSet = &ssc->rsi.set.assist.virtualPacer;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TK_VIRTUAL_PACER));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表 */
    setItem_State(lsu, INDEX_VP_STATE, defSet, rawSet, _ST(TK_VP_STATE));
    setItem_Pacer(lsu, INDEX_VP_PACER, defSet, rawSet, _ST(TK_VP_PACER), value);

    /* 创建列表 */
    listWidget_SetList(context->lw, itemList, lwUtils_GetMapSize(lsu));

    /* 设置是否置灰 */
    setStateItemState(context->lw, rawSet->onOff);
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

const sport_setting_ops setting_ops_virtual_pacer = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};
