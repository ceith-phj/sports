#ifndef SPORTS_DATA_PAGE_CHART_H
#define SPORTS_DATA_PAGE_CHART_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <presenter/sport_data_presenter.h>
#include <widget/sportChart/sportChart.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportDataPageChart SportDataPageChart;

    struct _SportDataPageChart
    {
        ScreenContainer container;

        void (*update)(SportDataPageChart* item);

        gr_application_t *app;
        ScreenContext *screenContext;
        sport_data_page *pageData;
        sportChart_t *sportChart;
        char* layerId;
        char containerID[64];
        sport_item_data itemInfo;
    };

    SportDataPageChart *newSportDataPageChartView(ScreenContext *screenContext, char* layerId, sport_data_page *pageData);

#ifdef __cplusplus
}
#endif

#endif