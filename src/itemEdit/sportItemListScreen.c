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
#include <hm_toast.h>
#include <system_locale.h>
#include "sports_list_manager.h"
#include "packages/apps/sports/include/utils/sports_list_page_data.h"
#include "packages/apps/sports/include/manager/sports_item_type.h"
#include <ui/Sports/include/itemEdit/sport_chart_utils.h>
#include <ui/Sports/include/itemEdit/sport_item_utils.h>
#include "ui/Sports/include/sport/data/SportDataEditViewAdapter.h"
#include "packages/apps/sports/include/manager/sports_control_manager.h"
#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif
#include <presenter/SportPresenter.h>

#define TAG "SportItemListScreen"

#define SCREEN_NAME                     SportItemList
#define TABLE_LIST                      "ListLayer.ListTable"
#define ID_SPORTLIST_LIST               "SportItemListLayer.sportItemList"
#define SPORTCATEGORYLIST_SCREEN_LAYER  "SportItemListScreen.SportItemListLayer"
#define PATH_SPORTLIST_ARCSCROLLBAR     "sportItemListScrollLayer.scroller"
#define ID_SPORTLIST_SCROLLBAR          "SportItemListLayer.scroller"

SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    sport_item_descrip_t *list;
    uint16_t list_size;
    int yoffset;
    adapter_data_t *adapter_data;
    uint32_t *info;
} appData_t;

static appData_t *getAppData(void)
{
    return getScreenData(getScreenContext(getScreen(SCREEN_NAME)), appData_t);
}

static int _createSportListScrollBar(gr_application_t *app)
{
    int viewHei = gre_get_layer_h(app, SPORTCATEGORYLIST_SCREEN_LAYER);
    int totalHei = 0;
    arcScrollBar *scroll = createArcScrollBarContainer(app);

    if (scroll)
    {
        scroll->init(app, scroll, PATH_SPORTLIST_ARCSCROLLBAR);
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORTLIST_SCROLLBAR,
                     scroll);
        scroll->setArcRange(scroll, -20, 20);
        unequalHeiList *list = (unequalHeiList *)findContainer(getScreenContext(
                                   getScreen(SCREEN_NAME)), ID_SPORTLIST_LIST);

        if (list)
        {
            totalHei = list->getTotalHei(list);
        }

        scroll->setPosRange(scroll, totalHei, viewHei);
        int offset = gre_get_layer_yoffset(app, SPORTCATEGORYLIST_SCREEN_LAYER);
        scroll->setPos(scroll, offset);
    }

    return 0;
}

static void sportList_onItemUpdate(unequalHeiList *list, int idx,
                                   const char *itemGroupPath)
{
    if (NULL == list || idx < 0 || NULL == itemGroupPath)
    {
        return;
    }

    char key[256] = {0};
#ifdef HM_DISPLAY_IS_ROUND

    if (idx == 0)
    {
        snprintf(key, sizeof(key), "%s.item.text", itemGroupPath);
        setSBStringData(getAppInstance(), key,
                        System_getLocaleText("id_sport_item_page_data"));
    }
    else
    {
        idx -= 1;
        snprintf(key, sizeof(key), "%s.item.title", itemGroupPath);
        setSBStringData(getAppInstance(), key,
                        sportItem_getGroupNameChar(getAppData()->list[idx].group));
        HLOG_D(TAG, "group list-%d name : %s", idx,
               sportItem_getGroupNameChar(getAppData()->list[idx].group));
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.item.subtitle", itemGroupPath);
        char value[100] = {0};
        snprintf(value, sizeof(value), System_getLocaleText("id_sport_item_number"),
                 getAppData()->info[idx]);
        setSBStringData(getAppInstance(), key, value);
    }

#else
    snprintf(key, sizeof(key), "%s.item.title", itemGroupPath);
    setSBStringData(getAppInstance(), key,
                    sportItem_getGroupNameChar(getAppData()->list[idx].group));
    HLOG_D(TAG, "group list-%d name : %s", idx,
           sportItem_getGroupNameChar(getAppData()->list[idx].group));
    memset(key, 0, sizeof(key));
    snprintf(key, sizeof(key), "%s.item.subtitle", itemGroupPath);
    char value[100] = {0};
    snprintf(value, sizeof(value), System_getLocaleText("id_sport_item_number"),
             getAppData()->info[idx]);
    setSBStringData(getAppInstance(), key, value);
#endif
}

static void sportList_onItemTouch(unequalHeiList *list, int idx,
                                  const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    HLOG_D(TAG, "%s idx=%d", __func__, idx);
#ifdef HM_DISPLAY_IS_ROUND
    --idx;
#endif

    if (NULL == list || idx < 0 || idx >= getAppData()->list_size)
    {
        return;
    }

    if (eventId != TOUCH_SELECT)
    {
        return;
    }

    adapter_data_t *adapter_data = (adapter_data_t *)getAppData()->adapter_data;
    if (adapter_data)
    {
        adapter_data->sportItemEdit.itemGroup = getAppData()->list[idx].group;
        getScreenManager()->startScreen("SportSubItemlistScreen", &adapter_data,
                                        sizeof(adapter_data));
    }
}

static void _createSportlist(ScreenContext *screenContext)
{
    unequalHeiList *sportlist = createUnequalHeiListContainer(
                                    screenContext->_grAppContext);

    if (sportlist)
    {
        uint32_t listSize = 0;
#ifdef HM_DISPLAY_IS_ROUND
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
        {
            "SportListItemStyleLayer.RichContent",
            "SportListItemStyleLayer.Title"
        };
        listSize = getAppData()->list_size + 1;
#else
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
        {
            "SportListItemStyleLayer.RichContent"
        };
        listSize = getAppData()->list_size;
#endif
        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);
        sportlist->create(screenContext->_grAppContext, sportlist,
                          "SportItemListScreen", "SportItemListLayer",
                          listSize, itemTemps, tempCnt, sportList_onItemUpdate, sportList_onItemTouch,
                          "item");
        addContainer(screenContext, ID_SPORTLIST_LIST, sportlist);
        unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext,
                                    ID_SPORTLIST_LIST);

        if (sportList)
        {
            sportList->setItemTemp(sportList, 0, 0, 1);
            sportList->setLastEmptySpace(sportList, 90);
            sportList->initItems(sportList);
        }
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

    appData->adapter_data = (adapter_data_t *)H_MALLOC(sizeof(adapter_data_t));

    if (data)
    {
        // copy data
        memcpy(appData->adapter_data, *((adapter_data_t **)data),
               sizeof(adapter_data_t));
    }
}

static void _getGroupList(ScreenContext *screenContext)
{
    appData_t *data = getAppData();
    data->list_size = sizeof(ItemDL) / sizeof(ItemDL[0]);
    data->list = hm_mem_malloc(data->list_size * sizeof(sport_item_descrip_t));
    data->info = hm_mem_malloc(data->list_size * sizeof(data->info));
    uint32_t size = 0;

    for (uint32_t i = 0; i < data->list_size; i++)
    {
        const sport_item_descrip_t *info = &ItemDL[i];

        if (sportItem_isGroupSupport(info->group, getSportsControlManager()->getType()))
        {
            memcpy(&data->list[size], info, sizeof(sport_item_descrip_t));

            for (uint32_t k = 0; k < info->size; k++)
            {
                if (sportItem_isSupport(info->table[k], getSportsControlManager()->getType()))
                {
                    data->info[size]++;
                }
            }

            size++;
        }
    }

    data->list_size = size;
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _createAppData(screenContext, data);

    if (data)
    {
        _getGroupList(screenContext);
        _createSportlist(screenContext);
        _createSportListScrollBar(getScreenContext(getScreen(
                                      SCREEN_NAME))->_grAppContext);
    }

#ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_updateTitle(System_getLocaleText("id_sport_item_page_data"));
#endif
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}

// static void _scrollYoffset(ScreenContext *screenContext)
// {
//     if (getAppData()->yoffset == 0)
//     {
//         return;
//     }

//     gre_set_layer_yoffset(screenContext->_grAppContext,
//                           SPORTCATEGORYLIST_SCREEN_LAYER, getAppData()->yoffset);
//     arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext,
//                            ID_SPORTLIST_SCROLLBAR);

//     if (scroll)
//     {
//         int offset = gre_get_layer_yoffset(screenContext->_grAppContext,
//                                            SPORTCATEGORYLIST_SCREEN_LAYER);
//         scroll->setPos(scroll, offset);
//     }

//     unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext,
//                                 ID_SPORTLIST_LIST);

//     if (sportList != H_NULL)
//     {
//         sportList->updateBotPaddingY(sportList);
//         sportList->updateScrolling(sportList);
//     }
// }

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

// static void _restoreYoffset(ScreenContext *screenContext, void *data)
// {
//     if (data != H_NULL)
//     {
//         appData_t *restoreData = (appData_t *)data;
//         getAppData()->yoffset = restoreData->yoffset;
//     }
// }

// static void _saveYoffset(ScreenContext *screenContext)
// {
//     appData_t *saveData = getScreenData(screenContext, appData_t);

//     if (saveData != H_NULL)
//     {
//         int yoffset = gre_get_layer_yoffset(screenContext->_grAppContext,
//                                             SPORTCATEGORYLIST_SCREEN_LAYER);
//         saveData->yoffset = yoffset;
//         getScreenManager()->saveScreenData(getScreen(SCREEN_NAME)->id, saveData,
//                                            sizeof(appData_t));
//     }
// }

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    // _restoreYoffset(screenContext, data);
    // if (data != H_NULL)
    // {
    //     _scrollYoffset(screenContext);
    // }
    memcpy(getAppData()->adapter_data, (adapter_data_t *)data,
           sizeof(adapter_data_t));
    _getGroupList(screenContext);
    _createSportlist(screenContext);
    _createSportListScrollBar(screenContext->_grAppContext);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    // _saveYoffset(screenContext);
    getScreenManager()->saveScreenData(getScreen(SCREEN_NAME)->id, getAppData()->adapter_data,
                                       sizeof(adapter_data_t));
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    hm_mem_free(getAppData()->info);
    hm_mem_free(getAppData()->list);

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
        getScreenManager()->finishCurrentScreen();
    }

    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    if (!strcmp(event, "scroll_change"))
    {
        arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext,
                               ID_SPORTLIST_SCROLLBAR);

        if (scroll)
        {
            int offset = gre_get_layer_yoffset(screenContext->_grAppContext,
                                               SPORTCATEGORYLIST_SCREEN_LAYER);
            scroll->setPos(scroll, offset);
        }
    }
    else if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext,
                                    ID_SPORTLIST_LIST);

        if (sportList)
        {
            sportList->container.handleEvent(screenContext, &sportList->container, id,
                                             event);
        }
    }

    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    ScreenContainer *list = findContainer(screenContext, ID_SPORTLIST_LIST);

    if (list)
    {
        list->handleTouchEvent(screenContext, list, id, eventId, info);
        return true;
    }

    return false;
}
