#ifndef SPORT_CHILD_SETTING_CONTAINER_H
#define SPORT_CHILD_SETTING_CONTAINER_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <setting/BaseSportSettingLayer.h>

#ifdef __cplusplus
extern "C"
{
#endif

    BaseSportSettingLayer *createMainSettingContainer(gr_application_t *app, char* layer);

#ifdef __cplusplus
}
#endif

#endif