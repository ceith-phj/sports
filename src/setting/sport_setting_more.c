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

#define SETTING_SCREEN              "SportSettingMoreScreen"                    /**< 设置屏幕. */
#define SETTING_LAYER               "SportSettingMoreLayer"                     /**< 设置LAYER. */

typedef enum _list_item_index {
    INDEX_DATA_EDIT,                                                            /**< 00' 数据页编辑. */
    INDEX_NAVIGATION,                                                           /**< 01' 导航. */
    INDEX_DATA_3D,                                                              /**< 02' 3D数据模式. */
    INDEX_EXT_DEV,                                                              /**< 03' 外接设备. */
    INDEX_WEAR_HANDS,                                                           /**< 04' 佩戴手. */
    INDEX_POOL_LEN,                                                             /**< 05' 泳道长度. */
    INDEX_RECALIBRATE,                                                          /**< 06' 重新校准. */
    INDEX_LOCK_BUTTON,                                                          /**< 07' 自动锁屏. */
    INDEX_LOCK_SCRREN,                                                          /**< 08' 自定锁屏. */
    INDEX_MAX,                                                                  /**< 09' ITEM数量. */
}list_item_index;

/*****************************************************************************************************
* @brief  : 3d事件
*
* @param  : SSC
* @return : None
******************************************************************************************************/
static void _data3dEvent(sport_setting_container *ssc, MoreMessage *rawSet, bool state)
{
    rawSet->DataMore3d.onOff = state;
    ssc->change = true;

    if(state == true)
        switchDialogScreen(DLG_ID_3D_DATA_MODE, &ssc->rsi.set, ssc->change);
}

/*****************************************************************************************************
* @brief  : 列表控件的事件
*
* @param  : 屏幕
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
    MoreMessage *rawSet = &ssc->rsi.set.more;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 判断事件 */
    switch(imap[index])
    {
    case INDEX_DATA_EDIT:
        switchSettingScreen("SportItemEditScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_NAVIGATION:
        break;

    case INDEX_DATA_3D:
        _data3dEvent(ssc, rawSet, state);        
        break;

    case INDEX_EXT_DEV:
        break;

    case INDEX_WEAR_HANDS:
        switchSettingScreen("SportSettingWearHandScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_POOL_LEN:
        switchSettingScreen("SportSettingSwimPoolLengthScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_RECALIBRATE:
        switchDialogScreen(DLG_ID_RECALIBRATE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_LOCK_BUTTON:
        rawSet->LockButton.onOff = state;
        ssc->change = true;
        break;

    case INDEX_LOCK_SCRREN:
        rawSet->LockScreen.onOff = state;
        ssc->change = true;
        break;
    
    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 设置开关(单行文本)
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Switch(lw_set_utils *lsu, uint16_t index, bool exist, SportSettingTypeOnOff *defSet, SportSettingTypeOnOff *rawSet, const char *text)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        listWidget_SetItemText(item, text, NULL, true);
        listWidget_SetItemIcon(item, LW_ITEM_SWITCH_TEXT, rawSet->onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置文本项
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Text(lw_set_utils *lsu, uint16_t index, bool exist, bool prestartOnly, const char *text)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, prestartOnly);
    if(item != NULL) {
        listWidget_SetItemText(item, text, NULL, true);
    }
}

/*****************************************************************************************************
* @brief  : 佩戴手
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Hand(lw_set_utils *lsu, uint16_t index, bool exist, SportSettingTypeChoice *defSet, SportSettingTypeChoice *rawSet, const char *text)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        listWidget_SetItemText(item, text, NULL, true);
    }
}

/*****************************************************************************************************
* @brief  : 游泳
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Swim(lw_set_utils *lsu, uint16_t index, bool exist, SwimPoolLength *defSet, SwimPoolLength *rawSet, const char *text)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, defSet->onOff.has_onlyPreStart & defSet->onOff.onlyPreStart);
    if(item != NULL) {
        listWidget_SetItemText(item, text, NULL, true);
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
    lw_item itemList[INDEX_MAX] = {0};

    /* 设置指针 */
    list_setting_context *context = (list_setting_context *)&ssc->context;
    MoreMessage *defSet = &ssc->rsi.def.more;
    MoreMessage *rawSet = &ssc->rsi.set.more;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TK_MORE));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表项 */
    setItem_Text(lsu,   INDEX_DATA_EDIT,   defSet->has_dataEdit,        false,                    _ST(TK_DATA_EDIT));
    setItem_Text(lsu,   INDEX_NAVIGATION,  defSet->has_navigation,      rawSet->navigation,       _ST(TK_NAVIGATION));
    setItem_Switch(lsu, INDEX_DATA_3D,     defSet->has_DataMore3d,      &defSet->DataMore3d,      &rawSet->DataMore3d,      _ST(TK_DATA_3D));
    setItem_Text(lsu,   INDEX_EXT_DEV,     defSet->has_externalDevices, rawSet->externalDevices,  _ST(TK_EXT_DEV));
    setItem_Hand(lsu,   INDEX_WEAR_HANDS,  defSet->has_wearHands,       &defSet->wearHands,       &rawSet->wearHands,       _ST(TK_WEAR_HANDS));
    setItem_Swim(lsu,   INDEX_POOL_LEN,    defSet->has_poolLength,      &defSet->poolLength,      &rawSet->poolLength,      _ST(TK_POOL_LEN));
    setItem_Text(lsu,   INDEX_RECALIBRATE, defSet->has_recalibration,   rawSet->recalibration,    _ST(TK_RECALIBRATE));
    setItem_Switch(lsu, INDEX_LOCK_BUTTON, defSet->has_LockButton,      &defSet->LockButton,      &rawSet->LockButton,      _ST(TK_LOCK_BUTTON));
    setItem_Switch(lsu, INDEX_LOCK_SCRREN, defSet->has_LockScreen,      &defSet->LockScreen,      &rawSet->LockScreen,      _ST(TK_LOCK_SCREEN));

    /* 创建列表 */
    listWidget_SetList(context->lw, itemList, lwUtils_GetMapSize(lsu));
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

const sport_setting_ops setting_ops_more = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};


