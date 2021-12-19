/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  wangxiaomeng@huami.com
 *  Time : 2021-3-20
 *
 *  Describe :
 *
 * ======================================================
 */
#include <stdbool.h>
#include "sport_utils.h"
#include "sport_type_default_map.h"
#include "sport_service.h"
#include "manager/sports_control_manager.h"
#include "sport_type_utils.h"
#include <hlog.h>
#include <system_manager.h>

#define TAG "Sport_start"

static void _initSuccessCallback(void * data)
{
    sport_type_t type = *(sport_type_t *)data;
    getSportUtils()->startByFlag(type, FLAG_SCREEN_REMOVE_CURRENT);
}

/*
    检测特定类型是否初始化过
    已初始化过返回true
    未初始化转跳初始化页面
*/
static bool _checkTypeInitialized(sportInitData *data)
{
    sportInitData *initData;
    initData = data;
    if (initData->callBack == NULL)
    {
        initData->callBack = _initSuccessCallback;
    }

    switch (initData->type)
    {
    case SPORT_TYPE_SWIMMING_POOL: //泳池泳道长度选择
    {
        SwimPoolLength swim;
        sportSetting_getSettingManager()->poolLength->get(initData->type, &swim);
        if(swim.onOff.onOff == 0)
        {
            getScreenManager()->startScreen("SportTypeInitScreen", initData, sizeof(sportInitData));
            return false;
        }
        else
        {
            return true;
        }
    }

    case SPORT_TYPE_TENNIS:         //网球 选左右手
    case SPORT_TYPE_ROWING_MACHINE: //划船机
    case SPORT_TYPE_GOLF_SWING:     //高尔夫挥杆
    case SPORT_TYPE_OUTDOOR_ROWING: //户外划船
    {
        SportSettingTypeChoice choice;
        sportSetting_getSettingManager()->wearHand->get(initData->type, &choice);
        if (choice.choice == 0)
        {
            getScreenManager()->startScreen("SportTypeInitScreen", initData, sizeof(sportInitData));
            return false;
        }
        else
        {
            return true;
        }
    }

    default:
        return true;
    }
}

static void _startByFlag(sport_type_t type, ScreenFlag flag)
{
    HLOG_I(TAG, "%s type %d", __func__, type);
    //type 合法
    if (sportType_isSupported(type) == false)
    {
        type = SPORT_TYPE_OUTDOOR_RUNNING;
    }
    SportStatusInfo sportInfo;
    SportService_getInstance()->getCtrlService()->getStatusInfo(&sportInfo);
    if (sportInfo.in_sport == false) //未运动开始 转跳开始运动
    {
        if (H_False == System_checkUserSpaceAvailable(H_True)) {
            HLOG_I(TAG, "system space is not enough\r\n");
            return;
        }
        sportInitData initData;
        initData.type = type;
        initData.callBack = _initSuccessCallback;
        if (_checkTypeInitialized(&initData) == true)
        {
            HLOG_I(TAG, "%s: %s\r\n", __func__, SportType_getTypeName(type));
            getSportsControlManager()->init();
            getSportsControlManager()->setType(type);
            getScreenManager()->startScreenByFlag("SportScreen", flag, H_NULL, 0);
            /* 标记进入运动 */
            SportService_getInstance()->getCtrlService()->enterSportView();
        }
        else
        {
            HLOG_I(TAG, "%s: %s need init", __func__, SportType_getTypeName(type));
        }
    }
    else //已经开始运动 回到运动中
    {
        getSportUtils()->openSport();
    }
}

static void _start(sport_type_t type)
{
    _startByFlag(type, FLAG_SCREEN_STANDAR);
}

static void _openSportByFlag(ScreenFlag flag)
{
    SportStatusInfo sportInfo = {0};
    H_Bool isRecogPhase = SportAutoRecogService_getOps()->isRecogPhase();
    SportService_getInstance()->getCtrlService()->getStatusInfo(&sportInfo);
    if (sportInfo.in_sport == true && !isRecogPhase) //已经开始运动 回到运动中
    {
        HLOG_I(TAG, "%s: %s", __func__, SportType_getTypeName(getSportsControlManager()->getType()));
        getScreenManager()->startScreenByFlag("SportScreen", flag, H_NULL, 0);
        /* 标记进入运动 */
        SportService_getInstance()->getCtrlService()->enterSportView();
    }
    else //未开始运动，跳转运动列表
    {
        if (isRecogPhase)
        {
            HLOG_I(TAG, "%s: in recog phase, abandon sport!");
            SportService_getInstance()->getCtrlService()->sendStateCmd(SPORT_CMD_ABANDON);
            SportService_getInstance()->getCtrlService()->exitSport();
        }
        getScreenManager()->startScreenByFlag("SportListScreen", flag, H_NULL, 0);
    }
}
static void _openSport(void)
{
    HLOG_I(TAG, "%s", __func__);
    _openSportByFlag(FLAG_SCREEN_STANDAR);
}

static void _closeSport(void)
{
    HLOG_I(TAG, "%s", __func__);
    SportStateType state = getSportsControlManager()->getState();
    if (SPORT_STATE_PRESTART == state)
    {
        getSportsControlManager()->exitSport();
    }
    else if (SPORT_STATE_RUN == state || SPORT_STATE_SUSPEND == state)
    {
        getSportsControlManager()->stop();
        getSportsControlManager()->exitSport();
    }
}

static bool _resumeSportByFlag(ScreenFlag flag)
{
    SportStatusInfo sportInfo;
    SportService_getInstance()->getCtrlService()->getStatusInfo(&sportInfo);
    if (sportInfo.in_sport && SportService_getInstance()->getCtrlService()->hasEnteredSportView())
    {
        HLOG_I(TAG, "%s", __func__);
        _openSportByFlag(flag);
        return true;
    }
    else
    {
        return false;
    }
}

static bool _isInSport(void)
{
    SportStateType state = SportService_getInstance()->getCtrlService()->getSportState();

    if (SPORT_STATE_STOP != state)
    {
        return H_True;
    }
    return H_False;
}

static sportUtils sportUtilsOperation = {
    .start = _start,
    .openSport = _openSport,
    .closeSport = _closeSport,
    .checkTypeInitialized = _checkTypeInitialized,
    .startByFlag = _startByFlag,
    .openSportByFlag = _openSportByFlag,
    .resumeSportByFlag = _resumeSportByFlag,
    .isInSport = _isInSport,
    };

sportUtils *getSportUtils(void)
{
    return &sportUtilsOperation;
}
