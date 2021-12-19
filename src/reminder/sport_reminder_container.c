/*
 * =====================================================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  fengdandan@huami.com
 *  Time : 2021-4-23
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
#include "packages/apps/sports/include/presenter/sport_reminder_presenter.h"
#include "packages/apps/settings/include/settings_general_utils.h"
#include <core/power/hm_power_manager.h>
#include "packages/services/syssevice/manager/system_manager.h"

//=========================================
//=========================================
#define TAG "srmr"
//pace reminder UI
#define ID_SPORT_REMINDER_LAYER "SportScreen.sport_reminder"

#define ID_SPORT_KILOMETER_CONTROL "sport_reminder.kilometer_control"
//Segmented reminder UI - three
#define ID_SPORT_LAP_THR_GROUP "sport_reminder.lap_thr_group"
//Segmented reminder UI - two
#define ID_SPORT_LAP_TWO_CONTROL "sport_reminder.lap_two_control"
//other reminder control
#define ID_SPORT_OTHER_REMINDER_CONTROL_1 "sport_reminder.other_reminder_control_1"
#define ID_SPORT_OTHER_REMINDER_CONTROL_2 "sport_reminder.other_reminder_control_2"
#define ID_SPORT_OTHER_REMINDER_CONTROL_3 "sport_reminder.other_reminder_control_3"
#define ID_SPORT_OTHER_REMINDER_CONTROL_4 "sport_reminder.other_reminder_control_4"
#define ID_SPORT_OTHER_REMINDER_CONTROL_5 "sport_reminder.other_reminder_control_5"

#define ID_SPORT_OTHER_REMINDER_CONTROL_1_IMAGE "images/sports/remind/sport_remind_icon_distance.png"
//=========================================

#ifndef HM_DISPLAY_IS_ROUND
static char *getString(const char *path)
{
    gr_application_t *app = getAppInstance();
    if(NULL == app)
    {
        return 0;
    }

    char *value = getSBStringData(app, path);

    return value;
}

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

// #define ID_SPORT_REMINDER_NUM_FONT "sport_reminder.sport_reminder_value_font"
#define ID_SPORT_REMINDER_MEDIUM_FONT "sport_reminder.sport_reminder_medium_font"
#define ID_SPORT_REMINDER_NUM_FONT_SIZE "sport_reminder.sport_reminder_value_font_size"
#define ID_SPORT_REMINDER_UNIT_FONT_SIZE "sport_reminder.sport_reminder_unit_font_size"
#define ID_SPORT_REMINDER_CONTROL1_NUM_SIZE "sport_reminder.sport_reminder_control1_value_font_size"
#define ID_SPORT_REMINDER_CONTROL2_TOP_NUM_SIZE "sport_reminder.sport_reminder_control2_top_value_font_size"
#define ID_SPORT_VALUE_UNIT_GAP_W "sport_reminder.sport_value_unit_gap_w"
#define ID_SPORT_OTHER_CONTROL4_ICON_W "sport_reminder.sport_other_control4_icon_width"
#define ID_SPORT_OTHER_CONTROL4_PADDING "sport_reminder.sport_other_control4_padding"
#define ID_SPORT_OTHER_CONTROL4_PADDING16 "sport_reminder.sport_other_control4_padding_16"

// #define SPORT_REMINDER_NUM_FONT getString(ID_SPORT_REMINDER_NUM_FONT)
#define SPORT_REMINDER_MEDIUM_FONT getString(ID_SPORT_REMINDER_MEDIUM_FONT)
#define SPORT_REMINDER_NUM_FONT_SIZE getInt(ID_SPORT_REMINDER_NUM_FONT_SIZE)
#define SPORT_REMINDER_UNIT_FONT_SIZE getInt(ID_SPORT_REMINDER_UNIT_FONT_SIZE)
#define SPORT_REMINDER_CONTROL1_NUM_SIZE getInt(ID_SPORT_REMINDER_CONTROL1_NUM_SIZE)
#define SPORT_REMINDER_CONTROL2_TOP_NUM_SIZE getInt(ID_SPORT_REMINDER_CONTROL2_TOP_NUM_SIZE)
#define SPORT_VALUE_UNIT_GAP_W getInt(ID_SPORT_VALUE_UNIT_GAP_W)
#define SPORT_OTHER_CONTROL4_ICON_WIDTH getInt(ID_SPORT_OTHER_CONTROL4_ICON_W)
#define SPORT_OTHER_CONTROL4_PADDING getInt(ID_SPORT_OTHER_CONTROL4_PADDING)
#define SPORT_OTHER_CONTROL4_PADDING_16 getInt(ID_SPORT_OTHER_CONTROL4_PADDING16)
#endif

#define SPORT_REMINDER_NUM_FONT "fonts/huamifont.ttf"
#define SPORT_REMINDER_REGULAR_FONT "fonts/allfont-Regular.ttf"
#define SPORT_REMINDER_FONT_SIZE (126)
#define SPORT_REMINDER_ZURICH_CONTROL1_NUM_SIZE (100)
#define SPORT_REMINDER_ZURICH_CONTROL2_TOPNUM_SIZE (48)
#define SPORT_TEXT_MAX_NUM (128)
#define SPORT_TOTAL_SHOW_TIME (6)



static SportReminderContainer *gReminderContainer = H_NULL;
static char titleStr[48] = {0}, str2[12] = {0};
//=========================================
// static void _hideReminder(SportReminderContainer *container);
static void _closeReminder();
// static void _clearTimerHandler(SportReminderContainer *container);
//=========================================
typedef struct
{
    uint32_t bgColor;
    const char *imgPath;
    int32_t img_x;
    const char *title;
    const char *unit;
    int32_t unit_x_pos;
    const char *value;
} SportOtherReminder1;
//-----------------------------
typedef struct
{
    uint32_t bgColor;
    const char *imgPath;
    int32_t img_x;
    const char *label;
    const char *unit;
    int32_t unit_x_pos;
    const char *unit2;
    int32_t unit2_x_pos;
    const char *value_top;
    const char *value;
} SportOtherReminder2;
//-----------------------------
typedef struct
{
    uint32_t bgColor;
    const char *imgPath;
    const char *title;
    const char *label;
} SportOtherReminder3;
//-----------------------------
typedef struct
{
    uint32_t bgColor;
    const char *imgPath;
    const char *title;
} SportOtherReminder5;
//-----------------------------
typedef struct
{
    const char *title;
    const char *hour_unit;
    int hour_unit_x;
    const char *hour_value;
    int hour_value_x;
    const char *image;
    int image_x;
    const char *min_unit;
    int min_unit_x;
    const char *min_value;
    int min_value_x;
} SportOtherReminder4;
//-----------------------------
typedef struct
{
    const char *distance;
    const char *km_val;
    const char *kmh_val;
    const char *label;
    const char *speed;
    uint32_t km_x;
    uint32_t kmh_x;
} SportKilometerReminder;
//---------------------------------------------
static void _setOtherReminder5(gr_application_t *app, SportOtherReminder5 *reminder5)
{
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_5, true);
    setSBIntData(app, "sport_reminder.other_reminder_control_5.bg_color", reminder5->bgColor);
    setSBStringData(app, "sport_reminder.other_reminder_control_5.image", reminder5->imgPath);
    setSBStringData(app, "sport_reminder.other_reminder_control_5.title", reminder5->title);
}
//---------------------------------------------
static void _setOtherReminder4(gr_application_t *app, SportOtherReminder4 *other4)
{
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_4, true);
    setSBIntData(app, "sport_reminder.other_reminder_control_4.bg_color", 0x108349);
    setSBStringData(app, "sport_reminder.other_reminder_control_4.hour_unit", other4->hour_unit);
    setSBIntData(app, "sport_reminder.other_reminder_control_4.hour_unit_x", other4->hour_unit_x);
    setSBStringData(app, "sport_reminder.other_reminder_control_4.hour_value", other4->hour_value);
    setSBIntData(app, "sport_reminder.other_reminder_control_4.hour_value_x", other4->hour_value_x);
    setSBStringData(app, "sport_reminder.other_reminder_control_4.image", other4->image);
    setSBIntData(app, "sport_reminder.other_reminder_control_4.image_x", other4->image_x);
    setSBStringData(app, "sport_reminder.other_reminder_control_4.min_unit", other4->min_unit);
    setSBIntData(app, "sport_reminder.other_reminder_control_4.min_unit_x", other4->min_unit_x);
    setSBStringData(app, "sport_reminder.other_reminder_control_4.min_value", other4->min_value);
    setSBIntData(app, "sport_reminder.other_reminder_control_4.min_value_x", other4->min_value_x);
    setSBStringData(app, "sport_reminder.other_reminder_control_4.title", other4->title);
}
//---------------------------------------------
static void _setOtherReminder3(gr_application_t *app, SportOtherReminder3 *reminder3)
{
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_3, true);
    setSBIntData(app, "sport_reminder.other_reminder_control_3.bg_color", reminder3->bgColor);
    setSBStringData(app, "sport_reminder.other_reminder_control_3.image", reminder3->imgPath);
    setSBStringData(app, "sport_reminder.other_reminder_control_3.label", reminder3->label);
    setSBStringData(app, "sport_reminder.other_reminder_control_3.title", reminder3->title);
}
//---------------------------------------------
static void _setOtherReminder2(gr_application_t *app, SportOtherReminder2 *reminder2)
{
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_2, true);
    setSBIntData(app, "sport_reminder.other_reminder_control_2.bg_color", reminder2->bgColor);
    setSBStringData(app, "sport_reminder.other_reminder_control_2.image", reminder2->imgPath);
    setSBIntData(app, "sport_reminder.other_reminder_control_2.image_x", reminder2->img_x);
    setSBStringData(app, "sport_reminder.other_reminder_control_2.label", reminder2->label);
    setSBStringData(app, "sport_reminder.other_reminder_control_2.uint", reminder2->unit);
    setSBIntData(app, "sport_reminder.other_reminder_control_2.unit_x", reminder2->unit_x_pos);
    setSBStringData(app, "sport_reminder.other_reminder_control_2.unit_2", reminder2->unit2);
    setSBIntData(app, "sport_reminder.other_reminder_control_2.unit_2_x", reminder2->unit2_x_pos);
    setSBStringData(app, "sport_reminder.other_reminder_control_2.value_top", reminder2->value_top);
    setSBStringData(app, "sport_reminder.other_reminder_control_2.value", reminder2->value);
}
//---------------------------------------------
static void _setOtherReminder1(gr_application_t *app, SportOtherReminder1 *reminder1)
{
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_1, true);
    setSBIntData(app, "sport_reminder.other_reminder_control_1.bg_color", reminder1->bgColor);
    setSBStringData(app, "sport_reminder.other_reminder_control_1.image", reminder1->imgPath);
    setSBIntData(app, "sport_reminder.other_reminder_control_1.image_x", reminder1->img_x);
    setSBStringData(app, "sport_reminder.other_reminder_control_1.title", reminder1->title);
    setSBStringData(app, "sport_reminder.other_reminder_control_1.unit", reminder1->unit);
    setSBIntData(app, "sport_reminder.other_reminder_control_1.unit_x", reminder1->unit_x_pos);
    setSBStringData(app, "sport_reminder.other_reminder_control_1.value", reminder1->value);
}
//---------------------------------------------
static void _setKilometerReminder(gr_application_t *app, SportKilometerReminder *kilor)
{
    gre_set_control_visible(app, ID_SPORT_KILOMETER_CONTROL, true);
    setSBStringData(app, "sport_reminder.kilometer_control.distance", kilor->distance);
    setSBStringData(app, "sport_reminder.kilometer_control.km_val", kilor->km_val);
    setSBStringData(app, "sport_reminder.kilometer_control.kmh_val", "");
    setSBStringData(app, "sport_reminder.kilometer_control.label", kilor->label);
    setSBStringData(app, "sport_reminder.kilometer_control.speed", kilor->speed);
    setSBIntData(app, "sport_reminder.kilometer_control.km_x", kilor->km_x);
    setSBIntData(app, "sport_reminder.kilometer_control.kmh_x", kilor->kmh_x);
}
//---------------------------------------------
static void _setLapThreeData(gr_application_t *app, SportLapData *lapData)
{
    gre_set_control_visible(app, ID_SPORT_LAP_THR_GROUP, true);
    setSBStringData(app, "sport_reminder.lap_thr_group.top_control.title", lapData->title);
    setSBStringData(app, "sport_reminder.lap_thr_group.top_control.label", lapData->label1);
    setSBStringData(app, "sport_reminder.lap_thr_group.top_control.value", lapData->value1);
    setSBStringData(app, "sport_reminder.lap_thr_group.left_control.lable", lapData->label2);
    setSBStringData(app, "sport_reminder.lap_thr_group.left_control.value", lapData->value2);
    setSBStringData(app, "sport_reminder.lap_thr_group.right_control.label", lapData->label3);
    setSBStringData(app, "sport_reminder.lap_thr_group.right_control.value", lapData->value3);
}
//---------------------------------------------
static void _setLapTwoData(gr_application_t *app, SportLapData *lapData)
{
    gre_set_control_visible(app, ID_SPORT_LAP_TWO_CONTROL, true);
    setSBStringData(app, "sport_reminder.lap_two_control.title", lapData->title);
    setSBStringData(app, "sport_reminder.lap_two_control.value_1", lapData->value1);
    setSBStringData(app, "sport_reminder.lap_two_control.value_1_label", lapData->label1);
    setSBStringData(app, "sport_reminder.lap_two_control.value_2", lapData->value2);
    setSBStringData(app, "sport_reminder.lap_two_control.value_2_label", lapData->label2);
}
//---------------------------------------------
//=========================================
//=========================================
static void _setControlInvisible(gr_application_t *app)
{
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_1, false);
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_2, false);
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_3, false);
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_4, false);
    gre_set_control_visible(app, ID_SPORT_OTHER_REMINDER_CONTROL_5, false);
    gre_set_control_visible(app, ID_SPORT_LAP_THR_GROUP, false);
    gre_set_control_visible(app, ID_SPORT_LAP_TWO_CONTROL, false);
    gre_set_control_visible(app, ID_SPORT_KILOMETER_CONTROL, false);
}
//---------------------------------------------
static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportReminderContainer *reminderContaianer = (SportReminderContainer *)container;
    reminderContaianer->presenter->destory(reminderContaianer->presenter);
    // _clearTimerHandler(reminderContaianer);
    if (reminderContaianer->isShowing)
    {
        _closeReminder();
    }
    gReminderContainer = NULL;
}
//---------------------------------------------
static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    return false;
}
//---------------------------------------------
static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container, const HmKeyEvent *info)
{
    if (info->code == HM_KEY_CODE_UP || info->code == HM_KEY_CODE_DOWN)
    {
        _closeReminder();
        return true;
    }

    return false;
}
//---------------------------------------------
static bool handleGestureEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    _closeReminder();
    return false;
}
//---------------------------------------------
static bool handleEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const char *event)
{
    return false;
}
//---------------------------------------------
// static void _clearTimerHandler(SportReminderContainer *container)
// {
//     if (container != NULL && container->timerHandle != NULL)
//     {
//         gre_timer_clear_timeout(container->app, &container->timerHandle);
//         container->timerHandle = NULL;
//     }
// }
//---------------------------------------------
static void _hideReminder(SportReminderContainer *container)
{
    container->isShowing = false;
    container->isFullScreen = false;
    _setControlInvisible(container->app);
    gre_set_layer_visible(container->app, ID_SPORT_REMINDER_LAYER, false);
    //_clearTimerHandler(container);
}
//---------------------------------------------
// static int _handleTimer(struct gr_timer *timer, void *data, int dropped)
// {
//     _closeReminder();
//     return 0;
// }
//---------------------------------------------
// static int _handleTimer(struct gr_timer *timer, void *data, int dropped)
// {
//     _closeReminder();
//     return 0;
// }
//---------------------------------------------
static int _getKilormeterUnintXPos(char *value)
{
    gr_string_size_t strInfo = {0};
#ifdef HM_DISPLAY_IS_ROUND
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        SPORT_REMINDER_FONT_SIZE, getDisplayMetrics()->width, &strInfo, false, 0);
    int x_pos = getDisplayMetrics()->width / 2 + strInfo.width / 2 + 10;
#else
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        SPORT_REMINDER_NUM_FONT_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);
    int x_pos = STORYBOARD_DISPLAY_WIDTH / 2 + strInfo.width / 2 + SPORT_VALUE_UNIT_GAP_W;

#endif
    return x_pos;
}
//---------------------------------------------
static int _getOtherControl1ImageXPos(char *value)
{

    gr_string_size_t strInfo = {0};
#ifdef HM_DISPLAY_IS_ROUND
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        120, getDisplayMetrics()->width, &strInfo, false, 0);
    int x_pos = getDisplayMetrics()->width / 2 - strInfo.width / 2 - 10 - 64;
#else
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        SPORT_REMINDER_NUM_FONT_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);

    int icon_w = 0;
    int icon_h = 0;
    gre_get_image_size(gReminderContainer->app, ID_SPORT_OTHER_REMINDER_CONTROL_1_IMAGE, &icon_w, &icon_h);
    int x_pos = STORYBOARD_DISPLAY_WIDTH / 2 - strInfo.width / 2 - SPORT_VALUE_UNIT_GAP_W - icon_w;
#endif
    return x_pos;
}
//---------------------------------------------
static void _getOtherControl4ImageXPos(SportOtherReminder4 *other4)
{
 #ifdef HM_DISPLAY_IS_ROUND
    int icon_width = 64, padding = 10, padding_16 = 16;
 #else
    int icon_width  = SPORT_OTHER_CONTROL4_ICON_WIDTH,
        padding     = SPORT_OTHER_CONTROL4_PADDING,
        padding_16  = SPORT_OTHER_CONTROL4_PADDING_16;
 #endif
    int total_width = 0, hWidth = 0, hutWidth = 0, mWidth = 0, mutWidth = 0;
    gr_string_size_t strInfo = {0};

#ifdef HM_DISPLAY_IS_ROUND
    if (strcmp(other4->hour_value, "") != 0)
    {
        gre_get_string_size(gReminderContainer->app, other4->hour_value, SPORT_REMINDER_NUM_FONT,
                            120, getDisplayMetrics()->width, &strInfo, false, 0);
        hWidth = strInfo.width;

        gre_get_string_size(gReminderContainer->app, other4->hour_unit, SPORT_REMINDER_REGULAR_FONT,
                            28, getDisplayMetrics()->width, &strInfo, false, 0);
        hutWidth = strInfo.width;
    }

    gre_get_string_size(gReminderContainer->app, other4->min_value, SPORT_REMINDER_NUM_FONT,
                        120, getDisplayMetrics()->width, &strInfo, false, 0);
    mWidth = strInfo.width;

    gre_get_string_size(gReminderContainer->app, other4->min_unit, SPORT_REMINDER_REGULAR_FONT,
                        28, getDisplayMetrics()->width, &strInfo, false, 0);
    mutWidth = strInfo.width;
#else
    if (strcmp(other4->hour_value, "") != 0)
    {
        gre_get_string_size(gReminderContainer->app, other4->hour_value, SPORT_REMINDER_NUM_FONT,
                            SPORT_REMINDER_NUM_FONT_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);
        hWidth = strInfo.width;

        gre_get_string_size(gReminderContainer->app, other4->hour_unit, SPORT_REMINDER_MEDIUM_FONT,
                            SPORT_REMINDER_UNIT_FONT_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);
        hutWidth = strInfo.width;
    }

    gre_get_string_size(gReminderContainer->app, other4->min_value, SPORT_REMINDER_NUM_FONT,
                        SPORT_REMINDER_NUM_FONT_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);
    mWidth = strInfo.width;

    gre_get_string_size(gReminderContainer->app, other4->min_unit, SPORT_REMINDER_MEDIUM_FONT,
                        SPORT_REMINDER_UNIT_FONT_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);
    mutWidth = strInfo.width;
#endif
    if (strcmp(other4->hour_value, "") != 0)
    {
        total_width = hWidth + hutWidth + mWidth + mutWidth + icon_width + padding * 2 + padding_16 * 2;
    }
    else
    {
        total_width = mWidth + mutWidth + icon_width + padding + padding_16;
    }
    other4->image_x = getDisplayMetrics()->width / 2 - total_width / 2;
    if (strcmp(other4->hour_value, "") != 0)
    {
        other4->hour_value_x = other4->image_x + icon_width + padding_16;
        other4->hour_unit_x = other4->hour_value_x + hWidth + padding;
        other4->min_value_x = other4->hour_unit_x + hutWidth + padding_16;
        other4->min_unit_x = other4->min_value_x + mWidth + padding;
    }
    else
    {
        other4->min_value_x = other4->image_x + icon_width + padding_16;
        other4->min_unit_x = other4->min_value_x + mWidth + padding;
    }
}
//---------------------------------------------
static int _getOtherControl1UnitXPos(char *value)
{

    gr_string_size_t strInfo = {0};
#ifdef HM_DISPLAY_IS_ROUND
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        120, getDisplayMetrics()->width, &strInfo, false, 0);
    int x_pos = getDisplayMetrics()->width / 2 + strInfo.width / 2 + 10;
#else
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        SPORT_REMINDER_CONTROL1_NUM_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);
    int x_pos = STORYBOARD_DISPLAY_WIDTH / 2 + strInfo.width / 2 + SPORT_VALUE_UNIT_GAP_W;
#endif
    return x_pos;
}
//---------------------------------------------
static int _getOtherControl2TopUnitXPos(char *value)
{

    gr_string_size_t strInfo = {0};
#ifdef HM_DISPLAY_IS_ROUND
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        54, getDisplayMetrics()->width, &strInfo, false, 0);
    int x_pos = getDisplayMetrics()->width / 2 + strInfo.width / 2 + 6;
#else
    gre_get_string_size(gReminderContainer->app, value, SPORT_REMINDER_NUM_FONT,
                        SPORT_REMINDER_CONTROL2_TOP_NUM_SIZE, STORYBOARD_DISPLAY_WIDTH, &strInfo, false, 0);
    int x_pos = STORYBOARD_DISPLAY_WIDTH / 2 + strInfo.width / 2 + SPORT_VALUE_UNIT_GAP_W;
#endif
    return x_pos;
}
//---------------------------------------------
static bool _showReminderUI(SportReminderContainer *container)
{
    if (container->timerHandle)
    {
        return true;
    }
    gre_set_layer_visible(container->app, ID_SPORT_REMINDER_LAYER, true);
    getPowerManager()->userActivity(); /* 点亮屏幕 */
    getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_REMIND2);
    // container->timerHandle = gre_timer_create_timeout(container->app, 6000, 0, NULL, _handleTimer);
    // if (container->timerHandle == NULL)
    // {
    //     return true;
    // }
    container->isShowing = true;
    return false;
}
//---------------------------------------------
//安全心率
static void _safeHrReminder(SportReminderHighOrLow uiType, char *value)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    HLOG_I(TAG, "safe hr %s", value);
    SportOtherReminder1 m1;
    m1.bgColor = 0x762900;
    m1.imgPath = "images/sports/remind/sport_remind_icon_hr.png";
    m1.img_x = _getOtherControl1ImageXPos(value);
    m1.unit = System_getLocaleText("id_sport_chart_hr_unit");
    m1.unit_x_pos = _getOtherControl1UnitXPos(value);
    m1.value = value;
    switch (uiType)
    {
    case UI_REMINDER_NORMAL:
        m1.title = System_getLocaleText("id_spr_safe_hr");
        break;
    case UI_REMINDER_LOW:
        m1.title = System_getLocaleText("id_spr_hr_low");
        break;
    case UI_REMINDER_HIGH:
        m1.title = System_getLocaleText("id_spr_hr_high");
        break;
    default:
        break;
    }
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
//划频
static void _strokeReminder(SportReminderHighOrLow uiType, char *value)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    HLOG_I(TAG, "stroke %s", value);
    SportOtherReminder1 m1;
    m1.bgColor = 0x762900;
    m1.imgPath = "images/sports/remind/sport_remind_icon_distance.png";
    m1.img_x = _getOtherControl1ImageXPos(value);
    m1.unit = System_getLocaleText("id_spr_stroke_unit");
    m1.unit_x_pos = _getOtherControl1UnitXPos(value);
    m1.value = value;
    switch (uiType)
    {
    case UI_REMINDER_HIGH:
        m1.title = System_getLocaleText("id_spr_stroke_high");
        break;
    case UI_REMINDER_LOW:
        m1.title = System_getLocaleText("id_spr_stroke_low");
        break;
    default:
        break;
    }
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _highSpeedReminder(SportSpeedUnit speedUnit, float speed)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    char val[40];
    snprintf(val, 40, "%.02f", speed);
    HLOG_I(TAG, "highspeed %s", val);
    SportOtherReminder1 m1;
    m1.bgColor = 0x762900;
    m1.imgPath = "images/sports/remind/sport_remind_icon_speed.png";
    m1.img_x = _getOtherControl1ImageXPos(val);
    m1.unit_x_pos = _getOtherControl1UnitXPos(val);
    m1.value = val;
    switch (speedUnit)
    {
    case UI_REMINDER_SPEED_MILE:
        m1.unit = System_getLocaleText("id_sport_chart_speed_imperial_unit");
        break;
    default:
        m1.unit = System_getLocaleText("id_sport_chart_speed_matric_unit");
        break;
    }
    m1.title = System_getLocaleText("id_spr_speed_high_msg");
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _speedReminder(SportReminderHighOrLow type, SportSpeedUnit unit, float speed)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    char val[40];
    snprintf(val, 40, "%.02f", speed);
    HLOG_I(TAG, "speed type %d %d %s", type, unit, val);
    SportOtherReminder1 m1;
    m1.bgColor = 0x762900;
    m1.imgPath = "images/sports/remind/sport_remind_icon_speed.png";
    m1.img_x = _getOtherControl1ImageXPos(val);
    m1.unit_x_pos = _getOtherControl1UnitXPos(val);
    m1.value = val;
    switch (type)
    {
    case UI_REMINDER_LOW:
        m1.title = System_getLocaleText("id_spr_speed_low");
        break;
    case UI_REMINDER_HIGH:
        m1.title = System_getLocaleText("id_spr_speed_high");
        break;
    default:
        break;
    }
    switch (unit)
    {
    case UI_REMINDER_SPEED_MILE:
        m1.unit = System_getLocaleText("id_sport_chart_speed_imperial_unit");
        break;
    default:
        m1.unit = System_getLocaleText("id_sport_chart_speed_matric_unit");
        break;
    }
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
//步频
static void _strideReminder(SportReminderHighOrLow type, char *stride)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    HLOG_I(TAG, "stride %d %s", type, stride);
    SportOtherReminder1 m1;
    m1.bgColor = 0x762900;
    m1.imgPath = "images/sports/remind/sport_remind_icon_step_freq.png";
    m1.img_x = _getOtherControl1ImageXPos(stride);
    m1.unit_x_pos = _getOtherControl1UnitXPos(stride);
    m1.value = stride;
    m1.unit = System_getLocaleText("id_spr_stride_unit");
    switch (type)
    {
    case UI_REMINDER_LOW:
        m1.title = System_getLocaleText("id_spr_stride_low");
        break;
    case UI_REMINDER_HIGH:
        m1.title = System_getLocaleText("id_spr_stride_high");
        break;
    default:
        break;
    }
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _paceReminder(SportReminderHighOrLow type, SportSpeedUnit unit, char *pace)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    HLOG_I(TAG, "pace %d %s", type, pace);
    SportOtherReminder1 m1;
    m1.bgColor = 0x762900;
    m1.imgPath = "images/sports/remind/sport_remind_icon_pace.png";
    m1.img_x = _getOtherControl1ImageXPos(pace);
    m1.unit_x_pos = _getOtherControl1UnitXPos(pace);
    m1.value = pace;
    switch (unit)
    {
    case UI_REMINDER_SPEED_M:
        m1.unit = System_getLocaleText("id_spr_swim_hundred_meter_unit");
        break;
    case UI_REMINDER_SPEED_KM:
        m1.unit = System_getLocaleText("id_sport_chart_pace_matric_unit");
        break;
    case UI_REMINDER_SPEED_YD:
        m1.unit = System_getLocaleText("id_spr_swim_hundred_yard_unit");
        break;
    case UI_REMINDER_SPEED_MILE:
        m1.unit = System_getLocaleText("id_sport_chart_pace_imperial_unit");
        break;
    default:
        break;
    }
    switch (type)
    {
    case UI_REMINDER_LOW:
        m1.title = System_getLocaleText("id_spr_pace_low");
        break;
    case UI_REMINDER_HIGH:
        m1.title = System_getLocaleText("id_spr_pace_high");
        break;
    default:
        break;
    }
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _caloriesReminder(char *goal, char *total)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    HLOG_I(TAG, "cal %s %s", goal, total);
    SportOtherReminder2 m2;
    m2.bgColor = 0x108349;
    // char imgpath[128];
    // snprintf(imgpath, 128, "%s", "images/sports/remind/sport_remind_icon_calorie.png");
    m2.imgPath = "images/sports/remind/sport_remind_icon_calorie.png";
    m2.img_x = _getOtherControl1ImageXPos(total);
    m2.value_top = goal;
    m2.value = total;
    m2.unit = System_getLocaleText("id_spr_kcal_unit");
    m2.unit2 = System_getLocaleText("id_spr_kcal_unit");
    m2.unit_x_pos = _getOtherControl2TopUnitXPos(goal);//48sp
    m2.unit2_x_pos = _getOtherControl1UnitXPos(total);
    m2.label = System_getLocaleText("id_spr_calories_total");
    _setOtherReminder2(gReminderContainer->app, &m2);
}
//---------------------------------------------
static void _waterReminder()
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    SportOtherReminder3 m3;
    m3.bgColor = 0x762900;
    m3.imgPath = "images/sports/remind/sport_remind_icon_drink.png";
    m3.title = System_getLocaleText("id_spr_water");
    m3.label = System_getLocaleText("id_spr_water_label");
    _setOtherReminder3(gReminderContainer->app, &m3);
}
//---------------------------------------------
static void _energyReminder()
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    SportOtherReminder3 m3;
    m3.bgColor = 0x762900;
    m3.imgPath = "images/sports/remind/sport_remind_icon_supply.png";
    m3.title = System_getLocaleText("id_spr_energy");
    m3.label = System_getLocaleText("id_spr_energy_label");
    _setOtherReminder3(gReminderContainer->app, &m3);
}
//---------------------------------------------
static void _returnReminder()
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    SportOtherReminder3 m3;
    m3.bgColor = 0x108349;
    m3.imgPath = "images/sports/remind/sport_remind_icon_retrace.png";
    m3.title = System_getLocaleText("id_spr_return");
    m3.label = System_getLocaleText("id_spr_return_label");
    _setOtherReminder3(gReminderContainer->app, &m3);
}

//---------------------------------------------
static void _gpsFixedReminder()
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    SportOtherReminder5 m5;
    m5.bgColor = 0x108349;
    m5.imgPath = "images/sports/remind/gnss_success.png";
    m5.title = System_getLocaleText("id_sport_gps_location_success");
    _setOtherReminder5(gReminderContainer->app, &m5);
}

//---------------------------------------------
static void _tripsReminder(char *goal, char *total)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    HLOG_I(TAG, "trip %s %s ", goal, total);
    SportOtherReminder2 m2;
    m2.bgColor = 0x108349;
    m2.imgPath = "images/sports/remind/sport_remind_icon_trips.png";
    m2.img_x = _getOtherControl1ImageXPos(total);
    m2.value_top = goal;
    m2.value = total;
    m2.unit = System_getLocaleText("id_spr_trip_unit");
    m2.unit2 = System_getLocaleText("id_spr_trip_unit");
    m2.unit_x_pos = _getOtherControl2TopUnitXPos(goal);//54sp
    m2.unit2_x_pos = _getOtherControl1UnitXPos(total);
    // m2.label = System_getLocaleText("id_spr_calories_total");
    m2.label = "";
    _setOtherReminder2(gReminderContainer->app, &m2);
}
static void _timeReminder(char *goal, char *total)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    SportOtherReminder2 m2;
    m2.bgColor = 0x108349;
    m2.imgPath = "images/sports/remind/sport_remind_icon_time.png";
    m2.img_x = _getOtherControl1ImageXPos(total);
    m2.value_top = goal;
    m2.value = total;
    m2.unit = "";
    m2.unit2 = "";
    m2.unit_x_pos = 0;
    m2.unit2_x_pos = 0;
    m2.label = System_getLocaleText("id_spr_total");
    _setOtherReminder2(gReminderContainer->app, &m2);
}
//---------------------------------------------
static void _kilometerReminder(SportSpeedUnit unitType, SportKilometerType uiType, char *auxiStr, char *distanceStr, char *goalValue)
{
    char labelStr[128];
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    HLOG_I(TAG, "kilometer %d %d %s %s %s", unitType, uiType, auxiStr, distanceStr, goalValue);
    gReminderContainer->isFullScreen = true;
    SportKilometerReminder kilor;
    kilor.km_x = _getKilormeterUnintXPos(distanceStr);
    kilor.kmh_x = _getKilormeterUnintXPos(auxiStr);
    kilor.km_val = System_getLocaleText("id_spr_km_unit");                    //km
    kilor.kmh_val = System_getLocaleText("id_sport_chart_speed_matric_unit"); //km/h
    if (unitType == UI_REMINDER_SPEED_MILE)
    {
        kilor.km_val = System_getLocaleText("id_spr_mile_unit");
        kilor.kmh_val = System_getLocaleText("id_sport_chart_speed_imperial_unit");
    }
    if (uiType == UI_REMINDER_SPACE)
    {
        kilor.kmh_val = "";
        snprintf(labelStr, 128, System_getLocaleText("id_spr_up_pace"), goalValue, kilor.km_val);
    }
    else
    {
        snprintf(labelStr, 128, System_getLocaleText("id_spr_up_speed"), goalValue, kilor.km_val);
    }
    kilor.label = labelStr;
    kilor.distance = distanceStr;
    kilor.speed = auxiStr;
    _setKilometerReminder(gReminderContainer->app, &kilor);
}
//---------------------------------------------
static void _lapReminder(SportLapReminderType reminderType, SportLapData *lapData)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    gReminderContainer->isFullScreen = true;
    snprintf(titleStr, 48, System_getLocaleText("id_spr_lap_title"), lapData->index);
    lapData->title = titleStr;
    switch (reminderType)
    {
    case REMINDER_LAP_TYPE_PACE:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_rt_distance_total");
        lapData->label3 = System_getLocaleText("id_rt_pace");
        _setLapThreeData(gReminderContainer->app, lapData);
        break;
    case REMINDER_LAP_TYPE_SPEED:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_rt_distance_total");
        lapData->label3 = System_getLocaleText("id_rt_speed");
        _setLapThreeData(gReminderContainer->app, lapData);
        break;
    case REMINDER_LAP_TYPE_POOL_SWIM:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_sportrecord_Number_of_trips");
        lapData->label3 = System_getLocaleText("id_rt_swim_avg_stroke_speed");
        _setLapThreeData(gReminderContainer->app, lapData);
        break;
    case REMINDER_LAP_TYPE_OPEN_WATE_SWIM:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_rt_distance_total");
        lapData->label3 = System_getLocaleText("id_rt_swim_avg_stroke_speed");
        _setLapThreeData(gReminderContainer->app, lapData);
        break;
    case REMINDER_LAP_TYPE_TIME:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_rt_duration_total");
        _setLapTwoData(gReminderContainer->app, lapData);
        break;
    case REMINDER_LAP_TYPE_STAIR_CLIMB:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_rt_climb_up_floors");
        _setLapTwoData(gReminderContainer->app, lapData);
        break;
    case REMINDER_LAP_TYPE_HR:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_rt_hr_avg");
        _setLapTwoData(gReminderContainer->app, lapData);
        break;
    case REMINDER_LAP_TYPE_PRESS:
        lapData->label1 = System_getLocaleText("id_spr_lap_time");
        lapData->label2 = System_getLocaleText("id_rt_pressure_avg");
        _setLapTwoData(gReminderContainer->app, lapData);
        break;
    default:
        break;
    }
}
//---------------------------------------------
static void _targetTripReminder(int value)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    snprintf(titleStr, 48, "%d", value);
    HLOG_I(TAG, "target trip %s", titleStr);
    SportOtherReminder1 m1;
    m1.bgColor = 0x108349;
    m1.imgPath = "images/sports/remind/sport_remind_icon_trips.png";
    m1.img_x = _getOtherControl1ImageXPos(titleStr);
    m1.unit = System_getLocaleText("id_spr_trip_unit");
    m1.unit_x_pos = _getOtherControl1UnitXPos(titleStr);
    m1.value = titleStr;
    m1.title = System_getLocaleText("id_spr_target_trip");
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _targetCountReminder(int value)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    snprintf(titleStr, 48, "%d", value);
    HLOG_I(TAG, "target count %s", titleStr);
    SportOtherReminder1 m1;
    m1.bgColor = 0x108349;
    m1.imgPath = "images/sports/remind/sport_remind_icon_count.png";
    m1.img_x = _getOtherControl1ImageXPos(titleStr);
    m1.unit = System_getLocaleText("id_spr_count_unit");
    m1.unit_x_pos = _getOtherControl1UnitXPos(titleStr);
    m1.value = titleStr;
    m1.title = System_getLocaleText("id_spr_target_count");
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _targetCaloriesReminder(int value)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    snprintf(titleStr, 48, "%d", value);
    HLOG_I(TAG, "target cals %s", titleStr);
    SportOtherReminder1 m1;
    m1.bgColor = 0x108349;
    m1.imgPath = "images/sports/remind/sport_remind_icon_calorie.png";
    m1.img_x = _getOtherControl1ImageXPos(titleStr);
    m1.unit = System_getLocaleText("id_spr_kcal_unit");
    m1.unit_x_pos = _getOtherControl1UnitXPos(titleStr);
    m1.value = titleStr;
    m1.title = System_getLocaleText("id_spr_target_cals");
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _targetDistanceReminder(SportSpeedUnit unit, int value)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    snprintf(titleStr, 48, "%d", value);
    HLOG_I(TAG, "target distance %s", titleStr);
    SportOtherReminder1 m1;
    m1.bgColor = 0x108349;
    m1.imgPath = "images/sports/remind/sport_remind_icon_distance.png";
    m1.img_x = _getOtherControl1ImageXPos(titleStr);
    switch (unit)
    {
    case UI_REMINDER_SPEED_M:
        m1.unit = System_getLocaleText("id_sport_chart_altitude_matric_unit");
        break;
    case UI_REMINDER_SPEED_KM:
        m1.unit = System_getLocaleText("id_spr_km_unit");
        break;
    case UI_REMINDER_SPEED_YD:
        m1.unit = System_getLocaleText("id_sport_unit_yard");
        break;
    case UI_REMINDER_SPEED_MILE:
        m1.unit = System_getLocaleText("id_spr_mile_unit");
        break;
    default:
        break;
    }
    m1.unit_x_pos = _getOtherControl1UnitXPos(titleStr);
    m1.value = titleStr;
    m1.title = System_getLocaleText("id_spr_target_distance");
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _targetTEReminder(uint8_t isAerobic, float value)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    snprintf(titleStr, 48, "%.01f", value);
    HLOG_I(TAG, "target te %s", titleStr);
    SportOtherReminder1 m1;
    m1.bgColor = 0x108349;
    m1.imgPath = "images/sports/remind/sport_remind_icon_te.png";
    m1.img_x = _getOtherControl1ImageXPos(titleStr);
    if (isAerobic)
    {
        /*有氧*/
        m1.unit = System_getLocaleText("id_spr_aerobic");
    }
    else
    {
        m1.unit = System_getLocaleText("id_spr_anaerobic");
    }
    m1.unit_x_pos = _getOtherControl1UnitXPos(titleStr);
    m1.value = titleStr;
    m1.title = System_getLocaleText("id_spr_target_te");
    _setOtherReminder1(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _targetTimeReminder(int hour, int mins)
{
    if (_showReminderUI(gReminderContainer))
    {
        return;
    }
    if (hour)
    {
        snprintf(titleStr, 48, "%d", hour);
    }
    else
    {
        snprintf(titleStr, 48, "%s", "");
    }
    snprintf(str2, 12, "%d", mins);
    HLOG_I(TAG, "target time %s:%s", titleStr, str2);
    SportOtherReminder4 m1 = {0};
    m1.title = System_getLocaleText("id_spr_target_time");
    m1.image = "images/sports/remind/sport_remind_icon_time.png";
    if (strcmp(titleStr, "") == 0)
    {
        m1.hour_unit = "";
    }
    else
    {
        m1.hour_unit = "h";
    }
    m1.hour_value = titleStr;
    m1.min_unit = "min";
    m1.min_value = str2;
    _getOtherControl4ImageXPos(&m1);
    _setOtherReminder4(gReminderContainer->app, &m1);
}
//---------------------------------------------
static void _closeReminder()
{
    _hideReminder(gReminderContainer);
}
//---------------------------------------------
SportReminderContainer *createReminderContainer(ScreenContext *screenContext)
{
    SportReminderContainer *reminderContainer = H_NULL;
    if (!reminderContainer)
    {
        createContainer(reminderContainer, SportReminderContainer, reminderContainer->container);
        reminderContainer->presenter = getSportReminderPresenter(screenContext);
        reminderContainer->presenter->safeHrReminder = _safeHrReminder;
        reminderContainer->presenter->strokeReminder = _strokeReminder;
        reminderContainer->presenter->highSpeedReminder = _highSpeedReminder;
        reminderContainer->presenter->strideReminder = _strideReminder;
        reminderContainer->presenter->paceReminder = _paceReminder;
        reminderContainer->presenter->speedReminder = _speedReminder;
        reminderContainer->presenter->caloriesReminder = _caloriesReminder;
        reminderContainer->presenter->waterReminder = _waterReminder;
        reminderContainer->presenter->energyReminder = _energyReminder;
        reminderContainer->presenter->returnReminder = _returnReminder;
        reminderContainer->presenter->tripsReminder = _tripsReminder;
        reminderContainer->presenter->timeReminder = _timeReminder;
        reminderContainer->presenter->kilometerReminder = _kilometerReminder;
        reminderContainer->presenter->lapReminder = _lapReminder;
        reminderContainer->presenter->closeReminder = _closeReminder;
        reminderContainer->presenter->targetTripReminder = _targetTripReminder;
        reminderContainer->presenter->targetCountReminder = _targetCountReminder;
        reminderContainer->presenter->targetCaloriesReminder = _targetCaloriesReminder;
        reminderContainer->presenter->targetDistanceReminder = _targetDistanceReminder;
        reminderContainer->presenter->targetTEReminder = _targetTEReminder;
        reminderContainer->presenter->targetTimeReminder = _targetTimeReminder;
        reminderContainer->presenter->gpxFixedReminder = _gpsFixedReminder;
        gReminderContainer = reminderContainer;
    }
    return reminderContainer;
}
