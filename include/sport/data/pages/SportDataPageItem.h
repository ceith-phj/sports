#ifndef SPORTS_DATA_PAGE_ITEM_H
#define SPORTS_DATA_PAGE_ITEM_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <presenter/sport_data_presenter.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportDataPageItem SportDataPageItem;

    struct _SportDataPageItem
    {
        ScreenContainer container;

        void (*update)(SportDataPageItem *item);
        gr_application_t *app;
        ScreenContext *screenContext;
        sport_data_page *pageData;
        char *layerId;
        void *timerHandle;
        uint32_t latestSec;
        bool isStarted;
        bool isPreStarted;
        bool isEditMode;
    };

    SportDataPageItem *newSportDataPageItemView(ScreenContext *screenContext, char *layerId, sport_data_page *pageData);
    SportDataPageItem *newDataEditPageItemView(ScreenContext *screenContext, char *layerId, sport_data_page *pageData, bool isEditMode);

#ifdef __cplusplus
}
#endif

#endif