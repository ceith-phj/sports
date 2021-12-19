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
#include <grelib.h>
#include <sb_screen_context.h>
#include <presenter/sports_list_presenter.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_translation.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <widget/arcScrollBar/arcScrollBar.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <system_locale.h>
#include "packages/apps/sports/include/struct/sports_manager_types.h"
#include <ui/Sports/include/sport/data/SportDataEditViewAdapter.h>
#include <ui/Sports/include/itemEdit/sport_chart_utils.h>
#include "packages/apps/sports/include/manager/sports_control_manager.h"
#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif
#include <presenter/SportPresenter.h>

#define TAG         "SportItemPageListScreen"
#define SCREEN_NAME SportItemPageList

#define ID_SPORTLIST_LIST           "SportItemPageListLayer.sportlist"
#define SPORTLIST_SCREEN_LAYER      "SportItemPageListScreen.SportItemPageListLayer"
#define PATH_SPORTLIST_ARCSCROLLBAR "SportItemPageListScrollLayer.arcScrollBar"
#define ID_SPORTLIST_SCROLLBAR      "SportItemPageListLayer.arcScrollBar"

SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    data_item_pattern *array;
    uint32_t array_size;
    data_item_pattern select_page_type;
} appData_t;

static const data_item_pattern itemPageList[] =
{
    ITEM_PATTERN_RT,    /**< 00' RT类数据. */
    ITEM_PATTERN_CHART, /**< 01' 图表类数据. */
};

static const char *itemPageNameList[2] =
{
    "id_sport_item_page_data",
    "id_sport_item_page_chart",
};

static void sportList_onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath);
static void sportList_onItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info);

//--------------------------------------------------------------------
static void _createSportlist(ScreenContext *screenContext)
{
    appData_t *data = getScreenData(screenContext, appData_t);
    int len = data->array_size;
    unequalHeiList *sportlist = createUnequalHeiListContainer(screenContext->_grAppContext);
    if (sportlist)
    {
        #ifdef HM_DISPLAY_IS_ROUND
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "SportListItemStyleLayer.RichContent",
                "SportListItemStyleLayer.Title"};

        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);

        sportlist->create(screenContext->_grAppContext, sportlist, "SportItemPageListScreen", "SportItemPageListLayer",
                          len + 1, itemTemps, tempCnt, sportList_onItemUpdate, sportList_onItemTouch, "item");
        #else
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "SportListItemStyleLayer.RichContent",
            };
        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);
        sportlist->create(screenContext->_grAppContext, sportlist, "SportItemPageListScreen", "SportItemPageListLayer",
                          len, itemTemps, tempCnt, sportList_onItemUpdate, sportList_onItemTouch, "item");
        #endif

        addContainer(screenContext, ID_SPORTLIST_LIST, sportlist);

        sportlist->setItemTemp(sportlist, 0, 0, 1);
        sportlist->setItemTemp(sportlist, 1, len, 2);
        sportlist->setItemSupportCircle(sportlist, 0, false);
        sportlist->setLastEmptySpace(sportlist, 90);
        sportlist->initItems(sportlist);
    }
}
//--------------------------------------------------------------------
static int _createSportListScrollBar(gr_application_t *app)
{
    int viewHei = gre_get_layer_h(app, SPORTLIST_SCREEN_LAYER);
    int totalHei = 0;
    arcScrollBar *scroll = createArcScrollBarContainer(app);
    if (scroll)
    {
        scroll->init(app, scroll, PATH_SPORTLIST_ARCSCROLLBAR);
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORTLIST_SCROLLBAR, scroll);
        scroll->setArcRange(scroll, -20, 20);
        unequalHeiList *list = (unequalHeiList *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORTLIST_LIST);
        if (list)
        {
            totalHei = list->getTotalHei(list);
        }
        scroll->setPosRange(scroll, totalHei, viewHei);
        int offset = gre_get_layer_yoffset(app, SPORTLIST_SCREEN_LAYER);
        scroll->setPos(scroll, offset);
    }
    return 0;
}

static void sportList_onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath)
{
    if (NULL == list || idx < 0 || NULL == itemGroupPath)
    {
        return;
    }

    Screen *screenInstance = getScreen(SCREEN_NAME);
    appData_t *data = getScreenData(getScreenContext(screenInstance), appData_t);

    char key[256] = {0};

    #ifdef HM_DISPLAY_IS_ROUND
    if (idx == 0)
    {
        snprintf(key, sizeof(key), "%s.item.text", itemGroupPath);
        setSBStringData(getAppInstance(), key, System_getLocaleText("id_sport_item_page_select"));
    }
    else if (idx <= data->array_size)
    {
        data_item_pattern *pageList = (data_item_pattern *)data->array;
        snprintf(key, sizeof(key), "%s.item.title", itemGroupPath);
        setSBStringData(getAppInstance(), key, System_getLocaleText(itemPageNameList[pageList[idx - 1]]));

        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.item.subtitle", itemGroupPath);
        char value[100] = {0};
        int subListSize = 0;
        uint32_t i = 0;
        const sport_chart_t *list = sportChart_getChartList();
        switch(idx - 1)
        {
            case ITEM_PATTERN_RT:
                // FIXME:
                subListSize = 6;
                break;
            case ITEM_PATTERN_CHART:
                for(i = 0; i < sportChart_getChartListSize(); i++)
                {
                    if(sportChart_isSupport(list[i].chartID, getSportsControlManager()->getType()))
                    {
                        subListSize++;
                    }
                }
                break;
        }

        snprintf(value, sizeof(value), System_getLocaleText("id_sport_item_number"), subListSize);
        setSBStringData(getAppInstance(), key, value);
    }
    #else
    if (idx <= data->array_size)
    {
        data_item_pattern *pageList = (data_item_pattern *)data->array;
        snprintf(key, sizeof(key), "%s.item.title", itemGroupPath);
        setSBStringData(getAppInstance(), key, System_getLocaleText(itemPageNameList[pageList[idx]]));

        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.item.subtitle", itemGroupPath);
        char value[100] = {0};
        int subListSize = 0;
        uint32_t i = 0;
        const sport_chart_t *list = sportChart_getChartList();
        switch(idx)
        {
            case ITEM_PATTERN_RT:
                // FIXME:
                subListSize = 6;
                break;
            case ITEM_PATTERN_CHART:
                for(i = 0; i < sportChart_getChartListSize(); i++)
                {
                    if(sportChart_isSupport(list[i].chartID, getSportsControlManager()->getType()))
                    {
                        subListSize++;
                    }
                }
                break;
        }

        snprintf(value, sizeof(value), System_getLocaleText("id_sport_item_number"), subListSize);
        setSBStringData(getAppInstance(), key, value);
    }
    #endif
}

static void sportList_onItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    #ifdef HM_DISPLAY_IS_ROUND
    if (NULL == list || (idx - 1) < 0)
    {
        return;
    }

    if (eventId != TOUCH_SELECT)
    {
        return;
    }
    Screen *screenInstance = getScreen(SCREEN_NAME);

    appData_t *data = getScreenData(getScreenContext(screenInstance), appData_t);
    data->select_page_type = data->array[idx - 1];
    #else
    if (NULL == list || idx < 0)
    {
        return;
    }

    if (eventId != TOUCH_SELECT)
    {
        return;
    }
    Screen *screenInstance = getScreen(SCREEN_NAME);

    appData_t *data = getScreenData(getScreenContext(screenInstance), appData_t);
    data->select_page_type = data->array[idx];
    #endif
    switch(data->select_page_type)
    {
        case ITEM_PATTERN_RT:
            getScreenManager()->startScreen("SportDataListScreen", NULL, 0);
            break;
        case ITEM_PATTERN_CHART:
            getScreenManager()->startScreen("SportChartListScreen", NULL, 0);
            break;
        default:
            break;
    }
}

static void _createAppData(ScreenContext *screenContext, void *data)
{
    appData_t *appData = (appData_t *)H_MALLOC(sizeof(appData_t));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(appData_t));
        setScreenData(screenContext, appData);
    }
}

static void _getDetailList(ScreenContext *screenContext)
{
    appData_t *appData = getScreenData(screenContext, appData_t);
    appData->array = (data_item_pattern *)itemPageList;
    appData->array_size = sizeof(itemPageList) / sizeof(itemPageList[0]);
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _createAppData(screenContext, data);
    _getDetailList(screenContext);
    _createSportlist(screenContext);
    _createSportListScrollBar(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext);

    #ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_updateTitle(System_getLocaleText("id_sport_item_page_select"));
    #endif
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{

}

static void onDataSave(ScreenContext *screenContext)
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

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, "%s\r\n", __func__);
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

    HLOG_D(TAG, "%s\r\n", __func__);
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    if (eventId == GESTURE_RIGHT)
    {
        getScreenManager()->finishCurrentScreen();
        return true;
    }
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    if (!strcmp(event, "scroll_change"))
    {
        arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
        if (scroll)
        {
            int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
            scroll->setPos(scroll, offset);
        }
    }
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    ScreenContainer *settingList = findContainer(screenContext, ID_SPORTLIST_LIST);
    if (settingList)
    {
        settingList->handleTouchEvent(screenContext, settingList, id, eventId, info);
        return true;
    }
    return false;
}
