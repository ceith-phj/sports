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

#define SETTING_SCREEN              "SportSettingReminderScreen"                /**< screen 名称. */
#define SETTING_LAYER               "SportSettingReminderLayer"                 /**< layer 名称. */

#include "hlog.h"
#define TAG "[LG_SSR]"

#define OF_RMD(x)                   offsetof(SportReminder, x)
#define OT_RMD(base, offset)        (SportSettingTypePicker *)((uint8_t *)base + offset)

typedef enum _translate_type {
    TRANSLATE_NORMAL,                                                           /**< 00' 单位. */
    TRANSLATE_EVERY,                                                            /**< 01' 每 */
}translate_type;

typedef enum _list_item_index {
    INDEX_HR,                                                                   /**< 00' 安全心率. */
    INDEX_HR_ZONE,                                                              /**< 01' 心率区间. */ 
    INDEX_KILOMETER,                                                            /**< 02' 整公里. */
    INDEX_TRIP,                                                                 /**< 03' 趟数. */
    INDEX_FAST_PACE,                                                            /**< 04' 最快配速. */
    INDEX_SLOW_PACE,                                                            /**< 05' 最慢配速. */
    INDEX_FAST_CADENCE,                                                         /**< 06' 最快步频. */
    INDEX_SLOW_CADENCE,                                                         /**< 07' 最慢步频. */
    INDEX_FAST_SPEED,                                                           /**< 08' 最快速度. */
    INDEX_SLOW_SPEED,                                                           /**< 09' 最慢速度. */
    INDEX_FAST_STROKES,                                                         /**< 10' 最高划频. */
    INDEX_SLOW_STROKES,                                                         /**< 11' 最低划频. */
    INDEX_HIGH_SPEED,                                                           /**< 12' 高速度. */
    INDEX_TIME,                                                                 /**< 13' 时间. */
    INDEX_CAL,                                                                  /**< 14' 消耗. */
    INDEX_DRINK,                                                                /**< 15' 饮水. */
    INDEX_ENERGY,                                                               /**< 16' 补给. */
    INDEX_BACK,                                                                 /**< 17' 返回. */
    INDEX_MAX,                                                                  /**< 18' ITEM数量. */
}list_item_index;

typedef enum _conflict_set_type {
    SET_MAX_PACE,                                                               /**< 00' 最快配速. */
    SET_MIN_PACE,                                                               /**< 01' 最慢配速. */
    SET_MAX_CADENCE,                                                            /**< 02' 最快步频. */
    SET_MIN_CADENCE,                                                            /**< 03' 最慢步频. */
    SET_MAX_SPEED,                                                              /**< 04' 最大速度. */
    SET_MIN_SPEED,                                                              /**< 05' 最小速度. */
    SET_MAX_STROKES,                                                            /**< 06' 最大划频. */
    SET_MIN_STROKES,                                                            /**< 07' 最小划频. */
}conflict_set_type;

typedef enum _compare_minner {
    MINNER_GREAT,                                                               /**< 00' 大于. */
    MINNER_LESS,                                                                /**< 01' 小于. */
}compare_minner;

typedef struct _conflict_set_relation {
    conflict_set_type type;                                                     /**@ 冲突类型. */
    uint16_t of_set;                                                            /**@ 自己所在偏移. */
    uint16_t of_aim;                                                            /**@ 比较目标偏移. */
    compare_minner minner;                                                      /**@ 比较方式. */
    int thr;                                                                    /**@ 比较阈值. */
    picker_id id;                                                               /**@ 选择器ID. */
}conflict_set_relation;

static const conflict_set_relation conflictSetRlt[] = {
    {SET_MAX_PACE,      OF_RMD(fast_pace),      OF_RMD(low_pace),       MINNER_LESS,    5,  PICKER_ID_REMINDER_MAX_PACE},
    {SET_MIN_PACE,      OF_RMD(low_pace),       OF_RMD(fast_pace),      MINNER_GREAT,   5,  PICKER_ID_REMINDER_MIN_PACE},
    {SET_MAX_CADENCE,   OF_RMD(max_cadence),    OF_RMD(min_cadence),    MINNER_GREAT,   5,  PICKER_ID_REMINDER_MAX_CADENCE},
    {SET_MIN_CADENCE,   OF_RMD(min_cadence),    OF_RMD(max_cadence),    MINNER_LESS,    5,  PICKER_ID_REMINDER_MIN_CADENCE},
    {SET_MAX_SPEED,     OF_RMD(max_speed),      OF_RMD(min_speed),      MINNER_GREAT,   5,  PICKER_ID_REMINDER_MAX_SPEED},
    {SET_MIN_SPEED,     OF_RMD(min_speed),      OF_RMD(max_speed),      MINNER_LESS,    5,  PICKER_ID_REMINDER_MIN_SPEED},
    {SET_MAX_STROKES,   OF_RMD(max_strokes),    OF_RMD(min_strokes),    MINNER_GREAT,   2,  PICKER_ID_REMINDER_MAX_STROKES},
    {SET_MIN_STROKES,   OF_RMD(min_strokes),    OF_RMD(max_strokes),    MINNER_LESS,    2,  PICKER_ID_REMINDER_MIN_STROKES},
};

/*****************************************************************************************************
* @brief  : 切换到心率区间SCREEN
*
* @param  : None
* @return : None
******************************************************************************************************/
static void switchHrZoneScreen(sport_setting_container *ssc)
{
    sport_hr_range_setting_t hrInfo;
    sportSetting_getSettingManager()->hrZone->getRange(&hrInfo);

    if(hrInfo.type == SPORT_HR_RANGE_SETTING_MAXIMUM)
        switchSettingScreen("SportSettingMaxHrZoneScreen", &ssc->rsi.set, ssc->change);
    else
        switchSettingScreen("SportSettingReserveHrZoneScreen", &ssc->rsi.set, ssc->change);
}

/*****************************************************************************************************
* @brief  : 处理列表选中事件
*
* @param  : 单选框控件
* @param  : 当前焦点位置
* @param  : 上一次焦点位置, -1代表无焦点
* @return : None
******************************************************************************************************/
static void _listTextEvent(sport_setting_container *ssc, uint16_t index)
{
    switch(index)
    {
    case INDEX_HR:
        switchPickerScreen(PICKER_ID_REMINDER_SAFE_HR, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_HR_ZONE:
        switchHrZoneScreen(ssc);
        break;

    case INDEX_KILOMETER:
        switchPickerScreen(PICKER_ID_REMINDER_DISTANCE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_TRIP:
        switchPickerScreen(PICKER_ID_REMNIDER_TRIPS, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_FAST_PACE:
        switchPickerScreen(PICKER_ID_REMINDER_MAX_PACE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_SLOW_PACE:
        switchPickerScreen(PICKER_ID_REMINDER_MIN_PACE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_FAST_CADENCE:
        switchPickerScreen(PICKER_ID_REMINDER_MAX_CADENCE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_SLOW_CADENCE:
        switchPickerScreen(PICKER_ID_REMINDER_MIN_CADENCE, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_FAST_SPEED:
        switchPickerScreen(PICKER_ID_REMINDER_MAX_SPEED, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_SLOW_SPEED:
        switchPickerScreen(PICKER_ID_REMINDER_MIN_SPEED, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_FAST_STROKES:
        switchPickerScreen(PICKER_ID_REMINDER_MAX_STROKES, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_SLOW_STROKES:
        switchPickerScreen(PICKER_ID_REMINDER_MIN_STROKES, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_HIGH_SPEED:
        switchPickerScreen(PICKER_ID_REMINDER_HIGH_SPEED, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_TIME:
        switchPickerScreen(PICKER_ID_REMINDER_TIME, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_CAL:
        switchPickerScreen(PICKER_ID_REMINDER_CALORIES, &ssc->rsi.set, ssc->change);
        break;

    case INDEX_DRINK:
        switchSettingScreen("SportSettingDrinkScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_ENERGY:
        switchSettingScreen("SportSettingEnergyScreen", &ssc->rsi.set, ssc->change);
        break;

    case INDEX_BACK:
        switchSettingScreen("SportSettingBackScreen", &ssc->rsi.set, ssc->change);
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 处理选择事件
*
* @param  : 单选框控件
* @param  : 当前焦点位置
* @param  : 上一次焦点位置, -1代表无焦点
* @return : None
******************************************************************************************************/
static void _checkBoxEvent_ChoiceType(void *set, bool state)
{
    sportBackToRemind *rawSet = set;

    if(state == false)
    {
        rawSet->back.time.onOff = false;
        rawSet->back.distance.onOff = false;
        rawSet->back.calories.onOff = false;
        return;
    }

    switch(rawSet->choice)
    {
    case 0:
        rawSet->back.time.onOff = true;
        break;

    case 1:
        rawSet->back.distance.onOff = true;
        break;

    case 2:
        rawSet->back.calories.onOff = true;
        break;
    }
}

/*****************************************************************************************************
* @brief  : 判断是否冲突
*
* @param  : 设置
* @param  : 比较值
* @return : None
******************************************************************************************************/
static void checkConflictSet(conflict_set_type type, SportSettingsMessage *ssm, bool state)
{
    float max, min;
    SportSettingTypePicker *set, *aim;
    SportReminder *rawSet = &ssm->assist.reminder;
    const conflict_set_relation *rlt = &conflictSetRlt[type];

    set = OT_RMD(rawSet, rlt->of_set);
    aim = OT_RMD(rawSet, rlt->of_aim);

    /* 取消选中直接返回 */
    if(state == false) {
        set->onOff = false; 
        return;
    }

    /* 设置比较方式 */
    (rlt->minner == MINNER_GREAT) ? (max = set->data, min = aim->data):
                                    (max = aim->data, min = set->data);

    /* 判断是否需要跳入选择器 */
    if(max - min < rlt->thr)
        switchPickerScreen(rlt->id, ssm, true);
    else
        set->onOff = true;
}

/*****************************************************************************************************
* @brief  : 处理列未选中事件
*
* @param  : 单选框控件
* @param  : 当前焦点位置
* @param  : 上一次焦点位置, -1代表无焦点
* @return : None
******************************************************************************************************/
static bool _listCheckBoxEvent(SportSettingsMessage *ssm, uint16_t index,  bool state)
{
    SportReminder *rawSet = &ssm->assist.reminder;

    switch(index)
    {
    case INDEX_HR:
        rawSet->safeHeartRate.heartRate.onOff = state;
        break;

    case INDEX_HR_ZONE:
        rawSet->heartRateZone.hrRange.onOff = state;
        break;

    case INDEX_KILOMETER:
        rawSet->kilometer.onOff = state;
        break;

    case INDEX_TRIP:
        rawSet->trip.onOff = state;
        break;

    case INDEX_FAST_PACE:
        checkConflictSet(SET_MAX_PACE, ssm, state);
        break;

    case INDEX_SLOW_PACE:
        checkConflictSet(SET_MIN_PACE, ssm, state);
        break;

    case INDEX_FAST_CADENCE:
        checkConflictSet(SET_MAX_CADENCE, ssm, state);
        break;

    case INDEX_SLOW_CADENCE:
        checkConflictSet(SET_MIN_CADENCE, ssm, state);
        break;

    case INDEX_FAST_SPEED:
        checkConflictSet(SET_MAX_SPEED, ssm, state);
        break;

    case INDEX_SLOW_SPEED:
        checkConflictSet(SET_MIN_SPEED, ssm, state);
        break;

    case INDEX_FAST_STROKES:
        checkConflictSet(SET_MAX_STROKES, ssm, state);
        break;

    case INDEX_SLOW_STROKES:
        checkConflictSet(SET_MIN_STROKES, ssm, state);
        break;

    case INDEX_HIGH_SPEED:
        rawSet->highSpeedWarning.onOff = state;
        break;

    case INDEX_TIME:
        rawSet->time.onOff = state;
        break;

    case INDEX_CAL:
        rawSet->calories.onOff = state;
        break;

    case INDEX_DRINK:
        _checkBoxEvent_ChoiceType(&rawSet->drinking, state);
        break;

    case INDEX_ENERGY:
        _checkBoxEvent_ChoiceType(&rawSet->energy, state);
        break;

    case INDEX_BACK:
        _checkBoxEvent_ChoiceType(&rawSet->backTo, state);
        break;

    default:
        return false;
    }

    return true;
}

/*****************************************************************************************************
* @brief  : 列表控件的事件
*
* @param  : 单选框控件
* @param  : 当前焦点位置
* @param  : 上一次焦点位置, -1代表无焦点
* @return : None
******************************************************************************************************/
static void _lwEvent(list_widget *lw, uint16_t index, lw_event_type event, bool state)
{
    sport_setting_container *ssc = lw->attr.param;
    list_setting_context *context = (list_setting_context *)&ssc->context;
    uint8_t *imap = lwUtils_GetMap(&context->lsu);

    /* 处理事件 */
    if(event == LW_EVENT_CHECK_BOX)
        ssc->change |= _listCheckBoxEvent(&ssc->rsi.set, imap[index], state);
    else
        _listTextEvent(ssc, imap[index]);
}

/*****************************************************************************************************
* @brief  : 设置单选ITEM
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_CheckText(lw_item *item, const char *text1, const char *text2, bool state)
{
    listWidget_SetItemText(item, text1, text2, true);
    listWidget_SetItemIcon(item, LW_ITEM_CHECK_TEXT, state);
}

/*****************************************************************************************************
* @brief  : 设置ITEM - HR ZONE
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_SafeHrType(lw_set_utils *lsu, uint16_t index, bool exist, SportReminder *defSet, SportReminder *rawSet,
                                            const char *text, const char *valKey, char *val)
{
    lw_item *item;
    uint8_t threshold;

    /* 计算阈值 */
    threshold = getHeartRateThreshold(&rawSet->safeHeartRate.heartRate);

    item = lwUtils_InitItem(lsu, index, exist, defSet->safeHeartRate.heartRate.has_onlyPreStart & defSet->safeHeartRate.heartRate.onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, valKey, threshold);
        setItem_CheckText(item, text, val, rawSet->safeHeartRate.heartRate.onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - HR ZONE
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_HrZoneType(lw_set_utils *lsu, uint16_t index, bool exist, SportReminderHRZone *defSet, SportReminderHRZone *rawSet,
                                            const char *text, const char *valKey, char *val)
{
    lw_item *item;
    uint8_t hr_min, hr_max;
    sport_hr_range_setting_t hrInfo;

    /* 获取心率区间 */
    sportSetting_getSettingManager()->hrZone->getRange(&hrInfo);
    (rawSet->hrRange.choice != 6) ? 
                (hr_min = hrInfo.value[rawSet->hrRange.choice - 1], hr_max = hrInfo.value[rawSet->hrRange.choice]):
                (hr_min = rawSet->customHR.min, hr_max = rawSet->customHR.max);

    item = lwUtils_InitItem(lsu, index, exist, defSet->hrRange.has_onlyPreStart & defSet->hrRange.onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, valKey, hr_min, hr_max);
        setItem_CheckText(item, text, val, rawSet->hrRange.onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置提醒类型的ITEM
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_PickerType(lw_set_utils *lsu, uint16_t index, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet,
                                            const char *text, const char *valKey, char *val)
{
    lw_item *item;

    item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        snprintf(val, SZ_SET_BUF, valKey, (int32_t)rawSet->data);
        setItem_CheckText(item, text, val, rawSet->onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置分秒类型的
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_MinSecType(lw_set_utils *lsu, uint16_t index, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet,
                                            const char *text, const char *valKey, char *val)
{
    lw_item *item;
    uint16_t raw, min, sec;

    item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        raw = rawSet->data;
        min = raw / 60;
        sec = raw % 60;
        snprintf(val, SZ_SET_BUF, valKey, min, sec);
        setItem_CheckText(item, text, val, rawSet->onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置ITEM - 时分类
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_TimeHmType(lw_set_utils *lsu, uint16_t index, bool exist, SportSettingTypePicker *defSet, SportSettingTypePicker *rawSet,
                                            const char *text, const char *valKey, char *val)
{
    lw_item *item;
    uint16_t raw, hour, min;

    item = lwUtils_InitItem(lsu, index, exist, defSet->has_onlyPreStart & defSet->onlyPreStart);
    if(item != NULL) {
        raw = rawSet->data;
        hour = raw / 60;
        min =  raw % 60;
        snprintf(val, SZ_SET_BUF, valKey, hour, min);
        setItem_CheckText(item, text, val, rawSet->onOff);
    }
}

/*****************************************************************************************************
* @brief  : 设置类型 - 选择类型
*
* @param  : None
* @return : 列表控件
******************************************************************************************************/
static void setItem_ChoiceType(lw_set_utils *lsu, uint16_t index, bool exist, void *defSet, void *rawSet, const char *text, translate_type type, char *val)
{
    bool select = false;
    lw_item *item;
    SportSettingTypePicker *def_picker;
    SportSettingTypePicker *raw_picker;
    sportBackToRemind *def = (sportBackToRemind *)defSet;
    sportBackToRemind *raw = (sportBackToRemind *)rawSet;
    SportSettingUseWorkoutStatistics *def_uws = &def->back;
    SportSettingUseWorkoutStatistics *raw_uws = &raw->back;
    const char *translate;

    switch(raw->choice)
    {
    case 0:
        def_picker = &def_uws->time;
        raw_picker = &raw_uws->time;
        translate = (type == TRANSLATE_NORMAL) ? _ST(TK_ARG_HOUR_MIN) : _ST(TK_ARG_PER_HOUR_MIN);
        snprintf(val, SZ_SET_BUF, translate, (int)raw_picker->data / 60, (int)raw_picker->data % 60);
        break;

    case 1:
        def_picker = &def_uws->distance;
        raw_picker = &raw_uws->distance;
        translate = (type == TRANSLATE_NORMAL) ? choiceUnitText(_ST(TK_ARG_KM), _ST(TK_ARG_MILE)) :
                                                 choiceUnitText(_ST(TK_ARG_PER_KM), _ST(TK_ARG_PER_MILE));
        snprintf(val, SZ_SET_BUF, translate, (int32_t)raw_picker->data);
        break;

    case 2:
        def_picker = &def_uws->calories;
        raw_picker = &raw_uws->calories;
        translate = (type == TRANSLATE_NORMAL) ? _ST(TK_ARG_KCAL) : _ST(TK_ARG_PER_KCAL);
        snprintf(val, SZ_SET_BUF, _ST(TK_ARG_PER_KCAL), (int32_t)raw_picker->data);
        break;

    default:
        return;
    }

    /* 有选中项, 则勾选 */
    if(raw_uws->time.onOff || raw_uws->distance.onOff || raw_uws->calories.onOff)
        select = true;

    /* 初始化ITEM */
    item = lwUtils_InitItem(lsu, index, exist, def_picker->has_onlyPreStart & def_picker->onlyPreStart);
    if(item != NULL) {
        setItem_CheckText(item, text, val, select);
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
    char value[INDEX_MAX][50] = {0};
    lw_item itemList[INDEX_MAX] = {0};

    /* 设置指针 */
    list_setting_context *context = (list_setting_context *)&ssc->context;
    SportReminder *defSet = &ssc->rsi.def.assist.reminder;
    SportReminder *rawSet = &ssc->rsi.set.assist.reminder;
    lw_set_utils *lsu = &context->lsu;

    /* 创建列表控件 */
    context->lw = listWidget_Create(SETTING_SCREEN, SETTING_LAYER, _lwEvent);
    listWidget_SetListAttr(context->lw, LW_ATTR_TITLE, (void *)_ST(TK_REMINDER));
    listWidget_SetListAttr(context->lw, LW_ATTR_PARAM, ssc);
    lwUtils_Init(lsu, itemList, INDEX_MAX);

    /* 兼容protobuf修改 */
    rawSet->has_time = (defSet->has_time) ? true : false;

    /* 设置ITEM */
    setItem_SafeHrType(lsu,  INDEX_HR,           defSet->has_safeHeartRate,    defSet,                      rawSet,                     _ST(TK_HR),           _ST(TK_ARG_OVER_CPM),      value[i++]);
    setItem_HrZoneType(lsu,  INDEX_HR_ZONE,      defSet->has_heartRateZone,    &defSet->heartRateZone,      &rawSet->heartRateZone,     _ST(TK_HR_ZONE),      _ST(TK_ARG_OVER_ZONE_CPM), value[i++]);
    setItem_PickerType(lsu,  INDEX_KILOMETER,    defSet->has_kilometer,        &defSet->kilometer,          &rawSet->kilometer,         _ST(TK_KILOMETER),    choiceUnitText(_ST(TK_ARG_PER_KM), _ST(TK_ARG_PER_MILE)),  value[i++]);
    setItem_PickerType(lsu,  INDEX_TRIP,         defSet->has_trip,             &defSet->trip,               &rawSet->trip,              _ST(TK_TRIP),         _ST(TK_ARG_PER_TRIP),      value[i++]);
    setItem_MinSecType(lsu,  INDEX_FAST_PACE,    defSet->has_fast_pace,        &defSet->fast_pace,          &rawSet->fast_pace,         _ST(TK_FAST_PACE),    _ST(TK_ARG_HIGHER_PACE),   value[i++]);
    setItem_MinSecType(lsu,  INDEX_SLOW_PACE,    defSet->has_low_pace,         &defSet->low_pace,           &rawSet->low_pace,          _ST(TK_SLOW_PACE),    _ST(TK_ARG_LOWER_PACE),    value[i++]);
    setItem_PickerType(lsu,  INDEX_FAST_CADENCE, defSet->has_max_cadence,      &defSet->max_cadence,        &rawSet->max_cadence,       _ST(TK_FAST_CADENCE), _ST(TK_ARG_HIGHER),        value[i++]);
    setItem_PickerType(lsu,  INDEX_SLOW_CADENCE, defSet->has_min_cadence,      &defSet->min_cadence,        &rawSet->min_cadence,       _ST(TK_SLOW_CADENCE), _ST(TK_ARG_LOWER),         value[i++]);
    setItem_PickerType(lsu,  INDEX_FAST_SPEED,   defSet->has_max_speed,        &defSet->max_speed,          &rawSet->max_speed,         _ST(TK_FAST_SPEED),   _ST(TK_ARG_HIGHER),        value[i++]);
    setItem_PickerType(lsu,  INDEX_SLOW_SPEED,   defSet->has_min_speed,        &defSet->min_speed,          &rawSet->min_speed,         _ST(TK_SLOW_SPEED),   _ST(TK_ARG_LOWER),         value[i++]);
    setItem_PickerType(lsu,  INDEX_FAST_STROKES, defSet->has_max_strokes,      &defSet->max_strokes,        &rawSet->max_strokes,       _ST(TK_FAST_STROKES), _ST(TK_ARG_HIGHER),        value[i++]);
    setItem_PickerType(lsu,  INDEX_SLOW_STROKES, defSet->has_min_strokes,      &defSet->min_strokes,        &rawSet->min_strokes,       _ST(TK_SLOW_STROKES), _ST(TK_ARG_LOWER),         value[i++]);
    setItem_PickerType(lsu,  INDEX_HIGH_SPEED,   defSet->has_highSpeedWarning, &defSet->highSpeedWarning,   &rawSet->highSpeedWarning,  _ST(TK_HIGH_SPEED),   _ST(TK_ARG_HIGHER),        value[i++]);
    setItem_TimeHmType(lsu,  INDEX_TIME,         defSet->has_time,             &defSet->time,               &rawSet->time,              _ST(TK_TIME),         _ST(TK_ARG_HOUR_MIN),      value[i++]);
    setItem_PickerType(lsu,  INDEX_CAL,          defSet->has_calories,         &defSet->calories,           &rawSet->calories,          _ST(TK_CAL),          _ST(TK_ARG_PER_KCAL),      value[i++]);
    setItem_ChoiceType(lsu,  INDEX_DRINK,        defSet->has_drinking,         &defSet->drinking,           &rawSet->drinking,          _ST(TK_DRINK),        TRANSLATE_EVERY,           value[i++]);
    setItem_ChoiceType(lsu,  INDEX_ENERGY,       defSet->has_energy,           &defSet->energy,             &rawSet->energy,            _ST(TK_ENERGY),       TRANSLATE_EVERY,           value[i++]);
    setItem_ChoiceType(lsu,  INDEX_BACK,         defSet->has_backTo,           &defSet->backTo,             &rawSet->backTo,            _ST(TK_BACK),         TRANSLATE_NORMAL,          value[i++]);

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

const sport_setting_ops setting_ops_reminder = {
    .create  = _settingCreate,
    .destory = _settingDestory,
    .event   = _settingEvent,
};

