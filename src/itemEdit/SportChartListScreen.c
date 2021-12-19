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
#include <hm_toast.h>
#include <system_locale.h>
#include "sport_utils.h"
#include "typeInit/sport_typeInit_dialog.h"
#include "packages/apps/sports/include/utils/sports_list_page_data.h"
#include "packages/apps/sports/include/manager/sports_chart_type.h"
#include <ui/Sports/include/itemEdit/sport_chart_utils.h>
#include "packages/services/sport/misc/sport_misc.h"
#include <ui/Sports/include/sport/data/SportDataEditViewAdapter.h>
#include "packages/apps/sports/include/manager/sports_control_manager.h"
#include "uiframework/ui_adapter/storyboard/engine/include/widget/list/list_widget.h"
#include <presenter/SportPresenter.h>

#define TAG         "SportChartListScreen"
#define SCREEN_NAME SportChartList

#define ID_SPORTLIST_LIST           "SportChartListLayer.sportlist"
#define SPORTLIST_SCREEN_LAYER      "SportChartListScreen.SportChartListLayer"
#define PATH_SPORTLIST_ARCSCROLLBAR "SportChartListScrollLayer.arcScrollBar"
#define ID_SPORTLIST_SCROLLBAR      "SportChartListLayer.arcScrollBar"

SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    sport_chart_rt_type_t chart;
    sport_chart_t *array;
    uint16_t array_size;
    list_widget *lw;
    int yoffset;
    uint16_t config;
    uint16_t cnt;
    int32_t initPage;
} appData_t;

static appData_t *getAppData(void)
{
    return getScreenData(getScreenContext(getScreen(SCREEN_NAME)), appData_t);
}

static void _lwEvent(list_widget *lw, uint16_t index, lw_event_type event, bool state)
{
    if(state)
    {
        getAppData()->config |= 1 << index;
        getAppData()->cnt++;
    }
    else
    {
        getAppData()->config &= ~(1 << index);
        getAppData()->cnt--;
    }
}

static void _createSportlist(ScreenContext *screenContext)
{
    appData_t *data = getAppData();
    sport_chart_t *chartList = (sport_chart_t *)data->array;
    lw_item lw_Item[data->array_size];
    data->lw = listWidget_Create("SportChartListScreen", "SportChartListLayer", _lwEvent);
    addContainer(screenContext, "SportChartList", data->lw);
    listWidget_SetListAttr(data->lw, LW_ATTR_TITLE, (void *)System_getLocaleText("id_sport_chart_list_title"));
    sport_page_handle *handle = getSportsDataPresenter()->open(getSportsControlManager()->getType());
    sport_data_page *page_info = NULL;
    int cnt = getSportsDataPresenter()->getDataPage(handle, &page_info);

    uint16_t i = 0;
    uint16_t k = 0;
    for(i = 0; i < data->array_size; i++)
    {
        sport_chart_t *chart = (sport_chart_t *)&chartList[i];
        listWidget_SetItemText(&lw_Item[i],
                                System_getLocaleText(chart->chartName),
                                NULL, false);

        for(k = 0; k < cnt; k++)
        {
            if(page_info[k].pattern == ITEM_PATTERN_CHART && page_info[k].item[0] == chart->chartID)
            {
                listWidget_SetItemIcon(&lw_Item[i], LW_ITEM_CHECK_TEXT, true);
                break;
            }
            else if(page_info[k].pattern == ITEM_PATTERN_TE && page_info[k].item[0] == chart->chartID)
            {
                listWidget_SetItemIcon(&lw_Item[i], LW_ITEM_CHECK_TEXT, true);
                break;
            }
            else
            {
                listWidget_SetItemIcon(&lw_Item[i], LW_ITEM_CHECK_TEXT, false);
            }
        }
    }

    listWidget_SetList(data->lw, lw_Item, data->array_size);
    for(i = 0; i < data->array_size; i++)
    {
        sport_chart_t *chart = (sport_chart_t *)&chartList[i];
        listWidget_SetItemText(&lw_Item[i],
                                System_getLocaleText(chart->chartName),
                                NULL, false);

        for(k = 0; k < cnt; k++)
        {
            if(page_info[k].pattern == ITEM_PATTERN_CHART && page_info[k].item[0] == chart->chartID)
            {
                listWidget_SetItemAttr(data->lw, i, LW_ITEM_ATTR_ITEM_STATE, false);
                break;
            }
            else if(page_info[k].pattern == ITEM_PATTERN_TE && page_info[k].item[0] == chart->chartID)
            {
                listWidget_SetItemAttr(data->lw, i, LW_ITEM_ATTR_ITEM_STATE, false);
                break;
            }
        }
    }

    getSportsDataPresenter()->close(handle);
}

static void _createAppData(ScreenContext *screenContext, void *data)
{
    appData_t *appData = (appData_t *)hm_mem_malloc(sizeof(appData_t));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(appData_t));
        setScreenData(screenContext, appData);
    }
}
static void _getDetailList(ScreenContext *screenContext)
{
    appData_t *appData = getScreenData(screenContext, appData_t);
    const sport_chart_t *list = sportChart_getChartList();
    appData->array = hm_mem_malloc(sizeof(sport_chart_t) * sportChart_getChartListSize());
    appData->array_size = 0;
    for(uint32_t i = 0; i < sportChart_getChartListSize(); i++)
    {
        if(sportChart_isSupport(list[i].chartID, getSportsControlManager()->getType()))
        {
            memcpy(&appData->array[appData->array_size], &list[i], sizeof(sport_chart_t));
            appData->array_size++;
        }
    }
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    _createAppData(screenContext, data);
    _getDetailList(screenContext);
    _createSportlist(screenContext);
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{

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

}

static void onDataSave(ScreenContext *screenContext)
{

}

static void onScreenPostDestory(ScreenContext *screenContext)
{
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
    #define SPORT_ITEM_PAGE_MAX_CNT     (10)

    if (eventId == GESTURE_RIGHT)
    {
        sport_page_handle *handle = getSportsDataPresenter()->open(getSportsControlManager()->getType());
        sport_data_page *page_info = NULL;
        int cnt = getSportsDataPresenter()->getDataPage(handle, &page_info);

        HLOG_D(TAG, "%s %d, cnt : %d, chart cnt : %d, config : 0x%X\r\n", __func__, __LINE__, cnt, getAppData()->cnt, getAppData()->config);
        if(cnt + getAppData()->cnt <= SPORT_ITEM_PAGE_MAX_CNT)
        {
            if(getAppData()->cnt > 0)
            {
                sport_data_page *new_page = hm_mem_malloc(sizeof(sport_data_page) * (cnt + getAppData()->cnt));
                memcpy(new_page, page_info, cnt * sizeof(sport_data_page));
                uint32_t index = cnt;
                sport_chart_t *chartList = (sport_chart_t *)getAppData()->array;

                // check each bit of getAppData()->config to find chart selected
                for(uint8_t i = 0; i < sizeof(getAppData()->config) * 8; i++)
                {
                    if(index >= cnt + getAppData()->cnt)
                    {
                        HLOG_E(TAG, "error, index overflow : %d, expect < %d\r\n", index, cnt + getAppData()->cnt);
                    }
                    else if(getAppData()->config & (1 << i))
                    {
                        switch (chartList[i].chartID)
                        {
                            case SPORT_RECORD_CHART_HR:
                            case SPORT_RECORD_CHART_SPEED:
                            case SPORT_RECORD_CHART_ROW_STROKE_FREQ:
                            case SPORT_RECORD_CHART_SWIM_STROKE_SPEED:
                            case SPORT_RECORD_CHART_PACE:
                            case SPORT_RECORD_CHART_ALTITUDE:
                            case SPORT_RECORD_CHART_SKIP_ROPE_FREQ:
                                new_page[index].pattern = ITEM_PATTERN_CHART;
                                new_page[index].item[0] = chartList[i].chartID;
                                index++;
                                break;
                            case SPORT_RECORD_CHART_TE:
                                new_page[index].pattern = ITEM_PATTERN_TE;
                                new_page[index].item[0] = chartList[i].chartID;
                                index++;
                                break;
                            default:
                                HLOG_I(TAG, "unknown chart id : %D\r\n", chartList[i].chartID);
                                H_ASSERT(0);
                                break;
                        }
                    }
                }

                getSportsDataPresenter()->setDataPage(handle, new_page, cnt + getAppData()->cnt);
                hm_mem_free(new_page);
            }

            // Go back to SportItemEditScreen
            getAppData()->initPage = cnt;
            getScreenManager()->startScreen("SportItemEditScreen", &getAppData()->initPage, sizeof(getAppData()->initPage));
        }
        else
        {
            Toast_show(System_getLocaleText("id_sport_rt_page_add_tips"));
        }

        getSportsDataPresenter()->close(handle);

        return true;
    }
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    ScreenContainer *container = &getAppData()->lw->container;
    getAppData()->lw->container.handleEvent(screenContext, container, id, event);
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    ScreenContainer *container = &getAppData()->lw->container;
    getAppData()->lw->container.handleTouchEvent(screenContext, container, id, eventId, info);
    return false;
}
