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

#ifndef SPORTS_DATA_VIEW_H
#define SPORTS_DATA_VIEW_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <widget/pageScrollerInLayer/pageScrollerInLayer.h>
#include <presenter/sports_data_presenter.h>
#include <widget/pageScrollerInLayer/ViewPager.h>
#include <sport/data/SportDataViewAdapter.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportDataContainer SportDataContainer;

    struct _SportDataContainer
    {
        ScreenContainer container;
        void (*initContainer)(SportDataContainer *dataView, ScreenContext *screenContext);
        void (*setVisiable)(SportDataContainer *dataView, H_Int8 isVisiable);
        bool (*getVisiable)(SportDataContainer *dataView);
        void (*initData)(SportDataContainer *sportDataView, const char *path, const char *screenName);
        void (*initPageView)(SportDataContainer *sportDataView);
        void (*secTickEvent)(SportDataContainer *container);
        void (*sportStatus)(SportDataContainer *dataView, void *event);

        char *screenName;
        gr_application_t *app;
        ScreenContext *screenContext;
        char rootPath[120];
        ViewPager *dataPageView;
        ViewPagerAdapter *dataViewPagerAdapter;
        OnPageChangeListener *onPageChangeListener;
        void *timerHandle;
        SportsDataPresenter *dataPresenter;
    };

    SportDataContainer *bindSportDataView(ScreenContext *screenContext);

#ifdef __cplusplus
}
#endif

#endif