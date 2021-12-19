/*
 * =====================================================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  fengdandan@huami.com
 *  Time : 2021-6-15
 *
 *  Describe :
 *
 * ======================================================================================
 */
#include <BaseSportView.h>
#include <stdbool.h>
#include <gre/gre.h>
#include <grelib.h>
#include <sb_screen_context.h>
#include <presenter/SportPresenter.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include "hm_time.h"
#include <gre/sdk/grelib.h>
#include "display_metrics.h"
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <widget/pageScrollerMultiLayers/pageScrollerMultiLayers.h>
#include <sport/SportPrestartContainer.h>
#include <sport/data/SportDataContainer.h>
#include <sport/SportOpreationView.h>
#include <sport/SportPauseOpreationView.h>
#include "core/display_metrics.h"
#include "packages/services/syssevice/screen_status/system_screen_status.h"
#include <record/record_detail_jump_utils.h>
#include "widget/alertDialog/alertDialog.h"
#include <sport/SportStateBarContainer.h>
#include <system_locale.h>
#include "sport/SportDialog.h"
#include "ui/Sports/include/reminder/sport_reminder_container.h"
#include "ui/Sports/include/transAnima/sport_trains_anima.h"
#include "packages/apps/settings/include/settings_general_utils.h"
#include <core/power/hm_power_manager.h>
#include "packages/services/syssevice/manager/system_manager.h"
#include "packages/services/sport/include/sport_service_message.h"
#include "ui_handle.h"
//=========================================
//=========================================
#define TAG "sptta"
//-----------------------------------------
#define SPORT_TRANS_ANIMA_TIME (600)
static bool _bIsPause = false;
static SportTransAnima *_gAnima = NULL;

//-----------------------------------------
static void _transAnimaLayerVisble(ScreenContext *screenContext, bool blayer, bool bstart, bool bpause, bool bpausebg);
static bool _autoPauseEventListener(EventData *eventData);
//-----------------------------------------
static void _destory(SportTransAnima *anima)
{
    if(!anima){
        return;
    }

    _transAnimaLayerVisble(anima->screenContext, false, false, false, false);

    if (anima->timerHandle)
    {
        gre_timer_clear_timeout(anima->screenContext->_grAppContext, &(anima->timerHandle));
        anima->timerHandle = NULL;
    }

    SportService_getInstance()->delTaskEventListener(getUIHandle(), _autoPauseEventListener);

    OS_FREE(anima);
    anima = NULL;
    _gAnima = NULL;
}
//-----------------------------------------
static void _finish(SportTransAnima *anima)
{
    if (anima)
    {
        _transAnimaLayerVisble(anima->screenContext, false, false, false, false);
        _bIsPause = false;
    }
}
//-----------------------------------------
static void _transAnimaLayerVisble(ScreenContext *screenContext, bool blayer, bool bstart, bool bpause, bool bpausebg)
{
    gre_set_layer_active(screenContext->_grAppContext, "SportScreen.sport_trans", 0);
    gre_set_layer_visible(screenContext->_grAppContext, "sport_trans", blayer);
    gre_set_control_visible(screenContext->_grAppContext, "sport_trans.start", bstart);
    gre_set_control_visible(screenContext->_grAppContext, "sport_trans.pause", bpause);
    gre_set_control_visible(screenContext->_grAppContext, "sport_trans.pause_bg", bpausebg);
}
//-----------------------------------------
static int _SportTransTimeOut(struct gr_timer *timer, void *data, int dropped)
{
    SportTransAnima *anima = (SportTransAnima *)data;
    _transAnimaLayerVisble(anima->screenContext, _bIsPause, false, false, _bIsPause);
    gre_timer_clear_timeout(anima->screenContext->_grAppContext, &(anima->timerHandle));
    if(anima->animaTimerOutCallBack){
        anima->animaTimerOutCallBack();
    }
    if (anima->timerHandle)
    {
        anima->timerHandle = NULL;
    }
    return 0;
}
//-----------------------------------------
static void _startAnima(SportTransAnima *anima)
{
    if (anima == NULL)
    {
        return;
    }
    if (anima->timerHandle)
    {
        gre_timer_clear_timeout(anima->screenContext->_grAppContext, &(anima->timerHandle));
        anima->timerHandle = NULL;
    }
    _bIsPause = false;
    _transAnimaLayerVisble(anima->screenContext, true, true, false, false);
    anima->timerHandle = gre_timer_create_timeout(anima->screenContext->_grAppContext, SPORT_TRANS_ANIMA_TIME, 0, (void *)anima, _SportTransTimeOut);
}

//-----------------------------------------
static bool _isPause(SportTransAnima *anima){
    if(!anima){
        return false;
    }

    return _bIsPause;
}
//-----------------------------------------
static void _pauseAnima(SportTransAnima *anima, bool withAnima)
{
    if (anima == NULL)
    {
        return;
    }
    if (anima->timerHandle)
    {
        gre_timer_clear_timeout(anima->screenContext->_grAppContext, &(anima->timerHandle));
        anima->timerHandle = NULL;
    }

	_bIsPause = true;
    if(withAnima){
        _transAnimaLayerVisble(anima->screenContext, true, false, true, true);
        anima->timerHandle = gre_timer_create_timeout(anima->screenContext->_grAppContext, SPORT_TRANS_ANIMA_TIME, 0, (void *)anima, _SportTransTimeOut);
    }else{
        _transAnimaLayerVisble(anima->screenContext, true, false, false, true);
    }
}
//-----------------------------------------
static bool _autoPauseEventListener(EventData *eventData)
{
    if (_gAnima == NULL)
    {
        HLOG_I(TAG, "%s gAnima is NULL", __func__);
        return false;
    }
    H_Int32 eventType = eventData->arg1;
    H_Int32 pState = eventData->arg2;
    HLOG_I(TAG, "auto pause %d %d", eventType, pState);
    if (eventType == EVT_AUTO_PAUSE)
    {
        if (pState == AUTO_PAUSED)
        {
            getSportsScreenPresenter()->requestPause();
            _pauseAnima(_gAnima, true);
        }
        else if (pState == AUTO_RESUMED)
        {
            getSportsScreenPresenter()->requestContinue();
            _startAnima(_gAnima);
        }
    }
    return true;
}
//-----------------------------------------
static void _registerAutoStop()
{
    HLOG_I(TAG, "register auto stop");
    SportService_getInstance()->addTaskEventListener(
        getUIHandle(),
        SPORT_SERVICE_EVT_REPORT_EVENT,
        _autoPauseEventListener);
}
//-----------------------------------------
// static void *tempHandle = NULL, *tempHandle2 = NULL;
// static int _testAutoPause(struct gr_timer *timer, void *data, int dropped)
// {
//     EventData *event_data = message_allocEvent();
//     if (H_NULL == event_data)
//     {
//         return 0;
//     }
//     event_data->name = "auto pause";
//     event_data->arg1 = EVT_AUTO_PAUSE;
//     event_data->arg2 = AUTO_PAUSED;
//     SportService_getInstance()->advertiseEvent(SPORT_SERVICE_EVT_REPORT_EVENT, event_data);

//     SportTransAnima *anima = (SportTransAnima *)data;
//     gre_timer_clear_timeout(anima->screenContext->_grAppContext, &tempHandle);
//     if (tempHandle)
//     {
//         tempHandle = NULL;
//     }
//     return 0;
// }

// static int _testAutoResume(struct gr_timer *timer, void *data, int dropped)
// {
//     EventData *event_data = message_allocEvent();
//     if (H_NULL == event_data)
//     {
//         return 0;
//     }
//     event_data->name = "auto pause";
//     event_data->arg1 = EVT_AUTO_PAUSE;
//     event_data->arg2 = AUTO_RESUMED;
//     SportService_getInstance()->advertiseEvent(SPORT_SERVICE_EVT_REPORT_EVENT, event_data);

//     SportTransAnima *anima = (SportTransAnima *)data;
//     gre_timer_clear_timeout(anima->screenContext->_grAppContext, &tempHandle2);
//     if (tempHandle2)
//     {
//         tempHandle2 = NULL;
//     }
//     return 0;
// }
//-----------------------------------------
// static void _testAutoPauseTimer(SportTransAnima *anima)
// {
//     tempHandle = gre_timer_create_timeout(anima->screenContext->_grAppContext, 1000 * 3, 0, (void *)anima, _testAutoPause);
//     tempHandle2 = gre_timer_create_timeout(anima->screenContext->_grAppContext, 1000 * 10, 0, (void *)anima, _testAutoResume);
// }
//-----------------------------------------
SportTransAnima *createSportTransAnima(ScreenContext *screenContext)
{
    SportTransAnima *anima = NULL;
    anima = OS_MALLOC(sizeof(SportTransAnima));
    if (anima == NULL)
    {
        return NULL;
    }
    _gAnima = anima;
    memset(anima, 0, sizeof(SportTransAnima));
    anima->screenContext = screenContext;
    anima->start = _startAnima;
    anima->pause = _pauseAnima;
    anima->destory = _destory;
    anima->finish = _finish;
    anima->isPause = _isPause;
    _registerAutoStop();
    // _testAutoPauseTimer(anima);
    return anima;
}
