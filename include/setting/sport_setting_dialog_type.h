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
#ifndef __SPORT_SETTING_DIALOG_TYPE_H__
#define __SPORT_SETTING_DIALOG_TYPE_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "widget/alertDialog/alertDialog.h"


typedef struct _dialog_ops {
    void (*create)(AlertDialog *dlg, ScreenContext *context);                   /**@ 创建. */
}dialog_ops;

typedef enum _dialog_id {
    DLG_ID_RECALIBRATE,                                                         /**@ 00 '重新校准. */
    DLG_ID_3D_DATA_MODE,                                                        /**@ 01' 3D模式. */
    DLG_ID_MIN_PACE,                                                            /**@ 02' 最慢配速. */
    DLG_ID_MAX_PACE,                                                            /**< 03' 最快配速. */
    DLG_ID_MIN_SPEED,                                                           /**< 04' 最慢速度. */
    DLG_ID_MAX_SPEED,                                                           /**< 05' 最快速度. */
    DLG_ID_MIN_CADENCE,                                                         /**< 06' 最慢步频. */
    DLG_ID_MAX_CADENCE,                                                         /**< 07' 最快步频. */
    DLG_ID_MIN_STROKES,                                                         /**< 08' 最慢划频. */
    DLG_ID_MAX_STROKES,                                                         /**< 09' 最快划频. */
    DLG_ID_GOAL,                                                                /**< 10' 运动目标. */
}dialog_id;

#ifdef __cplusplus
}
#endif
#endif