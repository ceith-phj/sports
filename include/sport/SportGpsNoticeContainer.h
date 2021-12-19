#ifndef __SPORTS_PRENOTICE_VIEW_H__
#define __SPORTS_PRENOTICE_VIEW_H__

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include "sport_type_auto.h"
#include <widget/frameImgAnimation/frameImgAnimation.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportGpsNoticeContainer SportGpsNoticeContainer;

    struct _SportGpsNoticeContainer
    {
        ScreenContainer container;

        void (*initContainer)(SportGpsNoticeContainer *container, ScreenContext *context);
        void (*setVisiable)(SportGpsNoticeContainer *container, int isVisiable);
        bool (*getVisiable)(SportGpsNoticeContainer *container);
        void (*secTickEvent)(SportGpsNoticeContainer *container);
        void (*restoreContainer)(SportGpsNoticeContainer *container);
        uint32_t startTime;
        gr_application_t *app;
        ScreenContext *context;
    };

    SportGpsNoticeContainer *bindSportGpsNoticeView(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif