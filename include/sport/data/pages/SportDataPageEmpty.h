#ifndef __SPORTS_DATA_PAGE_EMPTY_H__
#define __SPORTS_DATA_PAGE_EMPTY_H__

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <presenter/sport_data_presenter.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportDataPageEmpty SportDataPageEmpty;

    struct _SportDataPageEmpty
    {
        ScreenContainer container;
        gr_application_t *app;
        ScreenContext *screenContext;
        char *layerId;
        void (*update)(SportDataPageEmpty *page);
    };

    SportDataPageEmpty *newSportEmptyView(ScreenContext *screenContext, char *layerId);

#ifdef __cplusplus
}
#endif

#endif