#ifndef SPORT_DATA_CADENCE_ASSISTTANT_PAGE_H
#define SPORT_DATA_CADENCE_ASSISTTANT_PAGE_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <gre/sdk/greal_alloc.h>
#include <presenter/sport_data_presenter.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _CadenceAssistantContainer CadenceAssistantContainer;

    struct _CadenceAssistantContainer
    {
        ScreenContainer container;
        void (*update)(CadenceAssistantContainer *cadenceAssistantContainer);
        gr_application_t *app;
        ScreenContext *screenContext;
        char *layerId;
    };
    CadenceAssistantContainer *newCadenceAssistantContainer(ScreenContext *screenContext, char *layerId);
#ifdef __cplusplus
}
#endif

#endif