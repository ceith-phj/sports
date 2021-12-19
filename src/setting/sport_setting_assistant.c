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

#define SETTING_SCREEN              "SportSettingAssistantScreen"               /**< 设置屏幕. */
#define SETTING_LAYER               "SportSettingAssistantLayer"                /**< 运动辅助. */

typedef enum _list_item_index {
    INDEX_GOAL,                                                                 /**< 00' 运动目标. */
    INDEX_REMINDER,                                                             /**< 01' 提醒. */
    INDEX_AUTO_LAP,                                                             /**< 02' 自动分段. */
    INDEX_AUTO_PAUSE,                                                           /**< 03' 自动暂停. */
    INDEX_VIRTUAL_PACER,                                                        /**< 04' 虚拟兔子. */
    INDEX_CADENCE_ASSISTANT,                                                    /**< 05' 步频助手. */
    INDEX_PACE_STRATEGY,                                                        /**< 06' 配速策略. */
    INDEX_SPORT_COMPASS,                                                        /**< 07' 指南针. */
    INDEX_SPO2,                                                                 /**< 08' SPO2. */
    INDEX_MAX,                                                                  /**< 09' ID最大数量. */
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
    /* 设置指针 */
    sport_setting_container *ssc = lw->attr.param;
    list_setting_context *context = (list_setting_context *)&ssc->context;
    AssistMessage *rawSet = &ssc->rsi.set.assist;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 判断事件 */
    switch(imap[index])
    {
    case INDEX_GOAL:
        if(ssc->rsi.set.assist.target.noGoals.onOff == true)
            switchSettingScreen("SportSettingGoalScreen", &ssc->rsi.set, ssc->change);
        else
            switchDialogScreen(DLG_ID_GOAL, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_REMINDER:
        switchSettingScreen("SportSettingReminderScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_AUTO_LAP:
        switchSettingScreen("SportSettingAutoLapScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_AUTO_PAUSE:
        switchSettingScreen("SportSettingAutoPauseScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_VIRTUAL_PACER:
        switchSettingScreen("SportSettingVirtualPacerScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_CADENCE_ASSISTANT:
        switchSettingScreen("SportSettingCadenceAssistantScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_PACE_STRATEGY:
        break;

    case INDEX_SPORT_COMPASS:
        rawSet->compass.onOff = state;
        ssc->change = true;
        break;

    case INDEX_SPO2:
        rawSet->spO2.onOff = state;
        ssc->change = true;
        break;

    default:
        break;
    }
}

/*============================================================================================================ 01' 判断接口 ==== */
/*****************************************************************************************************
* @brief  : 判断周期 - on off类型
*
* @param  : 设置
* @return : None
******************************************************************************************************/
static bool checkPso_OnOff(SportSettingTypeOnOff *rawSet)
{
    return rawSet->has_onlyPreStart & rawSet->onlyPreStart;
}

/*****************************************************************************************************
* @brief  : 判断周期 - picker类型
*
* @param  : 设置
* @return : None
******************************************************************************************************/
static bool checkPso_Picker(SportSettingTypePicker *rawSet)
{
    return rawSet->has_onlyPreStart & rawSet->onlyPreStart;
}

/*****************************************************************************************************
* @brief  : 判断周期 - UWS类型
*
* @param  : 设置
* @return : None
******************************************************************************************************/
static bool checkPso_Uws(SportSettingUseWorkoutStatistics *rawSet)
{
    if(checkPso_Picker(&rawSet->distance) & rawSet->has_distance)
        return true;
    else if(checkPso_Picker(&rawSet->time) & rawSet->has_time)
        return true;
    else if(checkPso_Picker(&rawSet->calories) & rawSet->has_calories)
        return true;
    else if(checkPso_Picker(&rawSet->count) & rawSet->has_count)
        return true;
    else if(checkPso_Picker(&rawSet->trips) & rawSet->has_trips)
        return true;
    else
        return false;    
}

/*****************************************************************************************************
* @brief  : 判断是否运动前独有 - 运动目标
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool checkPso_Goal(SportSettingsMessage *defSet)
{
    SportTarget *rawSet = &defSet->assist.target;

    if(!defSet->assist.has_target)
        return false;
    else if(checkPso_Picker(&rawSet->useTrainingEffect.aerobicTE) & rawSet->useTrainingEffect.has_aerobicTE)
        return true;
    else if(checkPso_Picker(&rawSet->useTrainingEffect.anaerobicTE) & rawSet->useTrainingEffect.has_anaerobicTE)
        return true;
    else if(checkPso_Uws(&rawSet->useWorkoutStatistics) & rawSet->has_useWorkoutStatistics)
        return true;
    else if(checkPso_OnOff(&rawSet->noGoals) & rawSet->has_noGoals)
        return true;
    else
        return false;
}

/*****************************************************************************************************
* @brief  : 判断是否运动前独有 - 自动暂停
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool checkPso_AutoLap(SportSettingsMessage *defSet)
{
    SportAssistAutoLap *rawSet = &defSet->assist.autoLap;

    if(!defSet->assist.has_autoLap)
        return false;
    else if(checkPso_OnOff(&rawSet->onOff) & rawSet->has_onOff)
        return true;
    else if(checkPso_OnOff(&rawSet->reminder) & rawSet->has_reminder)
        return true;
    else if(checkPso_Picker(&rawSet->distance) & rawSet->has_distance)
        return true;
    else
        return false;
}

/*****************************************************************************************************
* @brief  : 判断是否运动前独有 - 自动暂停
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool checkPso_AutoPause(SportSettingsMessage *defSet)
{
    SportAssistAutoPause *rawSet = &defSet->assist.autoPause;

    if(!defSet->assist.has_autoPause)
        return false;
    else if(checkPso_OnOff(&rawSet->onOff) & rawSet->has_onOff)
        return true;
    else if(checkPso_OnOff(&rawSet->standing) & rawSet->has_standing)
        return true;
    else if(checkPso_Picker(&rawSet->bySpeed) & rawSet->has_bySpeed)
        return true;
    else if(checkPso_Picker(&rawSet->byPace) & rawSet->has_byPace)
        return true;
    else
        return false;
}

/*****************************************************************************************************
* @brief  : 判断是否运动前独有 - 虚拟兔子
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool checkPso_VirtualPacer(SportSettingsMessage *defSet)
{
    SportSettingTypePicker *rawSet = &defSet->assist.virtualPacer;

    if(!defSet->assist.has_virtualPacer)
        return false;
    else if(checkPso_Picker(rawSet))
        return true;
    else
        return false;
}

/*****************************************************************************************************
* @brief  : 判断是否运动前独有 - 步频助手
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool checkPso_CadenceAssist(SportSettingsMessage *defSet)
{
    SportAssistCandece *rawSet = &defSet->assist.cadenceAssistant;

    if(!defSet->assist.has_cadenceAssistant)
        return false;
    else if(checkPso_Picker(&rawSet->cadence) & rawSet->has_cadence)
        return true;
    else if(checkPso_Picker(&rawSet->reminderFrequence) & rawSet->has_reminderFrequence)
        return true;
    else
        return false;
}

/*****************************************************************************************************
* @brief  : 判断ITEM是否为运动前独有
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static bool itemPrestartOnly(SportSettingsMessage *defSet, uint16_t index)
{
    bool pso = false;

    switch(index)
    {
    case INDEX_GOAL:
        pso = checkPso_Goal(defSet);
        break;

    case INDEX_AUTO_LAP:
        pso = checkPso_AutoLap(defSet);
        break;

    case INDEX_AUTO_PAUSE:
        pso = checkPso_AutoPause(defSet);
        break;

    case INDEX_VIRTUAL_PACER:
        pso = checkPso_VirtualPacer(defSet);
        break;

    case INDEX_CADENCE_ASSISTANT:
        pso = checkPso_CadenceAssist(defSet);
        break;

    default:
        break;
    }

    return pso;
}

/*============================================================================================================ 02' 列表设置部分 ==== */
/*****************************************************************************************************
* @brief  : 设置开关(单行文本)
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Switch(lw_set_utils *lsu, uint16_t index, bool exist, bool preStartOnly, bool state, const char *text)
{
    lw_item *item = lwUtils_InitItem(lsu, index, exist, preStartOnly);
    if(item != NULL) {
        listWidget_SetItemText(item, text, NULL, true);
        listWidget_SetItemIcon(item, LW_ITEM_SWITCH_TEXT, state);
    }
}

/*****************************************************************************************************
* @brief  : 设置Assistant文本项
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_Text(lw_set_utils *lsu, uint16_t index, SportSettingsMessage *defSet, bool exist, const char *text)
{
    bool pso = itemPrestartOnly(defSet, index);
    lw_item *item = lwUtils_InitItem(lsu, index, exist, pso);
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
    AssistMessage *defSet = &ssc->rsi.def.assist;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TK_ASSISTANT));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 设置列表项 */
    setItem_Text(lsu, INDEX_GOAL,              &ssc->rsi.def,    defSet->has_target,           _ST(TK_GOAL));
    setItem_Text(lsu, INDEX_REMINDER,          &ssc->rsi.def,    defSet->has_reminder,         _ST(TK_REMINDER));
    setItem_Text(lsu, INDEX_AUTO_LAP,          &ssc->rsi.def,    defSet->has_autoLap,          _ST(TK_AUTO_ALP));
    setItem_Text(lsu, INDEX_AUTO_PAUSE,        &ssc->rsi.def,    defSet->has_autoPause,        _ST(TK_AUTO_PAUSE));
    setItem_Text(lsu, INDEX_VIRTUAL_PACER,     &ssc->rsi.def,    defSet->has_virtualPacer,     _ST(TK_VIRTUAL_PACER));
    setItem_Text(lsu, INDEX_CADENCE_ASSISTANT, &ssc->rsi.def,    defSet->has_cadenceAssistant, _ST(TK_CADENCE_ASSISTANT));
    setItem_Text(lsu, INDEX_PACE_STRATEGY,     &ssc->rsi.def,    defSet->has_paceStrategy,     _ST(TK_PACE_STRATEGY));

    setItem_Switch(lsu, INDEX_SPORT_COMPASS,   defSet->has_compass, defSet->compass.onlyPreStart, defSet->compass.onOff, _ST(TK_SPORT_COMPASS));
    setItem_Switch(lsu, INDEX_SPO2,            defSet->has_spO2,    defSet->spO2.onlyPreStart,    defSet->spO2.onOff,    _ST(TK_SPO2));

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

const sport_setting_ops setting_ops_assistant = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

