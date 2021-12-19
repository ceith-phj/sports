/*
* =====================================================================================
*
*  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
*  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
*  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
*  All Rights Reserved.
*
*  Author: liubingyan@zepp.com
*
* =====================================================================================
*/

#include <ui/Sports/include/itemEdit/sport_item_utils.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include "string.h"
#include <packages/services/syssevice/manager/system_locale.h>

// time
const int timeDL[6] =
{
    SPORT_RT_ATTR_TYPE_DURATION_TOTAL, /* 总用时 */
    SPORT_RT_ATTR_TYPE_DURATION_NET, /* 运动用时 */
    SPORT_RT_ATTR_TYPE_DURATION_CUR_SECTION, /* 本段用时 */
    SPORT_RT_ATTR_TYPE_DURATION_PREV_SECTION, /* 上段用时 */
    SPORT_RT_ATTR_TYPE_DURATION_AVG_SECTION, /* 每段平均用时 */
    SPORT_RT_ATTR_TYPE_DURATION_CUR_GROUP, /* 本组用时 */
};

// distance
const int distanceDL[3] =
{
    SPORT_RT_ATTR_TYPE_DISTANCE_TOTAL, /* 距离 */
    SPORT_RT_ATTR_TYPE_DISTANCE_CUR_SECTION, /* 本段距离 */
    SPORT_RT_ATTR_TYPE_DISTANCE_PREV_SECTION, /* 上段距离 */
};

// count
const int countDL[6] =
{
    SPORT_RT_ATTR_TYPE_COUNT_TOTAL, /* 总计数 */
    SPORT_RT_ATTR_TYPE_COUNT_CUR_ROPE, /* 本组计数 */
    SPORT_RT_ATTR_TYPE_COUNT_BROKEN_ROPE, /* 断绳次数 */
    SPORT_RT_ATTR_TYPE_COUNT_TOTAL_BOAT, /* 总划次 */
    SPORT_RT_ATTR_TYPE_COUNT_CUR_BOAT, /* 本组划次 */
    SPORT_RT_ATTR_TYPE_COUNT_CUR_FITNESS, /* 本组计次 */
};

// downhill
const int downhillDL[5] =
{
    SPORT_RT_ATTR_TYPE_GLIDE_COUNT, /* 滑降次数 */
    SPORT_RT_ATTR_TYPE_GLIDE_TOTAL_DISTANCE, /* 累计滑降距离 */
    SPORT_RT_ATTR_TYPE_GLIDE_CUR_DISTANCE, /* 本次滑降距离 */
    SPORT_RT_ATTR_TYPE_GLIDE_TOTAL_ALTITUDE, /* 累计滑降落差 */
    SPORT_RT_ATTR_TYPE_GLIDE_CUR_ALTITUDE, /* 本次滑降落差 */
};

// climbstair
const int climbstairsDL[13] =
{
    SPORT_RT_ATTR_TYPE_CLIMB_UP_FLOORS, /* 上楼层数 */
    SPORT_RT_ATTR_TYPE_CLIMB_UP_CUR_FLOORS, /* 本段上楼层数 */
    SPORT_RT_ATTR_TYPE_CLIMB_UP_PREV_FLOORS, /* 上段上楼层数 */
    SPORT_RT_ATTR_TYPE_CLIMB_DOWN_FLOORS, /* 下楼层数 */
    SPORT_RT_ATTR_TYPE_CLIMB_DOWN_CUR_FLOORS, /* 本段下楼层数 */
    SPORT_RT_ATTR_TYPE_CLIMB_DOWN_PREV_FLOORS, /* 上段下楼层数 */
    SPORT_RT_ATTR_TYPE_CLIMB_UP_FLOORS_IN_MIN, /* 每分钟上楼层数 */
    SPORT_RT_ATTR_TYPE_CLIMB_UP_TOTAL_ALTITUDE, /* 上楼高度 */
    SPORT_RT_ATTR_TYPE_CLIMB_UP_CUR_ALTITUDE, /* 本段上楼高度 */
    SPORT_RT_ATTR_TYPE_CLIMB_UP_PREV_ALTITUDE, /* 上段上楼高度 */
    SPORT_RT_ATTR_TYPE_CLIMB_DOWN_ALTITUDE_TOTAL, /* 下楼高度 */
    SPORT_RT_ATTR_TYPE_CLIMB_DOWN_CUR_ALTITUDE, /* 本段下楼高度 */
    SPORT_RT_ATTR_TYPE_CLIMB_DOWN_PREV_ALTITUDE, /* 上段下楼高度 */
};

// swimming
const int swimmingDL[15] =
{
    SPORT_RT_ATTR_TYPE_SWIM_TOTAL_LAPS, /* 趟数 */
    SPORT_RT_ATTR_TYPE_SWIM_CUR_LAPS, /* 本段趟数 */
    SPORT_RT_ATTR_TYPE_SWIM_PREV_LAPS, /* 上段趟数 */
    SPORT_RT_ATTR_TYPE_SWIM_TOTAL_STROKE_CNT, /* 总划水次数 */
    SPORT_RT_ATTR_TYPE_SWIM_CUR_STROKE_CNT, /* 本段划水次数 */
    SPORT_RT_ATTR_TYPE_SWIM_PREV_STROKE_CNT, /* 上段划水次数 */
    SPORT_RT_ATTR_TYPE_SWIM_AVG_STROKE_DISTANCE, /* 平均划水距离 */
    SPORT_RT_ATTR_TYPE_SWIM_AVG_SECTION_STROKE_CNT, /* 每段平均划水次数 */
    SPORT_RT_ATTR_TYPE_SWIM_STROKE_SPEED, /* 划水速率 */
    SPORT_RT_ATTR_TYPE_SWIM_CUR_STROKE_SPEED, /* 本段划水速率 */
    SPORT_RT_ATTR_TYPE_SWIM_PREV_STROKE_SPEED, /* 上段划水速率 */
    SPORT_RT_ATTR_TYPE_SWIM_AVG_STROKE_SPEED, /* 平均划水速率 */
    SPORT_RT_ATTR_TYPE_SWIM_AVG_SWOLF, /* 平均Swolf */
    SPORT_RT_ATTR_TYPE_SWIM_CUR_SWOLF, /* 本段Swolf */
    SPORT_RT_ATTR_TYPE_SWIM_PREV_SWOLF, /* 上段Swolf */
};

// pace
const int paceDL[4] =
{
    SPORT_RT_ATTR_TYPE_PACE, /* 配速 */
    SPORT_RT_ATTR_TYPE_PACE_AVG, /* 平均配速 */
    SPORT_RT_ATTR_TYPE_PACE_CUR_AVG, /* 本段配速  */
    SPORT_RT_ATTR_TYPE_PACE_PREV_AVG, /* 上段配速 */
};

// step
const int stepDL[9] =
{
    SPORT_RT_ATTR_TYPE_STRIDE_FREQ, /* 步频 */
    SPORT_RT_ATTR_TYPE_STRIDE_AVG_FREQ, /* 平均步频 */
    SPORT_RT_ATTR_TYPE_STRIDE_CUR_FREQ, /* 本段步频 */
    SPORT_RT_ATTR_TYPE_STRIDE_PREV_FREQ, /* 上段步频 */
    SPORT_RT_ATTR_TYPE_STRIDE, /* 步幅 */
    SPORT_RT_ATTR_TYPE_STRIDE_AVG, /* 平均步幅 */
    SPORT_RT_ATTR_TYPE_STRIDE_CUR, /* 本段步幅 */
    SPORT_RT_ATTR_TYPE_STRIDE_PREV, /* 上段步幅 */
    SPORT_RT_ATTR_TYPE_STRIDE_COUNT, /* 步数 */
};

// speed
const int speedDL[8] =
{
    SPORT_RT_ATTR_TYPE_SPEED, /* 速度 */
    SPORT_RT_ATTR_TYPE_SPEED_AVG, /* 平均速度 */
    SPORT_RT_ATTR_TYPE_SPEED_AVG_GLIDE, /* 平均滑降速度 */
    SPORT_RT_ATTR_TYPE_SPEED_PREV_GLIDE, /* 上次滑降速度 */
    SPORT_RT_ATTR_TYPE_SPEED_CUR_SECTION, /* 本段速度 */
    SPORT_RT_ATTR_TYPE_SPEED_PREV_SECTION, /* 上段速度 */
    SPORT_RT_ATTR_TYPE_SPEED_MAX, /* 最大速度 */
    SPORT_RT_ATTR_TYPE_SPEED_VERTICAL, /* 垂直速度 */
};

// altitude
const int altitudeDL[13] =
{
    SPORT_RT_ATTR_TYPE_ALTITUDE, /* 海拔 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_MAX, /* 最高海拔 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_MIN, /* 最低海拔 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_AVG, /* 平均海拔 */
    SPORT_RT_ATTR_TYPE_SLOPE_TOTAL_RISING_DISTANCE, /* 累计爬坡 */
    SPORT_RT_ATTR_TYPE_SLOPE_CUR_RISING_DISTANCE, /* 本段爬坡 */
    SPORT_RT_ATTR_TYPE_SLOPE_PREV_RISING_DISTANCE, /* 上段爬坡 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_TOTAL_UP, /* 累计上升 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_CUR_UP, /* 本段上升 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_PREV_UP, /* 上段上升 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_TOTAL_DOWN, /* 累计下降 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_CUR_DOWN, /* 本段下降 */
    SPORT_RT_ATTR_TYPE_ALTITUDE_PREV_DOWN, /* 上段下降 */
};

// slope
const int slopeDL[8] =
{
    SPORT_RT_ATTR_TYPE_SLOPE, /* 坡度 */
    SPORT_RT_ATTR_TYPE_SLOPE_AVG, /* 平均坡度 */
    SPORT_RT_ATTR_TYPE_SLOPE_CUR, /* 本段坡度 */
    SPORT_RT_ATTR_TYPE_SLOPE_PREV, /* 上段坡度 */
    SPORT_RT_ATTR_TYPE_SLOPE_GLIDE, /* 滑翔比 */
    SPORT_RT_ATTR_TYPE_SLOPE_AVG_GLIDE, /* 平均滑翔比 */
    SPORT_RT_ATTR_TYPE_SLOPE_CUR_GLIDE, /* 本段滑翔比 */
    SPORT_RT_ATTR_TYPE_SLOPE_PREV_GLIDE, /* 上段滑翔比 */
};

// swing
const int swingDL[4] =
{
    SPORT_RT_ATTR_TYPE_BRANDISH_TOTAL_CNT, /* 总挥拍次数 */
    SPORT_RT_ATTR_TYPE_BRANDISH_POSITIVE_CNT, /* 正手挥拍次数 */
    SPORT_RT_ATTR_TYPE_BRANDISH_NEGATIVE_CNT, /* 反手挥拍次数 */
    SPORT_RT_ATTR_TYPE_BRANDISH_SERVE_CNT, /* 发球次数 */
};

// consume
const int consumeDL[2] =
{
    SPORT_RT_ATTR_TYPE_CONSUME, /* 消耗 */
    SPORT_RT_ATTR_TYPE_CONSUME_CUR, /* 本组消耗 */
};

// boating
const int boatingDL[5] =
{
    SPORT_RT_ATTR_TYPE_BOATING_FREQ, /* 划频 */
    SPORT_RT_ATTR_TYPE_BOATING_AVG_FREQ, /* 平均划频 */
    SPORT_RT_ATTR_TYPE_BOATING_CUR_FREQ, /* 本组平均划频 */
    SPORT_RT_ATTR_TYPE_BOATING_PULL, /* 拉桨用时 */
    SPORT_RT_ATTR_TYPE_BOATING_PUSH, /* 收桨用时 */
};

// freq
const int freqDL[3] =
{
    SPORT_RT_ATTR_TYPE_FREQ, /* 频率 */
    SPORT_RT_ATTR_TYPE_FREQ_AVG, /* 平均频率 */
    SPORT_RT_ATTR_TYPE_FREQ_CUR, /* 本组平均频率 */
};

// golf
const int golfDL[8] =
{
    SPORT_RT_ATTR_TYPE_GOLF_SPEED, /* 手速 */
    SPORT_RT_ATTR_TYPE_GOLF_ANGLE, /* 平面 */
    SPORT_RT_ATTR_TYPE_GOLF_UP_TIME, /* 上杆时间 */
    SPORT_RT_ATTR_TYPE_GOLF_DOWN_TIME, /* 下杆时间 */
    SPORT_RT_ATTR_TYPE_GOLF_BEAT, /* 节奏 */
    SPORT_RT_ATTR_TYPE_GOLF_SWING_COUNTER, /* 挥杆次数 */
    SPORT_RT_ATTR_TYPE_GOLF_AVG_SCORE, /* 平均得分 */
    SPORT_RT_ATTR_TYPE_GOLF_SCORE, /* 得分 */
};

// hr
const int hrDL[14] =
{
    SPORT_RT_ATTR_TYPE_HR, /* 心率 */
    SPORT_RT_ATTR_TYPE_HR_AVG, /* 平均心率 */
    SPORT_RT_ATTR_TYPE_HR_CUR_AVG, /* 本组平均心率 */
    SPORT_RT_ATTR_TYPE_HR_INTERVAL, /* 心率区间 */
    SPORT_RT_ATTR_TYPE_HR_MAX_PERCENT, /* 最大心率百分比 */
    SPORT_RT_ATTR_TYPE_HR_RESERVED_PERCENT, /* 储备心率百分比 */
    SPORT_RT_ATTR_TYPE_HR_AVG_MAX_PERCENT, /* 平均最大心率百分比 */
    SPORT_RT_ATTR_TYPE_HR_AVG_RESERVED_PERCENT, /* 平均储备心率百分比 */
    SPORT_RT_ATTR_TYPE_HR_CUR_SECTION, /* 本段心率 */
    SPORT_RT_ATTR_TYPE_HR_CUR_MAX_PERCENT, /* 本段最大心率百分比 */
    SPORT_RT_ATTR_TYPE_HR_CUR_RESERVED_PERCENT, /* 本段储备心率百分比 */
    SPORT_RT_ATTR_TYPE_HR_PREV_SECTION, /* 上段心率 */
    SPORT_RT_ATTR_TYPE_HR_PREV_MAX_PERCENT, /* 上段最大心率百分比 */
    SPORT_RT_ATTR_TYPE_HR_PREV_RESERED_PERCENT, /* 上段储备心率百分比 */
};

// pressure
const int pressureDL[4] =
{
    SPORT_RT_ATTR_TYPE_PRESSURE, /* 压力 */
    SPORT_RT_ATTR_TYPE_PRESSURE_AVG, /* 平均压力 */
    SPORT_RT_ATTR_TYPE_PRESSURE_CUR, /* 本段压力 */
    SPORT_RT_ATTR_TYPE_PRESSURE_PREV, /* 上段压力 */
};

// temperatue
const int temperatueDL[3] =
{
    SPORT_RT_ATTR_TYPE_TEMP, /* 温度 */
    SPORT_RT_ATTR_TYPE_TEMP_MAX, /* 最高温度 */
    SPORT_RT_ATTR_TYPE_TEMP_MIN, /* 最低温度 */
};

// other
const int otherDataDL[8] =
{
    SPORT_RT_ATTR_TYPE_OTHER_SECTION_ORDER, /* 当前组数 */
    SPORT_RT_ATTR_TYPE_OTHER_AEROBIC_TE, /* 有氧TE */
    SPORT_RT_ATTR_TYPE_OTHER_ANAEROBIC_TE, /* 无氧TE */
    SPORT_RT_ATTR_TYPE_OTHER_TRAIN_LOAD, /* 训练负荷 */
    SPORT_RT_ATTR_TYPE_OTHER_CUR_TIME, /* 当前时间 */
    SPORT_RT_ATTR_TYPE_OTHER_SUNRISE_TIME, /* 日出时间 */
    SPORT_RT_ATTR_TYPE_OTHER_SUNSET_TIME, /* 日落时间 */
    SPORT_RT_ATTR_TYPE_OTHER_BORAMETER, /* 气压 */
};

/* 默认数据项类型列表 */
const sport_item_descrip_t ItemDL[20] =
{
    {SPORT_ITEM_GROUP_TIME,         timeDL,         ARRAY_SIZE(timeDL)},
    {SPORT_ITEM_GROUP_DISTANCE,     distanceDL,     ARRAY_SIZE(distanceDL)},
    {SPORT_ITEM_GROUP_COUNT,        countDL,        ARRAY_SIZE(countDL)},
    {SPORT_ITEM_GROUP_DOWNHILL,     downhillDL,     ARRAY_SIZE(downhillDL)},
    {SPORT_ITEM_GROUP_CLIMB_STAIRS, climbstairsDL,  ARRAY_SIZE(climbstairsDL)},
    {SPORT_ITEM_GROUP_SWIMMING,     swimmingDL,     ARRAY_SIZE(swimmingDL)},
    {SPORT_ITEM_GROUP_PACE,         paceDL,         ARRAY_SIZE(paceDL)},
    {SPORT_ITEM_GROUP_STEP,         stepDL,         ARRAY_SIZE(stepDL)},
    {SPORT_ITEM_GROUP_SPEED,        speedDL,        ARRAY_SIZE(speedDL)},
    {SPORT_ITEM_GROUP_ALTITUDE,     altitudeDL,     ARRAY_SIZE(altitudeDL)},
    {SPORT_ITEM_GROUP_SLOPE,        slopeDL,        ARRAY_SIZE(slopeDL)},
    {SPORT_ITEM_GROUP_SWING,        swingDL,        ARRAY_SIZE(swingDL)},
    {SPORT_ITEM_GROUP_CONSUME,      consumeDL,      ARRAY_SIZE(consumeDL)},
    {SPORT_ITEM_GROUP_BOATING,      boatingDL,      ARRAY_SIZE(boatingDL)},
    {SPORT_ITEM_GROUP_FREQ,         freqDL,         ARRAY_SIZE(freqDL)},
    {SPORT_ITEM_GROUP_GOLF,         golfDL,         ARRAY_SIZE(golfDL)},
    {SPORT_ITEM_GROUP_HR,           hrDL,           ARRAY_SIZE(hrDL)},
    {SPORT_ITEM_GROUP_PRESSURE,     pressureDL,     ARRAY_SIZE(pressureDL)},
    {SPORT_ITEM_GROUP_TEMPERATUTE,  temperatueDL,   ARRAY_SIZE(temperatueDL)},
    {SPORT_ITEM_GROUP_OTHER,        otherDataDL,        ARRAY_SIZE(otherDataDL)},
};

const SportItemGroup sportGroupArray[SPORT_ITEM_GROUP_SIZE] = {
    {SPORT_ITEM_GROUP_TIME,         "id_sport_item_group_time"},
    {SPORT_ITEM_GROUP_DISTANCE,     "id_sport_item_group_distance"},
    {SPORT_ITEM_GROUP_COUNT,        "id_sport_item_group_count"},
    {SPORT_ITEM_GROUP_DOWNHILL,     "id_sport_item_group_downhill"},
    {SPORT_ITEM_GROUP_CLIMB_STAIRS, "id_sport_item_group_climb_stairs"},
    {SPORT_ITEM_GROUP_SWIMMING,     "id_sport_item_group_swimming"},
    {SPORT_ITEM_GROUP_PACE,         "id_sport_item_group_pace"},
    {SPORT_ITEM_GROUP_STEP,         "id_sport_item_group_step"},
    {SPORT_ITEM_GROUP_SPEED,        "id_sport_item_group_speed"},
    {SPORT_ITEM_GROUP_ALTITUDE,     "id_sport_item_group_altitude"},
    {SPORT_ITEM_GROUP_SLOPE,        "id_sport_item_group_slope"},
    {SPORT_ITEM_GROUP_SWING,        "id_sport_item_group_swing"},
    {SPORT_ITEM_GROUP_CONSUME,      "id_sport_item_group_consume"},
    {SPORT_ITEM_GROUP_BOATING,      "id_sport_item_group_boating"},
    {SPORT_ITEM_GROUP_FREQ,         "id_sport_item_group_freq"},
    {SPORT_ITEM_GROUP_GOLF,         "id_sport_item_group_golf"},
    {SPORT_ITEM_GROUP_HR,           "id_sport_item_group_hr"},
    {SPORT_ITEM_GROUP_PRESSURE,     "id_sport_item_group_pressure"},
    {SPORT_ITEM_GROUP_TEMPERATUTE,  "id_sport_item_group_temperatute"},
    {SPORT_ITEM_GROUP_OTHER,        "id_sport_item_group_other"},
};

const sport_item_t SportItemArray[SPORT_RT_ATTR_TYPE_MAX] = {
    {SPORT_RT_ATTR_TYPE_DURATION_TOTAL, "id_rt_duration_total"},
    {SPORT_RT_ATTR_TYPE_DURATION_NET, "id_rt_duration_net"},
    {SPORT_RT_ATTR_TYPE_DURATION_CUR_SECTION, "id_rt_duration_cur_section"},
    {SPORT_RT_ATTR_TYPE_DURATION_PREV_SECTION, "id_rt_duration_prev_section"},
    {SPORT_RT_ATTR_TYPE_DURATION_AVG_SECTION, "id_rt_duration_avg_section"},
    {SPORT_RT_ATTR_TYPE_DURATION_CUR_GROUP, "id_rt_duration_cur_group"},
    {SPORT_RT_ATTR_TYPE_DISTANCE_TOTAL, "id_rt_distance_total"},
    {SPORT_RT_ATTR_TYPE_DISTANCE_CUR_SECTION, "id_rt_distance_cur_section"},
    {SPORT_RT_ATTR_TYPE_DISTANCE_PREV_SECTION, "id_rt_distance_prev_section"},
    {SPORT_RT_ATTR_TYPE_COUNT_TOTAL, "id_rt_count_total"},
    {SPORT_RT_ATTR_TYPE_COUNT_CUR_ROPE, "id_rt_count_cur_rope"},
    {SPORT_RT_ATTR_TYPE_COUNT_BROKEN_ROPE, "id_rt_count_broken_rope"},
    {SPORT_RT_ATTR_TYPE_COUNT_TOTAL_BOAT, "id_rt_count_total_boat"},
    {SPORT_RT_ATTR_TYPE_COUNT_CUR_BOAT, "id_rt_count_cur_boat"},
    {SPORT_RT_ATTR_TYPE_COUNT_CUR_FITNESS, "id_rt_count_cur_fitness"},
    {SPORT_RT_ATTR_TYPE_GLIDE_COUNT, "id_rt_glide_count"},
    {SPORT_RT_ATTR_TYPE_GLIDE_TOTAL_DISTANCE, "id_rt_glide_total_distance"},
    {SPORT_RT_ATTR_TYPE_GLIDE_CUR_DISTANCE, "id_rt_glide_cur_distance"},
    {SPORT_RT_ATTR_TYPE_GLIDE_TOTAL_ALTITUDE, "id_rt_glide_total_altitude"},
    {SPORT_RT_ATTR_TYPE_GLIDE_CUR_ALTITUDE, "id_rt_glide_cur_altitude"},
    {SPORT_RT_ATTR_TYPE_CLIMB_UP_FLOORS, "id_rt_climb_up_floors"},
    {SPORT_RT_ATTR_TYPE_CLIMB_UP_CUR_FLOORS, "id_rt_climb_up_cur_floors"},
    {SPORT_RT_ATTR_TYPE_CLIMB_UP_PREV_FLOORS, "id_rt_climb_up_prev_floors"},
    {SPORT_RT_ATTR_TYPE_CLIMB_DOWN_FLOORS, "id_rt_climb_down_floors"},
    {SPORT_RT_ATTR_TYPE_CLIMB_DOWN_CUR_FLOORS, "id_rt_climb_down_cur_floors"},
    {SPORT_RT_ATTR_TYPE_CLIMB_DOWN_PREV_FLOORS, "id_rt_climb_down_prev_floors"},
    {SPORT_RT_ATTR_TYPE_CLIMB_UP_FLOORS_IN_MIN, "id_rt_climb_up_floors_in_min"},
    {SPORT_RT_ATTR_TYPE_CLIMB_UP_TOTAL_ALTITUDE, "id_rt_climb_up_total_altitude"},
    {SPORT_RT_ATTR_TYPE_CLIMB_UP_CUR_ALTITUDE, "id_rt_climb_up_cur_altitude"},
    {SPORT_RT_ATTR_TYPE_CLIMB_UP_PREV_ALTITUDE, "id_rt_climb_up_prev_altitude"},
    {SPORT_RT_ATTR_TYPE_CLIMB_DOWN_ALTITUDE_TOTAL, "id_rt_climb_down_altitude_total"},
    {SPORT_RT_ATTR_TYPE_CLIMB_DOWN_CUR_ALTITUDE, "id_rt_climb_down_cur_altitude"},
    {SPORT_RT_ATTR_TYPE_CLIMB_DOWN_PREV_ALTITUDE, "id_rt_climb_down_prev_altitude"},
    {SPORT_RT_ATTR_TYPE_SWIM_TOTAL_LAPS, "id_rt_swim_total_laps"},
    {SPORT_RT_ATTR_TYPE_SWIM_CUR_LAPS, "id_rt_swim_cur_laps"},
    {SPORT_RT_ATTR_TYPE_SWIM_PREV_LAPS, "id_rt_swim_prev_laps"},
    {SPORT_RT_ATTR_TYPE_SWIM_TOTAL_STROKE_CNT, "id_rt_swim_total_stroke_cnt"},
    {SPORT_RT_ATTR_TYPE_SWIM_CUR_STROKE_CNT, "id_rt_swim_cur_stroke_cnt"},
    {SPORT_RT_ATTR_TYPE_SWIM_PREV_STROKE_CNT, "id_rt_swim_prev_stroke_cnt"},
    {SPORT_RT_ATTR_TYPE_SWIM_AVG_STROKE_DISTANCE, "id_rt_swim_avg_stroke_distance"},
    {SPORT_RT_ATTR_TYPE_SWIM_AVG_SECTION_STROKE_CNT, "id_rt_swim_avg_section_stroke_cnt"},
    {SPORT_RT_ATTR_TYPE_SWIM_STROKE_SPEED, "id_rt_swim_stroke_speed"},
    {SPORT_RT_ATTR_TYPE_SWIM_CUR_STROKE_SPEED, "id_rt_swim_cur_stroke_speed"},
    {SPORT_RT_ATTR_TYPE_SWIM_PREV_STROKE_SPEED, "id_rt_swim_prev_stroke_speed"},
    {SPORT_RT_ATTR_TYPE_SWIM_AVG_STROKE_SPEED, "id_rt_swim_avg_stroke_speed"},
    {SPORT_RT_ATTR_TYPE_SWIM_AVG_SWOLF, "id_rt_swim_avg_swolf"},
    {SPORT_RT_ATTR_TYPE_SWIM_CUR_SWOLF, "id_rt_swim_cur_swolf"},
    {SPORT_RT_ATTR_TYPE_SWIM_PREV_SWOLF, "id_rt_swim_prev_swolf"},
    {SPORT_RT_ATTR_TYPE_PACE, "id_rt_pace"},
    {SPORT_RT_ATTR_TYPE_PACE_AVG, "id_rt_pace_avg"},
    {SPORT_RT_ATTR_TYPE_PACE_CUR_AVG, "id_rt_pace_cur_avg"},
    {SPORT_RT_ATTR_TYPE_PACE_PREV_AVG, "id_rt_pace_prev_avg"},
    {SPORT_RT_ATTR_TYPE_STRIDE_FREQ, "id_rt_stride_freq"},
    {SPORT_RT_ATTR_TYPE_STRIDE_AVG_FREQ, "id_rt_stride_avg_freq"},
    {SPORT_RT_ATTR_TYPE_STRIDE_CUR_FREQ, "id_rt_stride_cur_freq"},
    {SPORT_RT_ATTR_TYPE_STRIDE_PREV_FREQ, "id_rt_stride_prev_freq"},
    {SPORT_RT_ATTR_TYPE_STRIDE, "id_rt_stride"},
    {SPORT_RT_ATTR_TYPE_STRIDE_AVG, "id_rt_stride_avg"},
    {SPORT_RT_ATTR_TYPE_STRIDE_CUR, "id_rt_stride_cur"},
    {SPORT_RT_ATTR_TYPE_STRIDE_PREV, "id_rt_stride_prev"},
    {SPORT_RT_ATTR_TYPE_STRIDE_COUNT, "id_rt_stride_count"},
    {SPORT_RT_ATTR_TYPE_SPEED, "id_rt_speed"},
    {SPORT_RT_ATTR_TYPE_SPEED_AVG, "id_rt_speed_avg"},
    {SPORT_RT_ATTR_TYPE_SPEED_AVG_GLIDE, "id_rt_speed_avg_glide"},
    {SPORT_RT_ATTR_TYPE_SPEED_PREV_GLIDE, "id_rt_speed_prev_glide"},
    {SPORT_RT_ATTR_TYPE_SPEED_CUR_SECTION, "id_rt_speed_cur_section"},
    {SPORT_RT_ATTR_TYPE_SPEED_PREV_SECTION, "id_rt_speed_prev_section"},
    {SPORT_RT_ATTR_TYPE_SPEED_MAX, "id_rt_speed_max"},
    {SPORT_RT_ATTR_TYPE_SPEED_VERTICAL, "id_rt_speed_vertical"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE, "id_rt_altitude"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_MAX, "id_rt_altitude_max"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_MIN, "id_rt_altitude_min"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_AVG, "id_rt_altitude_avg"},
    {SPORT_RT_ATTR_TYPE_SLOPE_TOTAL_RISING_DISTANCE, "id_rt_altitude_total_slope"},
    {SPORT_RT_ATTR_TYPE_SLOPE_CUR_RISING_DISTANCE, "id_rt_altitude_cur_slope"},
    {SPORT_RT_ATTR_TYPE_SLOPE_PREV_RISING_DISTANCE, "id_rt_altitude_prev_slope"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_TOTAL_UP, "id_rt_altitude_total_up"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_CUR_UP, "id_rt_altitude_cur_up"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_PREV_UP, "id_rt_altitude_prev_up"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_TOTAL_DOWN, "id_rt_altitude_total_down"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_CUR_DOWN, "id_rt_altitude_cur_down"},
    {SPORT_RT_ATTR_TYPE_ALTITUDE_PREV_DOWN, "id_rt_altitude_prev_down"},
    {SPORT_RT_ATTR_TYPE_SLOPE, "id_rt_slope"},
    {SPORT_RT_ATTR_TYPE_SLOPE_AVG, "id_rt_slope_avg"},
    {SPORT_RT_ATTR_TYPE_SLOPE_CUR, "id_rt_slope_cur"},
    {SPORT_RT_ATTR_TYPE_SLOPE_PREV, "id_rt_slope_prev"},
    {SPORT_RT_ATTR_TYPE_SLOPE_GLIDE, "id_rt_slope_glide"},
    {SPORT_RT_ATTR_TYPE_SLOPE_AVG_GLIDE, "id_rt_slope_avg_glide"},
    {SPORT_RT_ATTR_TYPE_SLOPE_CUR_GLIDE, "id_rt_slope_cur_glide"},
    {SPORT_RT_ATTR_TYPE_SLOPE_PREV_GLIDE, "id_rt_slope_prev_glide"},
    {SPORT_RT_ATTR_TYPE_BRANDISH_TOTAL_CNT, "id_rt_brandish_total_cnt"},
    {SPORT_RT_ATTR_TYPE_BRANDISH_POSITIVE_CNT, "id_rt_brandish_positive_cnt"},
    {SPORT_RT_ATTR_TYPE_BRANDISH_NEGATIVE_CNT, "id_rt_brandish_negative_cnt"},
    {SPORT_RT_ATTR_TYPE_BRANDISH_SERVE_CNT, "id_rt_brandish_serve_cnt"},
    {SPORT_RT_ATTR_TYPE_CONSUME, "id_rt_consume"},
    {SPORT_RT_ATTR_TYPE_CONSUME_CUR, "id_rt_consume_cur"},
    {SPORT_RT_ATTR_TYPE_BOATING_FREQ, "id_rt_boating_freq"},
    {SPORT_RT_ATTR_TYPE_BOATING_AVG_FREQ, "id_rt_boating_avg_freq"},
    {SPORT_RT_ATTR_TYPE_BOATING_CUR_FREQ, "id_rt_boating_cur_freq"},
    {SPORT_RT_ATTR_TYPE_BOATING_PULL, "id_rt_boating_pull"},
    {SPORT_RT_ATTR_TYPE_BOATING_PUSH, "id_rt_boating_push"},
    {SPORT_RT_ATTR_TYPE_FREQ, "id_rt_freq"},
    {SPORT_RT_ATTR_TYPE_FREQ_AVG, "id_rt_freq_avg"},
    {SPORT_RT_ATTR_TYPE_FREQ_CUR, "id_rt_freq_cur"},
    {SPORT_RT_ATTR_TYPE_GOLF_SPEED, "id_rt_golf_speed"},
    {SPORT_RT_ATTR_TYPE_GOLF_ANGLE, "id_rt_golf_angle"},
    {SPORT_RT_ATTR_TYPE_GOLF_UP_TIME, "id_rt_golf_up_time"},
    {SPORT_RT_ATTR_TYPE_GOLF_DOWN_TIME, "id_rt_golf_down_time"},
    {SPORT_RT_ATTR_TYPE_GOLF_BEAT, "id_rt_golf_beat"},
    {SPORT_RT_ATTR_TYPE_GOLF_SWING_COUNTER, "id_rt_golf_swing_counter"},
    {SPORT_RT_ATTR_TYPE_GOLF_AVG_SCORE, "id_rt_golf_avg_score"},
    {SPORT_RT_ATTR_TYPE_GOLF_SCORE, "id_rt_golf_score"},
    {SPORT_RT_ATTR_TYPE_HR, "id_rt_hr"},
    {SPORT_RT_ATTR_TYPE_HR_AVG, "id_rt_hr_avg"},
    {SPORT_RT_ATTR_TYPE_HR_CUR_AVG, "id_rt_hr_cur_avg"},
    {SPORT_RT_ATTR_TYPE_HR_INTERVAL, "id_rt_hr_interval"},
    {SPORT_RT_ATTR_TYPE_HR_MAX_PERCENT, "id_rt_hr_max_percent"},
    {SPORT_RT_ATTR_TYPE_HR_RESERVED_PERCENT, "id_rt_hr_reserved_percent"},
    {SPORT_RT_ATTR_TYPE_HR_AVG_MAX_PERCENT, "id_rt_hr_avg_max_percent"},
    {SPORT_RT_ATTR_TYPE_HR_AVG_RESERVED_PERCENT, "id_rt_hr_avg_reserved_percent"},
    {SPORT_RT_ATTR_TYPE_HR_CUR_SECTION, "id_rt_hr_cur_section"},
    {SPORT_RT_ATTR_TYPE_HR_CUR_MAX_PERCENT, "id_rt_hr_cur_max_percent"},
    {SPORT_RT_ATTR_TYPE_HR_CUR_RESERVED_PERCENT, "id_rt_hr_cur_reserved_percent"},
    {SPORT_RT_ATTR_TYPE_HR_PREV_SECTION, "id_rt_hr_prev_section"},
    {SPORT_RT_ATTR_TYPE_HR_PREV_MAX_PERCENT, "id_rt_hr_prev_max_percent"},
    {SPORT_RT_ATTR_TYPE_HR_PREV_RESERED_PERCENT, "id_rt_hr_prev_resered_percent"},
    {SPORT_RT_ATTR_TYPE_PRESSURE, "id_rt_pressure"},
    {SPORT_RT_ATTR_TYPE_PRESSURE_AVG, "id_rt_pressure_avg"},
    {SPORT_RT_ATTR_TYPE_PRESSURE_CUR, "id_rt_pressure_cur"},
    {SPORT_RT_ATTR_TYPE_PRESSURE_PREV, "id_rt_pressure_prev"},
    {SPORT_RT_ATTR_TYPE_TEMP, "id_rt_temp"},
    {SPORT_RT_ATTR_TYPE_TEMP_MAX, "id_rt_temp_max"},
    {SPORT_RT_ATTR_TYPE_TEMP_MIN, "id_rt_temp_min"},
    {SPORT_RT_ATTR_TYPE_OTHER_SECTION_ORDER, "id_rt_other_section_order"},
    {SPORT_RT_ATTR_TYPE_OTHER_AEROBIC_TE, "id_rt_other_aerobic_te"},
    {SPORT_RT_ATTR_TYPE_OTHER_ANAEROBIC_TE, "id_rt_other_anaerobic_te"},
    {SPORT_RT_ATTR_TYPE_OTHER_TRAIN_LOAD, "id_rt_other_train_load"},
    {SPORT_RT_ATTR_TYPE_OTHER_CUR_TIME, "id_rt_other_cur_time"},
    {SPORT_RT_ATTR_TYPE_OTHER_SUNRISE_TIME, "id_rt_other_sunrise_time"},
    {SPORT_RT_ATTR_TYPE_OTHER_SUNSET_TIME, "id_rt_other_sunset_time"},
    {SPORT_RT_ATTR_TYPE_OTHER_BORAMETER, "id_rt_other_borameter"},
};

const char *sportItem_getTypeNameId(sport_item_type_t sportItem)
{
    for (uint32_t i = 0; i < sizeof(SportItemArray) / sizeof(SportItemArray[0]); i++)
    {
        if (sportItem == SportItemArray[i].itemID)
        {
            static char nameAddr[256] = {0};
            int len = snprintf(nameAddr, sizeof(nameAddr), "id_sport_type_%s", SportItemArray[i].itemName);
            nameAddr[len] = 0;
            return nameAddr;
        }
    }
    return 0;
}

const char *sportItem_getTypeName(sport_item_type_t sportItem)
{
    for (uint32_t i = 0; i < sizeof(SportItemArray) / sizeof(SportItemArray[0]); i++)
    {
        if (sportItem == SportItemArray[i].itemID)
        {
            return SportItemArray[i].itemName;
        }
    }
    return 0;
}

const char *sportItem_getGroupNameId(sport_item_group_t groupType)
{
    for (uint32_t i = 0; i < sizeof(sportGroupArray); i++)
    {
        if (groupType == sportGroupArray[i].groupId)
        {
            return sportGroupArray[i].groupName;
        }
    }
    return 0;
}

const char* sportItem_getTypeNameChar(sport_item_type_t sportItem)
{
    const char *name = System_getLocaleText(sportItem_getTypeNameId(sportItem));
    if (name == NULL || strlen(name) == 0)
    {
        return "name not found";
    }
    else
    {
        return name;
    }
}

const char *sportItem_getGroupNameChar(sport_item_group_t group)
{
    const char *name = System_getLocaleText(sportItem_getGroupNameId(group));
    if (name == NULL || strlen(name) == 0)
    {
        return "name not found";
    }
    else
    {
        return name;
    }
}

const uint16_t sportItem_getGroupListSize(void)
{
    return sizeof(sportGroupArray) / sizeof(sportGroupArray[0]);
}

const SportItemGroup *sportItem_getGroupList(uint16_t group)
{
    return &sportGroupArray[group];
}

const sport_item_t *sportItem_getSubList(uint16_t item)
{
    return &SportItemArray[item];
}

bool sportItem_isSupport(sport_item_type_t itemType, sport_type_t sportType)
{
    sport_group_t group = sportGroup_get(sportType);
    const sport_rt_attr_t *attr = sportRt_getAttr(group);
    bool *list = (bool *)attr;
    if(list[itemType])
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool sportItem_isGroupSupport(sport_item_group_t itemGroup, sport_type_t sportType)
{
    sport_group_t group = sportGroup_get(sportType);
    const sport_rt_attr_t *attr = sportRt_getAttr(group);
    uint32_t i = 0;
    uint32_t size = 0;
    bool result = false;
    bool *list = (bool *)attr;
    bool *subList = NULL;
    switch(itemGroup)
    {
        case SPORT_ITEM_GROUP_TIME:
            subList = &list[SPORT_RT_ATTR_TYPE_DURATION_TOTAL];
            size = SPORT_RT_ATTR_TYPE_DURATION_CUR_GROUP - SPORT_RT_ATTR_TYPE_DURATION_TOTAL + 1;
            break;
        case SPORT_ITEM_GROUP_DISTANCE:
            subList = &list[SPORT_RT_ATTR_TYPE_DISTANCE_TOTAL];
            size = SPORT_RT_ATTR_TYPE_DISTANCE_PREV_SECTION - SPORT_RT_ATTR_TYPE_DISTANCE_TOTAL + 1;
            break;
        case SPORT_ITEM_GROUP_COUNT:
            subList = &list[SPORT_RT_ATTR_TYPE_COUNT_TOTAL];
            size = SPORT_RT_ATTR_TYPE_COUNT_CUR_FITNESS - SPORT_RT_ATTR_TYPE_COUNT_TOTAL + 1;
            break;
        case SPORT_ITEM_GROUP_DOWNHILL:
            subList = &list[SPORT_RT_ATTR_TYPE_GLIDE_COUNT];
            size = SPORT_RT_ATTR_TYPE_GLIDE_CUR_ALTITUDE - SPORT_RT_ATTR_TYPE_GLIDE_COUNT + 1;
            break;
        case SPORT_ITEM_GROUP_CLIMB_STAIRS:
            subList = &list[SPORT_RT_ATTR_TYPE_CLIMB_UP_FLOORS];
            size = SPORT_RT_ATTR_TYPE_CLIMB_DOWN_PREV_ALTITUDE - SPORT_RT_ATTR_TYPE_CLIMB_UP_FLOORS + 1;
            break;
        case SPORT_ITEM_GROUP_SWIMMING:
            subList = &list[SPORT_RT_ATTR_TYPE_SWIM_TOTAL_LAPS];
            size = SPORT_RT_ATTR_TYPE_SWIM_PREV_SWOLF - SPORT_RT_ATTR_TYPE_SWIM_TOTAL_LAPS + 1;
            break;
        case SPORT_ITEM_GROUP_PACE:
            subList = &list[SPORT_RT_ATTR_TYPE_PACE];
            size = SPORT_RT_ATTR_TYPE_PACE_PREV_AVG - SPORT_RT_ATTR_TYPE_PACE + 1;
            break;
        case SPORT_ITEM_GROUP_STEP:
            subList = &list[SPORT_RT_ATTR_TYPE_STRIDE_FREQ];
            size = SPORT_RT_ATTR_TYPE_STRIDE_COUNT - SPORT_RT_ATTR_TYPE_STRIDE_FREQ + 1;
            break;
        case SPORT_ITEM_GROUP_SPEED:
            subList = &list[SPORT_RT_ATTR_TYPE_SPEED];
            size = SPORT_RT_ATTR_TYPE_SPEED_VERTICAL - SPORT_RT_ATTR_TYPE_SPEED + 1;
            break;
        case SPORT_ITEM_GROUP_ALTITUDE:
            subList = &list[SPORT_RT_ATTR_TYPE_ALTITUDE];
            size = SPORT_RT_ATTR_TYPE_ALTITUDE_PREV_DOWN - SPORT_RT_ATTR_TYPE_ALTITUDE + 1;
            break;
        case SPORT_ITEM_GROUP_SLOPE:
            subList = &list[SPORT_RT_ATTR_TYPE_SLOPE];
            size = SPORT_RT_ATTR_TYPE_SLOPE_PREV_GLIDE - SPORT_RT_ATTR_TYPE_SLOPE + 1;
            break;
        case SPORT_ITEM_GROUP_SWING:
            subList = &list[SPORT_RT_ATTR_TYPE_BRANDISH_TOTAL_CNT];
            size = SPORT_RT_ATTR_TYPE_BRANDISH_SERVE_CNT - SPORT_RT_ATTR_TYPE_BRANDISH_TOTAL_CNT + 1;
            break;
        case SPORT_ITEM_GROUP_CONSUME:
            subList = &list[SPORT_RT_ATTR_TYPE_CONSUME];
            size = SPORT_RT_ATTR_TYPE_CONSUME_CUR - SPORT_RT_ATTR_TYPE_CONSUME + 1;
            break;
        case SPORT_ITEM_GROUP_BOATING:
            subList = &list[SPORT_RT_ATTR_TYPE_BOATING_FREQ];
            size = SPORT_RT_ATTR_TYPE_BOATING_PUSH - SPORT_RT_ATTR_TYPE_BOATING_FREQ + 1;
            break;
        case SPORT_ITEM_GROUP_FREQ:
            subList = &list[SPORT_RT_ATTR_TYPE_FREQ];
            size = SPORT_RT_ATTR_TYPE_FREQ_CUR - SPORT_RT_ATTR_TYPE_FREQ + 1;
            break;
        case SPORT_ITEM_GROUP_GOLF:
            subList = &list[SPORT_RT_ATTR_TYPE_GOLF_SPEED];
            size = SPORT_RT_ATTR_TYPE_GOLF_SCORE - SPORT_RT_ATTR_TYPE_GOLF_SPEED + 1;
            break;
        case SPORT_ITEM_GROUP_HR:
            subList = &list[SPORT_RT_ATTR_TYPE_HR];
            size = SPORT_RT_ATTR_TYPE_HR_PREV_RESERED_PERCENT - SPORT_RT_ATTR_TYPE_HR + 1;
            break;
        case SPORT_ITEM_GROUP_PRESSURE:
            subList = &list[SPORT_RT_ATTR_TYPE_PRESSURE];
            size = SPORT_RT_ATTR_TYPE_PRESSURE_PREV - SPORT_RT_ATTR_TYPE_PRESSURE + 1;
            break;
        case SPORT_ITEM_GROUP_TEMPERATUTE:
            subList = &list[SPORT_RT_ATTR_TYPE_TEMP];
            size = SPORT_RT_ATTR_TYPE_TEMP_MIN - SPORT_RT_ATTR_TYPE_TEMP + 1;
            break;
        case SPORT_ITEM_GROUP_OTHER:
            subList = &list[SPORT_RT_ATTR_TYPE_OTHER_SECTION_ORDER];
            size = SPORT_RT_ATTR_TYPE_OTHER_BORAMETER - SPORT_RT_ATTR_TYPE_OTHER_SECTION_ORDER + 1;
            break;
        default:
            break;
    }

    if(size == 0 || subList == NULL)
        return result;

    for(i = 0; i < size; i++)
    {
        result |= subList[i];
    }

    return result;
}