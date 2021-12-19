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

/*============================================================================================================ 00' 公共部分 ==== */
/*****************************************************************************************************
* @brief  : DLG 事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogEvent(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    getScreenManager()->finishCurrentScreen();
}

/*****************************************************************************************************
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogCreate(AlertDialog *dlg, const char *text)
{
    dlg->setTitleAndContent(dlg, NULL, text);
    dlg->setPositiveButton(dlg, H_NULL);
    dlg->setClickListener(dlg, _dialogEvent);
    dlg->showDialog(dlg);
}

/*============================================================================================================ 00' 最小配速 ==== */
static inline void _dialogCreate_MinPace(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MIN_PACE));
}

const dialog_ops dialog_ops_min_pace = {
    .create = _dialogCreate_MinPace,
};

/*============================================================================================================ 01' 最大配速 ==== */
static inline void _dialogCreate_MaxPace(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MAX_PACE));
}

const dialog_ops dialog_ops_max_pace = {
    .create = _dialogCreate_MaxPace,
};

/*============================================================================================================ 02' 最小速度 ==== */
static inline void _dialogCreate_MinSpeed(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MIN_SPEED));
}

const dialog_ops dialog_ops_min_speed = {
    .create = _dialogCreate_MinSpeed,
};

/*============================================================================================================ 03' 最大速度 ==== */
static inline void _dialogCreate_MaxSpeed(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MAX_SPEED));
}

const dialog_ops dialog_ops_max_speed = {
    .create = _dialogCreate_MaxSpeed,
};

/*============================================================================================================ 04' 最小步频 ==== */
static inline void _dialogCreate_MinCadence(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MIN_CADENCE));
}

const dialog_ops dialog_ops_min_cadence = {
    .create = _dialogCreate_MinCadence,
};

/*============================================================================================================ 05' 最大步频 ==== */
static inline void _dialogCreate_MaxCadence(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MAX_CADENCE));
}

const dialog_ops dialog_ops_max_cadence = {
    .create = _dialogCreate_MaxCadence,
};

/*============================================================================================================ 06' 最小划频 ==== */
static inline void _dialogCreate_MinStrokes(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MIN_STROKES));
}

const dialog_ops dialog_ops_min_strokes = {
    .create = _dialogCreate_MinStrokes,
};

/*============================================================================================================ 07' 最大划频 ==== */
static inline void _dialogCreate_MaxStrokes(AlertDialog *dlg, ScreenContext *context)
{
    _dialogCreate(dlg, _ST(TK_IND_MAX_STROKES));
}

const dialog_ops dialog_ops_max_strokes = {
    .create = _dialogCreate_MaxStrokes,
};


