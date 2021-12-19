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
#ifndef __SPORT_SETTING_CONFIG_H__
#define __SPORT_SETTING_CONFIG_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "ui/Sports/include/setting/sport_setting_type.h"
#define import_list(x)            extern const sport_setting_ops x

/*============================================================================================================ 00' 屏幕定义 ==== */
/* 运动辅助 */
SCREEN_DECLARE(SportSettingAssistant);
SCREEN_DECLARE(SportSettingGoal);
SCREEN_DECLARE(SportSettingUseTrainingEffect);
SCREEN_DECLARE(SportSettingUseWorkoutStatistic);
SCREEN_DECLARE(SportSettingReminder);
SCREEN_DECLARE(SportSettingMaxHrZone);
SCREEN_DECLARE(SportSettingReserveHrZone);
SCREEN_DECLARE(SportSettingAutoLap);
SCREEN_DECLARE(SportSettingAutoPause);
SCREEN_DECLARE(SportSettingVirtualPacer);
SCREEN_DECLARE(SportSettingCadenceAssistant);
SCREEN_DECLARE(SportSettingReminderMode);
SCREEN_DECLARE(SportSettingDrink);
SCREEN_DECLARE(SportSettingEnergy);
SCREEN_DECLARE(SportSettingBack);

/* 更多设置 */
SCREEN_DECLARE(SportSettingMore);
SCREEN_DECLARE(SportSettingSwimPoolLength);
SCREEN_DECLARE(SportSettingWearHand);

/*============================================================================================================ 01' 映射关系 ==== */
/* 操作函数定义 */
import_list(setting_ops_main);                                                  /**# 设置列表 - 主设置. */
import_list(setting_ops_assistant);                                             /**# 设置列表 - 运动辅助. */
import_list(setting_ops_goal);                                                  /**# 设置列表 - 运动目标. */
import_list(setting_ops_use_training_effect);                                   /**# 设置列表 - 按运动效果. */
import_list(setting_ops_use_workout_statistics);                                /**# 设置列表 - 按运动数据. */
import_list(setting_ops_reminder);                                              /**# 设置列表 - 提醒. */
import_list(setting_ops_max_hr_zone);                                           /**# 设置列表 - 安全心率. */
import_list(setting_ops_reserve_hr_zone);                                       /**# 设置列表 - 心率区间. */
import_list(setting_ops_auto_lap);                                              /**# 设置列表 - 自动分段. */
import_list(setting_ops_auto_pause);                                            /**# 设置列表 - 自动暂停. */
import_list(setting_ops_virtual_pacer);                                         /**# 设置列表 - 虚拟兔子. */
import_list(setting_ops_cadence_assistant);                                     /**# 设置列表 - 步频助手. */
import_list(setting_ops_reminder_mode);                                         /**# 设置列表 - 提醒模式. */
import_list(setting_ops_more);                                                  /**# 设置列表 - 更多. */
import_list(setting_ops_swim_pool_length);                                      /**# 设置列表 - 泳池长度. */
import_list(setting_ops_drink);                                                 /**# 设置列表 - 饮水. */
import_list(setting_ops_energy);                                                /**# 设置列表 - 能量. */
import_list(setting_ops_back);                                                  /**# 设置列表 - 返回. */
import_list(setting_ops_wear_hand);                                             /**# 设置列表 - 佩戴手. */



static const sport_setting_relation sportSettingRLT[] = {
    {SETTING_TYPE_LIST,        "SportScreen",                            &setting_ops_main},
    {SETTING_TYPE_LIST,        "SportSettingAssistantScreen",            &setting_ops_assistant},
    {SETTING_TYPE_LIST,        "SportSettingGoalScreen",                 &setting_ops_goal},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingUseTrainingEffectScreen",    &setting_ops_use_training_effect},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingUseWorkoutStatisticScreen",  &setting_ops_use_workout_statistics},
    {SETTING_TYPE_LIST,        "SportSettingReminderScreen",             &setting_ops_reminder},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingMaxHrZoneScreen",            &setting_ops_max_hr_zone},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingReserveHrZoneScreen",        &setting_ops_reserve_hr_zone},
    {SETTING_TYPE_LIST,        "SportSettingAutoLapScreen",              &setting_ops_auto_lap},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingAutoPauseScreen",            &setting_ops_auto_pause},
    {SETTING_TYPE_LIST,        "SportSettingVirtualPacerScreen",         &setting_ops_virtual_pacer},
    {SETTING_TYPE_LIST,        "SportSettingCadenceAssistantScreen",     &setting_ops_cadence_assistant},
    {SETTING_TYPE_LIST,        "SportSettingReminderModeScreen",         &setting_ops_reminder_mode},
    {SETTING_TYPE_LIST,        "SportSettingMoreScreen",                 &setting_ops_more},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingSwimPoolLengthScreen",       &setting_ops_swim_pool_length},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingDrinkScreen",                &setting_ops_drink},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingEnergyScreen",               &setting_ops_energy},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingBackScreen",                 &setting_ops_back},
    {SETTING_TYPE_RADIO_LIST,  "SportSettingWearHandScreen",             &setting_ops_wear_hand},
};

#ifdef __cplusplus
}
#endif
#endif