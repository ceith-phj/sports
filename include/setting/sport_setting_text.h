/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  lizheng1@huami.com
 *  Time : 2021-04-08
 *
 *  Describe : sport setting untils
 *
 * ======================================================
 */
#ifndef __SPORT_SETTING_TEXT_H__
#define __SPORT_SETTING_TEXT_H__
#ifdef __cplusplus
extern "C" {
#endif

#define TK_TEMP                     "com_disable"
/*============================================================================================================ ##' 文本 - ID ==== */
#define TEXT_ID_METER               "sport_chart_altitude_matric_unit"          /**< 米. */
#define TEXT_ID_YARDS               "sport_unit_yard"                           /**< 码. */
#define TEXT_ID_CUSTOM              "sportInit_swimLengthCustom"                /**< 自定义. */
#define TEXT_ID_KM                  "spr_km_unit"                               /**< 公里. */
#define TEXT_ID_MILE                "spr_mile_unit"                             /**< 英里. */
#define TEXT_ID_KCAL                "spr_kcal_unit"                             /**< 千卡. */
#define TEXT_ID_HOUR                "sport_unit_hour"                           /**< 小时. */
#define TEXT_ID_MIN                 "sport_unit_min"                            /**< 分. */
#define TEXT_ID_SEC                 "sport_unit_sec"                            /**< 秒. */
#define TEXT_ID_HOUR_KM             "sport_chart_speed_matric_unit"             /**< 公里/小时. */
#define TEXT_ID_HOUR_MILE           "sport_chart_speed_imperial_unit"           /**< 英里/小时. */
#define TEXT_ID_HR                  "sport_chart_hr_unit"                       /**< bpm. */

#define TEXT_ID_HR_ZONE_1           "sport_hrzone_1"                            /**< 区间1. */
#define TEXT_ID_HR_ZONE_2           "sport_hrzone_2"                            /**< 区间2. */
#define TEXT_ID_HR_ZONE_3           "sport_hrzone_3"                            /**< 区间3. */
#define TEXT_ID_HR_ZONE_4           "sport_hrzone_4"                            /**< 区间4. */
#define TEXT_ID_HR_ZONE_5           "sport_hrzone_5"                            /**< 区间5. */
#define TEXT_ID_HR_WARM_UP          "sport_hrzone_reserve_1"                    /**< 热身放松. */
#define TEXT_ID_HR_BURNING          "sport_hrzone_reserve_2"                    /**< 脂肪燃烧. */
#define TEXT_ID_HR_AEROBIC          "sport_hrzone_reserve_3"                    /**< 心肺强化. */
#define TEXT_ID_HR_ANAEROBIC        "sport_hrzone_reserve_4"                    /**< 耐力强化. */
#define TEXT_ID_HR_MAX              "sport_hrzone_reserve_5"                    /**< 无氧极限. */

#define TEXT_ID_CHOICE_WEAR_HAND    "sportInit_wearHand"                        /**< 选择佩戴手. */
#define TEXT_ID_LEFT_HAND           "sportInit_leftHand"                        /**< 左手. */
#define TEXT_ID_RIGHT_HAND          "sportInit_rightHand"                       /**< 右手. */
/*============================================================================================================ 00' 文本 - 公用 ==== */
#define TK_ARG_OVER_CPM             "arg_great_cpm"                             /**< 高于 %d次/分. */
#define TK_ARG_OVER_ZONE_CPM        "arg_great_zone_cpm"                        /**< 高于 %d次/分. */
#define TK_ARG_HIGHER_PACE          "arg_fast_pace"                             /**< 快于 %d'%d". */
#define TK_ARG_LOWER_PACE           "arg_slow_pace"                             /**< 低于 %d'%d". */
#define TK_ARG_HIGHER               "arg_higher"                                /**< 高于 %d. */
#define TK_ARG_LOWER                "arg_lower"                                 /**< 低于 %d. */
#define TK_ARG_MIN_SEC              "arg_min_sec"                               /**< %d'%d". */

#define TK_ARG_PER_KM               "arg_per_km"                                /**< 每 %d公里. */
#define TK_ARG_PER_MILE             "arg_per_mile"                              /**< 每 %d英里. */
#define TK_ARG_PER_TRIP             "arg_per_trip"                              /**< 每 %d趟. */
#define TK_ARG_PER_HOUR_MIN         "arg_per_hour_min"                          /**< 每 %d小时 %d分. */
#define TK_ARG_PER_KCAL             "arg_per_kcal"                              /**< 每 %d千卡. */
#define TK_ARG_PER_STRIDE           "arg_per_stride"                            /**< 每 %d步. */
#define TK_ARG_PER_METER            "arg_per_meter"                             /**< 每 %d米. */
#define TK_ARG_PER_COUNT            "arg_per_count"                             /**< 每 %d次. */

/* 按运动类型 */
#define TK_ARG_KM                   "arg_km"                                    /**< %d公里. */
#define TK_ARG_MILE                 "arg_mile"                                  /**< %d英里. */
#define TK_ARG_KM1                  "arg_km1"                                   /**< %.1f公里. */
#define TK_ARG_MILE1                "arg_mile1"                                 /**< %.1f英里. */
#define TK_ARG_HOUR_MIN             "arg_hour_min"                              /**< %d小时%d分. */
#define TK_ARG_KCAL                 "arg_kcal"                                  /**< %d千卡. */
#define TK_ARG_COUNT                "arg_count"                                 /**< %d次. */
#define TK_ARG_TRIPS                "arg_trip"                                  /**< %d趟. */

/* 自动暂停 */
#define TK_ARG_BELOW_PACE           "arg_below_pace"                            /**< 配速低于%d'%d". */
#define TK_ARG_BELOW_SPEED          "arg_below_speed"                           /**< 速度低于%d公里/时. */
#define TK_ARG_BELOW_SPEED_MILE     "arg_below_speed_mile"                      /**< 速度低于%d英里/时. */

/* 心率 */
#define TK_ARG_HR_ZONE              "arg_hr_zone"                               /**< %d-%d bpm. */

/* 通用变参 */
#define TK_ARG_METER                "arg_meter"                                 /**< %f 米. */
#define TK_ARG_YARDS                "arg_yards"                                 /**< %d 码. */
#define TK_ARG_METER_U              "arg_meter_u"                               /**< %d 米. */
#define TK_ARG_YARDS_F              "arg_yards_f"                               /**< %d 码. */

/* 通用 */
#define TK_COM_STATE                "com_state"                                 /**< 状态. */
#define TK_COM_ENABLE               "com_enable"                                /**< 已开启. */
#define TK_COM_DISABLE              "com_disable"                               /**< 关闭. */
#define TK_COM_VIBRATE              "com_vibrate"                               /**< 震动. */
#define TK_COM_VOLUME               "com_volume"                                /**< 声音. */
#define TK_COM_VIB_VOL              "com_vib_vol"                               /**< 震动+声音. */
#define TK_CLOSE                    TK_COM_DISABLE                              /**< 关闭. */
#define TK_COM_SET_SPEED            "com_set_speed"                             /**< 设置速度. */
#define TK_COM_SET_PACE             "com_set_pace"                              /**< 设置配速. */
#define TK_COM_SET_KCAL             "com_set_kcal"                              /**< 设置卡路里. */
#define TK_COM_SET_TIME             "com_set_time"                              /**< 设置时间. */
#define TK_COM_SET_DISTANCE         "com_set_distance"                          /**< 设置距离. */
#define TK_COM_SET_TRIP             "com_set_trip"                              /**< 设置趟数. */
#define TK_COM_SET_TIME_INTERRVAL   "com_set_time_interval"                     /**< 设置时间间隔. */
/*============================================================================================================ 01' 文本 - 运动设置 ==== */
#define TK_ASSISTANT                "exercise_assistance"                       /**< 运动辅助. */
#define TK_TRANING                  "workout_reminder"                          /**< 提醒. */
#define TK_MORE                     "more"                                      /**< 更多. */

/*============================================================================================================ 02' 文本 - 运动辅助 ==== */
#define TK_EXECRISE_ASSIST          "exercise_assistance"                       /**< 运动辅助. */
#define TK_GOAL                     "workout_goal"                              /**< 运动目标. */
#define TK_REMINDER                 "workout_reminder"                          /**< 运动提醒. */
#define TK_AUTO_ALP                 "auto_lap"                                  /**< 自动分段. */
#define TK_AUTO_PAUSE               "auto_pause"                                /**< 自动暂停. */
#define TK_VIRTUAL_PACER            "virtual_pacer"                             /**< 虚拟兔子. */
#define TK_CADENCE_ASSISTANT        "cadence_assistant"                         /**< 步频助手. */
#define TK_PACE_STRATEGY            "pace_strategy"                             /**< 配速策略. */
#define TK_SPORT_COMPASS            "compass"                                   /**< 指南针. */
#define TK_SPO2                     "spo2"                                      /**< 血氧饱和度. */

/*============================================================================================================ 03' 文本 - 运动目标 ==== */
#define TK_TRAIN_EFFECT             "use_training_effect"                       /**< 按运行效果. */
#define TK_STATISTICS               "workout_statistics"                        /**< 按运动数据. */
#define TK_NO_GOALS                 "no_goals"                                  /**< 不使用目标. */

/*============================================================================================================ 04' 文本 - 训练效果 ==== */
#define TK_AEROBIC_TE               "aerobic_te"                                /**< 有氧TE. */
#define TK_ANAEROBIC_TE             "anaerobic_te"                              /**< 无氧TE. */

#define TK_EFFECT_NO                "no_effect"                                 /**< 无效果. */
#define TK_EFFECT_MINOR             "minor"                                     /**< 效果微弱. */
#define TK_EFFECT_MAINTAINING       "maintaining"                               /**< 效果适中. */
#define TK_EFFECT_IMPROVING         "improving"                                 /**< 效果良好. */
#define TK_HIGHLY_IMPROVING         "high_improving"                            /**< 效果极佳. */
#define TK_OVER_REACHING            "over_reach"                                /**< 超量. */
/*============================================================================================================ 05' 文本 - 运动数据 ==== */
#define TK_TARGET_DISTANCE          "target_distance"                           /**< 距离. */
#define TK_TARGET_TIME              "target_time"                               /**< 时间. */
#define TK_TARGET_CALORIES          "target_calories"                           /**< 消耗. */
#define TK_TARGET_COUNT             "target_count"                              /**< 计数. */
#define TK_TARGET_TRIPS             "target_trips"                              /**< 趟数. */

/*============================================================================================================ 06' 文本 - 运动提醒 ==== */
#define TK_HR                       "reminder_hr"                               /**< 文本 - 心率. */
#define TK_SET_SFAE_HR              "set_safe_hr"                               /**< 文本 - 设置安全心率. */
#define TK_SET_HR_ZONE              "set_hr_zone"                               /**< 文本 - 设置心率区间. */
#define TK_HR_ZONE                  "reminder_hr_zone"                          /**< 文本 - 心率区间. */
#define TK_HR_RESERVE               "reminder_hr_reserve"                       /**< 文本 - 储备心率. */
#define TK_KILOMETER                "reminder_kilometer"                        /**< 文本 - 整公里. */
#define TK_TRIP                     "reminder_trip"                             /**< 文本 - 趟数. */
#define TK_FAST_PACE                "reminder_fast_pace"                        /**< 文本 - 最高配速. */
#define TK_SLOW_PACE                "reminder_slow_pace"                        /**< 文本 - 最低配速. */
#define TK_FAST_CADENCE             "reminder_fast_cadence"                     /**< 文本 - 最快步频. */
#define TK_SLOW_CADENCE             "reminder_slow_cadence"                     /**< 文本 - 最慢步频. */
#define TK_FAST_SPEED               "reminder_fast_speed"                       /**< 文本 - 最快速度. */
#define TK_SLOW_SPEED               "reminder_slow_speed"                       /**< 文本 - 最慢速度. */
#define TK_FAST_STROKES             "reminder_fast_strokes"                     /**< 文本 - 最高划频. */
#define TK_SLOW_STROKES             "reminder_slow_strokers"                    /**< 文本 - 最低划频. */
#define TK_HIGH_SPEED               "reminder_high_speed"                       /**< 文本 - 高速度. */
#define TK_TIME                     "reminder_time"                             /**< 文本 - 时间. */
#define TK_CAL                      "reminder_cal"                              /**< 文本 - 消耗. */
#define TK_DRINK                    "reminder_drink"                            /**< 文本 - 饮水. */
#define TK_ENERGY                   "reminder_energy"                           /**< 文本 - 能量. */
#define TK_BACK                     "reminder_back"                             /**< 文本 - 返回. */
#define TK_BY_TIME                  "by_time"                                   /**< 文本 - 按时间. */
#define TK_BY_DISTANCE              "by_distance"                               /**< 文本 - 按距离. */
#define TK_BY_KCAL                  "by_kcal"                                   /**< 文本 - 按卡路里. */
#define TK_BY_COUNT                 "by_count"                                  /**< 文本 - 按计数. */
#define TK_BY_TRIPS                 "by_trips"                                  /**< 文本 - 按趟数. */
#define TK_IND_MAX_PACE             "ind_max_pace"                              /**< 文本 - 最快配速必须快于最慢配速,请修改最快配速值. */
#define TK_IND_MIN_PACE             "ind_min_pace"                              /**< 文本 - 最慢配速必须慢于最快配速,请修改最慢配速值. */
#define TK_IND_MAX_SPEED            "ind_max_speed"                             /**< 文本 - 最快速度必须快于最慢速度,请修改最快速度值. */
#define TK_IND_MIN_SPEED            "ind_min_speed"                             /**< 文本 - 最慢速度必须慢于最快速度,请修改最慢速度值. */
#define TK_IND_MAX_CADENCE          "ind_max_cadence"                           /**< 文本 - 最快步频必须快于最慢步频,请修改最快步频值. */
#define TK_IND_MIN_CADENCE          "ind_min_cadence"                           /**< 文本 - 最慢步频必须慢于最快步频,请修改最慢步频值. */
#define TK_IND_MAX_STROKES          "ind_max_stroke"                            /**< 文本 - 最快滑频必须快于最慢滑频,请修改最快滑频值. */
#define TK_IND_MIN_STROKES          "ind_min_stroke"                            /**< 文本 - 最慢滑频必须慢于最快滑频,请修改最慢滑频值. */

/*============================================================================================================ 07' 文本 - 自动暂停 ==== */
#define TK_AUTO_LAP                 "auto_lap"                                  /**< 自动分段. */
#define TK_AUTO_LAP_DISTANCE        "lap_distance"                              /**< 自动分段距离. */
#define TK_AUTO_LAP_TRIPS           TK_TARGET_TRIPS                             /**< 分段趟数. */

/*============================================================================================================ 08' 文本 - 自动暂停 ==== */
#define TK_STANDING                 "standing"                                  /**< 静止时. */
#define TK_BY_PACE                  "by_pace"                                   /**< 按配速. */
#define TK_BY_SPEED                 "by_speed"                                  /**< 按速度. */

/*============================================================================================================ 09' 文本 - 虚拟兔子 ==== */
#define TK_VP_STATE                 TK_COM_STATE                                /**< 状态. */
#define TK_VP_PACER                 "vp_pacer"                                  /**< 配速. */
/*============================================================================================================ 10' 文本 - 步频助手 ==== */
#define TK_STATE                    TK_COM_STATE                                /**< 状态. */
#define TK_CADENCE                  "cadence"                                   /**< 步/分钟. */
#define TK_REMINDER_FREQ            "reminder_freq"                             /**< 提示频率. */
#define TK_REMINDER_MODE            "reminder_mode"                             /**< 提醒方式. */
#define TK_PREVIEW                  "preview"                                   /**< 预览. */

/*============================================================================================================ 40' 文本 - 更多 ==== */
#define TK_DATA_EDIT                "data_edit"                                 /**< 数据页编辑. */
#define TK_NAVIGATION               "navigation"                                /**< 导航. */
#define TK_DATA_3D                  "data_3d"                                   /**< 3D数据模式. */
#define TK_EXT_DEV                  "ext_dev"                                   /**< 外接设备. */
#define TK_WEAR_HANDS               "wear_hands"                                /**< 佩戴手. */
#define TK_POOL_LEN                 "pool_len"                                  /**< 泳道长度. */
#define TK_RECALIBRATE              "recalibrate"                               /**< 重新校准. */
#define TK_LOCK_BUTTON              "lock_button"                               /**< 按键锁. */
#define TK_LOCK_SCREEN              "lock_screen"                               /**< 自定锁屏. */

/*============================================================================================================ 60' 文本 - DIALOG ==== */
#define TK_MSG_RECALIBRATE          "msg_recalibrate"                           /**< 1.距离大于0.5公里的校准可... */
#define TK_MSG_3D_DATA              "msg_3d_data"                               /**< 开启3D数据模式后，系统会在2D距离基础上... */
#define TK_MSG_RECALIBRATE_OK       "msg_recalibrate_ok"                        /**< 重新校准成功 */

#ifdef __cplusplus
}
#endif
#endif
