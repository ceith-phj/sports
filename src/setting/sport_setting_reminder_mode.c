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

#define SETTING_SCREEN              "SportSettingReminderModeScreen"            /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingReminderModeLayer"             /**< 设置所在layer. */

/* 文本 ID */
#define TK_TITLE                    "reminder_mode"                             /**< 标题. */
#define TK_VIBRATE                  "com_vibrate"                               /**< 振动. */
#define TK_VOLUME                   "com_volume"                                /**< 声音. */
#define TK_VIB_VOL                  "com_vib_vol"                               /**< 振动 + 声音. */

typedef enum _list_item_index {
    INDEX_VIBRATE,                                                              /**< 00' 振动. */
    INDEX_VOLUME,                                                               /**< 01' 声音. */
    INDEX_VIB_VOL,                                                              /**< 02' 振动 + 声音. */
    INDEX_MAX,                                                                  /**< 03' ITEM数量. */
}list_item_index;

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
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    SportSettingTypeChoice *rawSet = &ssc->rsi.set.assist.cadenceAssistant.remindMode;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 设置选中项 */
    rawSet->choice = imap[index] + 1;
    ssc->change = true;
}

/*****************************************************************************************************
* @brief  : Picker类型
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Vibrate(lw_set_utils *lsu, uint16_t index, SportSettingTypeChoice *defSet, SportSettingTypeChoice *rawSet, const char *text1)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, index, true, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL)
        radioList_SetItem(item, text1, NULL, false);
}

/*****************************************************************************************************
* @brief  : 声音类型
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_VolType(lw_set_utils *lsu, uint16_t index, SportSettingTypeChoice *defSet, SportSettingTypeChoice *rawSet, const char *text1)
{
    lw_item *item;
    bool exist = device_hasSpeaker();

     item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL)
        radioList_SetItem(item, text1, NULL, false);
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
    lw_item itemList[INDEX_MAX] = {0};

    /* 保存信息 */
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    SportSettingTypeChoice *defSet = &ssc->rsi.def.assist.cadenceAssistant.remindMode;
    SportSettingTypeChoice *rawSet = &ssc->rsi.set.assist.cadenceAssistant.remindMode;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->rlw = radioList_Create(SETTING_SCREEN, SETTING_LAYER, _rlwEvent);
    radioList_SetListAttr(context->rlw, LW_ATTR_TITLE, (void *)_ST(TK_TITLE));
    radioList_SetListAttr(context->rlw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表 */
    setItem_Vibrate(lsu, INDEX_VIBRATE, defSet, rawSet, _ST(TK_VIBRATE));
    setItem_VolType(lsu, INDEX_VOLUME,  defSet, rawSet, _ST(TK_VOLUME));
    setItem_VolType(lsu, INDEX_VIB_VOL, defSet, rawSet, _ST(TK_VIB_VOL));

    select = rawSet->choice - 1;

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

const sport_setting_ops setting_ops_reminder_mode = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};