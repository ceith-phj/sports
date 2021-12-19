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

#define to_set(psi)             &psi->ssm.assist.reminder.safeHeartRate.heartRate

/*****************************************************************************************************
* @brief  : 保存设置
*
* @param  : None
* @return : None
******************************************************************************************************/
static void saveSetting(picker_screen_info *psi)
{
    SportSettingTypePicker *set = to_set(psi);
    set->onOff = true;

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
    sport_set_info hrInfo;
    SportSettingTypePicker *set = to_set(psi);
    PickerWidget *wgt = psi->picker;

    /* 获取安全心率 */
    getSafeHrInfo(set, &hrInfo);

    /* 组合数据 */
    set->data = wgt->tool.getNumberByIndex(wgt->index[0], hrInfo.min, hrInfo.max, set->interval);

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
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _settingCreate(picker_screen_info *psi)
{
    char *item_0;
    sport_set_info hrInfo;
    pickerInitData pdata = {0};
    SportSettingTypePicker *set = to_set(psi);

    /* 计算初始值 */
    getSafeHrInfo(set, &hrInfo);

    /* 创建选择器 */
    psi->picker = createPicker(psi->sc, &pdata, PICKER_SINGLE_WIDE, _ST(TK_SET_SFAE_HR), NULL);

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, NULL, _GT(TEXT_ID_HR), true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, hrInfo.min, hrInfo.max, set->interval, hrInfo.val);
    if(item_0 == NULL)
        return;

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    /* 释放内存 */
    H_FREE(item_0);
}

const picker_ops picker_ops_reminder_safe_hr = {
    .create = _settingCreate,
};