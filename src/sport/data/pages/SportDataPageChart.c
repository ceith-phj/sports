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

#include <sport/data/pages/SportDataPageChart.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <hlog.h>
#include <sport_engine_utils.h>
#include <system_locale.h>
#include <settings_general_utils.h>
#include "packages/services/sport/misc/sport_misc.h"

#define TAG "sportChart"

CONTAINER_FUNCTION_DEFINE();

#define RT_CHART_LINE_WIDTH     (getDimension(4))

static const chartColor_t color[] =
{
    CHART_COLOR_RED,
    CHART_COLOR_YELLOW,
    CHART_COLOR_GREEN,
    CHART_COLOR_GREEN,
    CHART_COLOR_YELLOW,
    CHART_COLOR_BLUE,
    CHART_COLOR_GREEN,
};

static sport_rt_attr_type_t rtType[] =
{
    SPORT_RT_ATTR_TYPE_HR,
    SPORT_RT_ATTR_TYPE_SPEED,
    SPORT_RT_ATTR_TYPE_BOATING_FREQ,
    SPORT_RT_ATTR_TYPE_SWIM_STROKE_SPEED,
    SPORT_RT_ATTR_TYPE_PACE,
    SPORT_RT_ATTR_TYPE_ALTITUDE,
    SPORT_RT_ATTR_TYPE_FREQ,
};

// title note mertic_unit imperial_unit
static const char *chartText[][4] =
{
    {
        "id_sport_chart_hr_name",
        "id_rt_chart_hr_unit",
        "id_rt_chart_hr_unit",
    },
    {
        "id_sport_chart_speed_name",
        "id_sport_chart_speed_matric_unit",
        "id_sport_chart_speed_imperial_unit",
    },
    {
        "id_sport_chart_stroke_freq_name",
        "id_rt_chart_stroke_frep_unit",
        "id_rt_chart_stroke_frep_unit",
    },
    {
        "id_sport_chart_stroke_rate_name",
        "id_rt_chart_stroke_speed_unit",
        "id_rt_chart_stroke_speed_unit",
    },
    {
        "id_sport_chart_pace_name",
        "id_sport_chart_pace_matric_unit",
        "id_sport_chart_pace_imperial_unit",
    },
    {
        "id_sport_chart_altitude_name",
        "id_sport_chart_altitude_matric_unit",
        "id_sport_chart_altitude_imperial_unit",
    },
    {
        "id_rt_chart_freq",
        "id_spr_stroke_unit",
        "id_spr_stroke_unit",
    },
};

static uint32_t canvasRefs = 0;

static uint32_t getChartInfo(sport_data_page *pageData);
static void getChartValue(uint32_t index, float max, float mid, float min,
                          char *topStr, char *midStr, char *btmStr, uint32_t size, bool isValid);
static uint32_t getMetricConfig(void);

/**
 * @brief update chart
 *
 * @param item
 */
static void update(SportDataPageChart *item)
{
    if (!item || !item->pageData)
    {
        return;
    }

    sport_item_data itemInfo;
    sportChart_t *sportChart = item->sportChart;
    bool isValid = true;

    if (!item->itemInfo.chartInfo.dataInfo.dataNum
            || !strcmp(itemInfo.rt.value, "--"))
    {
        isValid = false;
    }

    if (!isValid)
    {
        item->itemInfo.chartInfo.dataInfo.dataMax = 0;
        item->itemInfo.chartInfo.dataInfo.dataMin = 0;
    }

    chartDescrip_t *chartDescrip = (chartDescrip_t *)hm_mem_malloc(sizeof(
                                       chartDescrip_t));
    H_ASSERT(chartDescrip);
    const uint32_t cnt = 300;
    uint32_t chartIndex = getChartInfo(item->pageData);
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_RT, rtType[chartIndex],
                                          &itemInfo);
    hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->avgString), "%s",
                itemInfo.rt.value);
    float max = item->itemInfo.chartInfo.dataInfo.dataMax;
    float min = item->itemInfo.chartInfo.dataInfo.dataMin;
    float mid = (max + min) * 0.5f;
    getChartValue(chartIndex, max, mid, min, chartDescrip->topString,
                  chartDescrip->midString, chartDescrip->btmString,
                  sizeof(chartDescrip->topString), isValid);
#define MAX_TIME_WINDOW     (5 * 60 * 1000)
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_RT,
                                          SPORT_RT_ATTR_TYPE_DURATION_TOTAL, &itemInfo);
    hm_snprintf(chartDescrip->leftString, sizeof(chartDescrip->leftString), "%d",
                (int)((itemInfo.rt.integer < MAX_TIME_WINDOW) ? 0 : (itemInfo.rt.integer -
                        MAX_TIME_WINDOW) / 60000));
    hm_snprintf(chartDescrip->rightString, sizeof(chartDescrip->rightString), "%d",
                (int)((itemInfo.rt.integer < MAX_TIME_WINDOW) ? 5 : itemInfo.rt.integer /
                      60000));
    memset(chartDescrip->maxString, 0, sizeof(chartDescrip->maxString));
    memset(chartDescrip->minString, 0, sizeof(chartDescrip->minString));
    sportChart->initDescrip(sportChart, chartDescrip);
    // ##
    item->itemInfo.chartInfo.dataPos = (int16_t *)hm_mem_malloc(cnt * sizeof(
                                           int16_t));
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_CHART,
                                          item->pageData->item[0], &item->itemInfo);

    if (item->pageData->item[0] == SPORT_RECORD_CHART_PACE)
    {
        for (uint32_t n = 0; n < cnt; n++)
        {
            if (item->itemInfo.chartInfo.dataPos[n] != -1)
            {
                item->itemInfo.chartInfo.dataPos[n] = 1000 -
                                                      item->itemInfo.chartInfo.dataPos[n];
            }
        }
    }

    point_t *pos = sportChart->convertArray(sportChart,
                                            item->itemInfo.chartInfo.dataPos, cnt, isValid);
    sportChart->fresh(sportChart, pos);
    // ##
    hm_mem_free(chartDescrip);
    hm_mem_free(item->itemInfo.chartInfo.dataPos);
}

static void initCharts(SportDataPageChart *page)
{
    uint32_t config = getMetricConfig();
    uint32_t index = getChartInfo(page->pageData);
    sportChart_t *sportChart = createSportChart(page->app);
    os_err_t error = sportChart->create(sportChart, page->app, page->layerId, NULL,
                                        NULL);
    H_ASSERT(!error);
    hm_snprintf(page->containerID, sizeof(page->containerID), "sportChart_%s",
                page->layerId);
    addContainer(page->screenContext, page->containerID, sportChart);
    chartInfo_t *chartInfo = (chartInfo_t *)hm_mem_malloc(sizeof(chartInfo_t));
    H_ASSERT(chartInfo);
    memset(chartInfo, 0, sizeof(chartInfo_t));
    chartInfo->lineWidth = RT_CHART_LINE_WIDTH;
    chartInfo->attr = CHART_FOR_IN_SPORT;
    chartInfo->lineColor = chartColor[color[index]][0];
    chartInfo->bgColor = chartColor[color[index]][1];
    chartInfo->topColor = chartColor[color[index]][0];
    chartInfo->btmColor = chartColor[color[index]][1];
    strncpy(chartInfo->title, System_getLocaleText(chartText[index][0]),
            sizeof(chartInfo->title));
    strncpy(chartInfo->note, System_getLocaleText("id_sport_chart_bottom_note"),
            sizeof(chartInfo->note));
    strncpy(chartInfo->unitString,
            System_getLocaleText(chartText[index][1 + config]),
            sizeof(chartInfo->unitString));
    strncpy(chartInfo->avgNameString,
            System_getLocaleText("id_sportrecord_char_avg_title"),
            sizeof(chartInfo->avgNameString));
    sportChart->initInfo(sportChart, chartInfo);
    hm_mem_free(chartInfo);
    sportChart->initCanvas(sportChart, canvasRefs);
    canvasRefs++;
    canvasRefs %= 3;
    page->sportChart = sportChart;
    HLOG_D(TAG, "%s starts to init\r\n\r\n", __func__);
    update(page);
}

SportDataPageChart *newSportDataPageChartView(ScreenContext *screenContext,
        char *layerId, sport_data_page *pageData)
{
    SportDataPageChart *item = H_NULL;
    createContainer(item, SportDataPageChart, item->container);
    H_ASSERT(item);
    item->screenContext = screenContext;
    item->app = screenContext->_grAppContext;
    item->pageData = hm_mem_malloc(sizeof(sport_data_page));
    H_ASSERT(item->pageData);
    memset(item->pageData, 0, sizeof(sport_data_page));
    memcpy(item->pageData, pageData, sizeof(sport_data_page));
    item->layerId = layerId;
    item->update = update;
    initCharts(item);
    return item;
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container, const HmKeyEvent *info)
{
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext,
                               ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext,
                        ScreenContainer *container, const char *id, const char *event)
{
    return false;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportDataPageChart *page = (SportDataPageChart *)container;

    if (page->pageData)
    {
        hm_mem_free(page->pageData);
    }

    removeContainer(screenContext, page->containerID);
}

static uint32_t getChartInfo(sport_data_page *pageData)
{
    if (!pageData)
    {
        return 0;
    }

    switch (pageData->item[0])
    {
        case SPORT_RECORD_CHART_HR:
            return 0;

        case SPORT_RECORD_CHART_SPEED:
            return 1;

        case SPORT_RECORD_CHART_ROW_STROKE_FREQ:
            return 2;

        case SPORT_RECORD_CHART_SWIM_STROKE_SPEED:
            return 3;

        case SPORT_RECORD_CHART_PACE:
            return 4;

        case SPORT_RECORD_CHART_ALTITUDE:
            return 5;

        case SPORT_RECORD_CHART_SKIP_ROPE_FREQ:
            return 6;

        default:
            return 0;
    }
}

static uint32_t getMetricConfig(void)
{
    // 0 : metric
    // 1 : imperial
    return setting_getMileageUnit();
}

static void getChartValue(uint32_t index, float max, float mid, float min,
                          char *topStr, char *midStr, char *btmStr, uint32_t size, bool isValid)
{
    if (!isValid)
    {
        memset(topStr, 0, size);
        memset(midStr, 0, size);
        memset(btmStr, 0, size);
        return;
    }

    float topValue = max;
    float midValue = mid;
    float btmValue = min;

    switch (index)
    {
        case 0: // hr
            hm_snprintf(topStr, size, "%d", (int)topValue);
            hm_snprintf(midStr, size, "%d", (int)midValue);
            hm_snprintf(btmStr, size, "%d", (int)btmValue);
            break;

        case 1: // speed
            topValue = topValue * 0.01;
            midValue = midValue * 0.01;
            btmValue = btmValue * 0.01;

            if (getMetricConfig())
            {
                topValue = sportEngine_utils()->km2mile(topValue);
                midValue = sportEngine_utils()->km2mile(midValue);
                btmValue = sportEngine_utils()->km2mile(btmValue);
            }

            hm_snprintf(topStr, size, "%.02f", topValue);
            hm_snprintf(btmStr, size, "%.02f", btmValue);
            hm_snprintf(midStr, size, "%.02f", midValue);
            break;

        case 2: // stroke freq
            hm_snprintf(topStr, size, "%d", (int)topValue);
            hm_snprintf(btmStr, size, "%d", (int)btmValue);
            hm_snprintf(midStr, size, "%d", (int)midValue);
            break;

        case 3: // stroke speed
            hm_snprintf(topStr, size, "%d", (int)topValue);
            hm_snprintf(btmStr, size, "%d", (int)btmValue);
            hm_snprintf(midStr, size, "%d", (int)midValue);
            break;

        case 4: // pace
            #define PACE_RATIO  (1)
            topValue = topValue * PACE_RATIO;
            midValue = midValue * PACE_RATIO;
            btmValue = btmValue * PACE_RATIO;

            if (getMetricConfig())
            {
                topValue = sportEngine_utils()->mile2km(topValue);
                midValue = sportEngine_utils()->mile2km(midValue);
                btmValue = sportEngine_utils()->mile2km(btmValue);
            }

            hm_snprintf(topStr, size, "%d'%02d", (int)btmValue * PACE_RATIO / 60,
                        (int)btmValue * PACE_RATIO % 60);
            hm_snprintf(btmStr, size, "%d'%02d", (int)topValue * PACE_RATIO / 60,
                        (int)topValue * PACE_RATIO % 60);
            hm_snprintf(midStr, size, "%d'%02d", (int)midValue * PACE_RATIO / 60,
                        (int)midValue * PACE_RATIO % 60);
            break;

        case 5: // altitude
            if (getMetricConfig())
            {
                topValue = sportEngine_utils()->meter2foot(topValue);
                midValue = sportEngine_utils()->meter2foot(midValue);
                btmValue = sportEngine_utils()->meter2foot(btmValue);
            }

            hm_snprintf(topStr, size, "%d", (int)topValue);
            hm_snprintf(btmStr, size, "%d", (int)btmValue);
            hm_snprintf(midStr, size, "%d", (int)midValue);
            break;

        case 6: // freq
            hm_snprintf(topStr, size, "%d", (int)topValue);
            hm_snprintf(midStr, size, "%d", (int)midValue);
            hm_snprintf(btmStr, size, "%d", (int)btmValue);
            break;

        default:    // hr
            hm_snprintf(topStr, size, "%d", (int)topValue);
            hm_snprintf(btmStr, size, "%d", (int)btmValue);
            hm_snprintf(midStr, size, "%d", (int)midValue);
            break;
    }
}
