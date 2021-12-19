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
#include "device_info.h"

#define SETTING_SCREEN              "SportSettingCadenceAssistantScreen"        /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingCadenceAssistantLayer"         /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_STATE,                                                                /**< 00' 状态. */
    INDEX_CADENCE,                                                              /**< 01' 步/分钟. */
    INDEX_REMINDER_FREQ,                                                        /**< 02' 提示频率. */
    INDEX_REMINDER_MODE,                                                        /**< 03' 提示方式. */
    INDEX_PREVIEW,                                                              /**< 04' 预览. */
    INDEX_MAX,                                                                  /**< 05' ITEM数量. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 设置状态ITEM的状态(打开/关闭)
*
* @param  : index
* @param  : 事件类型
* @param  : 状态
* @return : None
******************************************************************************************************/
static void setMemeberState(list_widget *lw, SportAssistCandece *rawSet, bool state)
{
    if(rawSet->has_cadence)
        listWidget_SetItemAttr(lw, INDEX_CADENCE, LW_ITEM_ATTR_ITEM_STATE, (void *)state);

    if(rawSet->has_reminderFrequence)
        listWidget_SetItemAttr(lw, INDEX_REMINDER_FREQ, LW_ITEM_ATTR_ITEM_STATE, (void *)state);

    if(rawSet->has_remindMode)
        listWidget_SetItemAttr(lw, INDEX_REMINDER_MODE, LW_ITEM_ATTR_ITEM_STATE, (void *)state);

    if(rawSet->has_preview)
        listWidget_SetItemAttr(lw, INDEX_PREVIEW, LW_ITEM_ATTR_ITEM_STATE, (void *)state);

    /* 更新状态文本 */
    char *str = (state) ? TK_COM_ENABLE : TK_COM_DISABLE;
    listWidget_SetItemAttr(lw, INDEX_STATE, LW_ITEM_ATTR_UPDATE_TEXT2, (void *)_ST(str));
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
    case INDEX_CADENCE:
        switchPickerScreen(PICKER_ID_CADENCE_STEP_FREQ, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_REMINDER_FREQ:
        switchPickerScreen(PICKER_ID_CADENCE_REMINDER_FREQ, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_REMINDER_MODE:
        switchSettingScreen("SportSettingReminderModeScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_PREVIEW:
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
static void _switchEvent(list_widget *lw, uint16_t index, SportAssistCandece *rawSet, bool state)
{
    switch(index)
    {
    case INDEX_STATE:
        rawSet->onOff = state;
        setMemeberState(lw, rawSet, state);
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
    SportAssistCandece *rawSet = &ssc->rsi.set.assist.cadenceAssistant;
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
static void setItem_State(lw_set_utils *lsu, bool exist, bool state)
{
    lw_item *item;
    char *str;

    item = lwUtils_InitItem(lsu, INDEX_STATE, exist, false);
    if(item != NULL) {
        str = (state) ? TK_COM_ENABLE : TK_COM_DISABLE;
        listWidget_SetItemText(item, _ST(TK_COM_STATE), _ST(str), true);
        listWidget_SetItemIcon(item, LW_ITEM_SWITCH_TEXT, state);
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - CADENCE
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Cadence(lw_set_utils *lsu, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet, char *val)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, INDEX_CADENCE, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, "%d", (int)rawSet->data);
        listWidget_SetItemText(item, _ST(TK_CADENCE), val, true);
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - FREQ
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Freq(lw_set_utils *lsu, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet, char *val)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, INDEX_REMINDER_FREQ, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, _ST(TK_ARG_PER_STRIDE), (int32_t)rawSet->data);
        listWidget_SetItemText(item, _ST(TK_REMINDER_FREQ), val, true);
    }
}

/*****************************************************************************************************
* @brief  : 兼容提醒模式, 
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool compatibleReminderMode(SportSettingTypeChoice *rawSet, sport_setting_container *ssc)
{
    if(device_hasSpeaker())
        return true;

    if(rawSet->choice != 1) {
        rawSet->choice = 1;
        ssc->change = true;
    }

    return false;
}

/*****************************************************************************************************
* @brief  : 设置ITEM - MODE
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Mode(lw_set_utils *lsu, bool exist, SportSettingTypeChoice *defSet, SportSettingTypeChoice *rawSet, sport_setting_container *ssc)
{
    lw_item *item;
    char *val;

    /* 兼容性判断 */
    if(!compatibleReminderMode(rawSet, ssc))
        return;

    item = lwUtils_InitItem(lsu, INDEX_REMINDER_MODE, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        (rawSet->choice == 1) ? (val = TK_COM_VIBRATE) : (H_NULL);
        (rawSet->choice == 2) ? (val = TK_COM_VOLUME)  : (H_NULL);
        (rawSet->choice == 3) ? (val = TK_COM_VIB_VOL) : (H_NULL);
        listWidget_SetItemText(item, _ST(TK_REMINDER_MODE), _ST(val), true);
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - PREVIEW
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Preview(lw_set_utils *lsu, bool exist)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, INDEX_PREVIEW, exist, false);
    if(item != NULL) {        
        listWidget_SetItemText(item, _ST(TK_PREVIEW), NULL, true);
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
    char value[2][SZ_SET_BUF] = {0};
    lw_item itemList[INDEX_MAX] = {0};

    /* 保存信息 */
    list_setting_context *context = (list_setting_context *)&ssc->context;
    SportAssistCandece *defSet = &ssc->rsi.def.assist.cadenceAssistant;
    SportAssistCandece *rawSet = &ssc->rsi.set.assist.cadenceAssistant;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TK_CADENCE_ASSISTANT));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表 */
    setItem_State(lsu, defSet->has_onOff, rawSet->onOff);
    setItem_Cadence(lsu, defSet->has_cadence, &defSet->cadence, &rawSet->cadence, value[0]);
    setItem_Freq(lsu, defSet->has_reminderFrequence, &defSet->reminderFrequence, &rawSet->reminderFrequence, value[1]);
    setItem_Mode(lsu, defSet->has_remindMode, &defSet->remindMode, &rawSet->remindMode, ssc);
    setItem_Preview(lsu, defSet->has_preview & defSet->preview);

    /* 创建列表 */
    listWidget_SetList(context->lw, itemList, lwUtils_GetMapSize(lsu));

    /* 设置是否置灰 */
    setMemeberState(context->lw, rawSet, rawSet->onOff);
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

const sport_setting_ops setting_ops_cadence_assistant = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};
