/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  lizheng1@huami.com
 *  Time : 2021-03-27
 *
 *  Describe : sport setting confg
 *              ssc     -       sport setting screen(container)
 *
 * ======================================================
 */
#ifndef __SPORT_SETTING_PICKER_SCREEN_H__
#define __SPORT_SETTING_PICKER_SCREEN_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <sb_screen_context.h>
#include <widget/picker/pickerWidget.h>
#include "manager/sports_setting_manager.h"
#include "ui/Sports/include/setting/sport_setting_utils.h"
#include <widget/list/list_widget_utils.h>

typedef struct _picker_screen_info {
    ScreenContext *sc;                                                          /**@ 屏幕上下文. */
    PickerWidget *picker;                                                       /**@ 选择器. */
    SportSettingsMessage ssm;                                                   /**@ 设置信息. */
}picker_screen_info;

typedef struct _picker_ops {
    void (*create)(picker_screen_info *psi);                                    /**@ 创建. */
}picker_ops;

#ifdef __cplusplus
}
#endif
#endif
