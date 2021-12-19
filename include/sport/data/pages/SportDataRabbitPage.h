#ifndef SPORT_DATA_RABBIT_PAGE_H
#define SPORT_DATA_RABBIT_PAGE_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <gre/sdk/greal_alloc.h>
#include <presenter/sport_data_presenter.h>
#include <widget/frameImgAnimation/frameImgAnimation.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportRabbiteContainer SportRabbiteContainer;

    struct _SportRabbiteContainer
    {
        ScreenContainer container;
        void (*update)(SportRabbiteContainer *sportRabbiteContainer);
        void (*startSport)(SportRabbiteContainer *sportRabbiteContainer);
        void (*stopSport)(SportRabbiteContainer *sportRabbiteContainer);
        void (*startVillain)(SportRabbiteContainer *sportRabbiteContainer);
        void (*stopVillain)(SportRabbiteContainer *sportRabbiteContainer);
        gr_application_t *app;
        ScreenContext *screenContext;
        char *layerId;
        int arrowWidth;
        int arrowHeight;
        int rabbitWidth;
        int rabbitHeight;
        int villainWidth;
        int villainHeight;
        int rabbitX;
        int rabbitY;
        int villainX;
        int villainY;
        int displayWidth;
        int moveWidth;
        frameImgAnimation *rabbitContainer;
        frameImgAnimation *villainContainer;
    };
    SportRabbiteContainer *newSportRabbiteContainer(ScreenContext *screenContext, char *layerId);
#ifdef __cplusplus
}
#endif
#endif