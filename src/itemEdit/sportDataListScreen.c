/*
* =====================================================================================
*
*  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
*  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
*  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
*  All Rights Reserved.
*
*  Author: liubingyan@zepp.com
*
* =====================================================================================
*/

#include <stdbool.h>
#include <gre/gre.h>
#include <sb_screen_context.h>
#include <presenter/SportPresenter.h>
#include <BaseSportView.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include "hm_time.h"
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <core/power/hm_power_manager.h>
#include <widget/pageScrollerMultiLayers/pageScrollerMultiLayers.h>
#include "core/display_metrics.h"
#include <system_locale.h>
#include "sport/SportDialog.h"
#include <ui/Sports/include/itemEdit/sport_item_utils.h>
#include "presenter/sport_data_presenter.h"
#include "ui/Sports/include/sport/data/SportDataEditViewAdapter.h"
#include "packages/services/sport/misc/sport_misc.h"
#include <presenter/SportPresenter.h>

#define TAG "SportDataListScreen"
#define SCREEN_NAME SportDataList
#define ID_DATALIST_SCROLL  "SportDataListScreen.scroll"
#define ID_DATALIST_LAYER   "SportDataListScreen.sportDataListLayer"

typedef struct
{
    gr_application_t *app;
    int32_t initPage;
} appData_t;

#define ITEM_KEY_PREFIX     "pageControl"
#define ITEM_BACK_PREFIX    "btnReturn"

// 0 ~ 5
#define DEFAULT_FUCOS_ITEM  (3)

static appData_t *getAppData(void);

static H_Int32 getInt(const char * path)
{
    gr_application_t *app = getAppInstance();
    if(NULL == app)
    {
        return 0;
    }

    H_Int32 value = getDimension(getSBIntData(app, path));
    return value;
}

#define ID_DATA_LIST_LAYER_SNAP "sportDataListLayer.layer_snap"
#define DATA_LIST_LAYER_SNAP (getInt(ID_DATA_LIST_LAYER_SNAP))


static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    appData_t *appData = (appData_t *)H_MALLOC(sizeof(appData_t));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(appData_t));
        setScreenData(screenContext, appData);
    }

    appData->app = screenContext->_grAppContext;
    gre_set_layer_xoffset(appData->app, ID_DATALIST_LAYER, DEFAULT_FUCOS_ITEM * (0 - DATA_LIST_LAYER_SNAP));
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{

}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    if (getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_SUSPEND ||
        getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_RUN)
    {
        System_setKeyFilter(HM_KEY_CODE_HOME, HM_KEY_ACT_FILTER_CLICK | KEY_EVENT_LONG_PRESS);
        System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
    }
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    System_resetKeyFilter(HM_KEY_CODE_HOME);
    System_resetKeyFilter(HM_KEY_CODE_SHORTCUT);
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    if (info->code == HM_KEY_CODE_HOME &&
        getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_SUSPEND)
    {
        getScreenManager()->startScreen("SportScreen", NULL, 0);
        return true;
    }

    if (info->code == HM_KEY_CODE_SHORTCUT &&
        getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_SUSPEND)
    {
        getScreenManager()->startScreen("SportScreen", NULL, 0);
        return true;
    }

    return true;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    return false;
}

static char *searchButton(const char *id, char *name)
{
    return strstr(id, name);
}

static uint16_t getPageNumber(const char *id)
{
    char keyID[64] = {0};
    strncpy(keyID, id, sizeof(keyID));
    char *ret = searchButton(keyID, ITEM_KEY_PREFIX);
    ret += strlen(ITEM_KEY_PREFIX);
    ret[2] = '\0';
    return atoi(ret);
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if(searchButton(id, ITEM_KEY_PREFIX))
    {
        sport_data_page *page_info = NULL;
        sport_page_handle *handle = getSportsDataPresenter()->open(getSportsControlManager()->getType());
        int cnt = getSportsDataPresenter()->getDataPage(handle, &page_info);
        sport_data_page *new_page = hm_mem_malloc((cnt + 1) * sizeof(sport_data_page));
        memcpy(new_page, page_info, cnt * sizeof(sport_data_page));
        new_page[cnt].pattern = ITEM_PATTERN_RT;
        new_page[cnt].count = getPageNumber(id);
        for(uint8_t i = 0; i < new_page[cnt].count; i++)
        {
            new_page[cnt].item[i] = SPORT_RT_ATTR_TYPE_DURATION_NET;
        }
        getSportsDataPresenter()->setDataPage(handle, new_page, cnt + 1);
        getSportsDataPresenter()->close(handle);
        hm_mem_free(new_page);

        getAppData()->initPage = cnt;
        getScreenManager()->startScreen("SportItemEditScreen", &getAppData()->initPage, sizeof(getAppData()->initPage));
    }
    else if(searchButton(id, ITEM_BACK_PREFIX))
    {
        getScreenManager()->startScreen("SportItemEditScreen", H_NULL, 0);
    }
    return false;
}

DEFINE_SB_DATA_START(sCustomData)
    .isHideSystemBar = H_True,
DEFINE_SB_DATA_END

INTERFACE_DEFINED_START(sScreenInterface)
    .onScreenPreCreate = onScreenPreCreate,
    .onScreenPostCreate = onScreenPostCreate,
    .onScreenPreDestory = onScreenPreDestory,
    .onScreenPostDestory = onScreenPostDestory,
    .onNewScreenData = onNewScreenData,
    .onDataRestore = onDataRestore,
    .onDataSave = onDataSave,
    .onAnimationComplete = onAnimationComplete,
    .onAnimationStop = onAnimationStop,
    .handleTouchEvent = handleTouchEvent,
    .handleKeyEvent = handleKeyEvent,
    .handleGestureEvent = handleGestureEvent,
    .handleEvent = handleEvent,
INTERFACE_DEFINED_END

SCREEN_DECLARE_CUSTOM_START(SCREEN_NAME)
    .priority = PRIORITY_SCREEN_LOW,
    .data = &sCustomData,
    .screenInterface = &sScreenInterface,
SCREEN_DECLARE_CUSTOM_END


static appData_t *getAppData(void)
{
    return getScreenData(getScreenContext(getScreen(SCREEN_NAME)), appData_t);
}
