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



#define SETTING_SCREEN              "SportSettingSwimPoolLengthScreen"          /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingSwimPoolLengthLayer"           /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_SPL_25_METER,                                                         /**< 00' 25米. */
    INDEX_SPL_50_METER,                                                         /**< 01' 50米. */
    INDEX_SPL_25_YARD,                                                          /**< 02' 25码.*/
    INDEX_SPL_50_YARD,                                                          /**< 03' 50码.*/
    INDEX_SPL_CUSTOM,                                                           /**< 04' 自定义.*/
    INDEX_MAX,                                                                  /**< 02' ITEM数量. */
}list_item_index;

typedef enum _pool_length_unit {
    SPL_UNIT_METER,                                                             /**< 00' 米. */
    SPL_UNIT_YARDS,                                                             /**< 01' 码. */
}pool_length_unit;

static sport_setting_container *ssc_g;

/*****************************************************************************************************
* @brief  : 文本事件
*
* @param  : index
* @return : None
******************************************************************************************************/
static void _textEvent(sport_setting_container *ssc, int index)
{
    if(index == INDEX_SPL_CUSTOM)
        switchPickerScreen(PICKER_ID_SWIM_POOL_LENGTH, &ssc->rsi.set, ssc->change);
}

/*****************************************************************************************************
* @brief  : 多选框事件
*
* @param  : index
* @return : None
******************************************************************************************************/
static void _radioListEvent(sport_setting_container *ssc, int index)
{
    ssc->rsi.set.more.poolLength.choice.choice = index;
    ssc->change = true;
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

    switch(event)
    {
    case LW_EVENT_TEXT:
        _textEvent(ssc, imap[index]);
        break;

    case LW_EVENT_RADIO_BOX:
        _radioListEvent(ssc, imap[index]);
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 设置固定选项
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_StandOption(lw_set_utils *lsu, uint16_t index, bool exist, char *val, 
                                                            int len, pool_length_unit unit)
{
    lw_item *item;
    const char *format;

    format = (unit == SPL_UNIT_METER) ? _ST(TK_ARG_METER_U) : _ST(TK_ARG_YARDS);
    item = lwUtils_InitItem(lsu, index, exist, true);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, format, len);
        radioList_SetItem(item, val, NULL, false);
    }
}

/*****************************************************************************************************
* @brief  : 自定义选项
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_CustomOption(lw_set_utils *lsu, uint16_t index, bool exist)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, true);
    if(item != NULL)
        radioList_SetItem(item, _GT(TEXT_ID_CUSTOM), NULL, true);
}

/*****************************************************************************************************
* @brief  : 设置监听
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool _settingListener(EventData *eventData)
{
    if(!eventData || eventData->arg1 != SPT_SETTING_POOL_LENGTH)
        return false;

    radio_list_setting_context *context = (radio_list_setting_context *)&ssc_g->context;
    sportManager()->utilsManager()->common()->getSet(&ssc_g->rsi);
    radioList_SetListAttr(context->rlw, LW_ATTR_RADIO_LIST_RESELECT, (void *)ssc_g->rsi.set.more.poolLength.choice.choice);
    return true;
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
    char value[INDEX_MAX][SZ_SET_BUF] = {0};
    lw_item itemList[INDEX_MAX] = {0};

    /* 监听设置变更事件 */
    ssc_g = ssc;
    SportService_getInstance()->addEventListener(SPORT_SERVICE_EVT_SETTING_CHG, _settingListener);

    /* 保存信息 */
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    SwimPoolLength *defSet = &ssc->rsi.def.more.poolLength;
    SwimPoolLength *rawSet = &ssc->rsi.set.more.poolLength;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->rlw = radioList_Create(SETTING_SCREEN, SETTING_LAYER, _rlwEvent);
    radioList_SetListAttr(context->rlw, LW_ATTR_TITLE, (void *)_ST(TK_POOL_LEN));
    radioList_SetListAttr(context->rlw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表 */
    setItem_StandOption(lsu,  INDEX_SPL_25_METER, defSet->has_choice, value[0], 25, SPL_UNIT_METER);
    setItem_StandOption(lsu,  INDEX_SPL_50_METER, defSet->has_choice, value[1], 50, SPL_UNIT_METER);
    setItem_StandOption(lsu,  INDEX_SPL_25_YARD,  defSet->has_choice, value[2], 25, SPL_UNIT_YARDS);
    setItem_StandOption(lsu,  INDEX_SPL_50_YARD,  defSet->has_choice, value[3], 50, SPL_UNIT_YARDS);
    setItem_CustomOption(lsu, INDEX_SPL_CUSTOM,   defSet->has_length);

    /* 判断默认选中值 */
    select = rawSet->choice.choice;

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
    SportService_getInstance()->delEventListener(_settingListener);
    ssc_g = H_NULL;
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

const sport_setting_ops setting_ops_swim_pool_length = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

