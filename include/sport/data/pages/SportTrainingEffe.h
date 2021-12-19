#ifndef SPORT_TRAINING_EFFE_H
#define SPORT_TRAINING_EFFE_H

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <widget/base/sb_container.h>
#include <gre/sdk/greal_alloc.h>

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct _TrainingEffeContainer TrainingEffeContainer;

struct _TrainingEffeContainer
{
    ScreenContainer container;
    void (*update)(TrainingEffeContainer *trainingEffeContainer);
    gr_application_t *app;
    ScreenContext *screenContext;
    char *layerId;
    int layerX;
    int layerY;
    int layerW;
    int layerH;
};
TrainingEffeContainer *newTrainingEffeContainer(
    ScreenContext *screenContext, char *layerId);

#ifdef __cplusplus
}
#endif

#endif