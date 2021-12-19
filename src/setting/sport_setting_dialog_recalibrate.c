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

#define THIS_SCREEN             "SportSettingDialogScreen"                      /**< SCREEN. */

/*****************************************************************************************************
* @brief  : DLG 事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogEvent_RecalibrateOK(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    getScreenManager()->finishCurrentScreen();
}

/*****************************************************************************************************
* @brief  : 运动设置重新校准
*
* @param  : None
* @return : None
******************************************************************************************************/
static void sportSettingRecalibrate(sport_dialog_screen *sds, ScreenContext *context)
{
    /* 重新校准 */
    SportService_getInstance()->getCtrlService()->clearCaliInfo();

    /* 销毁上一个DLG */
    removeContainer(context, sds->dlg->mDialogLayerPath);

    /* 创建新的DLG */
    sds->dlg = creatAlertDialog(context, THIS_SCREEN);
    addContainer(context, sds->dlg->mDialogLayerPath, sds->dlg);

    /* 设置DLG */
    sds->dlg->setTitleAndContent(sds->dlg, NULL, _ST(TK_MSG_RECALIBRATE_OK));
    sds->dlg->setPositiveButton(sds->dlg, H_NULL);
    sds->dlg->setClickListener(sds->dlg, _dialogEvent_RecalibrateOK);
    sds->dlg->showDialog(sds->dlg);
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

    if(type == BUTTON_LEFT_CLICK)
        getScreenManager()->finishCurrentScreen();
    else
        sportSettingRecalibrate(sds, contenxt);
}

/*****************************************************************************************************
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogCreate(AlertDialog *dlg, ScreenContext *context)
{
    dlg->setTitleAndContent(dlg, NULL, _ST(TK_MSG_RECALIBRATE));
    dlg->setPositiveButton(dlg, H_NULL);
    dlg->setNegativeButton(dlg, NULL);
    dlg->setClickListener(dlg, _dialogEvent);
    dlg->showDialog(dlg);
}

const dialog_ops dialog_ops_recalibrate = {
    .create = _dialogCreate,
};
