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

#define to_set(psi)             &psi->ssm.assist.reminder.heartRateZone.customHR

#define HR_ZONE_DIFF            5                                               /**< 区间比较. */
#define HR_MAX_MIN_DIFF         2                                               /**< 最大值大于最小值. */

/*****************************************************************************************************
* @brief  : 保存设置
*
* @param  : None
* @return : None
******************************************************************************************************/
static void saveSetting(picker_screen_info *psi)
{
    psi->ssm.assist.reminder.heartRateZone.onOff = true;
    psi->ssm.assist.reminder.heartRateZone.hrRange.choice = 6;
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
    sport_hr_range_setting_t hrInfo;
    sportSetting_getSettingManager()->hrZone->getRange(&hrInfo);

    /* 组合数据 */
    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], hrInfo.value[0], hrInfo.value[5] - HR_ZONE_DIFF, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], hrInfo.value[0] + HR_ZONE_DIFF, hrInfo.value[5], 1);

    set->min = data_0;
    set->max = data_1;

    /* 保存设置 */
    saveSetting(psi);
}

/*****************************************************************************************************
* @brief  : 验证数据合法性
*
* @param  : None
* @return : None
******************************************************************************************************/
static int checkVaildData(picker_screen_info *psi)
{
    float data_0, data_1;
    PickerWidget *wgt = psi->picker;
    sport_hr_range_setting_t hrInfo;
    sportSetting_getSettingManager()->hrZone->getRange(&hrInfo);

    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], hrInfo.value[0], hrInfo.value[5] - HR_ZONE_DIFF, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], hrInfo.value[0] + HR_ZONE_DIFF, hrInfo.value[5], 1);

    if(data_0 > data_1 - HR_MAX_MIN_DIFF)
    {
        wgt->index[1] = wgt->tool.getNumberIndex(data_0 + HR_MAX_MIN_DIFF, hrInfo.value[0] + HR_ZONE_DIFF, hrInfo.value[5], 1);
        wgt->setValue(wgt, true);
        return false;
    }

    return true;
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
        if(checkVaildData(psi)) {
            _pickerCheckEvent(psi);
            getScreenManager()->finishCurrentScreen();
        }
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
    char *item_0, *item_1;
    sport_hr_range_setting_t hrInfo;
    pickerInitData pdata = {0};
    SportSettingTypePicker *set = to_set(psi);

    /* 获取心率区间 */
    sportSetting_getSettingManager()->hrZone->getRange(&hrInfo);

    /* 自定义需要先判断一下是否有默认值 */
    if(set->min == 10 && set->max == 200) {
        set->min = hrInfo.value[2];
        set->max = hrInfo.value[3];
    }

    /* 创建选择器 */
    psi->picker = createPicker(psi->sc, &pdata, PICKER_DOUBLE, _ST(TK_SET_HR_ZONE), NULL);

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, "-", _ST(TEXT_ID_KCAL), true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, hrInfo.value[0], hrInfo.value[5] - 5, 1, set->min);
    if(item_0 == NULL)
        return;

    /* 设置列 2 */
    setPicker_ItemAttr(psi->picker, &pdata, 1, NULL, NULL, true, false);
    item_1 = setPicker_ItemNum(psi->picker, &pdata, 1, hrInfo.value[0] + 5, hrInfo.value[5], 1, set->max);
    if(item_1 == NULL)
        goto err1;

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    /* 释放内存 */
    H_FREE(item_1);
err1:
    H_FREE(item_0);
}

const picker_ops picker_ops_reminder_hr_zone = {
    .create = _settingCreate,
};