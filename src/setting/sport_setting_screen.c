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
#include <widget/list/list_widget_utils.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include "ui/Sports/include/setting/sport_setting_screen.h"
#include "ui/Sports/include/setting/sport_setting_config.h"


#include <hlog.h>
#define TAG "[LG_SSC]"

typedef void (*data_save_fn)(const char *screen, sport_setting_context *settingContext);
typedef void (*data_restore_fn)(sport_setting_context *settingContext, void *data);

/*============================================================================================================ 00' 公共接口 ==== */
/*****************************************************************************************************
* @brief  : 通过action_context 获取设置的操作函数
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static const sport_setting_relation *getSportSettingRlt(const char *screen)
{
    for(uint16_t i = 0 ; i < ARRAY_SIZE(sportSettingRLT) ; i++)
    {
        if(!strcmp(screen, sportSettingRLT[i].screen))
            return &sportSettingRLT[i];
    }

    return &sportSettingRLT[0];
}

/*============================================================================================================ 01' 数据保存 ==== */
/*****************************************************************************************************
* @brief  : 保存数据 - 列表
*
* @param  : 设置上下文
* @return : None
******************************************************************************************************/
static void dataSave_List(const char *screen, sport_setting_context *settingContext)
{
    lw_context lc;
    list_setting_context *context = (list_setting_context *)settingContext;

    listWidget_GetListAttr(context->lw, LW_ATTR_CONTEXT, &lc);
    getScreenManager()->saveScreenData(screen, &lc, sizeof(lw_context));
}

/*****************************************************************************************************
* @brief  : 保存数据 - 单选列表
*
* @param  : 设置上下文
* @return : None
******************************************************************************************************/
static void dataSave_RadioList(const char *screen, sport_setting_context *settingContext)
{
    lw_context lc;
    radio_list_setting_context *context = &settingContext->radioList;

    radioList_GetListAttr(context->rlw, LW_ATTR_CONTEXT, &lc);
    getScreenManager()->saveScreenData(screen, &lc, sizeof(lw_context));
}

static data_save_fn dataSave[] = {
    dataSave_List,
    dataSave_RadioList,
};

/*============================================================================================================ 02' 数据恢复 ==== */
/*****************************************************************************************************
* @brief  : 数据恢复 - 列表
*
* @param  : 设置上下文
* @return : None
******************************************************************************************************/
static void dataRestore_List(sport_setting_context *settingContext, void *data)
{
    list_setting_context *context = (list_setting_context *)settingContext;
    listWidget_SetListAttr(context->lw, LW_ATTR_CONTEXT, data);
}

/*****************************************************************************************************
* @brief  : 数据恢复 - 单选列表
*
* @param  : 设置上下文
* @return : None
******************************************************************************************************/
static void dataRestore_RadioList(sport_setting_context *settingContext, void *data)
{
    radio_list_setting_context *context = (radio_list_setting_context *)settingContext;
    radioList_SetListAttr(context->rlw, LW_ATTR_CONTEXT, data);
}

static const data_restore_fn dataRestore[] = {
    dataRestore_List,
    dataRestore_RadioList
};

/*============================================================================================================ 03' 容器操作 ==== */
/*****************************************************************************************************
* @brief  : 初始化Container
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static void initSettingContainer(ScreenContext* screenContext, sport_setting_container *ssc)
{
    char screen[100];
    const sport_setting_relation *rlt;

    /* 获取当前screen name */
    gr_context_get_screen(screenContext->_grActioncontext, screen, sizeof(screen));

    /* 初始化参数 */
    rlt = getSportSettingRlt(screen);
    ssc->type = rlt->type;
    ssc->ops = rlt->ops;
    ssc->change = false;
    strncpy(ssc->name, screen, sizeof(screen));

    /* 保存上下文 */
    ssc->sc = screenContext;
}

/*============================================================================================================ 04' 生命周期 ==== */
/*****************************************************************************************************
* @brief  : 屏幕预创建事件
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static void onScreenPreCreate(ScreenContext *screenContext, void* data)
{
    /* 创建SSC */
    sport_setting_container *ssc = H_MALLOC(sizeof(sport_setting_container));
    checkPointer(ssc);

    /* 存入上下文 */
    memset(ssc, 0, sizeof(sport_setting_container));
    setScreenData(screenContext, ssc);

    /* 读取设置 */
    sportManager()->utilsManager()->common()->getSet(&ssc->rsi);

    /* 初始化SSC */
    initSettingContainer(screenContext, ssc);
    ssc->ops->create(ssc);
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
    sport_setting_container *ssc = getScreenData(screenContext, sport_setting_container);

    /* 如果设置有修改, 那么保存 */
    if(ssc->change)
        sportManager()->utilsManager()->common()->saveSet(&ssc->rsi.set);

    /* 销毁资源 */
    ssc->ops->destory(ssc);
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
    sport_setting_container *ssc = getScreenData(screenContext, sport_setting_container);

    ssc->event.style  = SB_EVENT_STYLE_TOUCH;
    ssc->event.screen = screenContext;
    ssc->event.id     = id;
    ssc->event.type   = eventId;
    ssc->event.info   = info;

    ssc->ops->event(ssc);
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
    sport_setting_container *ssc = getScreenData(screenContext, sport_setting_container);

    if(eventId != GESTURE_RIGHT)
        return false;

    /* 左滑事件导致的保存 */
    if(ssc->change) {
        sportManager()->utilsManager()->common()->saveSet(&ssc->rsi.set);
        ssc->change = false;
    }

    /* 回退屏幕 */
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
    sport_setting_container *ssc = getScreenData(screenContext, sport_setting_container);

    ssc->event.style  = SB_EVENT_STYLE_CUSTOM;
    ssc->event.screen = screenContext;
    ssc->event.id     = id;
    ssc->event.event  = event;

    ssc->ops->event(ssc);
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
* @brief  : 数据恢复
*
* @param  : humi screen上下文
* @param  : 传入的数据
* @return : None
******************************************************************************************************/
static void onDataRestore(ScreenContext *screenContext, void* data)
{
    sport_setting_container *ssc = getScreenData(screenContext, sport_setting_container);
    dataRestore[ssc->type](&ssc->context, data);
}

/*****************************************************************************************************
* @brief  : 数据保存
*
* @param  : humi screen上下文
* @return : None
******************************************************************************************************/
static void onDataSave(ScreenContext* screenContext)
{
    sport_setting_container *ssc = getScreenData(screenContext, sport_setting_container);
    dataSave[ssc->type](ssc->name, &ssc->context);
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