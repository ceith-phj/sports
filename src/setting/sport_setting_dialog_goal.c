/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: lizheng1@huami.com
 *  Maintainer: lizheng1@huami.com
 *
 * =====================================================================================
 */
#include "ui/Sports/include/setting/sport_setting_dialog_screen.h"

#define BUF_SIZE                100
#define _map(x)                 (x - 2)
#define OFFSET_TE               offsetof(SportTarget, useTrainingEffect)
#define OFFSET_UWS              offsetof(SportTarget, useWorkoutStatistics)
#define OF_TE(x)                offsetof(SportTargetUseTrainingEffect, x) + OFFSET_TE
#define OF_UWS(x)               offsetof(SportSettingUseWorkoutStatistics, x) + OFFSET_UWS
#define OT_SET(base, offset)    (SportSettingTypePicker *)((uint8_t *)base + offset)

#define PATH_DLG_ICON           "images/sports/setting/sport_setting_icon_goal.png"

typedef struct _goal_relation {
    SPORT_TARGET_TYPE_E type;
    uint16_t offset;
    char *goalType;
    char *goalVal;
}goal_relation;

static const goal_relation goalRlt[] = {
    {SPORT_TARGET_TIME,             OF_UWS(time),           TK_TARGET_TIME,         TK_ARG_HOUR_MIN},
    {SPORT_TARGET_DISTANCE,         OF_UWS(distance),       TK_TARGET_DISTANCE,     TK_ARG_KM},
    {SPORT_TARGET_CONSUME,          OF_UWS(calories),       TK_TARGET_CALORIES,     TK_ARG_KCAL},
    {SPORT_TARGET_TRIPS,            OF_UWS(trips),          TK_TARGET_TRIPS,        TK_ARG_TRIPS},
    {SPORT_TARGET_COUNT,            OF_UWS(count),          TK_TARGET_COUNT,        TK_ARG_COUNT},
    {SPORT_TARGET_AEROBICTE,        OF_TE(aerobicTE),       TK_AEROBIC_TE,          "%.1f"},
    {SPORT_TARGET_ANAEROBICTE,      OF_TE(anaerobicTE),     TK_ANAEROBIC_TE,        "%.1f"},
};

/*****************************************************************************************************
* @brief  : DLG 事件
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogEvent(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    getScreenManager()->startScreenByFlag("SportSettingGoalScreen",
                                            FLAG_SCREEN_REMOVE_CURRENT | FLAG_SCREEN_FRONT_OF_PASSWORD,
                                            H_NULL,
                                            0);
}

/*****************************************************************************************************
* @brief  : 获取目标页要展示的文本
*
* @param  : None
* @return : None
******************************************************************************************************/
static inline const goal_relation *getGoalInfo(SPORT_TARGET_TYPE_E type, SportSettingTypePicker *picker, float *data)
{
    *data = picker->data;
    return &goalRlt[_map(SPORT_TARGET_ANAEROBICTE)];
}

/*****************************************************************************************************
* @brief  : 获取距离目标
*
* @param  : None
* @return : None
******************************************************************************************************/
static void getDistanceGoalVal(char *val, float data)
{
    const char *tk;
    system_unit unit = sportManager()->utilsManager()->common()->getSystemUnit();

    if(isSportGroup(SPORT_GROUP_OPEN_WATER_SWIMMING))
        tk = (unit) ? TK_ARG_YARDS_F : TK_ARG_METER;
    else
        tk = (unit) ? TK_ARG_MILE : TK_ARG_KM;

    snprintf(val, BUF_SIZE, _ST(tk), (int32_t)data);
}

/*****************************************************************************************************
* @brief  : 检测目标
*
* @param  : None
* @return : None
******************************************************************************************************/
static bool checkGoal(char *text, const goal_relation *rlt, SportTarget *goal)
{
    char goalVal[BUF_SIZE];
    SportSettingTypePicker *rawSet = OT_SET(goal, rlt->offset);

    if(rawSet->onOff == false)
        return false;

    switch(rlt->type)
    {
    case SPORT_TARGET_TIME:
        snprintf(goalVal, BUF_SIZE, _ST(rlt->goalVal), (int)(rawSet->data)/60, (int)rawSet->data % 60);
        break;

    case SPORT_TARGET_DISTANCE:
        getDistanceGoalVal(goalVal, rawSet->data);
        break;

    case SPORT_TARGET_AEROBICTE:
    case SPORT_TARGET_ANAEROBICTE:
        snprintf(goalVal, BUF_SIZE, rlt->goalVal, rawSet->data);
        break;

    case SPORT_TARGET_CONSUME:
        snprintf(goalVal, BUF_SIZE, _ST(rlt->goalVal), (int32_t)rawSet->data);
        break;

    case SPORT_TARGET_TRIPS:
        snprintf(goalVal, BUF_SIZE, _ST(rlt->goalVal), (int32_t)rawSet->data);
        break;

    case SPORT_TARGET_COUNT:
        snprintf(goalVal, BUF_SIZE, _ST(rlt->goalVal), (int32_t)rawSet->data);
        break;

    default:
        snprintf(goalVal, BUF_SIZE, _ST(rlt->goalVal), rawSet->data);
        break;

    }

    snprintf(text, BUF_SIZE, "%s: %s", _ST(rlt->goalType), goalVal);
    return true;
}

/*****************************************************************************************************
* @brief  : 获取目标页要展示的文本
*
* @param  : None
* @return : None
******************************************************************************************************/
static void getGoalText(char *text, SportTarget *rawSet)
{
    for(int i = 0 ; i < ARRAY_SIZE(goalRlt) ; i++)
    {
        if(checkGoal(text, &goalRlt[i], rawSet))
            return;
    }

    strncpy(text, _ST(TK_NO_GOALS), BUF_SIZE);
}

/*****************************************************************************************************
* @brief  : 创建设置页
*
* @param  : None
* @return : None
******************************************************************************************************/
static void _dialogCreate(AlertDialog *dlg, ScreenContext *context)
{
    char text[BUF_SIZE];

    /* 获取文本信息 */
    sport_dialog_screen *sds = getScreenData(context, sport_dialog_screen);
    getGoalText(text, &sds->rsi.set.assist.target);

    /* 展示文本 */
    dlg->setTitleAndContent(dlg, NULL, text);
    dlg->setPositiveButton(dlg, PATH_DLG_ICON);
    dlg->setClickListener(dlg, _dialogEvent);
    dlg->showDialog(dlg);
}

const dialog_ops dialog_ops_goal = {
    .create = _dialogCreate,
};
