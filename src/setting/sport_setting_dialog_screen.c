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
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sb_screen_util.h>
#include <sb_screen_context.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include "ui/Sports/include/setting/sport_setting_dialog_config.h"
#include "ui/Sports/include/setting/sport_setting_dialog_screen.h"
#include "widget/list/list_widget_utils.h"

#define THIS_SCREEN             "SportSettingDialogScreen"                      /**< SCREEN. */
#define THIS_LAYER              "SportSettingDialogLayer"                       /**< LAYER. */

SCREEN_DECLARE(SportSettingDialog);

static dialog_id dialogID;                                                      /**# DLG ID. */

/*****************************************************************************************************
* @brief  : 获取DLG Container
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static inline ScreenContainer *getDialogContainer(ScreenContext *scontext)
{
    sport_dialog_screen *sds = getScreenData(scontext, sport_dialog_screen);
    return &sds->dlg->container;
}

/*****************************************************************************************************
* @brief  : 获取DLG的操作函数
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static const sport_dialog_relation *getDialogRlt(dialog_id id)
{
    const sport_dialog_relation *rlt;

    for(int i = 0 ; i < ARRAY_SIZE(sportDialogRlt) ; i++)
    {
        rlt = &sportDialogRlt[i];
        if(rlt->id == id)
            return rlt;
    }

    return &sportDialogRlt[0];
}

/*****************************************************************************************************
* @brief  : 屏幕预创建事件
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    sport_dialog_screen *sds;
    const sport_dialog_relation *rlt;

    /* 设置DLG ID */
    (data != NULL) ? (dialogID = *(dialog_id *)data) : H_NULL;

    /* 创建屏幕数据 */
    sds = H_MALLOC(sizeof(sport_dialog_screen));
    checkPointer(sds);
    setScreenData(screenContext, sds);

    /* 获取映射关系 */
    rlt = getDialogRlt(dialogID);

    /* 创建DLG */
    sds->dlg = creatAlertDialog(screenContext, THIS_SCREEN);
    addContainer(screenContext, sds->dlg->mDialogLayerPath, sds->dlg);

    /* 读取设置 */
    sportManager()->utilsManager()->common()->getSet(&sds->rsi);

    /* 进入子页面 */
    rlt->ops->create(sds->dlg, screenContext);
}

/*****************************************************************************************************
* @brief  : 屏幕创建事件
*
* @param  : humi screen上下文
* @return : None
******************************************************************************************************/
static void onScreenPostCreate(ScreenContext *screenContext)
{
    /* 屏蔽按键 */
    if(sportManager()->ctrlManager()->getState() == SPORT_STATE_SUSPEND) {
        System_setKeyFilter(HM_KEY_CODE_HOME, HM_KEY_ACT_FILTER_CLICK | HM_KEY_ACT_FILTER_LONG_PRESS);
        System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
    }
}

/*****************************************************************************************************
* @brief  : 屏幕预销毁事件
*
* @param  : humi screen上下文
* @return : None
******************************************************************************************************/
static void onScreenPreDestory(ScreenContext *screenContext)
{
    /* 恢复按键 */
    if(sportManager()->ctrlManager()->getState() == SPORT_STATE_SUSPEND) {
        System_resetKeyFilter(HM_KEY_CODE_HOME);
        System_resetKeyFilter(HM_KEY_CODE_SHORTCUT);
    }
}

/*****************************************************************************************************
* @brief  : 屏幕销毁事件
*
* @param  : humi screen上下文
* @return : None
******************************************************************************************************/
static void onScreenPostDestory(ScreenContext *screenContext)
{
    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

/*****************************************************************************************************
* @brief  : 屏幕点击事件
*
* @param  : humi screen上下文
* @param  : 字符串id
* @param  : 事件id
* @param  : 事件数据
* @return : None
******************************************************************************************************/
static bool handleTouchEvent(ScreenContext* screenContext, const char* id, const SBTouchEvent eventId, const SBEventInfo* info)
{
    ScreenContainer *container = getDialogContainer(screenContext);
    return container->handleTouchEvent(screenContext, container, id, eventId, info);
}

/*****************************************************************************************************
* @brief  : 屏幕手势事件
*
* @param  : humi screen上下文
* @param  : 字符串id
* @param  : 手势类型
* @return : None
******************************************************************************************************/
static bool handleGestureEvent(ScreenContext* screenContext, const char* id, const SBGestureEvent eventId)
{
    if(eventId != GESTURE_RIGHT)
        return true;

    getScreenManager()->finishCurrentScreen();
    return true;
}

/*****************************************************************************************************
* @brief  : 自定义事件
*
* @param  : humi screen上下文
* @param  : 字符串id
* @param  : 事件名
* @return : None
******************************************************************************************************/
static bool handleEvent(ScreenContext* screenContext, const char* id, const char* event)
{
    return true;
}

/*****************************************************************************************************
* @brief  : 按键事件
*
* @param  : humi screen上下文
* @param  : 按键信息
* @return : None
******************************************************************************************************/
static bool handleKeyEvent(ScreenContext* screenContext,  const HmKeyEvent *info)
{
    SportStateType state = sportManager()->ctrlManager()->getState();

    if(info->code == HM_KEY_CODE_HOME && state == SPORT_STATE_SUSPEND)
        getScreenManager()->startScreen("SportScreen", NULL, 0);

    if(info->code == HM_KEY_CODE_SHORTCUT && state == SPORT_STATE_SUSPEND)
        getScreenManager()->startScreen("SportScreen", NULL, 0);

    return true;
}

/*****************************************************************************************************
* @brief  : 功能未知
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static void onNewScreenData(ScreenContext *screenContext, void* data)
{
}

/*****************************************************************************************************
* @brief  : 功能未知
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static void onDataRestore(ScreenContext *screenContext, void* data)
{
}

/*****************************************************************************************************
* @brief  : 功能未知
*
* @param  : humi screen上下文
* @return : None
******************************************************************************************************/
static void onDataSave(ScreenContext* screenContext)
{
}

/*****************************************************************************************************
* @brief  : 动画完成事件
*
* @param  : humi screen上下文
* @param  : 控件ID
* @return : None
******************************************************************************************************/
static void onAnimationComplete(ScreenContext* screenContext, const char* id)
{
}

/*****************************************************************************************************
* @brief  : 动画停止事件
*
* @param  : humi screen上下文
* @param  : 控件ID
* @return : None
******************************************************************************************************/
static void onAnimationStop(ScreenContext* screenContext, const char* id)
{
}