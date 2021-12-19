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

#define ARG_PICKER_1            0, 9, 1                                     /**< 第1列选择器. */
#define to_set(psi)             &psi->ssm.assist.target.useTrainingEffect.anaerobicTE

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
    rawSet->useTrainingEffect.anaerobicTE.onOff = true;
    rawSet->useTrainingEffect.aerobicTE.onOff = false;
    rawSet->useWorkoutStatistics.distance.onOff = false;
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
    float data_0, data_1;
    SportSettingTypePicker *set = to_set(psi);
    PickerWidget *wgt = psi->picker;

    /* 组合数据 */
    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], set->min, set->max, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], ARG_PICKER_1);
    set->data = data_0 + data_1 / 10;

    saveSetting(psi);
}

/*****************************************************************************************************
* @brief  : 根据数值计算副标题
*
* @param  : None
* @return : None
******************************************************************************************************/
static char *getSubTitle(float data)
{
    if(data < 1)
        return TK_EFFECT_NO;
    else if(data >= 1 && data < 2)
        return TK_EFFECT_MINOR;
    else if(data >= 2 && data < 3)
        return TK_EFFECT_MAINTAINING;
    else if(data >= 3 && data < 4)
        return TK_EFFECT_IMPROVING;
    else if(data >= 4 && data < 5)
        return TK_HIGHLY_IMPROVING;
    else
        return TK_OVER_REACHING;
}

/*****************************************************************************************************
* @brief  : 检测是否更新副标题
*
* @param  : None
* @return : None
******************************************************************************************************/
static void checkUpdateSubTitle(PickerWidget *wgt, float data)
{
    char *tk = getSubTitle(data);
    wgt->updataSubTitle(wgt, (char *)_ST(tk));
}

/*****************************************************************************************************
* @brief  : 验证数据合法性
*
* @param  : None
* @return : None
******************************************************************************************************/
static void checkVaildData(picker_screen_info *psi)
{
    float data, data_0, data_1;
    PickerWidget *wgt = psi->picker;
    SportSettingTypePicker *set = to_set(psi);

    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], set->min, set->max, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], ARG_PICKER_1);
    data   = data_0 + data_1 / 10;

    /* 修正非法值 */
    if(data_0 == 0 && data_1 == 0) {
        wgt->index[1] = wgt->tool.getNumberIndex(1, set->min / 60, set->max / 60, 1);
        wgt->setValue(wgt, true);
    }
    else if(data_0 == 5 && data_1 != 0) {
        wgt->index[1] = wgt->tool.getNumberIndex(0, set->min / 60, set->max / 60, 1);
        wgt->setValue(wgt, true);
    }

    /* 更新副标题 */
    checkUpdateSubTitle(wgt, data);
}

/*****************************************************************************************************
* @brief  : 选择器回调
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _pickerEvent(PickerWidget *picker, pickerEventTypeE event, pickerUpdateInfo *updateInfo)
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
        checkVaildData(psi);
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
static void getInitVal(SportSettingTypePicker *set, int *val)
{
    double integer, fract;

    fract = modf(set->data, &integer);
    fract = fract * 10 + 0.05;

    val[0] = integer;
    val[1] = fract;
}

/*****************************************************************************************************
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _settingCreate(picker_screen_info *psi)
{
    int initVal[2];
    const char *subtitle;
    char *item_0, *item_1;
    pickerInitData pdata = {0};
    SportSettingTypePicker *set = to_set(psi);

    /* 计算初始值 */
    getInitVal(to_set(psi), initVal);
    
    /* 创建选择器 */
    subtitle = getSubTitle(set->data);
    psi->picker = createPicker(psi->sc, &pdata, PICKER_DOUBLE, _ST(TK_ANAEROBIC_TE), _ST(subtitle));

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, ".", NULL, true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, set->min, set->max, 1, initVal[0]);
    if(item_0 == NULL)
        return;

    /* 设置列 2 */
    setPicker_ItemAttr(psi->picker, &pdata, 1, NULL, NULL, true, false);
    item_1 = setPicker_ItemNum(psi->picker, &pdata, 1, 0, 9, 1, initVal[1]);
    if(item_1 == NULL)
        goto err1;

    /* 更新副标题 */
    checkUpdateSubTitle(psi->picker, set->data);

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    H_FREE(item_1);
err1:
    H_FREE(item_0);
}

const picker_ops picker_ops_anaerobic_te = {
    .create = _settingCreate,
};

