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
#include "sport_engine_utils.h"

#define to_set(psi)             &psi->ssm.assist.reminder.highSpeedWarning

/*****************************************************************************************************
* @brief  : 选择器勾选事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _pickerCheckEvent(picker_screen_info *psi)
{
    PickerWidget *wgt = psi->picker;
    SportSettingTypePicker *set = to_set(psi);

    /* 组合数据 */
    set->data = wgt->tool.getNumberByIndex(wgt->index[0], set->min, set->max, set->interval);

    /* 保存设置 */
    set->onOff = true;
    saveSportSetting(&psi->ssm, true);
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
* @brief  : 获取副标题
*
* @param  : None
* @return : None
******************************************************************************************************/
static void getPickerParamInfo(picker_screen_info *psi, const char **str_unit, int *initVal)
{
    SportSettingTypePicker *set = to_set(psi);

    /* 数字不关心公英制 */
    *str_unit = _UT(KEY_KPH_MIPH);
    *initVal = set->data;
}

/*****************************************************************************************************
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _settingCreate(picker_screen_info *psi)
{
    int initVal;
    char *item_0;
    const char *str_unit;
    pickerInitData pdata = {0};
    SportSettingTypePicker *set = to_set(psi);

    getPickerParamInfo(psi, &str_unit, &initVal);
    psi->picker = createPicker(psi->sc, &pdata, PICKER_SINGLE_WIDE, _ST(TK_COM_SET_SPEED), str_unit);

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, NULL, NULL, true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, set->min, set->max, set->interval, initVal);
    if(item_0 == NULL)
        return;

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    /* 释放内存 */
    H_FREE(item_0);
}

const picker_ops picker_ops_reminder_high_speed = {
    .create = _settingCreate,
};