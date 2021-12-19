#ifndef SPORT_DATA_COMPASS_PAGE_H
#define SPORT_DATA_COMPASS_PAGE_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <gre/sdk/greal_alloc.h>
#include <packages/apps/compass/include/compass_presenter.h>
#include "hlog.h"
#include <gre/grecanvas.h>
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _CompassContainer CompassContainer;

struct _CompassContainer
{
    ScreenContainer container;
    void (*update)(CompassContainer *cadenceAssistantContainer);
    gr_application_t *app;
    ScreenContext *screenContext;
    CompassPresenter *compassPresenter;
    char *layerId;
    H_Uint32 currentPage;
    CompassResult currentValue;
    gr_animation_handle_t *g_ready_ball_anim;
    void *g_timerHander;
    H_Int32 readyCurrentDegree;
    H_Int32 readyTargetDegree;
    H_Int32 pointCurrentDegree;
    H_Int32 pointTargetDegree;
    H_Int32 pointLastDegree;
    gr_canvas_t *g_ready_canvas;
    H_Int32 readyCanvasCenterX;
    H_Int32 readyCanvasCenterY;
    H_Uint8 userExit;
    const char *str_e;
    const char *str_n;
    const char *str_ne;
    const char *str_nw;
    const char *str_s;
    const char *str_se;
    const char *str_sw;
    const char *str_w;
    const char *str_pa;
    const char *str_m;
#if HMI_COMP_STATISTICS && !SIMULATOR
    H_Bool pointUsed2Sec;
    uint8_t pointUsedTick;
#endif
};
CompassContainer *newCompassContainer(ScreenContext *screenContext,
                                      char *layerId);
#ifdef __cplusplus
}
#endif

#endif