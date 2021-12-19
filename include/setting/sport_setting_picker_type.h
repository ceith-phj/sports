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
#ifndef __SPORT_SETTING_PICKER_TYPE_H__
#define __SPORT_SETTING_PICKER_TYPE_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <sb_screen_context.h>

#define SPORT_PICKER_SCREEN_NAME SportPicker                                    /**< 屏幕名称. */
#define SPORT_PICKER_CONTAINER_NAME "SportPickerContainer"                      /**< Container名称. */
importScreen(SPORT_PICKER_SCREEN_NAME);

typedef enum _picker_id {
    PICKER_ID_AEROBIC_TE,                                                       /**< 00' 有氧TE. */
    PICKER_ID_ANAEROBIC_TE,                                                     /**< 01' 无氧TE. */
    PICKER_ID_TARGET_DISTANCE_METER,                                            /**< 02' 距离. */
    PICKER_ID_TARGET_DISTANCE_KM,                                               /**< 03' 公里. */
    PICKER_ID_TARGET_TIME,                                                      /**< 04' 用时. */
    PICKER_ID_TARGET_CALORIES,                                                  /**< 05' 消耗. */
    PICKER_ID_TARGET_COUNT,                                                     /**< 06' 计数. */
    PICKER_ID_TARGET_TRIPS,                                                     /**< 07' 趟数. */
    PICKER_ID_AUTO_LAP_RUN,                                                     /**< 08' 跑走类. */
    PICKER_ID_AUTO_LAP_SWIM,                                                    /**< 09' 游泳. */
    PICKER_ID_AUTO_LAP_OTHER,                                                   /**< 10' 其他. */
    PICKER_ID_REMINDER_SAFE_HR,                                                 /**< 11' 安全心率. */
    PICKER_ID_REMINDER_HR_ZONE,                                                 /**< 12' 心率区间. */
    PICKER_ID_REMINDER_DISTANCE,                                                /**< 13' 距离. */
    PICKER_ID_REMNIDER_TRIPS,                                                   /**< 14' 趟数. */
    PICKER_ID_REMINDER_MAX_PACE,                                                /**< 15' 最大配速. */
    PICKER_ID_REMINDER_MIN_PACE,                                                /**< 16' 最小配速. */
    PICKER_ID_REMINDER_MAX_CADENCE,                                             /**< 17' 最大步频. */
    PICKER_ID_REMINDER_MIN_CADENCE,                                             /**< 18' 最小步频. */
    PICKER_ID_REMINDER_MAX_SPEED,                                               /**< 19' 最大速度. */
    PICKER_ID_REMINDER_MIN_SPEED,                                               /**< 20' 最小速度. */
    PICKER_ID_REMINDER_MAX_STROKES,                                             /**< 21' 最大划频. */
    PICKER_ID_REMINDER_MIN_STROKES,                                             /**< 22' 最小划频. */
    PICKER_ID_REMINDER_HIGH_SPEED,                                              /**< 23' 高速度. */
    PICKER_ID_REMINDER_TIME,                                                    /**< 24' 时间. */
    PICKER_ID_REMINDER_CALORIES,                                                /**< 25' 卡路里. */
    PICKER_ID_REMINDER_DRINK_TIME,                                              /**< 26' 时间. */
    PICKER_ID_REMINDER_DRINK_DISTANCE,                                          /**< 27' 距离. */
    PICKER_ID_REMINDER_ENERGY_TIME,                                             /**< 28' 时间. */
    PICKER_ID_REMINDER_ENERGY_DISTANCE,                                         /**< 29' 距离. */
    PICKER_ID_REMINDER_ENERGY_CONSUME,                                          /**< 30' 消耗. */
    PICKER_ID_REMINDER_BACK_TIME,                                               /**< 31' 时间. */
    PICKER_ID_REMINDER_BACK_DISTANCE,                                           /**< 32' 距离. */
    PICKER_ID_PAUSE_SPEED,                                                      /**< 33' 速度. */
    PICKER_ID_PAUSE_PACE,                                                       /**< 34' 配速. */
    PICKER_ID_VIRTUAL_PACER,                                                    /**< 35' 配速. */
    PICKER_ID_CADENCE_STEP_FREQ,                                                /**< 36' 步频. */
    PICKER_ID_CADENCE_REMINDER_FREQ,                                            /**< 37' 提醒频率. */
    PICKER_ID_SWIM_POOL_LENGTH,                                                 /**< 38' 泳池长度. */
}picker_id;

typedef struct _picker_data_arg {
    int start;                                                                  /**@ 开始. */
    int end;                                                                    /**@ 结束. */
    int step;                                                                   /**@ 步长. */
}picker_data_arg;

#ifdef __cplusplus
}
#endif
#endif
