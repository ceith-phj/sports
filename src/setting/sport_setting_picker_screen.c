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

#include "ui/Sports/include/setting/sport_setting_picker_config.h"
#include "sports/include/utils/sports_resource_utils.h"

SCREEN_DECLARE(SPORT_PICKER_SCREEN_NAME);

static int pickerID;                                                            /**@ 选择器ID. */

/*****************************************************************************************************
* @brief  : 获取picker的操作函数
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static const sport_picker_relation *getPickerRlt(picker_id id)
{
    const sport_picker_relation *rlt;

    for(int i = 0 ; i < ARRAY_SIZE(sportPickerRlt) ; i++)
    {
        rlt = &sportPickerRlt[i];
        if(rlt->id == id)
            return rlt;
    }

    return &sportPickerRlt[0];
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
    const sport_picker_relation *rlt;

    /* 设置选择器ID */
    if(data != NULL)
        pickerID = *(int *)data;

    /* 申请应用数据空间 */
    picker_screen_info *psi = H_MALLOC(sizeof(picker_screen_info));
    if(psi == NULL)
        return;

    /* 获取设置和映射关系 */
    getSportSetting(&psi->ssm);
    rlt = getPickerRlt((picker_id)pickerID);

    /* 设置屏幕信息 */
    psi->sc = screenContext;
    setScreenData(screenContext, psi);

    /* 创建设置页 */
    rlt->ops->create(psi);
}

/*****************************************************************************************************
* @brief  : 屏幕创建事件
*
* @param  : humi screen上下文
* @return : None
******************************************************************************************************/
static void onScreenPostCreate(ScreenContext* screenContext)
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
static void onScreenPreDestory(ScreenContext* screenContext)
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
static void onScreenPostDestory(ScreenContext* screenContext)
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
    picker_screen_info *psi = getScreenData(screenContext, picker_screen_info);
    if(psi->picker != NULL)
        psi->picker->container.handleTouchEvent(screenContext, (ScreenContainer *)psi->picker, id, eventId, info);

    return true;
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
    picker_screen_info *psi = getScreenData(screenContext, picker_screen_info);
    if(psi->picker != NULL)
        psi->picker->container.handleGestureEvent(screenContext, (ScreenContainer *)psi->picker, id, eventId);
    else if(eventId == GESTURE_RIGHT)
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
    picker_screen_info *psi = getScreenData(screenContext, picker_screen_info);
    if(psi->picker != NULL)
        psi->picker->container.handleEvent(screenContext, (ScreenContainer *)psi->picker, id, event);

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