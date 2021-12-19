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
#ifndef __SPORT_SETTING_DIALOG_SCREEN_H__
#define __SPORT_SETTING_DIALOG_SCREEN_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <sb_screen_context.h>
#include "ui/Sports/include/setting/sport_setting_dialog_type.h"
#include "ui/Sports/include/setting/sport_setting_text.h"
#include "ui/Sports/include/setting/sport_setting_utils.h"
#include "sports/include/manager/sports_setting_manager.h"

typedef struct _sport_dialog_screen {
    AlertDialog *dlg;                                                           /**@ dlg信息. */
    raw_set_info rsi;                                                           /**@ 设置参数. */
}sport_dialog_screen;

#ifdef __cplusplus
}
#endif
#endif