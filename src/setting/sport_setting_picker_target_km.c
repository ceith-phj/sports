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

#include "ui/Sports/include/setting/sport_setting_picker_screen.h"


#define to_set(psi)             &psi->ssm.assist.target.useWorkoutStatistics.distance
#define ARG_PICKER_VAR          arg.start, arg.end, arg.step                    /**< 参数类. */

static const picker_data_arg argRun  = {1, 200, 1};                             /**# 骑行. */
static const picker_data_arg argRide = {5, 500, 5};                             /**# 跑走. */

/* 骑行类参数 */
static const int sportRideType[] = {
    SPORT_GROUP_OUTDOOR_CYCLING,
    SPORT_GROUP_OUTDOOR_ROWING,
    SPORT_GROUP_DRIVING,
};

/*****************************************************************************************************
* @brief  : 设置选择器参数
*
* @param  : None
* @return : None
******************************************************************************************************/
static void setPickerArg(picker_data_arg *arg)
{
    sport_group_t group = getSportGroup();

    if(checkTypeIsMatch(group, sportRideType, ARRAY_SIZE(sportRideType)))
        memcpy(arg, &argRide, sizeof(picker_data_arg));
    else
        memcpy(arg, &argRun,  sizeof(picker_data_arg));
}

/*****************************************************************************************************
* @brief  : 保存设置
*
* @param  : None
* @return : None
******************************************************************************************************/
static void saveSetting(picker_screen_info *psi)
{
    SportTarget *rawSet = &psi->ssm.assist.target;

    rawSet->noGoals.onOff = false;
    rawSet->useTrainingEffect.anaerobicTE.onOff = false;
    rawSet->useTrainingEffect.aerobicTE.onOff = false;
    rawSet->useWorkoutStatistics.distance.onOff = true;
    rawSet->useWorkoutStatistics.time.onOff = false;
    rawSet->useWorkoutStatistics.calories.onOff = false;
    rawSet->useWorkoutStatistics.count.onOff = false;
    rawSet->useWorkoutStatistics.trips.onOff = false;

    /* 保存设置 */
    saveSportSetting(&psi->ssm, true);
}

/*****************************************************************************************************
* @brief  : 选择器勾选事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _pickerCheckEvent(picker_screen_info *psi)
{
    SportSettingTypePicker *set = to_set(psi);
    PickerWidget *wgt = psi->picker;
    picker_data_arg arg;

    /* 组合数据 */
    setPickerArg(&arg);
    set->data = wgt->tool.getNumberByIndex(wgt->index[0], ARG_PICKER_VAR);

    /* 保存设置 */
    saveSetting(psi);
}

/*****************************************************************************************************
* @brief  : 选择器回调
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _pickerEvent(PickerWidget *wgt, pickerEventTypeE event, pickerUpdateInfo *updateInfo)
{
    Screen *screenInstance = getScreen(SPORT_PICKER_SCREEN_NAME);
    picker_screen_info *psi = getScreenData(getScreenContext(screenInstance), picker_screen_info);
    
    switch(event)
    {
    case PICKER_COMPLETE:
        _pickerCheckEvent(psi);
        getScreenManager()->finishCurrentScreen();
        break;

    case PICKER_CANCEL:
        getScreenManager()->finishCurrentScreen();
        break;

    case PICKER_UPDATE:
        break;

    default:
        break;
    }
}

/*****************************************************************************************************
* @brief  : 计算初始化值
*
* @param  : None
* @return : None
******************************************************************************************************/
static void inline getInitVal(SportSettingTypePicker *set, int *val)
{
    *val = set->data;
}

/*****************************************************************************************************
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _settingCreate(picker_screen_info *psi)
{
    char *item_0;
    int initVal;
    picker_data_arg arg;
    pickerInitData pdata = {0};

    /* 计算初始值 */
    setPickerArg(&arg);
    getInitVal(to_set(psi), &initVal);

    /* 创建选择器 */
    psi->picker = createPicker(psi->sc, &pdata, PICKER_SINGLE, _ST(TK_TARGET_DISTANCE), NULL);

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, NULL, _UT(KEY_KM_MILE), true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, ARG_PICKER_VAR, initVal);
    if(item_0 == NULL)
        return;

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    /* 释放内存 */
    H_FREE(item_0);
}

const picker_ops picker_ops_target_km = {
    .create = _settingCreate,
};

