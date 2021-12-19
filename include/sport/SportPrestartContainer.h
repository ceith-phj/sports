#ifndef SPORTS_PRESTART_VIEW_H
#define SPORTS_PRESTART_VIEW_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include "sport_type_auto.h"
#include <widget/frameImgAnimation/frameImgAnimation.h>
#include <setting/MainSettingLayer.h>
#include "ui/Sports/include/setting/sport_setting_screen.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _SportPrestartContainer SportPrestartContainer;

    typedef enum
    {
        SPORT_AGPS_TIPS_NONE = 0,
        SPORT_AGPS_TIPS_SHOWING,
        SPORT_APGS_TIPS_DISMISS,
    } SPORT_AGPS_TIPS_E;
    typedef struct
    {
        bool isInSetting;
        H_Int16 offsetY;
        uint32_t preStartTime;  /* 运动预开时时间 */
        uint32_t gpsSuccessTime;/* GPS定位成功时间 */
        uint32_t gpsStartSearchTime;/* GPS开始定位时间 */
        bool ExitSportIsShow;   /* 关闭运动正在显示 */
        SPORT_AGPS_TIPS_E AGPSTipsIsShow; /* AGPS 过期提醒显示过了 */
        bool GPSFailedIsShow;             /* GPS定位失败正在显示 */
        H_Int8 gpsStatus;           /* GPS状态 */
    } SportPrestartStatus;

    struct _SportPrestartContainer
    {
        ScreenContainer container;

        void (*initContainer)(SportPrestartContainer *prestartContainer, ScreenContext *screenContext);
        void (*setVisiable)(SportPrestartContainer *prestartContainer, int isVisiable);
        bool (*getVisiable)(SportPrestartContainer *dataView);
        void (*secTickEvent)(SportPrestartContainer *dataView);
        void (*restoreContainer)(SportPrestartContainer *container);
        void (*resetPresatrtTime)(SportPrestartContainer *container);
        gr_application_t *app;
        ScreenContext *screenContext;
        sport_type_t sportType;
        sport_setting_container *sportSettingContainer;
        frameImgAnimation *gpsIconAnim;
        frameImgAnimation *hrIconAnim;
        gr_animation_handle_t *slideAnimation;
        BaseSportSettingLayer *settingContainer;
        H_Int16 btnStartOriginalY;
        H_Int16 btnStartOffsetY;
        H_Int16 btnStartMoveY;
        H_Int16 btnStartOriginalRadius;
        H_Int16 btnStartOriginalHeight;
        H_Int16 btnStartOriginalWidth;
        H_Int16 btnStartFinalHeight;
        H_Int16 btnStartShapeAnimLastRadius;
        H_Int16 settingLayerHeight;
        SportPrestartStatus status;
        bool hasGPS;
    };

    SportPrestartContainer *bindSportPrestartView(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif