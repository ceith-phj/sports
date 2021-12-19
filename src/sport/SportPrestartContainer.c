#include <BaseSportView.h>
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sport/SportPrestartContainer.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <sport_type_utils.h>
#include <packages/apps/sports/include/presenter/SportPresenter.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include "core/display_metrics.h"
#include "hm_time.h"
#include <sport_type_utils.h>
#include <core/power/hm_power_manager.h>
#include "sport_engine_utils.h"

#define TAG "SportPrestartContainer"
#define ID_ANIMATE_GPS_IMAGE "sportPreStartLayer.SensorValueGroup.GpsSensorValueControl.image"
#define ID_ANIMATE_GPS_VIEW "sportPreStartLayer.SensorValueGroup.GpsSensorValueControl"
#define PATH_HR_VALUE "sportPreStartLayer.SensorValueGroup.HrSensorValueControl.value"
#define PATH_SPORT_TYPE_VALUE "sportPreStartLayer.SensorHintControl.value"
#define ID_ANIMATE_HR_IMAGE "sportPreStartLayer.SensorValueGroup.HrSensorValueControl.image"
#define PATH_BTN_START_BG_VALUE "sportPreStartLayer.BtnStartControl.btnStartBg"
#define PATH_SPORT_TYPE_ICON_VALUE "sportPreStartLayer.SensorValueGroup.SportTypeIconControl.sportTypeIcon"
#define PATH_TARGET_TEXT_VALUE "sportPreStartLayer.BtnSettingControl.text"
#define ID_SPORT_SETTING_MAIN "sportSettingList.mainsetting"
#define PATH_BTN_START_COLOR_VALUE "sportPreStartLayer.SensorValueGroup.BtnStartControl.btnColor"

#define SETTING_OFFSETY 112
#define GET_SETTING_OFFSETY getSBIntData(getAppInstance(),"sportPreStartLayer.SettingBgControl.settingoffsety")
#define SLIDEANIMATION_TIME 500
#define SLIDEANIMATION_FPS 50
#define BTN_START_RED_COLOR 0xAD3C23
#define BTN_START_GREEN_COLOR 0x108349
#define GPSTIPSTIMEOUT 6 * 60 /* 6分钟后，弹出GPS定位失败弹窗 */
#define GPSTIMEOUT 30 * 60    /* 30分钟未开始 退出运动 */
#define AGPSTIPSTIMEOUT 5    /* 5S AGPS过期自动消失 */
CONTAINER_FUNCTION_DEFINE();

static void _initGPSView(SportPrestartContainer *prestartContainer);
static void _initHRView(SportPrestartContainer *prestartContainer);
static void _initViews(SportPrestartContainer *prestartContainer);
static void _initData(SportPrestartContainer *prestartContainer);
static void _updateGpsStatus(SportPrestartContainer *prestartContainer,
                             H_Int8 status);
static void _updateHrVaule(SportPrestartContainer *prestartContainer,
                           H_Uint8 value);
static void _updateBtnStart(SportPrestartContainer *prestartContainer,
                            H_Int8 gpsValue);
static void _updatePosition(SportPrestartContainer *prestartContainer,
                            H_Int16 offsetY);
static void _initSettingView(SportPrestartContainer *prestartContainer);
static void _initSportTypeView(SportPrestartContainer *prestartContainer);
static void _updateBtnStartPosition(SportPrestartContainer *prestartContainer,
                                    H_Int16 offsetY);
static void _updateBtnStartShape(SportPrestartContainer *prestartContainer,
                                 H_Int16 offsetY);
static void _initAGPSDialogView(SportPrestartContainer *container);
static void _initGoalView(SportPrestartContainer *prestartContainer);
static bool _isAGNSSOverdue(void);

static void initContainer(SportPrestartContainer *prestartContainer,
                          ScreenContext *screenContext)
{
    prestartContainer->screenContext = screenContext;
    prestartContainer->app = screenContext->_grAppContext;

    if (getSportsScreenPresenter()->requestRestoreSport() == true)
    {
        prestartContainer->setVisiable(prestartContainer, false);
        return;
    }

    prestartContainer->setVisiable(prestartContainer, true);
    _initData(prestartContainer);
    _initViews(prestartContainer);
}

static void _initViews(SportPrestartContainer *prestartContainer)
{
    _initGPSView(prestartContainer);
    _initHRView(prestartContainer);
    _initSettingView(prestartContainer);
    _initSportTypeView(prestartContainer);
    _initAGPSDialogView(prestartContainer);
    _initGoalView(prestartContainer);
}

static void _initAGPSDialogView(SportPrestartContainer *container)
{
    /* AGPS 过期 弹起提示 */
    if (container->hasGPS == true && _isAGNSSOverdue())
    {
        container->status.AGPSTipsIsShow = SPORT_AGPS_TIPS_SHOWING;
        getSportsScreenPresenter()
        ->sendUIEvent(SPORTUI_EVENT_SHOW_NO_AGNSS);
    }
}

static void _initSettingView(SportPrestartContainer *prestartContainer)
{
    sport_setting_container *sportSettingContainer;
    extern const sport_setting_ops setting_ops_main;
    sportSettingContainer = H_MALLOC(sizeof(sport_setting_container));
    sportSettingContainer->sc = prestartContainer->screenContext;
    sportManager()->utilsManager()->common()->getSet(&sportSettingContainer->rsi);
    prestartContainer->sportSettingContainer = sportSettingContainer;
    sportSettingContainer->ops = &setting_ops_main;
    sportSettingContainer->ops->create(sportSettingContainer);
    prestartContainer->settingLayerHeight = getSBIntData(prestartContainer->app,
                                            "sportPreStartLayer.SettingBgControl.grd_height");
}

static uint32_t _getTimeStamp(void)
{
    HmTime time;
    hmTime_getTime(&time);
    return time.sec;
}

static void _initData(SportPrestartContainer *prestartContainer)
{
    prestartContainer->hasGPS = getSportsScreenPresenter()->sportTypeSupportGPS();
    prestartContainer->status.offsetY = 0;
    prestartContainer->btnStartOriginalRadius = getSBIntData(prestartContainer->app,
            "sportPreStartLayer.SensorValueGroup.BtnStartControl.sportPrestartBtnGoRadius");
    prestartContainer->btnStartOriginalHeight = getSBIntData(prestartContainer->app,
            "sportPreStartLayer.SensorValueGroup.BtnStartControl.grd_height");
    prestartContainer->btnStartOriginalY = getSBIntData(prestartContainer->app,
                                           "sportPreStartLayer.SensorValueGroup.grd_y");
    prestartContainer->btnStartOriginalWidth = getSBIntData(prestartContainer->app,
            "sportPreStartLayer.SensorValueGroup.BtnStartControl.btnWidth");
#ifndef HM_FEATURE_BLED_UI
    prestartContainer->btnStartFinalHeight = prestartContainer->btnStartOriginalY +
            prestartContainer->btnStartOriginalHeight - getDimension(SETTING_OFFSETY);
#else
    prestartContainer->btnStartFinalHeight = prestartContainer->btnStartOriginalY +
            prestartContainer->btnStartOriginalHeight - GET_SETTING_OFFSETY;
#endif

#ifdef HM_DISPLAY_IS_ROUND
    prestartContainer->btnStartMoveY = getDimension(480) - getDimension(
                                           SETTING_OFFSETY);
#else

#ifndef HM_FEATURE_BLED_UI
    prestartContainer->btnStartMoveY = getDimension(450) - getDimension(
                                           SETTING_OFFSETY);
#else
    prestartContainer->btnStartMoveY = getDisplayMetrics()->height - GET_SETTING_OFFSETY;
#endif

#endif
    prestartContainer->sportType = getSportsScreenPresenter()->getSportType();
    prestartContainer->btnStartShapeAnimLastRadius = -1;
    prestartContainer->status.ExitSportIsShow = false;
    prestartContainer->status.preStartTime = _getTimeStamp();
    prestartContainer->status.gpsStartSearchTime = _getTimeStamp();
    prestartContainer->status.gpsStatus = 0;
    setSBIntData(prestartContainer->app,
                 "SportScreen.sportPreStartLayer.userYOffset", 0);
}

static void _initGPSView(SportPrestartContainer *prestartContainer)
{
    if (prestartContainer->hasGPS == false)
    {
        prestartContainer->gpsIconAnim = NULL;
        gre_set_control_visible(prestartContainer->app,
                                "sportPreStartLayer.SensorValueGroup.GpsSensorValueControl", false);
        setSBIntData(getAppInstance(), PATH_BTN_START_COLOR_VALUE,
                     BTN_START_GREEN_COLOR);
        return;
    }

    gre_set_control_visible(prestartContainer->app,
                            "sportPreStartLayer.SensorValueGroup.GpsSensorValueControl", true);
    prestartContainer->gpsIconAnim = createFrameImgAnimContainer(
                                         prestartContainer->app);

    if (!prestartContainer->gpsIconAnim)
    {
        return;
    }

    if (prestartContainer->gpsIconAnim->create(prestartContainer->app,
            prestartContainer->gpsIconAnim, ID_ANIMATE_GPS_IMAGE, "images/sports/go/gps/",
            "gps", "png", 9))
    {
        return;
    }

#ifdef SPORT_PRESTART_ANIM_FPS
    prestartContainer->gpsIconAnim->setup(prestartContainer->gpsIconAnim, 1,
                                          SPORT_PRESTART_ANIM_FPS, 1, 0, 0);
#else
    prestartContainer->gpsIconAnim->setup(prestartContainer->gpsIconAnim, 1, 9, 1,
                                          0, 0);
#endif
    prestartContainer->gpsIconAnim->start(prestartContainer->gpsIconAnim);
    _updateBtnStart(prestartContainer, prestartContainer->status.gpsStatus);
    _updateGpsStatus(prestartContainer, prestartContainer->status.gpsStatus);
}

static void _initHRView(SportPrestartContainer *prestartContainer)
{
    if (getSportsScreenPresenter()->sportTypeSupportHR() == false)
    {
        prestartContainer->hrIconAnim = NULL;
        gre_set_control_visible(prestartContainer->app,
                                "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", false);
        return;
    }

    gre_set_control_visible(prestartContainer->app,
                            "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", true);
    prestartContainer->hrIconAnim = createFrameImgAnimContainer(
                                        prestartContainer->app);
#ifdef HM_DISPLAY_IS_ROUND

    if (prestartContainer->hasGPS == false)
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", getDimension(285));
    }
    else
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", getDimension(346));
    }

#else

#ifndef HM_FEATURE_BLED_UI
    if (prestartContainer->hasGPS == false)
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", getDimension(225));
    }
    else
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", getDimension(287));
    }
#else
    if (prestartContainer->hasGPS == false)
    {
        int HrSensorValuex = getSBIntData(getAppInstance(),"sportPreStartLayer.SensorValueGroup.HrSensorValueControl.nogps");
        gre_set_control_x(prestartContainer->app,
                      "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", HrSensorValuex);
    }
    else
    {
        int HrSensorValuex = getSBIntData(getAppInstance(),"sportPreStartLayer.SensorValueGroup.HrSensorValueControl.hasgps");
        gre_set_control_x(prestartContainer->app,
                      "sportPreStartLayer.SensorValueGroup.HrSensorValueControl", HrSensorValuex);
    }
#endif

#endif

    if (!prestartContainer->hrIconAnim)
    {
        return;
    }

    if (prestartContainer->hrIconAnim->create(prestartContainer->app,
            prestartContainer->hrIconAnim, ID_ANIMATE_HR_IMAGE, "images/sports/go/hr/",
            "hr", "png", 8))
    {
        return;
    }

#ifdef SPORT_PRESTART_ANIM_FPS
    prestartContainer->hrIconAnim->setup(prestartContainer->hrIconAnim, 1,
                                         SPORT_PRESTART_ANIM_FPS, 1, 0, 0);
#else
    prestartContainer->hrIconAnim->setup(prestartContainer->hrIconAnim, 1, 9, 1, 0,
                                         0);
#endif
    prestartContainer->hrIconAnim->start(prestartContainer->hrIconAnim);
    _updateHrVaule(prestartContainer, getSportsScreenPresenter()->getHrValue());
}

static void _getGoalString(SportPrestartContainer *prestartContainer, char *buf,
                           goal_data *goal)
{
    switch (goal->sport_goal_type)
    {
        case SPORT_TARGET_TIME:
            if (goal->value < 60.0)
            {
                snprintf(buf, 128, "%s\r\n%d %s",
                         System_getLocaleText("id_spset_target_time"),
                         (int)goal->value, System_getLocaleText("id_sport_unit_min"));
            }
            else
            {
                snprintf(buf, 128, "%s\r\n%d %s %02d %s",
                         System_getLocaleText("id_spset_target_time"),
                         (uint16_t)goal->value / 60, System_getLocaleText("id_sport_unit_hour"),
                         (uint16_t)goal->value % 60, System_getLocaleText("id_sport_unit_min"));
            }

            break;

        case SPORT_TARGET_DISTANCE:
            if (prestartContainer->sportType == SPORT_TYPE_OPEN_WATER_SWIMMING)
            {
                system_unit distanceUnit =
                    sportManager()->utilsManager()->common()->getSystemUnit();
                float distance = goal->value;

                if (distanceUnit == METRIC_SYSTEM)
                {
                    snprintf(buf, 128, "%s\r\n%d %s",
                             System_getLocaleText("id_spset_target_distance"),
                             (int)distance, System_getLocaleText("id_sport_chart_altitude_matric_unit"));
                }
                else
                {
                    snprintf(buf, 128, "%s\r\n%d %s",
                             System_getLocaleText("id_spset_target_distance"),
                             (int)distance, System_getLocaleText("id_sport_unit_yard"));
                }
            }
            else
            {
                system_unit distanceUnit =
                    sportManager()->utilsManager()->common()->getSystemUnit();
                float distance = goal->value;

                if (distanceUnit == METRIC_SYSTEM)
                {
                    snprintf(buf, 128, "%s\r\n%d %s",
                             System_getLocaleText("id_spset_target_distance"),
                             (int)distance, System_getLocaleText("id_spr_km_unit"));
                }
                else
                {
                    snprintf(buf, 128, "%s\r\n%d %s",
                             System_getLocaleText("id_spset_target_distance"),
                             (int)distance, System_getLocaleText("id_spr_mile_unit"));
                }
            }

            break;

        case SPORT_TARGET_CONSUME:
            snprintf(buf, 128, "%s\r\n%d %s ",
                     System_getLocaleText("id_spset_target_calories"),
                     (int)goal->value, System_getLocaleText("id_spr_kcal_unit"));
            break;

        /* TODO 复数 */
        case SPORT_TARGET_TRIPS:
            snprintf(buf, 128, "%s\r\n%d %s",
                     System_getLocaleText("id_spset_target_trips"),
                     (int)goal->value, System_getLocaleText("id_spr_trip_unit"));
            break;

        /* TODO 复数 */
        case SPORT_TARGET_COUNT:
            snprintf(buf, 128, "%s\r\n%d %s",
                     System_getLocaleText("id_spset_target_count"),
                     (int)goal->value, System_getLocaleText("id_spr_count_unit"));
            break;

        case SPORT_TARGET_AEROBICTE:
            snprintf(buf, 128, "%s\r\n%.1f",
                     System_getLocaleText("id_spset_aerobic_te"),
                     goal->value);
            break;

        case SPORT_TARGET_ANAEROBICTE:
            snprintf(buf, 128, "%s\r\n%.1f",
                     System_getLocaleText("id_spset_anaerobic_te"),
                     goal->value);
            break;

        default:
            break;
    }
}

static void _initGoalView(SportPrestartContainer *prestartContainer)
{
    goal_data goal = getSportsScreenPresenter()->getGoal();

    if (goal.isHave)
    {
        char text[128] = {0};
        _getGoalString(prestartContainer, text, &goal);
        setSBStringData(getAppInstance(), PATH_TARGET_TEXT_VALUE, text);
    }
    else
    {
        setSBStringData(getAppInstance(), PATH_TARGET_TEXT_VALUE,
                        System_getLocaleText("id_sport_setting"));
    }
}

static void _initSportTypeView(SportPrestartContainer *prestartContainer)
{
    setSBStringData(getAppInstance(), PATH_SPORT_TYPE_VALUE,
                    sportType_getTypeNameChar(prestartContainer->sportType));
    setSBStringData(getAppInstance(), PATH_SPORT_TYPE_ICON_VALUE,
                    sportType_getTypeIconSizeId(prestartContainer->sportType, 160));
#ifdef HM_DISPLAY_IS_ROUND

    if (prestartContainer->hasGPS == false)
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.SportTypeIconControl", getDimension(101));
    }
    else
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.SportTypeIconControl", getDimension(160));
    }

#else

#ifndef HM_FEATURE_BLED_UI
    if (prestartContainer->hasGPS == false)
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.SportTypeIconControl", getDimension(82));
    }
    else
    {
        gre_set_control_x(prestartContainer->app,
                          "sportPreStartLayer.SensorValueGroup.SportTypeIconControl", getDimension(136));
    }
#else
    if (prestartContainer->hasGPS == false)
    {
        int typeiconx = getSBIntData(getAppInstance(),"sportPreStartLayer.SensorValueGroup.SportTypeIconControl.nogps");
        gre_set_control_x(prestartContainer->app,
                      "sportPreStartLayer.SensorValueGroup.SportTypeIconControl", typeiconx);
    }
    else
    {
        int typeiconx = getSBIntData(getAppInstance(),"sportPreStartLayer.SensorValueGroup.SportTypeIconControl.hasgps");
        gre_set_control_x(prestartContainer->app,
                      "sportPreStartLayer.SensorValueGroup.SportTypeIconControl", typeiconx);
    }
#endif

#endif
}

static void _updateGpsStatus(SportPrestartContainer *prestartContainer,
                             H_Int8 status)
{
    if (prestartContainer->gpsIconAnim == NULL)
    {
        return;
    }

    if (status == 0)
    {
        if (prestartContainer->status.gpsStatus != status) /* 状态变化 */
        {
            prestartContainer->status.gpsStatus = status;
            prestartContainer->status.gpsStartSearchTime = _getTimeStamp();

            if (prestartContainer->gpsIconAnim->status != FRAME_IMG_ANIMATION_PLAYING)
            {
                prestartContainer->gpsIconAnim->start(prestartContainer->gpsIconAnim);
            }

            _updateBtnStart(prestartContainer, status);
        }
    }
    else if (status == 1)
    {
        if (prestartContainer->status.gpsStatus != status) /* 状态变化 */
        {
            prestartContainer->status.gpsStatus = status;
            prestartContainer->gpsIconAnim->stop(prestartContainer->gpsIconAnim);
            setSBStringData(getAppInstance(), ID_ANIMATE_GPS_IMAGE,
                            "images/sports/go/gps/gps_8.png");

            getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_SUCCESS);
            if (prestartContainer->status.gpsSuccessTime ==
                    0) /* 仅首次定位成功进行提醒 */
            {
                prestartContainer->status.gpsSuccessTime = _getTimeStamp();
                getPowerManager()->userActivity();
                HLOG_I(TAG, "gps success at:%d prestart time:%d",
                       prestartContainer->status.gpsSuccessTime,
                       prestartContainer->status.preStartTime);
            }

            if (prestartContainer->status.AGPSTipsIsShow == SPORT_AGPS_TIPS_SHOWING)
            {
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
                prestartContainer->status.AGPSTipsIsShow = SPORT_APGS_TIPS_DISMISS;
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_RESET_PRESTART_TIME);
            }

            _updateBtnStart(prestartContainer, status);
        }
    }
}

static void _updateHrVaule(SportPrestartContainer *prestartContainer,
                           H_Uint8 value)
{
    char buffer[10] = "--";

    if (value > 40)
    {
        snprintf(buffer, sizeof(buffer), "%d", value);
    }

    setSBStringData(getAppInstance(), PATH_HR_VALUE, (const char *)buffer);
}

static void setVisiable(SportPrestartContainer *prestartContainer,
                        int isVisiable)
{
    if (isVisiable)
    {
        if (prestartContainer->gpsIconAnim)
        {
            prestartContainer->gpsIconAnim->resume(prestartContainer->gpsIconAnim);
        }

        if (prestartContainer->hrIconAnim)
        {
            prestartContainer->hrIconAnim->resume(prestartContainer->hrIconAnim);
        }
    }
    else
    {
        if (prestartContainer->gpsIconAnim)
        {
            prestartContainer->gpsIconAnim->pause(prestartContainer->gpsIconAnim);
        }

        if (prestartContainer->hrIconAnim)
        {
            prestartContainer->hrIconAnim->pause(prestartContainer->hrIconAnim);
        }
    }

    gre_set_layer_visible(prestartContainer->app, "SportScreen.sportPreStartLayer",
                          isVisiable);
}

static void _updateBtnStart(SportPrestartContainer *prestartContainer,
                            H_Int8 gpsValue)
{
    if (gpsValue == 1)
    {
        setSBIntData(getAppInstance(), PATH_BTN_START_COLOR_VALUE,
                     BTN_START_GREEN_COLOR);
    }
    else
    {
        setSBIntData(getAppInstance(), PATH_BTN_START_COLOR_VALUE, BTN_START_RED_COLOR);
    }
}

static bool _isInSlideRegion(SportPrestartContainer *prestartContainer,
                             H_Int16 offsetY)
{
    if (offsetY > 0 || offsetY < -prestartContainer->settingLayerHeight)
    {
        return false;
    }

    return true;
}

static void _updatePosition(SportPrestartContainer *prestartContainer,
                            H_Int16 offsetY)
{
    HLOG_W(TAG, "_updatePosition : %d", offsetY);
    prestartContainer->status.offsetY = offsetY;
    _updateBtnStartPosition(prestartContainer, offsetY);
    _updateBtnStartShape(prestartContainer, offsetY);
}

static void _updateBtnStartPosition(SportPrestartContainer *prestartContainer,
                                    H_Int16 offsetY)
{
    if (offsetY <= -(prestartContainer->btnStartFinalHeight))
    {
        int newY = -(offsetY + prestartContainer->btnStartFinalHeight);

        if (prestartContainer->btnStartOffsetY != (prestartContainer->btnStartOriginalY
                + newY))
        {
            prestartContainer->btnStartOffsetY = prestartContainer->btnStartOriginalY +
                                                 newY;
            setSBIntData(prestartContainer->app,
                         "sportPreStartLayer.SensorValueGroup.grd_y",
                         prestartContainer->btnStartOffsetY);
        }
    }
    else
    {
        if (prestartContainer->btnStartOffsetY != prestartContainer->btnStartOriginalY)
        {
            setSBIntData(prestartContainer->app,
                         "sportPreStartLayer.SensorValueGroup.grd_y",
                         prestartContainer->btnStartOriginalY);
            prestartContainer->btnStartOffsetY = prestartContainer->btnStartOriginalY;
        }
    }
}

static void _updateBtnStartShape(SportPrestartContainer *prestartContainer,
                                 H_Int16 offsetY)
{
    H_Int16 radius = 0;

    if (offsetY <= -prestartContainer->btnStartOriginalY)
    {
        //去除btnStartOriginalY
        offsetY += prestartContainer->btnStartOriginalY;
        //计算radius [0,btnStartOriginalRadius] 闭区间
        radius = prestartContainer->btnStartOriginalRadius;
        radius += offsetY;
        radius = radius > prestartContainer->btnStartOriginalRadius ?
                 prestartContainer->btnStartOriginalRadius : radius;
        radius = radius < 0 ? 0 : radius;
    }
    else
    {
        //处理无效 和 异常
        if (prestartContainer->btnStartShapeAnimLastRadius !=
                prestartContainer->btnStartOriginalRadius)
        {
            radius = prestartContainer->btnStartOriginalRadius;
        }
        else
        {
            return;
        }
    }

    //复合条件就刷新
    if (prestartContainer->btnStartShapeAnimLastRadius != radius)
    {
        setSBIntData(prestartContainer->app,
                     "sportPreStartLayer.SensorValueGroup.BtnStartControl.sportPrestartBtnGoRadius",
                     radius);
        float progress = (float)(radius > 0 ? radius : 0) /
                         prestartContainer->btnStartOriginalRadius;
        //HLOG_W(TAG, "progress 2: %f", progress);
        int alpha = 255 * progress;
        int width = (1 - progress) * (getDisplayMetrics()->width -
                                      prestartContainer->btnStartOriginalWidth) +
                    prestartContainer->btnStartOriginalWidth;
        setSBIntData(prestartContainer->app,
                     "sportPreStartLayer.SensorValueGroup.GpsSensorValueControl.sportGpsSensorGroupAlpha",
                     alpha);
        setSBIntData(prestartContainer->app,
                     "sportPreStartLayer.SensorValueGroup.HrSensorValueControl.sportHrSensorGroupAlpha",
                     alpha);
        setSBIntData(prestartContainer->app,
                     "sportPreStartLayer.SensorValueGroup.SportTypeIconControl.alpha", alpha);
        setSBIntData(prestartContainer->app,
                     "sportPreStartLayer.SensorValueGroup.BtnStartControl.iconGoAlpha", 255 - alpha);
        setSBIntData(prestartContainer->app,
                     "sportPreStartLayer.SensorValueGroup.BtnStartControl.btnWidth", width);
        prestartContainer->btnStartShapeAnimLastRadius = radius;
    }
}

static void pageScrollLayersCallback(gr_application_t *app, void *handle,
                                     void *param)
{
    HLOG_D(TAG, "pageScrollLayersCallback");
    SportPrestartContainer *prestartContainer = param;
    prestartContainer->slideAnimation = NULL;
}
static void pageScrollLayersEveryCallback(gr_application_t *app, void *handle,
        void *param)
{
    //HLOG_D(TAG, "pageScrollLayersEveryCallback");
    SportPrestartContainer *container = param;
    H_Int16 offsetY = getSBIntData(((SportPrestartContainer *)container)->app,
                                   "SportScreen.sportPreStartLayer.userYOffset");
    gre_set_layer_yoffset(((SportPrestartContainer *)container)->app,
                          "sportPreStartLayer", offsetY);
    _updatePosition((SportPrestartContainer *)container, offsetY);
}
static void _startSlideAnimation(SportPrestartContainer *prestartContainer,
                                 bool isUp, bool isForce)
{
    if (prestartContainer->slideAnimation != NULL)
    {
        gre_animation_stop(prestartContainer->app, prestartContainer->slideAnimation);
    }

    prestartContainer->slideAnimation = gre_animation_create_ex(getAppInstance(),
                                        SLIDEANIMATION_FPS, 1, pageScrollLayersEveryCallback, pageScrollLayersCallback,
                                        prestartContainer);
    gr_animation_data_t data[5] = {0};
    char time[8] = {0};
    char to[8] = {0};
    char from[8] = {0};
    data[0].key = "rate";
    data[0].val = "easeout";
    data[1].key = "key";
    data[1].val = "SportScreen.sportPreStartLayer.userYOffset";
    data[2].key = "to";
    int16_t toVal = isUp ? 0 : -prestartContainer->btnStartMoveY;
    snprintf(to, sizeof(to), "%d", toVal);
    data[2].val = to;
    uint16_t time_ms = 0;
    time_ms = SLIDEANIMATION_TIME * fabs((isUp ? (float)(0 -
                                          prestartContainer->status.offsetY) / prestartContainer->btnStartMoveY : (float)(
                                                  prestartContainer->btnStartMoveY + prestartContainer->status.offsetY) /
                                          prestartContainer->btnStartMoveY));
    time_ms = time_ms < 1000 / SLIDEANIMATION_FPS ? (1000 / SLIDEANIMATION_FPS) *
              2 : time_ms; // At least two times
    snprintf(time, sizeof(time), "%d", time_ms);
    data[3].key = "duration";
    data[3].val = time;
    snprintf(from, sizeof(from), "%d",
             getSBIntData(((SportPrestartContainer *)prestartContainer)->app,
                          "SportScreen.sportPreStartLayer.grd_yoffset"));
    data[4].key = "from";
    data[4].val = from;
    gre_animation_add_step(getAppInstance(), prestartContainer->slideAnimation,
                           data, sizeof(data) / sizeof(data[0]));
    gre_animation_trigger(getAppInstance(), prestartContainer->slideAnimation);
}

static bool getVisiable(SportPrestartContainer *dataView)
{
    int visiable = gre_get_layer_visible(dataView->app,
                                         "SportScreen.sportPreStartLayer");
    return visiable ? true : false;
}

/* AGPS 过期 */
static bool _isAGNSSOverdue(void)
{
    return getSportsScreenPresenter()->getAGNSSStatusIsOverdue();
}

static void _resetPresatrtTime(SportPrestartContainer *container)
{
    container->status.preStartTime = _getTimeStamp();
    container->status.GPSFailedIsShow = false;
    container->status.gpsStartSearchTime = _getTimeStamp();
}

static void _secTickEvent(SportPrestartContainer *container)
{
    if (!container)
    {
        return;
    }

    if (!container->getVisiable(container))
    {
        return;
    }

    _updateHrVaule(container, getSportsScreenPresenter()->getHrValue());

    /* 持续30分钟没有开始 退出运动 */
    if (container->status.ExitSportIsShow == false)
    {
        uint32_t lastUpdateTime = container->status.gpsSuccessTime == 0 ?
                                  container->status.preStartTime : container->status.gpsSuccessTime;

        if (_getTimeStamp() - lastUpdateTime > GPSTIMEOUT)
        {
            if (container->status.GPSFailedIsShow == true)
            {
                container->status.GPSFailedIsShow = false;
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
                HLOG_I(TAG, "dismiss_dialog");
            }

            container->status.ExitSportIsShow = true;
            getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_PRESTART_EXITSPORT);
            HLOG_I(TAG, "prestart time : %d  success time:%d now: %d auto exit sport",
                   container->status.preStartTime, container->status.gpsSuccessTime,
                   _getTimeStamp());
        }
    }

    if (container->hasGPS == false)
    {
        return;
    }

    _updateGpsStatus(container, getSportsScreenPresenter()->getGpsStatus());

    /* 5s后消失 AGPS提醒*/
    if (container->status.AGPSTipsIsShow != SPORT_APGS_TIPS_DISMISS &&
            _getTimeStamp() - container->status.preStartTime > AGPSTIPSTIMEOUT)
    {
        if (container->status.AGPSTipsIsShow == SPORT_AGPS_TIPS_SHOWING)
        {
            getSportsScreenPresenter()
            ->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
        }

        container->status.AGPSTipsIsShow = SPORT_APGS_TIPS_DISMISS;
    }
    /* GPS定位失败 超时后弹提示 */
    else if ((_getTimeStamp() - container->status.gpsStartSearchTime >
              GPSTIPSTIMEOUT) &&
             (container->status.gpsStatus == 0) &&
             (container->status.GPSFailedIsShow == false) &&
             (container->status.ExitSportIsShow == false))
    {
        container->status.GPSFailedIsShow = true;

        if (_isAGNSSOverdue())
        {
            getSportsScreenPresenter()->sendUIEvent(
                SPORTUI_EVENT_SHOW_GNSS_FAILED_NO_AGNSS);
        }
        else
        {
            getSportsScreenPresenter()->sendUIEvent(
                SPORTUI_EVENT_SHOW_GNSS_FAILED_HAS_AGNSS);
        }

        HLOG_I(TAG, "prestart time : %d StartSearchTime：%d now: %d show_gnss_failed",
               container->status.preStartTime, container->status.gpsStartSearchTime,
               _getTimeStamp());
    }
}

static void _restoreContainer(SportPrestartContainer *container)
{
    /* if(!container->getVisiable(container))
        return; */
    if (container->status.AGPSTipsIsShow == SPORT_APGS_TIPS_DISMISS)
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
    }

    if (container->status.isInSetting == true)
    {
#ifdef HM_DISPLAY_IS_ROUND
        H_Int16 layerOffsetMax = getDimension(-400);
#else

#ifndef HM_FEATURE_BLED_UI
        H_Int16 layerOffsetMax = getDimension(-339);
#else
        H_Int16 layerOffsetMax = -gre_get_control_h(getAppInstance(), "sportPreStartLayer.SettingBgControl");
#endif

#endif

        if (layerOffsetMax > container->status.offsetY)
        {
            container->status.offsetY = layerOffsetMax;
        }

        setSBIntData(container->app, "SportScreen.sportPreStartLayer.grd_yoffset",
                     container->status.offsetY);
        _updatePosition(container, container->status.offsetY);
    }

    if (container->status.GPSFailedIsShow == true)
    {
        if (_isAGNSSOverdue())
        {
            getSportsScreenPresenter()->sendUIEvent(
                SPORTUI_EVENT_SHOW_GNSS_FAILED_NO_AGNSS);
        }
        else
        {
            getSportsScreenPresenter()->sendUIEvent(
                SPORTUI_EVENT_SHOW_GNSS_FAILED_HAS_AGNSS);
        }
    }

    if (container->status.ExitSportIsShow == true)
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_PRESTART_EXITSPORT);
    }

    if (container->status.gpsStatus != 0)
    {
        container->status.gpsStatus = 0;
        _updateGpsStatus(container, true);
    }
}

SportPrestartContainer *bindSportPrestartView(gr_application_t *app)
{
    SportPrestartContainer *prestartContainer = H_NULL;

    if (!prestartContainer)
    {
        createContainer(prestartContainer, SportPrestartContainer,
                        prestartContainer->container);
        prestartContainer->initContainer = initContainer;
        prestartContainer->setVisiable = setVisiable;
        prestartContainer->getVisiable = getVisiable;
        prestartContainer->secTickEvent = _secTickEvent;
        prestartContainer->restoreContainer = _restoreContainer;
        prestartContainer->resetPresatrtTime = _resetPresatrtTime;
    }

    return prestartContainer;
}

static int g_dragType = DRAG_NONE;
static int g_pressDown = false;
static int g_lastLayerY = 0;
static SPoint g_pressPos = {.x = 0, .y = 0};
static bool isSlideToUp = false;

static bool matchListEvent(sport_setting_container *sportSettingContainer,
                           ScreenContext *screenContext, const char *id, const SBTouchEvent eventId,
                           const SBEventInfo *info)
{
    sportSettingContainer->event.style  = SB_EVENT_STYLE_TOUCH;
    sportSettingContainer->event.screen = screenContext;
    sportSettingContainer->event.id     = id;
    sportSettingContainer->event.type   = eventId;
    sportSettingContainer->event.info   = info;
    return sportSettingContainer->ops->event(sportSettingContainer);
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    SportPrestartContainer *prestartContainer = (SportPrestartContainer *)container;

    if (!prestartContainer->getVisiable(prestartContainer))
    {
        return false;
    }

    if (eventId == TOUCH_SELECT)
    {
        if (prestartContainer->status.offsetY == 0
                || prestartContainer->status.offsetY <= -prestartContainer->btnStartMoveY)
        {
            HLOG_D(TAG, "touch id : %s", id);

            if (STR_MATCH("sportPreStartLayer.SensorValueGroup.BtnStartControl", id) ||
                    STR_MATCH("sportPreStartLayer.SensorValueGroup.HrSensorValueControl", id) ||
                    STR_MATCH("sportPreStartLayer.SensorValueGroup.GpsSensorValueControl", id))
            {
                if(prestartContainer->status.gpsStatus || !prestartContainer->hasGPS)
                {
                    getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_START_SPORT);
                }
                else
                {
                    getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_HIDE_STATUS_BAR);
                    getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_GPS_NOTICE);
                }
                prestartContainer->status.isInSetting = false;
                return true;
            }
            else if (STR_MATCH("sportPreStartLayer.BtnSettingControl", id))
            {
                _startSlideAnimation(prestartContainer, false, true);
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_HIDE_STATUS_BAR);
                prestartContainer->status.isInSetting = true;
                return true;
            }
            else
            {
                return matchListEvent(prestartContainer->sportSettingContainer, screenContext,
                                      id, eventId, info);
            }
        }
    }

    if (eventId == TOUCH_DOWN)
    {
        g_pressDown = true;
        g_pressPos.x = info->x;
        g_pressPos.y = info->y;
        g_lastLayerY = gre_get_layer_yoffset(((SportPrestartContainer *)container)->app,
                                             "sportPreStartLayer");
    }
    else if (eventId == TOUCH_MOVE)
    {
        if (g_pressDown)
        {
            if (prestartContainer->slideAnimation != NULL)
            {
                gre_animation_stop(prestartContainer->app, prestartContainer->slideAnimation);
            }

            int deltaX = info->x - g_pressPos.x;
            int deltaY = info->y - g_pressPos.y;
            isSlideToUp = (info->y - g_pressPos.y) > 0;

            if (abs(deltaY) > SCROLL_DRAG_THREHOLD && abs(deltaY) > abs(deltaX)
                    && g_dragType == DRAG_NONE)
            {
                g_dragType = DRAG_VER;
            }

            if (abs(deltaX) > SCROLL_DRAG_THREHOLD && g_dragType != DRAG_VER)
            {
                g_dragType = DRAG_HOR;
            }
        }

        if (g_dragType == DRAG_VER)
        {
            if (prestartContainer->status.isInSetting == false)
            {
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_HIDE_STATUS_BAR);
            }

            H_Int16 offsetY = g_lastLayerY + info->y - g_pressPos.y;

            if (offsetY > 0)
            {
                offsetY = 0;
            }

#ifdef HM_DISPLAY_IS_ROUND

            if (offsetY < getDimension(-400))
            {
                offsetY = getDimension(-400);
            }

#else

#ifndef HM_FEATURE_BLED_UI
            if (offsetY < getDimension(-339))
            {
                offsetY = getDimension(-339);    //   338 offset=337
            }
#else
            if (offsetY < -gre_get_control_h(getAppInstance(), "sportPreStartLayer.SettingBgControl"))
            {
                offsetY = -gre_get_control_h(getAppInstance(), "sportPreStartLayer.SettingBgControl");    //   338 offset=337
            }
#endif

#endif
            gre_set_layer_yoffset(((SportPrestartContainer *)container)->app,
                                  "sportPreStartLayer", offsetY);
            _updatePosition((SportPrestartContainer *)container, offsetY);
            return true;
        }
    }
    else if (eventId == TOUCH_UP)
    {
        g_pressDown = false;

        if (g_dragType == DRAG_VER)
        {
            g_dragType = DRAG_NONE;

            if (_isInSlideRegion(prestartContainer, prestartContainer->status.offsetY))
            {
                _startSlideAnimation(prestartContainer, isSlideToUp, true);

                if (isSlideToUp == false)
                {
                    getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_HIDE_STATUS_BAR);
                    prestartContainer->status.isInSetting = true;
                }
                else
                {
                    prestartContainer->status.isInSetting = false;
                }
            }

            if (prestartContainer->status.isInSetting == false)
            {
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
            }

            return true;
        }
        else if (g_dragType == DRAG_HOR)
        {
            g_dragType = DRAG_NONE;
        }
    }

    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container, const HmKeyEvent *info)
{
    SportPrestartContainer *prestartContainer = (SportPrestartContainer *)container;

    if (!prestartContainer->getVisiable(prestartContainer))
    {
        return false;
    }

    if (info->code == HM_KEY_CODE_SELECT && info->action == HM_KEY_ACTION_CLICK)
    {
        getSportsScreenPresenter()->requestStartAnim();
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
        prestartContainer->status.isInSetting = false;
        return true;
    }

    if (info->code == HM_KEY_CODE_BACK && info->action == HM_KEY_ACTION_CLICK)
    {
        getSportsScreenPresenter()->requestExit();
        return true;
    }

    if (info->code == HM_KEY_CODE_UP && info->action == HM_KEY_ACTION_CLICK)
    {
        if (prestartContainer->status.isInSetting == true)
        {
            _startSlideAnimation(prestartContainer, true, true);
            {
                prestartContainer->status.isInSetting = false;
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
            }
        }

        return true;
    }

    if (info->code == HM_KEY_CODE_DOWN && info->action == HM_KEY_ACTION_CLICK)
    {
        if (prestartContainer->status.isInSetting == false)
        {
            _startSlideAnimation(prestartContainer, false, true);
            {
                prestartContainer->status.isInSetting = true;
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_HIDE_STATUS_BAR);
            }
        }

        return true;
    }

    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext,
                               ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext,
                        ScreenContainer *container, const char *id, const char *event)
{
    /* if (strcmp("layer_yoffset_change", event) == 0)
    {
        //HLOG_W(TAG, "touch id : %s", id);
        //H_Int16 offsetY = gre_get_layer_yoffset(((SportPrestartContainer *)container)->app, "sportPreStartLayer");
        //_updatePosition((SportPrestartContainer *)container, offsetY);
        return true;
    } */
    return false;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    //stop timer
    if (((SportPrestartContainer *)container)->slideAnimation != NULL)
    {
        gre_animation_stop(((SportPrestartContainer *)container)->app,
                           ((SportPrestartContainer *)container)->slideAnimation);
    }

    // reset offsetY
    setSBIntData(((SportPrestartContainer *)container)->app,
                 "SportScreen.sportPreStartLayer.grd_yoffset", 0);
    _updatePosition(((SportPrestartContainer *)container), 0);

    if (((SportPrestartContainer *)container)->gpsIconAnim)
    {
        ((ScreenContainer *)((SportPrestartContainer *)
                             container)->gpsIconAnim)->destroy(screenContext,
                                     (ScreenContainer *)((SportPrestartContainer *)container)->gpsIconAnim);
        H_FREE(((SportPrestartContainer *)container)->gpsIconAnim);
        ((SportPrestartContainer *)container)->gpsIconAnim = NULL;
    }

    if (((SportPrestartContainer *)container)->hrIconAnim)
    {
        ((ScreenContainer *)((SportPrestartContainer *)container)->hrIconAnim)->destroy(
            screenContext, (ScreenContainer *)((SportPrestartContainer *)
                                               container)->hrIconAnim);
        H_FREE(((SportPrestartContainer *)container)->hrIconAnim);
        ((SportPrestartContainer *)container)->hrIconAnim = NULL;
    }

    if (((SportPrestartContainer *)container)->sportSettingContainer)
    {
        sport_setting_container *sportSettingContainer = ((SportPrestartContainer *)
                container)->sportSettingContainer;
        sportSettingContainer->ops->destory(sportSettingContainer);
        H_FREE(sportSettingContainer);
        sportSettingContainer = NULL;
    }
}