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

#include "hlog.h"
#define TAG "[LG_SSR]"

#ifdef SIMULATOR
#undef HLOG_I
#define HLOG_I(tag, fmt, ...)       printf(fmt"\r\n", ##__VA_ARGS__)
#endif

#define SETTING_SCREEN              "SportSettingReserveHrZoneScreen"           /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingReserveHrZoneLayer"            /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_HR_ZONE_1,                                                            /**< 00' 区间1. */
    INDEX_HR_ZONE_2,                                                            /**< 01' 区间2. */
    INDEX_HR_ZONE_3,                                                            /**< 02' 区间3. */
    INDEX_HR_ZONE_4,                                                            /**< 03' 区间4. */
    INDEX_HR_ZONE_5,                                                            /**< 04' 区间5. */
    INDEX_HR_ZONE_CUSTOM,                                                       /**< 05' 自定义. */
    INDEX_MAX,                                                                  /**< 06' 最大值. */
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
    case INDEX_HR_ZONE_CUSTOM:
        switchPickerScreen(PICKER_ID_REMINDER_HR_ZONE, &ssc->rsi.set, ssc->change);
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 单选框事件
*
* @param  : index
* @return : None
******************************************************************************************************/
static void _radioEvent(sport_setting_container *ssc, uint16_t index)
{
    SportReminderHRZone *rawSet = &ssc->rsi.set.assist.reminder.heartRateZone;
    rawSet->hrRange.choice = index + 1;
    rawSet->onOff = true;
    ssc->change = true;

    /* 调试信息 */
    if(rawSet->hrRange.choice > 6 || rawSet->hrRange.choice < 1) {
        HLOG_I(TAG, "DDDDDDDDDDDDDDD hr range choice = %d", rawSet->hrRange.choice);
        rawSet->hrRange.choice = 6;
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
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 文本点击事件 */
    if(event == LW_EVENT_TEXT)
        _textEvent(ssc, imap[index]);
    else
        _radioEvent(ssc, imap[index]);
}


/*****************************************************************************************************
* @brief  : 设置单选文本
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_RadioText(lw_set_utils *lsu, uint16_t index, SportReminderHRZone *defSet, 
                                    const char *text, float min, float max, char *val, bool arrow)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, index, true, defSet->hrRange.has_onlyPreStart & defSet->hrRange.onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, _ST(TK_ARG_HR_ZONE), (int32_t)min, (int32_t)max);
        radioList_SetItem(item, text, val, arrow);
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
    sport_hr_range_setting_t hrInfo;
    char value[INDEX_MAX][SZ_SET_BUF] = {0};
    lw_item itemList[INDEX_MAX] = {0};

    /* 获取心率区间 */
    sportSetting_getSettingManager()->hrZone->getRange(&hrInfo);

    /* 保存信息 */
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;    
    SportReminderHRZone *defSet = &ssc->rsi.def.assist.reminder.heartRateZone;
    SportReminderHRZone *rawSet = &ssc->rsi.set.assist.reminder.heartRateZone;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->rlw = radioList_Create(SETTING_SCREEN, SETTING_LAYER, _rlwEvent);
    radioList_SetListAttr(context->rlw, LW_ATTR_TITLE, (void *)_ST(TK_HR_ZONE));
    radioList_SetListAttr(context->rlw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 自定义需要先判断一下是否有默认值 */
    if(rawSet->customHR.min == 10 && rawSet->customHR.max == 200) {
        rawSet->customHR.min = hrInfo.value[2];
        rawSet->customHR.max = hrInfo.value[3];
    }

    /* 设置列表 */
    setItem_RadioText(lsu, INDEX_HR_ZONE_1,       defSet, _GT(TEXT_ID_HR_ZONE_1),   hrInfo.value[0],       hrInfo.value[1],      value[i++], false);
    setItem_RadioText(lsu, INDEX_HR_ZONE_2,       defSet, _GT(TEXT_ID_HR_ZONE_2),   hrInfo.value[1],       hrInfo.value[2],      value[i++], false);
    setItem_RadioText(lsu, INDEX_HR_ZONE_3,       defSet, _GT(TEXT_ID_HR_ZONE_3),   hrInfo.value[2],       hrInfo.value[3],      value[i++], false);
    setItem_RadioText(lsu, INDEX_HR_ZONE_4,       defSet, _GT(TEXT_ID_HR_ZONE_4),   hrInfo.value[3],       hrInfo.value[4],      value[i++], false);
    setItem_RadioText(lsu, INDEX_HR_ZONE_5,       defSet, _GT(TEXT_ID_HR_ZONE_5),   hrInfo.value[4],       hrInfo.value[5],      value[i++], false);
    setItem_RadioText(lsu, INDEX_HR_ZONE_CUSTOM,  defSet, _GT(TEXT_ID_CUSTOM),      rawSet->customHR.min,  rawSet->customHR.max, value[i++], true);

    /* 创建列表 */
    select = rawSet->hrRange.choice - 1;
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

const sport_setting_ops setting_ops_reserve_hr_zone = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};
