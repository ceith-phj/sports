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


#define ARG_PICKER_1            0, 59, 1                                     /**< 第1列选择器. */
#define to_set(psi)             &psi->ssm.assist.autoPause.byPace

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
    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], set->min / 60, set->max / 60, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], ARG_PICKER_1);
    set->data = data_0 * 60 + data_1;

    /* 保存设置 */
    psi->ssm.assist.autoPause.standing.onOff = false;
    psi->ssm.assist.autoPause.bySpeed.onOff = false;
    psi->ssm.assist.autoPause.byPace.onOff = true;
    psi->ssm.assist.autoPause.onOff.onOff = false;
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
    SportSettingTypePicker *set = to_set(psi);
    PickerWidget *wgt = psi->picker;

    /* 计算当前配速 */
    data_0 = wgt->tool.getNumberByIndex(wgt->index[0], set->min / 60, set->max / 60, 1);
    data_1 = wgt->tool.getNumberByIndex(wgt->index[1], ARG_PICKER_1);

    if(data_0 == 0 && data_1 == 0)
    {
        wgt->index[1] = wgt->tool.getNumberIndex(1, set->min / 60, set->max / 60, 1);
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
* @brief  : 获取副标题
*
* @param  : None
* @return : None
******************************************************************************************************/
static void getInitVal(picker_screen_info *psi, int *initVal)
{
    SportSettingTypePicker *set = to_set(psi);

    /* 数据不参与公英制转换 */
    initVal[0] = set->data / 60;
    initVal[1] = (int)set->data % 60;
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

    /* 创建选择器 */
    getInitVal(psi, initVal);
    psi->picker = createPicker(psi->sc, &pdata, PICKER_DOUBLE, _ST(TK_AUTO_PAUSE), NULL);

    /* 设置列 0 */
    setPicker_ItemAttr(psi->picker, &pdata, 0, NULL, _GT(TEXT_ID_MIN), true, false);
    item_0 = setPicker_ItemNum(psi->picker, &pdata, 0, set->min / 60, set->max / 60, 1, initVal[0]);
    if(item_0 == NULL)
        return;

    /* 设置列 1 */
    setPicker_ItemAttr(psi->picker, &pdata, 1, NULL, _GT(TEXT_ID_SEC), true, false);
    item_1 = setPicker_ItemNumEx(psi->picker, &pdata, 1, ARG_PICKER_1, initVal[1], "%02d");
    if(item_1 == NULL)
        return;

    /* 启动选择器 */
    startPicker(psi->sc, psi->picker, &pdata, _pickerEvent);

    /* 释放内存 */
    H_FREE(item_0);
}

const picker_ops picker_ops_pause_pace = {
    .create = _settingCreate,
};