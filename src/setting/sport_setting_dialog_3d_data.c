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
#include "ui/Sports/include/setting/sport_setting_dialog_screen.h"

/*****************************************************************************************************
* @brief  : 窗口事件 - 确认
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogEventTrue(raw_set_info *rsi)
{
    rsi->set.more.DataMore3d.onOff = true;
    switchSettingScreen(NULL, &rsi->set, true);
}

/*****************************************************************************************************
* @brief  : 窗口事件 - 取消
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogEventFalse(raw_set_info *rsi)
{
    rsi->set.more.DataMore3d.onOff = false;
    switchSettingScreen(NULL, &rsi->set, true);
}

/*****************************************************************************************************
* @brief  : DLG 事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogEvent(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    sport_dialog_screen *sds = getScreenData(contenxt, sport_dialog_screen);

    if(type == BUTTON_RIGHT_CLICK)
        _dialogEventTrue(&sds->rsi);
    else
        _dialogEventFalse(&sds->rsi);
}

/*****************************************************************************************************
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogCreate(AlertDialog *dlg, ScreenContext *context)
{
    dlg->setTitleAndContent(dlg, NULL, _ST(TK_MSG_3D_DATA));
    dlg->setPositiveButton(dlg, H_NULL);
    dlg->setNegativeButton(dlg, NULL);
    dlg->setClickListener(dlg, _dialogEvent);
    dlg->showDialog(dlg);
}

const dialog_ops dialog_ops_3d_data = {
    .create = _dialogCreate,
};
