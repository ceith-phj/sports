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

#define SETTING_SCREEN              "SportScreen"                               /**< 设置屏幕. */
#define SETTING_LAYER               "sportPreStartLayer.SettingGroup"           /**< 运动辅助. */

/* LOGO */
#define LOGO_ASSISTANT              "images/sports/setting/sport_setting_icon_assistant.png"        /**< LOGO - 辅助. */
#define LOGO_TRANING                "images/sports/setting/sport_setting_icon_traning.png"          /**< LOGO - 训练. */
#define LOGO_MORE                   "images/sports/setting/sport_setting_icon_more.png"             /**< LOGO - 更多. */

#define MAIN_LIST_START_OFFSET      60                                          /**< 列表开始偏移. */

typedef enum _list_item_index {
    INDEX_ASSISTANT,                                                            /**< 00' 运动辅助. */
    INDEX_TRANING,                                                              /**< 01' 训练. */
    INDEX_MORE,                                                                 /**< 02' 更多. */
    INDEX_MAX,                                                                  /**< 03' ITEM数量. */
}list_item_index;

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
    sport_setting_container *ssc = lw->attr.param;
    list_setting_context *context = (list_setting_context *)&ssc->context;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    switch(imap[index])
    {
    case INDEX_ASSISTANT:
        getScreenManager()->startScreen("SportSettingAssistantScreen", H_NULL, 0);
        break;

    case INDEX_TRANING:
        getScreenManager()->startScreen("SportSettingTrainScreen", H_NULL, 0);
        break;

    case INDEX_MORE:
        getScreenManager()->startScreen("SportSettingMoreScreen", H_NULL, 0);
        break;
    
    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 设置Assistant文本项
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Logo(lw_set_utils *lsu, uint16_t index, bool exist, const char *text, const char *logo)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, false);
    if(item != NULL) {
        listWidget_SetItemText(item, text, NULL, true);
        listWidget_SetItemLogo(item, logo);
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
    SportSettingsMessage *defSet = &ssc->rsi.def;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    listWidget_SetListAttr(context->lw, LW_ATTR_START_POS_Y, (void *)MAIN_LIST_START_OFFSET);
    listWidget_SetListAttr(context->lw, LW_ATTR_ENABLE_SCROLL_BAR, (void *)false);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表项 */
    setItem_Logo(lsu, INDEX_ASSISTANT, defSet->has_assist,    _ST(TK_ASSISTANT), LOGO_ASSISTANT);
    setItem_Logo(lsu, INDEX_TRANING,   false,                 _ST(TK_TRANING),   LOGO_TRANING);
    setItem_Logo(lsu, INDEX_MORE,      defSet->has_more,      _ST(TK_MORE),      LOGO_MORE);

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

const sport_setting_ops setting_ops_main = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};
