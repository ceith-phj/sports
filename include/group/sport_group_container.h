/*
 * =====================================================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  fengdandan@huami.com
 *  Time : 2021-5-31
 *
 *  Describe : sport group page
 *
 * ======================================================================================
 */

#ifndef SPORT_GROUP_CONTAINER_H
#define SPORT_GROUP_CONTAINER_H
//===============================
#include <gre/gre.h>
#include <widget/base/sb_container.h>
//===============================
typedef struct _SportGroupContainer SportGroupContainer;
//===============================
//-------------------------------


typedef enum {
    SPTGP_NUMBER = 0,
    SPTGP_DETAIL,
    SPTGP_EDIT,
}SptGpType;

typedef void (*groupHandler_t)(void *data);

struct _SportGroupContainer
{
    ScreenContainer container;
    gr_application_t *app;
    ScreenContext *screenContext;
    SportReminderPresenter *presenter;
    bool isGroupNumberShowing;
    bool isGroupDetailShow;
    bool isShow;
    void *timerHandle;
    void (*timerOutCallBack)(void);
    bool (*isSptGroupType)(SportGroupContainer *container);
    void (*SptGroupUIShow)(SportGroupContainer *container, SptGpType type);
    void (*setNextGroupHandler)(SportGroupContainer *container, void *data, groupHandler_t callback);
    void (*eventHandle)(SportGroupContainer *container, SportEventType event);
    void (*parentScreenDataInit)(void);
    groupHandler_t startNextGroupHandle;
    void *menuContainer;
    PickerWidget *groupPicker;
    bool isPause;
    int detailTick;
    SptGpSpType currentSportType;
    HmTimeStamp startTime;
    void *detailTickHandle;
    int16_t _gGoupNumber;
};

//-------------------------------
SportGroupContainer *createGroupContainer(ScreenContext *screenContext, SportReminderPresenter *presenter);
//-------------------------------
#endif
