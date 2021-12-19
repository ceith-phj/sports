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
#include "packages/apps/sports/include/manager/sports_item_type.h"
#include <ui/Sports/include/itemEdit/sport_item_utils.h>
#include <widget/list/radio_list_widget.h>
#include "ui/Sports/include/sport/data/SportDataEditViewAdapter.h"
#include "packages/apps/sports/include/manager/sports_control_manager.h"
#include <presenter/SportPresenter.h>

#define TAG         "SportSubItemlistScreen"
#define SCREEN_NAME SportSubItemlist

#define ID_SPORTLIST_LIST           "SportSubItemListLayer.sportlist"
#define SPORTLIST_SCREEN_LAYER      "SportSubItemlistScreen.SportSubItemListLayer"
#define PATH_SPORTLIST_ARCSCROLLBAR "SportSubItemlistScrollLayer.arcScrollBar"
#define ID_SPORTLIST_SCROLLBAR      "SportSubItemListLayer.arcScrollBar"

#define ID_SUBLIST_ITEM_SCREEN      "SportSubItemlistScreen"
#define ID_SUBLIST_ITEM_LAYER       "SportSubItemListLayer"
#define ID_SUBLIST_ITEM             "item"

SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    int *array;
    int array_size;
    radio_list_widget *rlw;
    adapter_data_t *adapter_data;
    int32_t initPage;
    lw_item *item;
} appData_t;

static void _rlwEvent(radio_list_widget *rlw, lw_event_type event, int index,
                      int lastIndex);

static appData_t *getAppData(void)
{
    return getScreenData(getScreenContext(getScreen(SCREEN_NAME)), appData_t);
}

static void _createSportlist(ScreenContext *screenContext)
{
    appData_t *data = getAppData();
    lw_item *itemList = H_MALLOC(sizeof(lw_item) * data->array_size);
    H_ASSERT(itemList);
    data->item = itemList;
    int selected = -1;
    sport_item_edit_t *item_edit = &data->adapter_data->sportItemEdit;
    SportItemGroup *groupList = (SportItemGroup *)sportItem_getGroupList(
                                    item_edit->itemGroup);
    data->rlw = radioList_Create(ID_SUBLIST_ITEM_SCREEN, ID_SUBLIST_ITEM_LAYER,
                                 _rlwEvent);
    addContainer(screenContext, "sportItemSublist", data->rlw);
    radioList_SetListAttr(data->rlw, LW_ATTR_TITLE,
                          (void *)System_getLocaleText(groupList->groupName));
    HLOG_D(TAG, "SportSubItemlistScreen : %s\r\n",
           System_getLocaleText(groupList->groupName));

    for (uint16_t i = 0; i < data->array_size; i++)
    {
        sport_item_type_t rt = (sport_item_type_t)data->array[i];
        sport_item_t *item = (sport_item_t *)sportItem_getSubList(rt);
        radioList_SetItem(&itemList[i], System_getLocaleText(item->itemName), NULL,
                          false);

        if (rt == data->adapter_data->sportItemEdit.sportItem)
        {
            selected = i;
        }
    }

    radioList_SetList(data->rlw, itemList, data->array_size, selected);
}

static void _rlwEvent(radio_list_widget *rlw, lw_event_type event, int index,
                      int lastIndex)
{
    if (event == LW_EVENT_TEXT)
    {
        return;
    }

    // ##
    adapter_data_t *adapter_data = getAppData()->adapter_data;
    sport_data_page *page_info = NULL;
    adapter_data->handle = getSportsDataPresenter()->open(
                               adapter_data->sportItemEdit.sportType);
    int cnt = getSportsDataPresenter()->getDataPage(adapter_data->handle,
              &page_info);
    sport_data_page *new_page = hm_mem_malloc((cnt + 1) * sizeof(sport_data_page));
    memcpy(new_page, page_info, cnt * sizeof(sport_data_page));
    adapter_data->sportItemEdit.sportItem = (sport_item_type_t)
                                            getAppData()->array[index];
    new_page[adapter_data->page].item[adapter_data->pos] =
        adapter_data->sportItemEdit.sportItem;
    getSportsDataPresenter()->setDataPage(adapter_data->handle, new_page, cnt);
    getSportsDataPresenter()->close(adapter_data->handle);
    hm_mem_free(new_page);
    // ##
    getAppData()->initPage = adapter_data->page;
    getScreenManager()->startScreen("SportItemEditScreen", &getAppData()->initPage,
                                    sizeof(getAppData()->initPage));
}

static void _createAppData(ScreenContext *screenContext, void *data)
{
    appData_t *appData = (appData_t *)H_MALLOC(sizeof(appData_t));

    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(appData_t));
        setScreenData(screenContext, appData);
    }

    appData->adapter_data = (adapter_data_t *)H_MALLOC(sizeof(adapter_data_t));

    if (data)
    {
        // copy data
        memcpy(appData->adapter_data, *((adapter_data_t **)data),
               sizeof(adapter_data_t));
    }
}
static void _getDetailList(ScreenContext *screenContext)
{
    appData_t *appData = getAppData();
    sport_item_edit_t *item_edit = &appData->adapter_data->sportItemEdit;
    appData->array_size = ItemDL[item_edit->itemGroup].size;
    appData->array = hm_mem_malloc(appData->array_size * sizeof(int));
    uint32_t size = 0;

    for (uint32_t i = 0; i < appData->array_size; i++)
    {
        sport_item_type_t item = ItemDL[item_edit->itemGroup].table[i];

        if (sportItem_isSupport(item, getSportsControlManager()->getType()))
        {
            appData->array[size] = item;
            size++;
        }
    }

    appData->array_size = size;
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _createAppData(screenContext, data);

    if (data)
    {
        _getDetailList(screenContext);
        _createSportlist(screenContext);
    }
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
    HLOG_D(TAG, "%s\r\n", __func__);
    memcpy(getAppData()->adapter_data, (adapter_data_t *)data,
           sizeof(adapter_data_t));
    _getDetailList(screenContext);
    _createSportlist(screenContext);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    getScreenManager()->saveScreenData(getScreen(SCREEN_NAME)->id, getAppData()->adapter_data,
                                       sizeof(adapter_data_t));
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);

    if (getAppData()->item)
    {
        hm_mem_free(getAppData()->item);
    }

    if (getAppData()->array)
    {
        hm_mem_free(getAppData()->array);
    }

    if (getAppData()->adapter_data)
    {
        hm_mem_free(getAppData()->adapter_data);
    }

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

static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);

    if (eventId == GESTURE_RIGHT)
    {
        adapter_data_t *adapter_data = getAppData()->adapter_data;
        getAppData()->initPage = adapter_data->page;
        getScreenManager()->startScreen("SportItemEditScreen", &getAppData()->initPage,
                                        sizeof(getAppData()->initPage));
        return true;
    }

    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    ScreenContainer *container = &getAppData()->rlw->container;
    getAppData()->rlw->container.handleEvent(screenContext, container, id, event);
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    ScreenContainer *container = &getAppData()->rlw->container;
    getAppData()->rlw->container.handleTouchEvent(screenContext, container, id,
            eventId, info);
    return false;
}

