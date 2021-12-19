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
#ifndef __SPORT_SETTING_H__
#define __SPORT_SETTING_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "ui/Sports/include/setting/sport_setting_utils.h"
#include "ui/Sports/include/setting/sport_setting_text.h"
#include "ui/Sports/include/setting/sport_setting_type.h"
#include "manager/sports_manager.h"
#include <widget/base/sb_container.h>
#include "hmstatistics.h"

typedef struct _sport_setting_container sport_setting_container;                /**# 声明运动设置. */

typedef struct _list_setting_context {
    list_widget *lw;                                                            /**@ 列表. */
    lw_set_utils lsu;                                                           /**@ 设置工具. */
}list_setting_context;

typedef struct _radio_list_setting_context {
    radio_list_widget *rlw;                                                     /**@ 单选列表控件. */
    lw_set_utils lsu;                                                           /**@ 设置工具. */
}radio_list_setting_context;

typedef union _sport_setting_context {
    list_setting_context list;                                                  /**@ list类型的上下文. */
    radio_list_setting_context radioList;                                       /**@ 单选列表类型的上下文. */
}sport_setting_context;

typedef struct _sport_setting_ops {
    void (*create)(sport_setting_container *ssc);                               /**@ 创建设置UI. */
    void (*destory)(sport_setting_container *ssc);                              /**@ 销毁. */
    bool (*event)(sport_setting_container *ssc);                                /**@ 事件. */
}sport_setting_ops;

typedef struct _sport_setting_container {
    ScreenContext *sc;                                                          /**@ 屏幕上下文. */
    sport_setting_type type;                                                    /**@ 设置类型. */
    char name[100];                                                             /**@ 屏幕名称. */
    const sport_setting_ops *ops;                                               /**@ 该设置的操作函数. */
    raw_set_info rsi;                                                           /**@ 设置信息. */
    bool change;                                                                /**@ 是否修改. */
    sb_screen_event event;                                                      /**@ 屏幕事件. */
    sport_setting_context context;                                              /**@ 该设置的上下文. */
}sport_setting_container;

/**
 * 运动设置关系
 */
typedef struct _sport_setting_relation {
    sport_setting_type type;                                                    /**@ 设置类型. */
    const char *screen;                                                         /**@ 屏幕名字. */
    const sport_setting_ops *ops;                                               /**@ 操作函数. */
}sport_setting_relation;

#ifdef __cplusplus
}
#endif
#endif
