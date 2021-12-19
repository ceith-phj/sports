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
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include "hm_time.h"
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <core/power/hm_power_manager.h>
#include <widget/pageScrollerMultiLayers/pageScrollerMultiLayers.h>
#include <ui/Sports/include/sport/data/SportDataEditViewAdapter.h>
#include "core/display_metrics.h"
#include <system_locale.h>
#include <ui/Sports/include/itemEdit/sport_item_utils.h>
#include "uiframework/ui_adapter/storyboard/engine/include/widget/sortList/sortList.h"
#include <presenter/SportPresenter.h>

#define TAG "SportItemSortingScreen"
#define SCREEN_NAME SportItemSorting

#define ID_SPORT_ITEM_SORTING_SCREEN        "SportItemSortingScreen"

#define SPORT_EDIT_SORT_NORMAL_ITEM_STYLE   "sport_edit_tmp_style_layer.tinyItem"
#define SPORT_EDIT_SORT_FOCUS_ITEM_STYLE    "sport_edit_tmp_style_layer.focusTinyItem"

#define NORMAL_ITEM_FONT                    "fonts/allfont-Medium.ttf"
#define NORMAL_ITEM_FONT_SIZE               getDimension(36)

#define NORMAL_FOCUS_FONT                   "fonts/allfont-Medium.ttf"
#define NORMAL_FOCUS_FONT_SIZE              getDimension(36)

#define SORTING_ITEM_WIDTH                  getDimension(280)

#define ID_ITEM_SORTING_LIST_LAYER          "SportItemSortingListLayer"
#define ID_SPORT_EDIT_SORT_LIST             "SportItemSortingListLayer.list"

#define SORTING_KEY_CONFIRM                 "btnConfirm"

SCREEN_DECLARE(SCREEN_NAME);
typedef struct
{
    gr_application_t *app;

    int *array;
    int array_size;
    int pageSelectd;
    int newPos;
    int yoffset;
    adapter_data_t *adapter_data;
} appData_t;

static const char *chartName[] =
{
    "id_rt_chart_hr",
    "id_rt_chart_speed",
    "id_rt_chart_stroke_frep",
    "id_rt_chart_stroke_speed",
    "id_rt_chart_pace",
    "id_rt_chart_altitude",
    "id_rt_chart_freq",
};

static void initSortingPage(ScreenContext *screenContext);
static void getItemPageListInfo(ScreenContext *screenContext);
static void createItemPageList(ScreenContext *screenContext);

static appData_t *getAppData(void)
{
    return getScreenData(getScreenContext(getScreen(SCREEN_NAME)), appData_t);
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    appData_t *appData = (appData_t *)H_MALLOC(sizeof(appData_t));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(appData_t));
        setScreenData(screenContext, appData);
    }

    appData->app = screenContext->_grAppContext;
    getAppData()->adapter_data = (adapter_data_t *)H_MALLOC(sizeof(adapter_data_t));

    // copy data
    if (data != H_NULL)
    {
        HLOG_D(TAG, "%s\r\n", __func__);
        memcpy(appData->adapter_data, *((adapter_data_t **)data), sizeof(adapter_data_t));

        initSortingPage(screenContext);
    }
}

static void createItemString(char *key, uint16_t size, uint16_t idx)
{
    #define MAX_ITEM_NAME_LEN   (28)

    int id = 0;
    int len = 0;
    adapter_data_t *adapter_data = (adapter_data_t *)getAppData()->adapter_data;
    sport_page_handle *handle = getSportsDataPresenter()->open(adapter_data->sportItemEdit.sportType);
    sport_data_page *page_info = NULL;
    getSportsDataPresenter()->getDataPage(handle, &page_info);

    switch(page_info[idx].pattern)
    {
        case ITEM_PATTERN_RT:
            len = hm_snprintf(key, size, "%d - ", page_info[idx].count);
            for(uint8_t i = 0; i < page_info[idx].count; i++)
            {
                len += hm_snprintf(&key[len], size - len, "%s, ", System_getLocaleText(sportItem_getTypeName(page_info[idx].item[i])));
            }

            break;
        case ITEM_PATTERN_CHART:
            id = page_info[idx].item[0];
            hm_snprintf(key, size, "%s", System_getLocaleText(chartName[id]));
            break;
        case ITEM_PATTERN_TE:
            hm_snprintf(key, size, "%s", System_getLocaleText("id_rt_chart_te"));
            break;
        // TODO:
        // OTA1
        // https://jira.huami.com/browse/GT3-24794
        #if 0
        case ITEM_PATTERN_RABBITE:
            break;
        case ITEM_PATTERN_CADENCE_ASSISTANT:
            break;
        case ITEM_PATTERN_GOAL:
            break;
        #endif
        default:
            break;
    }

    // ##
    getSportsDataPresenter()->close(handle);
}

static void _onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath)
{
    if (NULL == list || idx < 0 || NULL == itemGroupPath)
    {
        return;
    }

    #define STRING_SIZE     256

    char path[STRING_SIZE];
    char key[STRING_SIZE] = {0};
    createItemString(key, sizeof(key), idx);
    hm_snprintf(path, sizeof(path), "%s.item.content", itemGroupPath);
    setSBStringData(getAppInstance(), path, key);

    char ctrlPath[STRING_SIZE] = {0};
    hm_snprintf(ctrlPath, sizeof(ctrlPath), "%s.item", itemGroupPath);

    char newstr[STRING_SIZE] = {0};
    int is_trunk = 0;
    gre_get_textext_height_limit(getAppInstance(), ctrlPath, 0, 2, newstr,
                STRING_SIZE,
                &is_trunk);

    if(is_trunk)
    {
        strncpy(key, newstr, STRING_SIZE);
    }
    setSBStringData(getAppInstance(), path, key);

    getAppData()->newPos = idx;
}

static H_VOID _onSortItemUpdate(SortList *list, int idx, const char *itemGroupPath)
{
    char path[256];
    char key[256] = {0};
    createItemString(key, sizeof(key), idx);
    hm_snprintf(path, sizeof(path), "%s.item.content", itemGroupPath);
    setSBStringData(getAppInstance(), path, key);

    char ctrlPath[STRING_SIZE] = {0};
    hm_snprintf(ctrlPath, sizeof(ctrlPath), "%s.item", itemGroupPath);

    char newstr[STRING_SIZE] = {0};
    int is_trunk = 0;
    gre_get_textext_height_limit(getAppInstance(), ctrlPath, 0, 2, newstr,
                STRING_SIZE,
                &is_trunk);

    if(is_trunk)
    {
        strncpy(key, newstr, STRING_SIZE);
    }
    setSBStringData(getAppInstance(), path, key);
}

static void initSortingPage(ScreenContext *screenContext)
{
    getItemPageListInfo(screenContext);
    createItemPageList(screenContext);
}

static void getItemPageListInfo(ScreenContext *screenContext)
{
    adapter_data_t *adapter_data = (adapter_data_t *)getAppData()->adapter_data;
    appData_t *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)), appData_t);
    data->array_size = adapter_data->page_cnt;
    data->array = (int *)hm_mem_malloc(sizeof(int) * data->array_size);
    data->pageSelectd = adapter_data->page;
}

static void createItemPageList(ScreenContext *screenContext)
{
    appData_t *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)), appData_t);
    int len = data->array_size;

    SortList *sortList = createSortContainer(screenContext->_grAppContext);

    sortList->setItemUpdateListener(sortList, _onItemUpdate);
    sortList->setItemTemplate(sortList, SPORT_EDIT_SORT_NORMAL_ITEM_STYLE);
    sortList->setItemCount(sortList, len);
    sortList->setSortIndex(sortList, data->pageSelectd);
    sortList->setSortItemTemplate(sortList, SPORT_EDIT_SORT_FOCUS_ITEM_STYLE);
    sortList->setOnSortItemUpdateListener(sortList, _onSortItemUpdate);
    sortList->init(sortList, ID_SPORT_ITEM_SORTING_SCREEN, ID_ITEM_SORTING_LIST_LAYER);

    addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_EDIT_SORT_LIST, sortList);
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    memcpy(getAppData()->adapter_data, (adapter_data_t *)data, sizeof(adapter_data_t));

    initSortingPage(screenContext);

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
    getScreenManager()->saveScreenData(getScreen(SCREEN_NAME)->id, getAppData()->adapter_data, sizeof(adapter_data_t));
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    if(getAppData()->adapter_data)
    {
        hm_mem_free(getAppData()->adapter_data);
    }

    hm_mem_free(getAppData()->array);
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
    SortList *listContainer = (SortList *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_EDIT_SORT_LIST);
    if (listContainer != H_NULL)
    {
        return ((ScreenContainer *)listContainer)->handleKeyEvent(screenContext, (ScreenContainer *)listContainer, info);
    }
    return true;
}

static void setSortingResult(ScreenContext *screenContext, adapter_data_t *data)
{
    sport_page_handle *handle = getSportsDataPresenter()->open(data->sportItemEdit.sportType);
    sport_data_page *page_info = NULL;
    int cnt = getSportsDataPresenter()->getDataPage(handle, &page_info);
    sport_data_page *new_page = hm_mem_malloc(sizeof(sport_data_page) * cnt);

    SortList *listContainer = (SortList *)findContainer(screenContext, ID_SPORT_EDIT_SORT_LIST);
    H_Uint8 arrayLen = 0;
    H_Uint8 *array = listContainer->getItemArray(listContainer, &arrayLen);

    for(uint8_t k = 0; k < arrayLen; k++)
    {
        memcpy(&new_page[k], &page_info[array[k]], sizeof(sport_data_page));
    }

    getSportsDataPresenter()->setDataPage(handle, new_page, cnt);
    getSportsDataPresenter()->close(handle);
    hm_mem_free(new_page);
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    if(eventId == GESTURE_RIGHT)
    {
        adapter_data_t *adapter_data = (adapter_data_t *)getAppData()->adapter_data;
        setSortingResult(screenContext, adapter_data);
        getScreenManager()->startScreen("SportItemSettingScreen", &adapter_data, sizeof(adapter_data));
        return true;
    }
    return false;
}

static char *searchButton(const char *id, char *name)
{
    return strstr(id, name);
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    HLOG_D(TAG, "%s, id: %s, event: %s\r\n", __func__, id, event);
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if(searchButton(id, SORTING_KEY_CONFIRM))
    {
        adapter_data_t *adapter_data = (adapter_data_t *)getAppData()->adapter_data;
        setSortingResult(screenContext, adapter_data);
        getScreenManager()->startScreen("SportItemEditScreen", NULL, 0);
        return true;
    }
    else
    {
        SortList *listContainer = (SortList *)findContainer(screenContext, ID_SPORT_EDIT_SORT_LIST);
        if (listContainer != H_NULL)
        {
            return ((ScreenContainer *)listContainer)->handleTouchEvent(screenContext, (ScreenContainer *)listContainer, id, eventId, info);
        }
    }
    return false;
}
