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
#ifndef __SPORT_SETTING_PICKER_CONFIG_H__
#define __SPORT_SETTING_PICKER_CONFIG_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "ui/Sports/include/setting/sport_setting_picker_screen.h"
#define import_picker(x)            extern const picker_ops x

typedef struct _sport_picker_relation {
    picker_id id;
    const picker_ops *ops;
}sport_picker_relation;

import_picker(picker_ops_aerobic_te);
import_picker(picker_ops_anaerobic_te);
import_picker(picker_ops_target_meter);
import_picker(picker_ops_target_km);
import_picker(picker_ops_target_time);
import_picker(picker_ops_target_calories);
import_picker(picker_ops_target_count);
import_picker(picker_ops_target_trips);
import_picker(picker_ops_reminder_safe_hr);
import_picker(picker_ops_reminder_hr_zone);
import_picker(picker_ops_reminder_distance);
import_picker(picker_ops_reminder_trips);
import_picker(picker_ops_reminder_max_pace);
import_picker(picker_ops_reminder_min_pace);
import_picker(picker_ops_reminder_max_cadence);
import_picker(picker_ops_reminder_min_cadence);
import_picker(picker_ops_reminder_max_speed);
import_picker(picker_ops_reminder_min_speed);
import_picker(picker_ops_reminder_max_strokes);
import_picker(picker_ops_reminder_min_strokes);
import_picker(picker_ops_reminder_high_speed);
import_picker(picker_ops_reminder_time);
import_picker(picker_ops_reminder_calories);

import_picker(picker_ops_reminder_drink_time);
import_picker(picker_ops_reminder_drink_distance);
import_picker(picker_ops_reminder_energy_time);
import_picker(picker_ops_reminder_energy_distance);
import_picker(picker_ops_reminder_energy_consume);
import_picker(picker_ops_reminder_back_time);
import_picker(picker_ops_reminder_back_distance);

import_picker(picker_ops_auto_lap_run);
import_picker(picker_ops_auto_lap_swim);
import_picker(picker_ops_auto_lap_other);
import_picker(picker_ops_pause_speed);
import_picker(picker_ops_pause_pace);
import_picker(picker_ops_virtual_pacer);
import_picker(picker_ops_cadence_step_freq);
import_picker(picker_ops_cadence_reminder_freq);
import_picker(picker_ops_swim_pool_length);

static const sport_picker_relation sportPickerRlt[] = {
    {PICKER_ID_AEROBIC_TE,                  &picker_ops_aerobic_te},
    {PICKER_ID_ANAEROBIC_TE,                &picker_ops_anaerobic_te},
    {PICKER_ID_TARGET_DISTANCE_METER,       &picker_ops_target_meter},
    {PICKER_ID_TARGET_DISTANCE_KM,          &picker_ops_target_km},
    {PICKER_ID_TARGET_TIME,                 &picker_ops_target_time},
    {PICKER_ID_TARGET_CALORIES,             &picker_ops_target_calories},
    {PICKER_ID_TARGET_COUNT,                &picker_ops_target_count},
    {PICKER_ID_TARGET_TRIPS,                &picker_ops_target_trips},
    {PICKER_ID_REMINDER_SAFE_HR,            &picker_ops_reminder_safe_hr},
    {PICKER_ID_REMINDER_HR_ZONE,            &picker_ops_reminder_hr_zone},
    {PICKER_ID_REMINDER_DISTANCE,           &picker_ops_reminder_distance},
    {PICKER_ID_REMNIDER_TRIPS,              &picker_ops_reminder_trips},
    {PICKER_ID_REMINDER_MAX_PACE,           &picker_ops_reminder_max_pace},
    {PICKER_ID_REMINDER_MIN_PACE,           &picker_ops_reminder_min_pace},
    {PICKER_ID_REMINDER_MAX_CADENCE,        &picker_ops_reminder_max_cadence},
    {PICKER_ID_REMINDER_MIN_CADENCE,        &picker_ops_reminder_min_cadence},
    {PICKER_ID_REMINDER_MAX_SPEED,          &picker_ops_reminder_max_speed},
    {PICKER_ID_REMINDER_MIN_SPEED,          &picker_ops_reminder_min_speed},
    {PICKER_ID_REMINDER_MAX_STROKES,        &picker_ops_reminder_max_strokes},
    {PICKER_ID_REMINDER_MIN_STROKES,        &picker_ops_reminder_min_strokes},
    {PICKER_ID_REMINDER_HIGH_SPEED,         &picker_ops_reminder_high_speed},
    {PICKER_ID_REMINDER_TIME,               &picker_ops_reminder_time},
    {PICKER_ID_REMINDER_CALORIES,           &picker_ops_reminder_calories},

    {PICKER_ID_REMINDER_DRINK_TIME,         &picker_ops_reminder_drink_time},
    {PICKER_ID_REMINDER_DRINK_DISTANCE,     &picker_ops_reminder_drink_distance},
    {PICKER_ID_REMINDER_ENERGY_TIME,        &picker_ops_reminder_energy_time},
    {PICKER_ID_REMINDER_ENERGY_DISTANCE,    &picker_ops_reminder_energy_distance},
    {PICKER_ID_REMINDER_ENERGY_CONSUME,     &picker_ops_reminder_energy_consume},
    {PICKER_ID_REMINDER_BACK_TIME,          &picker_ops_reminder_back_time},
    {PICKER_ID_REMINDER_BACK_DISTANCE,      &picker_ops_reminder_back_distance},

    {PICKER_ID_AUTO_LAP_RUN,                &picker_ops_auto_lap_run},
    {PICKER_ID_AUTO_LAP_SWIM,               &picker_ops_auto_lap_swim},
    {PICKER_ID_AUTO_LAP_OTHER,              &picker_ops_auto_lap_other},
    {PICKER_ID_PAUSE_SPEED,                 &picker_ops_pause_speed},
    {PICKER_ID_PAUSE_PACE,                  &picker_ops_pause_pace},
    {PICKER_ID_VIRTUAL_PACER,               &picker_ops_virtual_pacer},
    {PICKER_ID_CADENCE_STEP_FREQ,           &picker_ops_cadence_step_freq},
    {PICKER_ID_CADENCE_REMINDER_FREQ,       &picker_ops_cadence_reminder_freq},
    {PICKER_ID_SWIM_POOL_LENGTH,            &picker_ops_swim_pool_length},
};


#ifdef __cplusplus
}
#endif
#endif
