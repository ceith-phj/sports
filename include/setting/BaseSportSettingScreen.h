#ifndef SPORT_BASE_SETTING_SCREEN_H
#define SPORT_BASE_SETTING_SCREEN_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <BaseSportView.h>
#include <setting/SportSettingTypeDef.h>
#include <manager/sports_control_manager.h>
#include <manager/sports_setting_manager.h>
#include <setting/BaseSportSettingLayer.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _BaseSportSettingScreen BaseSportSettingScreen;
    typedef struct _SportSettingScreenImpl SportSettingScreenImpl;

    struct _SportSettingScreenImpl
    {
        BaseSportSettingLayer *(*getBaseSportLayer)(ScreenContext *screenContext);
        BaseSportSettingScreen *baseScreen;
    };

    struct _BaseSportSettingScreen
    {
        void (*onScreenPreCreate)(BaseSportSettingScreen *screen, ScreenContext *screenContext, void *data);
        void (*onNewScreenData)(BaseSportSettingScreen *screen, ScreenContext *screenContext, void *data);
        void (*onDataRestore)(BaseSportSettingScreen *screen, ScreenContext *screenContext, void *data);
        void (*onScreenPostCreate)(BaseSportSettingScreen *screen, ScreenContext *screenContext);
        void (*onScreenPreDestory)(BaseSportSettingScreen *screen, ScreenContext *screenContext);
        void (*onDataSave)(BaseSportSettingScreen *screen, ScreenContext *screenContext);
        void (*onScreenPostDestory)(BaseSportSettingScreen *screen, ScreenContext *screenContext);
        void (*onAnimationComplete)(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id);
        void (*onAnimationStop)(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id);
        bool (*handleTouchEvent)(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info);
        bool (*handleKeyEvent)(BaseSportSettingScreen *screen, ScreenContext *screenContext, const HmKeyEvent *info);
        bool (*handleGestureEvent)(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id, const SBGestureEvent eventId);
        bool (*handleEvent)(BaseSportSettingScreen *screen, ScreenContext *screenContext, const char *id, const char *event);
        gr_application_t *app;
        ScreenContext *screenContext;
        SportSettingScreenImpl *childScreen;
        SportSettingsMessage *setting;
        H_Int16 gestureCount;
    };

    BaseSportSettingScreen *newSettingScreen();

#ifdef __cplusplus
}
#endif

#endif