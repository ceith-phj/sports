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

#define SETTING_SCREEN              "SportSettingWearHandScreen"                /**< 设置所在screen. */
#define SETTING_LAYER               "SportSettingWearHandLayer"                 /**< 设置所在layer. */

typedef enum _list_item_index {
    INDEX_LEFT_HAND,                                                            /**< 00' 左手. */
    INDEX_RIGHT_HAND,                                                           /**< 01' 右手. */
    INDEX_MAX,                                                                  /**< 02' ITEM数量. */
}list_item_index;

static sport_setting_container *ssc_g;

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
    SportSettingTypeChoice *rawSet = &ssc->rsi.set.more.wearHands;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    rawSet->choice = imap[index] + 1;
    ssc->change = true;
}

/*****************************************************************************************************
* @brief  : 设置单选文本
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_RadioText(lw_set_utils *lsu, uint16_t index, const char *text)
{
    lw_item *item = lwUtils_InitItem(lsu, index, true, false);
    if(item != NULL)
        radioList_SetItem(item, text, NULL, false);
}

/*****************************************************************************************************
* @brief  : 设置监听
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool _settingListener(EventData *eventData)
{
    if(!eventData || eventData->arg1 != SPT_SETTING_WEAR_HAND)
        return false;

    radio_list_setting_context *context = (radio_list_setting_context *)&ssc_g->context;
    sportManager()->utilsManager()->common()->getSet(&ssc_g->rsi);
    radioList_SetListAttr(context->rlw, LW_ATTR_RADIO_LIST_RESELECT, (void *)ssc_g->rsi.set.more.wearHands.choice);

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
    lw_item itemList[INDEX_MAX] = {0};

    /* 监听设置变更事件 */
    ssc_g = ssc;
    SportService_getInstance()->addEventListener(SPORT_SERVICE_EVT_SETTING_CHG, _settingListener);

    /* 保存信息 */
    radio_list_setting_context *context = (radio_list_setting_context *)&ssc->context;
    SportSettingTypeChoice *rawSet = &ssc->rsi.set.more.wearHands;
    lw_set_utils *lsu = &context->lsu;

    /* 没有佩戴手直接返回 */
    if(!ssc->rsi.def.more.has_wearHands)
        return;

    /* 创建列表控件 */
    context->rlw = radioList_Create(SETTING_SCREEN, SETTING_LAYER, _rlwEvent);
    radioList_SetListAttr(context->rlw, LW_ATTR_TITLE, (void *)_GT(TEXT_ID_CHOICE_WEAR_HAND));
    radioList_SetListAttr(context->rlw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表 */
    setItem_RadioText(lsu, INDEX_LEFT_HAND,  _GT(TEXT_ID_LEFT_HAND));
    setItem_RadioText(lsu, INDEX_RIGHT_HAND, _GT(TEXT_ID_RIGHT_HAND));

    /* 默认选中项 */
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

const sport_setting_ops setting_ops_wear_hand = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};
