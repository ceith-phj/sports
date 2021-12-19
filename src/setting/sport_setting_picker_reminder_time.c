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

#define ARG_PICKER_1            0, 59, 1                                    /**< 第二列选择器. */
#define to_set(psi)             &psi->ssm.assist.reminder.time

/*****************************************************************************************************
* @brief  : 选择器勾选事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _pickerCheckEvent(picker_screen_info *psi)
{
    int data_0, data_1;
    SportSettingTypePicker *set = to_set(psi);
    PickerWidget *wgt = psi->picker;

    /* 组合数据 */
    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], set->min / 60, set->max / 60, set->interval);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], ARG_PICKER_1);
    set->data = (data_0 * 60 + data_1);

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

    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], set->min / 60, set->max / 60, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], ARG_PICKER_1);

    if(data_0 == 0 && data_1 < set->min)
    {
        wgt->index[1] = wgt->tool.getNumberIndex(set->min, set->min / 60, set->max / 60, 1);
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
static void inline getInitVal(SportSettingTypePicker *set, int *val)
{
    int time = set->data;

    val[0] = time / 60;
    val[1] = time % 60;
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
    psi->picker = createPicker(psi->sc, &pdata, PICKER_DOUBLE, _ST(TK_COM_SET_TIME), NULL);

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, NULL, _GT(TEXT_ID_HOUR), true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, set->min / 60, set->max / 60, set->interval, initVal[0]);
    if(item_0 == NULL)
        return;

    /* 设置列 2 */
    setPicker_ItemAttr(psi->picker, &pdata, 1, NULL, _GT(TEXT_ID_MIN), true, false);
    item_1 = setPicker_ItemNum(psi->picker, &pdata, 1, ARG_PICKER_1, initVal[1]);
    if(item_1 == NULL)
        goto err1;

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    /* 释放内存 */
    H_FREE(item_1);
err1:
    H_FREE(item_0);
}

const picker_ops picker_ops_reminder_time = {
    .create = _settingCreate,
};