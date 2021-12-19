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
#include <ui/Sports/include/itemEdit/sport_chart_utils.h>
#include "framework/engine/sportEngine/sport_sensor_attr_auto.h"

const sport_chart_t sportChartArray[] = {
    {SPORT_RECORD_CHART_HR,                 "id_rt_chart_hr"},
    {SPORT_RECORD_CHART_SPEED,              "id_rt_chart_speed"},
    {SPORT_RECORD_CHART_ROW_STROKE_FREQ,    "id_rt_chart_stroke_frep"},
    {SPORT_RECORD_CHART_SWIM_STROKE_SPEED,  "id_rt_chart_stroke_speed"},
    {SPORT_RECORD_CHART_PACE,               "id_rt_chart_pace"},
    {SPORT_RECORD_CHART_ALTITUDE,           "id_rt_chart_altitude"},
    {SPORT_RECORD_CHART_SKIP_ROPE_FREQ,     "id_rt_chart_freq"},
    {SPORT_RECORD_CHART_TE,                 "id_rt_chart_te"},
    // {SPORT_RECORD_CHART_STRESS,             "id_rt_chart_press"},
    // {SPORT_RECORD_CHART_TEMPERATURE,        "id_rt_chart_temperature"},
};

const sport_chart_t *sportChart_getChartList(void)
{
    return &sportChartArray[0];
}

const uint16_t sportChart_getChartListSize(void)
{
    return sizeof(sportChartArray) / sizeof(sportChartArray[0]);
}

bool sportChart_isSupport(sport_chart_rt_type_t chartType, sport_type_t sportType)
{
    sport_group_t group = sportGroup_get(sportType);
    const sport_rt_attr_t *attr = sportRt_getAttr(group);

    if(chartType == SPORT_RECORD_CHART_HR &&
        attr->has_chart_hr)
    {
        return true;
    }

    if(chartType == SPORT_RECORD_CHART_SPEED &&
        attr->has_chart_speed)
    {
        return true;
    }

    if(chartType == SPORT_RECORD_CHART_ROW_STROKE_FREQ &&
        attr->has_chart_stroke_frep)
    {
        return true;
    }

    if(chartType == SPORT_RECORD_CHART_SWIM_STROKE_SPEED &&
        attr->has_chart_stroke_speed)
    {
        return true;
    }

    if(chartType == SPORT_RECORD_CHART_PACE &&
        attr->has_chart_pace)
    {
        return true;
    }

    if(chartType == SPORT_RECORD_CHART_ALTITUDE &&
        attr->has_chart_altitude)
    {
        return true;
    }

    if(chartType == SPORT_RECORD_CHART_SKIP_ROPE_FREQ &&
        attr->has_chart_freq)
    {
        return true;
    }

    if(chartType == SPORT_RECORD_CHART_TE &&
        attr->has_chart_te)
    {
        return true;
    }

    return false;
}
