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
#ifndef __SPORT_SETTING_PICKER_CONFIG_H__
#define __SPORT_SETTING_PICKER_CONFIG_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "ui/Sports/include/setting/sport_setting_dialog_type.h"
#define import_dialog(x)            extern const dialog_ops (x)

typedef struct _sport_dialog_relation {
    dialog_id id;                                                               /**@ DLG ID. */
    const dialog_ops *ops;                                                      /**@ DLG OPS. */
}sport_dialog_relation;

import_dialog(dialog_ops_recalibrate);
import_dialog(dialog_ops_3d_data);
import_dialog(dialog_ops_min_pace);
import_dialog(dialog_ops_max_pace);
import_dialog(dialog_ops_min_speed);
import_dialog(dialog_ops_max_speed);
import_dialog(dialog_ops_min_cadence);
import_dialog(dialog_ops_max_cadence);
import_dialog(dialog_ops_min_strokes);
import_dialog(dialog_ops_max_strokes);
import_dialog(dialog_ops_goal);

static const sport_dialog_relation sportDialogRlt[] = {
    {DLG_ID_RECALIBRATE,        &dialog_ops_recalibrate},
    {DLG_ID_3D_DATA_MODE,       &dialog_ops_3d_data},
    {DLG_ID_MIN_PACE,           &dialog_ops_min_pace},
    {DLG_ID_MAX_PACE,           &dialog_ops_max_pace},
    {DLG_ID_MIN_SPEED,          &dialog_ops_min_speed},
    {DLG_ID_MAX_SPEED,          &dialog_ops_max_speed},
    {DLG_ID_MIN_CADENCE,        &dialog_ops_min_cadence},
    {DLG_ID_MAX_CADENCE,        &dialog_ops_max_cadence},
    {DLG_ID_MIN_STROKES,        &dialog_ops_min_strokes},
    {DLG_ID_MAX_STROKES,        &dialog_ops_max_strokes},
    {DLG_ID_GOAL,               &dialog_ops_goal},
};

#ifdef __cplusplus
}
#endif
#endif
