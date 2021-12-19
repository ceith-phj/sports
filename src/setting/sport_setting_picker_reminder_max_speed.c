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

#define to_set(psi)             &psi->ssm.assist.reminder.max_speed

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
* @brief  : 验证数据合法性
*
* @param  : None
* @return : None
******************************************************************************************************/
static bool checkVaildData(picker_screen_info *psi)
{
    PickerWidget *wgt = psi->picker;
    SportSettingTypePicker *set = to_set(psi);
    SportSettingTypePicker *aim = &psi->ssm.assist.reminder.min_speed;
    float val = wgt->tool.getNumberByIndex(wgt->index[0], set->min, set->max, set->interval);

    /* 当前值是否合法 */
    if(val > aim->data)
        return true;

    /* 比较目标打开并且VALUE冲突时弹窗 */
    if(aim->onOff == true && val < aim->data + 5)
    {
        set->data = aim->data + 5;
        switchDialogScreen(DLG_ID_MAX_SPEED, &psi->ssm, true);
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
* @brief  : 获取副标题
*
* @param  : None
* @return : None
******************************************************************************************************/
static void getPickerParamInfo(picker_screen_info *psi, const char **sub_title, int *initVal)
{
    SportSettingTypePicker *set = to_set(psi);

    /* 数字不关心公英制 */
    *sub_title = _UT(KEY_KPH_MIPH);
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
    const char *sub_title;
    pickerInitData pdata = {0};
    SportSettingTypePicker *set = to_set(psi);

    getPickerParamInfo(psi, &sub_title, &initVal);
    psi->picker = createPicker(psi->sc, &pdata, PICKER_SINGLE_WIDE, _ST(TK_FAST_SPEED), sub_title);

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

const picker_ops picker_ops_reminder_max_speed = {
    .create = _settingCreate,
};