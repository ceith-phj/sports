#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <sport_type_utils.h>
#include <sb_screen_context.h>
#include <gre/sdk/grelib.h>

#define TAG "SC_DETAIL"
#include "log.h"

#define ID_SPORT_COURSE_LIST            "SportCourseDetailMainLayer.SportCourseDetail"
#define SCREEN_NAME                     SportCourseDetail

SCREEN_DECLARE(SCREEN_NAME);

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
}

static void onNewScreenData(ScreenContext* screenContext, void* data)
{
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
}

static void onDataSave(ScreenContext *screenContext)
{
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
}

static bool handleKeyEvent(ScreenContext *screenContext,  const HmKeyEvent *info)
{
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    return true;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    return true;
}