/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  liubingyan@huami.com
 *  Time : 2021-05-14
 *
 *  Describe : sports presenter for sports app
 *
 * ======================================================
 */

#ifndef SPORT_CHART_UTILS_H
#define SPORT_CHART_UTILS_H

#include <hformat.h>
#include "packages/apps/sports/include/manager/sports_chart_type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SPORT_CHART_LIST_SIZE SPORT_RECORD_CHART_MAX

    typedef struct sport_chart_t
    {
        uint16_t chartID;
        char *chartName;
    } sport_chart_t;

    const sport_chart_t *sportChart_getChartList(void);
    const uint16_t sportChart_getChartListSize(void);
    bool sportChart_isSupport(sport_chart_rt_type_t chartType, sport_type_t sportType);

#ifdef __cplusplus
}
#endif

#endif