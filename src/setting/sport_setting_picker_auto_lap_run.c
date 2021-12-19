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

#define ARG_PICKER_1            0, 9, 1
#define to_set(psi)             &psi->ssm.assist.autoLap.distance

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

    /* 保存设置 */
    set->onOff = true;
    saveSportSetting(&psi->ssm, true);    
}

/*****************************************************************************************************
* @brief  : 验证数据合法性
*
* @param  : None
* @return : None
******************************************************************************************************/
static void checkVaildData(picker_screen_info *psi)
{
    float data_0, data_1;
    PickerWidget *wgt = psi->picker;
    SportSettingTypePicker *set = to_set(psi);

    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], set->min, set->max, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], ARG_PICKER_1);

    if(data_0 == 0 && data_1 == 0)
    {
        wgt->index[1] = wgt->tool.getNumberIndex(1, set->min / 60, set->max / 60, 1);
        wgt->setValue(wgt, true);
        return;
    }

    if(data_0 == set->max && data_1 != 0)
    {
        wgt->index[1] = wgt->tool.getNumberIndex(0, set->min / 60, set->max / 60, 1);
        wgt->setValue(wgt, true);
        return;
    }
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
    char *item_0, *item_1;
    pickerInitData pdata = {0};
    SportSettingTypePicker *set = to_set(psi);

    /* 计算初始值 */
    getInitVal(set, initVal);

    /* 创建选择器 */
    psi->picker = createPicker(psi->sc, &pdata, PICKER_DOUBLE, _ST(TK_AUTO_LAP_DISTANCE), NULL);

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, ".", NULL, true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, set->min, set->max, 1, initVal[0]);
    if(item_0 == NULL)
        return;

    /* 设置列 2 */
    setPicker_ItemAttr(psi->picker, &pdata, 1, NULL, _UT(KEY_KM_MILE), true, false);
    item_1 = setPicker_ItemNum(psi->picker, &pdata, 1, ARG_PICKER_1, initVal[1]);
    if(item_1 == NULL)
        goto err1;

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    H_FREE(item_1);
err1:
    H_FREE(item_0);
}

const picker_ops picker_ops_auto_lap_run = {
    .create = _settingCreate,
};

