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
#include "core/display_metrics.h"
#include "packages/services/syssevice/screen_status/system_screen_status.h"
#include <record/record_detail_jump_utils.h>
#include "widget/alertDialog/alertDialog.h"
#include <sport/SportStateBarContainer.h>
#include <system_locale.h>
#include "sport/SportDialog.h"
#include "ui/Sports/include/reminder/sport_reminder_container.h"
#include "packages/apps/settings/include/settings_general_utils.h"
#include <core/power/hm_power_manager.h>
#include "packages/services/syssevice/manager/system_manager.h"
#include "ui/Sports/include/group/sport_group_container.h"
#include <presenter/sport_data_presenter.h>
#include <hm_toast.h>
#include "packages/services/sport/misc/sport_misc.h"

#define TAG "sptgp"
#define ID_FIND_SCHEDULE "SportScreen.sportStateBarLayer"
#define ID_FIND_STATEBAR_CONTAINER "SportScreen.sportStateBarContainer"
#define ID_SPTGP_LAYER "sportGroupLayer"
#define ID_SPTGP_DETAIL_GROUP "sportGroupLayer.group_detail"
#define ID_SPTGP_GROUP_NUMBER_CONTROL "sportGroupLayer.group_number"
#define ID_SPTGP_GROUP_NUMBER_LABEL_CONTROL "sportGroupLayer.group_number_label"
#define ID_SPTGP_GROUP_EDIT "sportGroupLayer.group_edit"
#define ID_SPTGP_GROUP_NUMBER_VAR "sportGroupLayer.group_number.group_value"

#ifndef HM_DISPLAY_IS_ROUND
#define ID_SPORT_GROUP_EDIT_GR1_MARK_DEFAULT_X "sportGroupLayer.group_edit.gr_1_value.sport_group_edit_gr1_mark_default_x"
#define ID_SPORT_GROUP_DETAIL_GR1_MARK_DEFAULT_X "sportGroupLayer.group_detail.gr_1_value.sport_group_detail_gr1_mark_default_x"
#define ID_SPORT_GROUP_DETAIL_GR4_MARK_DEFAULT_X "sportGroupLayer.group_detail.gr_4_value.sport_group_detail_gr4_mark_default_x"

static H_Int32 getInt(const char * path)
{
    gr_application_t *app = getAppInstance();
    if(NULL == app)
    {
        return 0;
    }
    H_Int32 value = getSBIntData(app, path);
    return value;
}

#define SPORT_GROUP_EDIT_GR1_MARK_DEFAULT_X getInt(ID_SPORT_GROUP_EDIT_GR1_MARK_DEFAULT_X)
#define SPORT_GROUP_DETAIL_GR1_MARK_DEFAULT_X getInt(ID_SPORT_GROUP_DETAIL_GR1_MARK_DEFAULT_X)
#define SPORT_GROUP_DETAIL_GR4_MARK_DEFAULT_X getInt(ID_SPORT_GROUP_DETAIL_GR4_MARK_DEFAULT_X)
#endif


//---------------------------------------------
#define SPTGP_CHAR_MAX_NUM (24)
//---------------------------------------------
static SportGroupContainer *_gGroupContainer = NULL;

//---------------------------------------------
#define CNT_PICKER_INFO 1, 999, 1
#define GROUP_PICKER_CONTAINER_NAME "SportTypeInitpickerContainer"
#include "packages/apps/sports/include/presenter/sportTypeInitPresenter.h"
// static void _openPicker(ScreenContext *context);
//---------------------------------------------
static void _SptGp_showUI(gr_application_t *app, bool layerVisible, bool numberVisible, bool detailVisible, bool editVisible)
{
    gre_set_layer_visible(app, ID_SPTGP_LAYER, layerVisible);
    gre_set_control_visible(app, ID_SPTGP_GROUP_NUMBER_CONTROL, numberVisible);
    gre_set_control_visible(app, ID_SPTGP_GROUP_NUMBER_LABEL_CONTROL, numberVisible);
    gre_set_control_visible(app, ID_SPTGP_DETAIL_GROUP, detailVisible);
    gre_set_control_visible(app, ID_SPTGP_GROUP_EDIT, editVisible);
}

static void _SptGp_hideUI(gr_application_t *app)
{
    _SptGp_showUI(app, false, false, false, false);
}

//---------------------------------------------
// static void _SptGp_setNumberVar(gr_application_t *app, int num)
static void _SptGp_setNumber(gr_application_t *app)
{
    _SptGp_showUI(app, true, true, false, false);
    /* set group number */
    //_gGroupContainer->_gGoupNumber++;
    char value[12];
    snprintf(value, 12, "%d", _gGroupContainer->_gGoupNumber+1);
    setSBStringData(app, ID_SPTGP_GROUP_NUMBER_VAR, value);
    setSBStringData(app, "sportGroupLayer.group_number_label.value", System_getLocaleText("id_sptgp_group_number"));
}
//---------------------------------------------
static void _SptGp_hideNumber()
{
    if (_gGroupContainer)
    {
        #ifndef HM_DISPLAY_IS_ROUND
        SportStateBarContainer *pStateBarContainer = (SportStateBarContainer *)findContainer(_gGroupContainer->screenContext, ID_FIND_STATEBAR_CONTAINER);

        if (pStateBarContainer)
        {
            pStateBarContainer->setVisiable(pStateBarContainer, true);
            pStateBarContainer->setScheduleDefaultY(pStateBarContainer);
        }
        #endif
        _SptGp_showUI(_gGroupContainer->screenContext->_grAppContext, false, false, false, false);
        if (_gGroupContainer->timerHandle != NULL)
        {
            gre_timer_clear_timeout(_gGroupContainer->screenContext->_grAppContext, &_gGroupContainer->timerHandle);
            _gGroupContainer->isGroupNumberShowing = false;
            if (_gGroupContainer->timerOutCallBack)
                _gGroupContainer->timerOutCallBack();
            _gGroupContainer->timerHandle = NULL;
        }
    }
}
//---------------------------------------------
static int _numberShowTimeOutCb(struct gr_timer *timer, void *data, int dropped)
{
    _SptGp_hideNumber();
    return 0;
}

//---------------------------------------------
static void _SptGroupUIShow(SportGroupContainer *container, SptGpType type)
{
    if (container)
    {
        if(type==SPTGP_NUMBER){
            #ifndef HM_DISPLAY_IS_ROUND
            SportStateBarContainer *pStateBarContainer = (SportStateBarContainer *)findContainer(container->screenContext, ID_FIND_STATEBAR_CONTAINER);

            if (pStateBarContainer)
            {
                pStateBarContainer->setVisiable(pStateBarContainer, false);
                pStateBarContainer->setScheduleDisappearY(pStateBarContainer);
            }

            #endif
            container->isGroupNumberShowing = true;
            container->isGroupDetailShow = false;
            _SptGp_setNumber(container->screenContext->_grAppContext);
            container->timerHandle = gre_timer_create_timeout(container->screenContext->_grAppContext, 2000, 0, NULL, _numberShowTimeOutCb);
        }else if(type==SPTGP_DETAIL){
            #ifndef HM_DISPLAY_IS_ROUND
            SportStateBarContainer *pStateBarContainer = (SportStateBarContainer *)findContainer(container->screenContext, ID_FIND_STATEBAR_CONTAINER);

            if (pStateBarContainer)
            {
                pStateBarContainer->setVisiable(pStateBarContainer, true);
                pStateBarContainer->setScheduleDisappearY(pStateBarContainer);
            }
            #endif

            container->isGroupDetailShow = true;
            if (_gGroupContainer->currentSportType == GPTYPE_ROWING_MACHINE ||
                _gGroupContainer->currentSportType == GPTYPE_ROPE_SKIPPING ||
                _gGroupContainer->currentSportType == GPTYPE_INDOOR_FITNESS) {
                _SptGp_showUI(container->screenContext->_grAppContext, true, false, true, false);
            }else {
                _SptGp_showUI(container->screenContext->_grAppContext, true, false, false, true);
            }
        }else if(type==SPTGP_EDIT){
        }
    }
}


//---------------------------------------------
static bool _isSptGroupType(SportGroupContainer *container)
{
    bool isGroupType = false;
    sport_type_t type = getSportsScreenPresenter()->getSportType();
    switch (type)
    {
    case SPORT_TYPE_ROWING_MACHINE: /* 划船机 */
        isGroupType = true;
        break;
    case SPORT_TYPE_SKIPPING_ROPE: /* 跳绳 */
        isGroupType = true;
        break;
    case SPORT_TYPE_STRENGTH_TRAINING: /* 力量训练 */
        isGroupType = true;
        break;
    case SPORT_TYPE_INDOOR_FITNESS: /* 室内健身 */
        isGroupType = true;
        break;
    default:
        break;
    }
    return isGroupType;
}

/*
  for group detail page, for resest time to update
*/
static int _handleDetailTickEvent(struct gr_timer *timer, void *data, int dropped)
{
    sport_item_data itemInfo;
    int restHour = 0;
    if (_gGroupContainer->detailTickHandle == NULL)
    {
        HLOG_I(TAG, "%s timer handle null ", __func__);
        return 0;
    }
    if (data == NULL)
    {
        HLOG_I(TAG, "%s para data null ", __func__);
        return 0;
    }
    ScreenContext *screenContext = (ScreenContext *)data;
    if (screenContext == NULL || screenContext->_grAppContext == NULL)
    {
        HLOG_I(TAG, "%s para screenContext or appcontext null ", __func__);
        return 0;
    }

    _gGroupContainer->detailTick = hmTime_clock()/1000 - _gGroupContainer->startTime;

    /****刷新休息时间****/
    //int sec = _gGroupContainer->detailTick % 60;
    //int mins = _gGroupContainer->detailTick / 60 % 60;
    //int hour = _gGroupContainer->detailTick / 60 / 60;
    char timeStr[SPTGP_CHAR_MAX_NUM]="", timeStrHour[SPTGP_CHAR_MAX_NUM]="", heartRate[SPTGP_CHAR_MAX_NUM]="";


    restHour = _gGroupContainer->detailTick / 60 / 60;
    if(restHour>0){
        snprintf(timeStr, SPTGP_CHAR_MAX_NUM, "%02d:%02d", _gGroupContainer->detailTick / 60 % 60, _gGroupContainer->detailTick % 60);
        snprintf(timeStrHour, SPTGP_CHAR_MAX_NUM, "%d", _gGroupContainer->detailTick / 60 / 60);
    }else{
        snprintf(timeStr, SPTGP_CHAR_MAX_NUM, "%d:%02d", _gGroupContainer->detailTick / 60 % 60, _gGroupContainer->detailTick % 60);
        memset(timeStrHour, 0, SPTGP_CHAR_MAX_NUM);
    }

        /*更新心率*/
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_RT, SPORT_RT_ATTR_TYPE_HR, &itemInfo);
    if(itemInfo.rt.number_sign <=0 ){
        snprintf(heartRate, SPTGP_CHAR_MAX_NUM, "--");
    }else{
        snprintf(heartRate, SPTGP_CHAR_MAX_NUM, "%d", (int)itemInfo.rt.number_sign);
    }


    switch(_gGroupContainer->currentSportType){
        case GPTYPE_ROWING_MACHINE:   /**划船机*/
        case GPTYPE_ROPE_SKIPPING:    /**跳绳*/
        case GPTYPE_INDOOR_FITNESS:   /**室内健身*/
            #ifndef HM_DISPLAY_IS_ROUND
            if(strcmp(timeStrHour, "") != 0)
            {
                gre_set_control_x(screenContext->_grAppContext, "sportGroupLayer.group_detail.gr_4_value", SPORT_GROUP_DETAIL_GR4_MARK_DEFAULT_X);
            }
            #endif
            setSBStringData(screenContext->_grAppContext, "sportGroupLayer.group_detail.gr_4_value.value", timeStr); /** break time*/
            setSBStringData(screenContext->_grAppContext, "sportGroupLayer.group_detail.gr_4_corner_mark.value", timeStrHour); /** break time*/

            setSBStringData(screenContext->_grAppContext, "sportGroupLayer.group_detail.gr_5.value", heartRate);

            break;

        case GPTYPE_STRENGTH_TRAINING:    /**力量训练*/
            setSBStringData(screenContext->_grAppContext, "sportGroupLayer.group_edit.gr_1_value.value", timeStr); /** break time*/
            setSBStringData(screenContext->_grAppContext, "sportGroupLayer.group_edit.gr_1_corner_mark.value", timeStrHour); /** break time*/
            setSBStringData(screenContext->_grAppContext, "sportGroupLayer.group_edit.gr_2.value", heartRate);

            break;
    }
    gre_timer_set_repeat(screenContext->_grAppContext, timer, 100, 0);
    return 0;
}

//---------------------------------------------
static void _clearDetailTick(ScreenContext *screenContext)
{
    if (_gGroupContainer->detailTickHandle != NULL){
        gre_timer_clear_timeout(screenContext->_grAppContext, &_gGroupContainer->detailTickHandle);
        _gGroupContainer->detailTickHandle = NULL;
    }

}

//---------------------------------------------
static bool _startDetailTick(ScreenContext *screenContext)
{
    if (_gGroupContainer->detailTickHandle)
    {
        return false;
    }

    _gGroupContainer->detailTickHandle = gre_timer_create_timeout(screenContext->_grAppContext, 100, 0, screenContext, _handleDetailTickEvent);
    if (_gGroupContainer->detailTickHandle == NULL)
    {
        return true;
    }
    return true;
}

static void _groupReminder(SptGroupData *gpData)
{
    gr_application_t *app = _gGroupContainer->screenContext->_grAppContext;
    SptGpSpType spType = gpData->spType;

    char useTime[SPTGP_CHAR_MAX_NUM]="",
         useTimeHour[SPTGP_CHAR_MAX_NUM]="",
         heartRate[SPTGP_CHAR_MAX_NUM]="",
         title[SPTGP_CHAR_MAX_NUM]="",
         restTime[SPTGP_CHAR_MAX_NUM]="",
         restTimeHour[SPTGP_CHAR_MAX_NUM]="",
         tmpValue[SPTGP_CHAR_MAX_NUM]="";

    _gGroupContainer->currentSportType = gpData->spType;
    //_gGroupContainer->detailTick = 0;

    if(_gGroupContainer->parentScreenDataInit){
        _gGroupContainer->parentScreenDataInit();
    }
    int restHour=0, useHour=0;
    /*** init  work ***/
    getPowerManager()->userActivity(); /* 点亮屏幕 */
    if(!_gGroupContainer->isGroupDetailShow){
        getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_REMIND2);
    }
    _SptGroupUIShow(_gGroupContainer, SPTGP_DETAIL);


	/*to get title*/
    snprintf(title, SPTGP_CHAR_MAX_NUM, System_getLocaleText("id_sptgp_done"), gpData->index);

    _gGroupContainer->_gGoupNumber = gpData->index;
    /* to get 分组用时 **/
    //uint32_t time = gpData->time;
    //int sec = time % 60;
    //int mins = time / 60 % 60;
    //int resthour = gpData->time / 60 / 60;

    useHour = (int)(gpData->time / 60 / 60);
    if(useHour>0){
        snprintf(useTime, SPTGP_CHAR_MAX_NUM, "%02d:%02d", (int)(gpData->time/60%60), (int)(gpData->time%60));
        snprintf(useTimeHour, SPTGP_CHAR_MAX_NUM, "%d", useHour);
    }else{
        snprintf(useTime, SPTGP_CHAR_MAX_NUM, "%d:%02d", (int)(gpData->time/60%60), (int)(gpData->time%60));
        memset(useTimeHour, 0, SPTGP_CHAR_MAX_NUM);
    }

    _gGroupContainer->startTime = hmTime_clock()/1000 - _gGroupContainer->detailTick;


    /* to get heartbeat*/
    if(gpData->hr<=0){																  /**心率*/
        snprintf(heartRate, SPTGP_CHAR_MAX_NUM, "--");
    }else{
        snprintf(heartRate, SPTGP_CHAR_MAX_NUM, "%d", (int)gpData->hr);
    }

    /* to set rest time*/
    restHour = _gGroupContainer->detailTick/60/60;

    if(restHour>0){
        snprintf(restTime, SPTGP_CHAR_MAX_NUM, "%02d:%02d", _gGroupContainer->detailTick/60%60, _gGroupContainer->detailTick%60);
        snprintf(restTimeHour, SPTGP_CHAR_MAX_NUM, "%d", restHour);
    }else{
        snprintf(restTime, SPTGP_CHAR_MAX_NUM, "%d:%02d", _gGroupContainer->detailTick/60%60, _gGroupContainer->detailTick%60);
        memset(restTimeHour, 0, SPTGP_CHAR_MAX_NUM);
    }


    #ifndef HM_DISPLAY_IS_ROUND
    if (strcmp(restTimeHour,"") == 0)
    {
        int16_t gr_1_label_w = gre_get_control_w(app, "sportGroupLayer.group_detail.gr_1_label");
        int16_t gr_1_label_x = gre_get_control_x(app, "sportGroupLayer.group_detail.gr_1_label");
        int16_t gr_1_value_w = gre_get_control_w(app, "sportGroupLayer.group_detail.gr_1_value");
        int16_t gr_1_value_x = (gr_1_label_w - gr_1_value_w)/2+gr_1_label_x;
        gre_set_control_x(app, "sportGroupLayer.group_detail.gr_1_value", gr_1_value_x);

        int16_t gr_4_label_w = gre_get_control_w(app, "sportGroupLayer.group_detail.gr_4_label");
        int16_t gr_4_label_x = gre_get_control_x(app, "sportGroupLayer.group_detail.gr_4_label");
        int16_t gr_4_value_w = gre_get_control_w(app, "sportGroupLayer.group_detail.gr_4_value");
        int16_t gr_4_value_x = (gr_4_label_w - gr_4_value_w)/2+gr_4_label_x;
        gre_set_control_x(app, "sportGroupLayer.group_detail.gr_4_value", gr_4_value_x);
    }
    else
    {
        gre_set_control_x(app, "sportGroupLayer.group_detail.gr_1_value", SPORT_GROUP_DETAIL_GR1_MARK_DEFAULT_X);
        gre_set_control_x(app, "sportGroupLayer.group_detail.gr_4_value", SPORT_GROUP_DETAIL_GR4_MARK_DEFAULT_X);
    }

    SportStateBarContainer *pStateBarContainer = (SportStateBarContainer *)findContainer(_gGroupContainer->screenContext, ID_FIND_STATEBAR_CONTAINER);
    #endif
    switch(spType){
    case GPTYPE_ROWING_MACHINE:   /**划船机*/
        #ifdef HM_DISPLAY_IS_ROUND
        setSBStringData(app, "sportGroupLayer.group_detail.title.goup_value", title);
        #else
        setSBStringData(pStateBarContainer->app, "sportStateBarLayer.titleControl.title", title);
        #endif

        /** 组用时 */
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_label.value", System_getLocaleText("id_sptgp_time"));
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_value.value", useTime);
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_corner_mark.value", useTimeHour);

        /**组划数*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_2.label", System_getLocaleText("id_sptgp_number"));
        snprintf(tmpValue, SPTGP_CHAR_MAX_NUM, "%d", (int)gpData->count_cur_boat);           /**组划数*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_2.value", tmpValue);

        /**组划频*/
        snprintf(tmpValue, SPTGP_CHAR_MAX_NUM, "%d", (int)(gpData->boating_freq+0.5f));
        setSBStringData(app, "sportGroupLayer.group_detail.gr_3.label", System_getLocaleText("id_sptgp_stroke"));
        setSBStringData(app, "sportGroupLayer.group_detail.gr_3.value", tmpValue);

        /**本次休息**/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_label.value", System_getLocaleText("id_sptgp_break")); /**本次休息*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_value.value", restTime); /** break time*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_corner_mark.value", restTimeHour); /** break time*/

        /**心率**/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_5.label", System_getLocaleText("id_rt_hr")); /**心率*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_5.value", heartRate);
        break;

    case GPTYPE_ROPE_SKIPPING:    /**跳绳*/
        #ifdef HM_DISPLAY_IS_ROUND
        setSBStringData(app, "sportGroupLayer.group_detail.title.goup_value", title);
        #else
        setSBStringData(pStateBarContainer->app, "sportStateBarLayer.titleControl.title", title);
        #endif
        /** 组用时 */
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_label.value", System_getLocaleText("id_sptgp_time"));
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_value.value", useTime);
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_corner_mark.value", useTimeHour);

        /**组计数*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_2.label", System_getLocaleText("id_sptgp_cnt"));  /**组计数*/
        snprintf(tmpValue, SPTGP_CHAR_MAX_NUM, "%d", (int)gpData->count_cur_rope);
        setSBStringData(app, "sportGroupLayer.group_detail.gr_2.value", tmpValue);

        /**组频率*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_3.label", System_getLocaleText("id_sptgp_freq"));
        snprintf(tmpValue, SPTGP_CHAR_MAX_NUM, "%d", (int)(gpData->freq_cur+0.5f));               /**组频率*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_3.value", tmpValue);

        /**本次休息**/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_label.value", System_getLocaleText("id_sptgp_break")); /**本次休息*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_value.value", restTime); /** break time*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_corner_mark.value", restTimeHour); /** break time*/

        /**心率**/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_5.label", System_getLocaleText("id_rt_hr")); /**心率*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_5.value", heartRate);
        break;

    case GPTYPE_INDOOR_FITNESS:   /**室内健身*/
        #ifdef HM_DISPLAY_IS_ROUND
        setSBStringData(app, "sportGroupLayer.group_detail.title.goup_value", title);
        #else
        setSBStringData(pStateBarContainer->app, "sportStateBarLayer.titleControl.title", title);
        #endif

        /** 组用时 */
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_label.value", System_getLocaleText("id_sptgp_time"));
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_value.value", useTime);
        setSBStringData(app, "sportGroupLayer.group_detail.gr_1_corner_mark.value", useTimeHour);

        /**组消耗*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_2.label", System_getLocaleText("id_sptgp_consp"));
        snprintf(tmpValue, SPTGP_CHAR_MAX_NUM, "%d", (int)gpData->consume_cur);
        setSBStringData(app, "sportGroupLayer.group_detail.gr_2.value", tmpValue);

        /* 平均心率 */
        setSBStringData(app, "sportGroupLayer.group_detail.gr_3.label", System_getLocaleText("id_rt_hr_avg"));
        snprintf(tmpValue, SPTGP_CHAR_MAX_NUM, "%d", (int)gpData->hr_avg);
        setSBStringData(app, "sportGroupLayer.group_detail.gr_3.value", tmpValue);

        /**本次休息**/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_label.value", System_getLocaleText("id_sptgp_break")); /**本次休息*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_value.value", restTime); /** break time*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_4_corner_mark.value", restTimeHour); /** break time*/

        /**心率**/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_5.label", System_getLocaleText("id_rt_hr")); /**心率*/
        setSBStringData(app, "sportGroupLayer.group_detail.gr_5.value", heartRate);
        break;

    case GPTYPE_STRENGTH_TRAINING:    /**力量训练*/
        #ifdef HM_DISPLAY_IS_ROUND
        setSBStringData(app, "sportGroupLayer.group_edit.title.goup_value", title);
        #else
        setSBStringData(pStateBarContainer->app, "sportStateBarLayer.titleControl.title", title);
        #endif
        /**本次休息**/
        setSBStringData(app, "sportGroupLayer.group_edit.gr_1_label.value", System_getLocaleText("id_sptgp_break"));
        setSBStringData(app, "sportGroupLayer.group_edit.gr_1_value.value", restTime); /** break time*/
        setSBStringData(app, "sportGroupLayer.group_edit.gr_1_corner_mark.value", restTimeHour); /** break time*/

        /**心率**/
        setSBStringData(app, "sportGroupLayer.group_edit.gr_2.label", System_getLocaleText("id_rt_hr"));
        setSBStringData(app, "sportGroupLayer.group_edit.gr_2.value", heartRate);

        /**组计次*/
        snprintf(tmpValue, SPTGP_CHAR_MAX_NUM, "%d", (int)gpData->count_cur_fitness);
        setSBStringData(app, "sportGroupLayer.group_edit.gr_3.label", System_getLocaleText("id_sptgp_cnt_times"));
        setSBStringData(app, "sportGroupLayer.group_edit.gr_3.value", tmpValue);
        break;
    default:
        break;
	}

    /*启动时钟，定时更新数据*/
    _startDetailTick(_gGroupContainer->screenContext);
}


//---------------------------------------------
static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    _SptGp_hideUI(screenContext->_grAppContext);
    _clearDetailTick(screenContext);
    _gGroupContainer->_gGoupNumber = 0;

    // _SptGp_showUI(screenContext->_grAppContext, false, false, false, false);
    _gGroupContainer = NULL;

    HLOG_I(TAG, "group container destory");
}
//---------------------------------------------
static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    SportGroupContainer *groupContainer = (SportGroupContainer *)container;

    if(groupContainer->isPause){
        return false;
    }

    if (screenContext->_grAppContext && eventId == TOUCH_UP)
    {
        if (!strcmp(id, "sportGroupLayer.group_detail.gr_btn") ||
            !strcmp(id, "sportGroupLayer.group_edit.gr_btn"))
        {
            if (groupContainer->menuContainer && groupContainer->startNextGroupHandle)
            {
                groupContainer->startNextGroupHandle(groupContainer->menuContainer);
            }
            _clearDetailTick(screenContext);
            groupContainer->detailTick = 0;
            _SptGroupUIShow(groupContainer, SPTGP_NUMBER);
            getSportsScreenPresenter()->requestLap();
            getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_REMIND2);
        }
        // else if (!strcmp(id, "sportGroupLayer.group_edit.gr_3"))
        // {
        //     //goto selected page
        //     // _SptGroupUIShow(groupContainer);
        //     _openPicker(screenContext);
        // }
    }else if(eventId == TOUCH_SELECT){
        char hintKey[64] = {0};
        if(!strcmp(id, "sportGroupLayer.group_detail.title") ||
           !strcmp(id, "sportGroupLayer.group_edit.title")){
            hm_snprintf(hintKey, sizeof(hintKey), "%s.goup_value", id);
        }else if(!strcmp(id, "sportGroupLayer.group_detail.gr_1_label") ||
            !strcmp(id, "sportGroupLayer.group_detail.gr_1_value")){
            hm_snprintf(hintKey, sizeof(hintKey), "sportGroupLayer.group_detail.gr_1_label.value");
        }else if(!strcmp(id, "sportGroupLayer.group_detail.gr_4_label") ||
            !strcmp(id, "sportGroupLayer.group_detail.gr_4_value")){
            hm_snprintf(hintKey, sizeof(hintKey), "sportGroupLayer.group_detail.gr_4_label.value");
        }else if(!strcmp(id, "sportGroupLayer.group_edit.gr_1_label") ||
            !strcmp(id, "sportGroupLayer.group_edit.gr_1_value")){
            hm_snprintf(hintKey, sizeof(hintKey), "sportGroupLayer.group_edit.gr_1_label.value");
        }else{
            hm_snprintf(hintKey, sizeof(hintKey), "%s.label", id);
        }

        char *name = getSBStringData(screenContext->_grAppContext, hintKey);
        if(name){
            Toast_show(name);
            hm_mem_free(name);
        }
        return true;
    }else if(groupContainer->isGroupDetailShow || groupContainer->isGroupNumberShowing){
        return true;
    }
    return false;
}
//---------------------------------------------
static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container, const HmKeyEvent *info)
{
	SportGroupContainer *groupContainer = (SportGroupContainer *)container;

    if(info->code == HM_KEY_CODE_SHORTCUT && info->action == HM_KEY_ACTION_CLICK){

        if(groupContainer->isPause){
            return false;
        }

        if (screenContext->_grAppContext)
        {
            if(!groupContainer->isGroupNumberShowing){
                if (groupContainer->menuContainer && groupContainer->startNextGroupHandle)
                {
                    groupContainer->startNextGroupHandle(groupContainer->menuContainer);
                }
                _SptGroupUIShow(groupContainer, SPTGP_NUMBER);
                _clearDetailTick(screenContext);
                groupContainer->detailTick = 0;
                getSportsScreenPresenter()->requestLap();
                getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_REMIND2);
                //to close detail, for it close
                groupContainer->isGroupDetailShow = false;
                return true;
            }else{
                return true;
            }
        }
    }
    return false;
}
//---------------------------------------------
static bool handleGestureEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    SportGroupContainer *groupContainer = (SportGroupContainer *)container;

    if (eventId == GESTURE_RIGHT)
    {
        _SptGp_hideNumber();
        if (groupContainer->menuContainer && groupContainer->startNextGroupHandle){
            groupContainer->startNextGroupHandle(groupContainer->menuContainer);
        }
        return true;
    }
    return false;
}
//---------------------------------------------
static bool handleEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const char *event)
{
    return false;
}

//---------------------------------------------
static void setNextGroupHandler(SportGroupContainer *container, void *data, groupHandler_t callback)
{
    container->menuContainer = data;
    container->startNextGroupHandle = callback;
}

static void eventHandle(SportGroupContainer *container, SportEventType event)
{
    SportGroupContainer *groupContainer = (SportGroupContainer *)container;

    switch(event){
        case SPORT_EVENT_SUSPEND:
            _SptGp_hideUI(container->screenContext->_grAppContext);
            _clearDetailTick(container->screenContext);
            groupContainer->isGroupNumberShowing = false;
            if (groupContainer->menuContainer && groupContainer->startNextGroupHandle){
                groupContainer->startNextGroupHandle(groupContainer->menuContainer);
            }
            groupContainer->isPause = true;
            break;
        case SPORT_EVENT_CONTINUE:
            if(container->isGroupDetailShow){
                _SptGroupUIShow(container, SPTGP_DETAIL);
                _startDetailTick(container->screenContext);
                container->startTime = hmTime_clock()/1000 - container->detailTick;
            }
            groupContainer->isPause = false;
            break;
        default:
            groupContainer->isPause = false;
            break;
    }
}

//---------------------------------------------
SportGroupContainer *createGroupContainer(ScreenContext *screenContext, SportReminderPresenter *presenter)
{
    SportGroupContainer *groupContainer = H_NULL;
    if (!groupContainer)
    {
        createContainer(groupContainer, SportGroupContainer, groupContainer->container);
        groupContainer->presenter = presenter;
        groupContainer->presenter->groupReminder = _groupReminder;
        groupContainer->isSptGroupType = _isSptGroupType;
        groupContainer->SptGroupUIShow = _SptGroupUIShow;
        groupContainer->setNextGroupHandler = setNextGroupHandler;
        groupContainer->eventHandle = eventHandle;
        groupContainer->menuContainer = NULL;
        groupContainer->startNextGroupHandle = NULL;
        groupContainer->isPause = false;
        groupContainer->startTime = 0;
        groupContainer->isGroupDetailShow = false;
        groupContainer->isGroupNumberShowing = false;
        groupContainer->detailTick = 0;
        groupContainer->currentSportType = 0;
        groupContainer->detailTickHandle = NULL;
        groupContainer->_gGoupNumber = 0;
        groupContainer->parentScreenDataInit = NULL;
        _gGroupContainer = groupContainer;
    }
    return groupContainer;
}

#if 0
//--------------------------------------------
static void _PickerCallback(PickerWidget *picker, pickerEventTypeE Event, pickerUpdateInfo *updateInfo)
{
    if (PICKER_CANCEL == Event)
    {
        /* 滑动取消，建议不保存，移除container*/
        // removeContainer(getScreenContext(screenInstance), GROUP_PICKER_CONTAINER_NAME);
    }
    else if (PICKER_UPDATE == Event)
    {
        /* 数据更新，用来处理非法值或更新副标题*/
        if (picker->index[1] == 2)
        {
            picker->index[1] = 0;
            picker->setValue(picker, true);
        }
    }
    else if (PICKER_COMPLETE == Event)
    {
        // uint16_t value = updateInfo->value;
        if (_gGroupContainer != NULL)
        {
            removeContainer(_gGroupContainer->screenContext, GROUP_PICKER_CONTAINER_NAME);
            _gGroupContainer->groupPicker = NULL;
        }
    }
    return;
}
static void _openPicker(ScreenContext *context)
{
    /* 创建picker */
    PickerWidget *gPicker = creatPickerWidget(context);
    addContainer(context, GROUP_PICKER_CONTAINER_NAME, gPicker);
    /* 初始化数据 */
    /* 设置picker初始化数据 subTitle 不需的话要填 NULL */
    pickerInitData data = {
        .type = PICKER_SINGLE,
        .subTitle = NULL,
        .title = "Edit group count",
    };
    /* 初始化数据 */
    uint16_t item1IndexNum = gPicker->tool.getIndexMax(CNT_PICKER_INFO); //使用工具获取第一列项目数，输入参数必须“合法合理”
#define item2IndexNum 3
    /* 填充列表信息， 支持参数 连接符，单位，循环，是否为字符串 */
    pickerItemInfo item1 = {.length = item1IndexNum, .connector = NULL, .unit = NULL, .supportLoop = true, .isString = false};
    // pickerItemInfo item2 = {.length = item2IndexNum, .connector = NULL, .unit = NULL, .supportLoop = false, .isString = true};
    /* 填充列表值 ，直接填充，或使用工具填充 */
    char *itemStrs1 = H_MALLOC(SELECTOR_TEXT_MAX * item1IndexNum);
    // char itemStrs2[item2IndexNum][SELECTOR_TEXT_MAX] = {"m", "yard", " "};
    if (gPicker->tool.initNumberData(itemStrs1, CNT_PICKER_INFO) != item1IndexNum)
        HLOG_W("TAG", "initNumberData Not meeting expectations");

    item1.itemStrs = (char *)itemStrs1;
    // item2.itemStrs = (char *)itemStrs2;
    data.itemInfo[0] = item1;
    // data.itemInfo[1] = item2;
    /* 必须先填充数据，再初始化 */
    gPicker->init(context, &data, gPicker);
    gPicker->index[0] = gPicker->tool.getNumberIndex(50, CNT_PICKER_INFO);
    // picker->index[1] = 0;
    gPicker->setValue(gPicker, false);
    gPicker->setCallback(gPicker, _PickerCallback);
    gPicker->show(gPicker);
    /* 保存handle */
    // AppData *appData = getScreenData(context, AppData);
    // appData->picker = picker;
    _gGroupContainer->groupPicker = gPicker;
    /* 数据初始化之后，填充的数据可以释放 */
    H_FREE(itemStrs1);
}
#endif
