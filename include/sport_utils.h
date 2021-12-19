/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  wangxiaomeng@huami.com
 *  Time : 2021-3-20
 *
 *  Describe :
 *
 * ======================================================
 */

#ifndef SPORT_UTIL_H
#define SPORT_UTIL_H
#include "sport_type_auto.h"
#include <manager/sports_setting_manager.h>
#include <stdbool.h>
#include "screen_manager.h"

typedef struct _sportInitData
{
    void (*callBack)(void *data);
    sport_type_t type;
} sportInitData;

/* 调用接口时，无需关注运动是否已经启动，会根据当前运动状态自动转跳到对于页面 */
typedef struct _sportUtils
{
    void (*start)(sport_type_t type);   /* 直接打开指定运动类型的运动 */
    void (*openSport)(void);            /* 转跳到运动应用或运动列表 */
    void (*closeSport)(void);
    bool (*checkTypeInitialized)(sportInitData *data);  /* 检测运动类型是否已经进行过初始化 */
    void (*startByFlag)(sport_type_t type, ScreenFlag flag); /* 直接打开指定运动类型的运动，并带 ScreenFlag 参数 */
    void (*openSportByFlag)(ScreenFlag flag);                /* 转跳到运动应用或运动列表，并带 ScreenFlag 参数 */
    bool (*resumeSportByFlag)(ScreenFlag flag);              /* 恢复运动页面，并带 ScreenFlag 参数，return false 为运动未开启或运动自动识别静默记录 */
    bool (*isInSport)(void);            /* 当前运动应用是否打开 */
} sportUtils;
sportUtils *getSportUtils(void);

#endif