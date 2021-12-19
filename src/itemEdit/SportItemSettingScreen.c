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
#include "manager/sports_control_manager.h"
#include "widget/alertDialog/alertDialog.h"
#include <presenter/SportPresenter.h>

#define SCREEN_NAME SportItemSetting

#define ID_SPORT_ITEM_SETTING_SCREEN        "SportItemSettingScreen"
#define ID_ITEM_OPERATE_LIST_LAYER          "SportItemOperateLayer"

#define SETTING_KEY_SORT    "btnSorting"
#define SETTING_KEY_DELETE  "btnDelete"
#define SETTING_KEY_RESTORE "btnRestore"

#define TAG     "itemSet"

SCREEN_DECLARE(SCREEN_NAME);
typedef struct
{
    adapter_data_t *adapter_data;
    gr_application_t *app;
    int yoffset;
    AlertDialog *dialog;
    int32_t initPage;
} appData_t;

static void deleteWarningListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);
static void deleteOperateListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);
static void restoreOperateListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);
static void initOperatePage(ScreenContext *screenContext);

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

    // copy data
    appData->adapter_data = (adapter_data_t *)H_MALLOC(sizeof(adapter_data_t));
    appData->app = screenContext->_grAppContext;

    if(data != NULL)
    {
        memcpy(appData->adapter_data, *((adapter_data_t **)data), sizeof(adapter_data_t));
        initOperatePage(screenContext);
    }
}

static void initOperatePage(ScreenContext *screenContext)
{
    int len = 0;
    char key[64] = {0};
    len = snprintf(key, sizeof(key) - 1, "%s.%s.text", ID_ITEM_OPERATE_LIST_LAYER, SETTING_KEY_SORT);
    key[len] = '\0';
    setSBStringData(screenContext->_grAppContext, key, System_getLocaleText("id_sport_item_btn_sort"));

    len = snprintf(key, sizeof(key) - 1, "%s.%s.text", ID_ITEM_OPERATE_LIST_LAYER, SETTING_KEY_DELETE);
    key[len] = '\0';
    setSBStringData(screenContext->_grAppContext, key, System_getLocaleText("id_sport_item_btn_delete"));

    len = snprintf(key, sizeof(key) - 1, "%s.%s.text", ID_ITEM_OPERATE_LIST_LAYER, SETTING_KEY_RESTORE);
    key[len] = '\0';
    setSBStringData(screenContext->_grAppContext, key, System_getLocaleText("id_sport_item_btn_restore"));
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    memcpy(getAppData()->adapter_data, data, sizeof(adapter_data_t));
    initOperatePage(screenContext);
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

    if (getAppData()->dialog != NULL && getAppData()->dialog->isShowing(getAppData()->dialog))
    {
        getAppData()->dialog->dismissDialog(getAppData()->dialog);
    }

    if(getAppData()->adapter_data)
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

    return true;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    if(eventId == GESTURE_RIGHT)
    {
        getScreenManager()->finishCurrentScreen();
    }
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    HLOG_D(TAG, "%s, id: %s, event: %s\r\n", __func__, id, event);
    return false;
}

static char *searchButton(const char *id, char *name)
{
    return strstr(id, name);
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if (getAppData()->dialog != H_NULL && getAppData()->dialog->isShowing(getAppData()->dialog))
    {
        getAppData()->dialog->container.handleTouchEvent(screenContext, &getAppData()->dialog->container, id, eventId, info);
        return true;
    }

    if (eventId == TOUCH_SELECT)
    {
        adapter_data_t *adapter_data = getAppData()->adapter_data;
        if(searchButton(id, SETTING_KEY_SORT))
        {
            HLOG_D(TAG, "%s, %s pressed\r\n", __func__, id);
            getScreenManager()->startScreen("SportItemSortingScreen", &adapter_data, sizeof(adapter_data));
        }
        else if(searchButton(id, SETTING_KEY_DELETE))
        {
            HLOG_D(TAG, "%s, %s pressed\r\n", __func__, id);
            sport_page_handle *handle = getSportsDataPresenter()->open(adapter_data->sportItemEdit.sportType);
            sport_data_page *page_info = NULL;
            int cnt = getSportsDataPresenter()->getDataPage(handle, &page_info);
            int data_page_cnt = 0;
            bool isDataPage = false;
            for(uint32_t g = 0; g < cnt; g++)
            {
                // rt page count
                if(page_info[g].pattern == ITEM_PATTERN_RT)
                {
                    // count
                    data_page_cnt++;
                }

                // check if current page is data page or not
                if(page_info[g].pattern == ITEM_PATTERN_RT &&
                    adapter_data->page == g &&
                    !isDataPage)
                {
                    isDataPage = true;
                }
            }
            getSportsDataPresenter()->close(handle);

            // 数据页至少保留一个
            // 图表页可以没有
            if((data_page_cnt > 1) || (data_page_cnt == 1 && !isDataPage))
            {
                HLOG_D(TAG, "%s, %s pressed\r\n", __func__, id);
                HLOG_D(TAG, "%s, data_page_cnt : %d, isDataPage : %d\r\n", __func__, data_page_cnt, isDataPage);
                AlertDialog *dialog = creatAlertDialog(screenContext, ID_SPORT_ITEM_SETTING_SCREEN);
                dialog->setTitleAndContent(dialog, NULL, System_getLocaleText("id_sport_item_setting_delete_page"));
                dialog->setImagePath(dialog, "images/sports/dialog/abnormal.png");
                dialog->setPositiveButton(dialog, H_NULL);
                dialog->setNegativeButton(dialog, H_NULL);
                dialog->setClickListener(dialog, deleteOperateListener);
                dialog->showDialog(dialog);
                getAppData()->dialog = dialog;
            }
            else
            {
                AlertDialog *dialog = creatAlertDialog(screenContext, ID_SPORT_ITEM_SETTING_SCREEN);
                dialog->setTitleAndContent(dialog, NULL, System_getLocaleText("id_sport_item_setting_delete_note"));
                dialog->setImagePath(dialog, "images/sports/dialog/abnormal.png");
                dialog->setPositiveButton(dialog, H_NULL);
                dialog->setClickListener(dialog, deleteWarningListener);
                dialog->showDialog(dialog);
                getAppData()->dialog = dialog;
            }
        }
        else if(searchButton(id, SETTING_KEY_RESTORE))
        {
            HLOG_D(TAG, "%s, %s pressed\r\n", __func__, id);

            AlertDialog *dialog = creatAlertDialog(screenContext, ID_SPORT_ITEM_SETTING_SCREEN);
            dialog->setTitleAndContent(dialog, NULL, System_getLocaleText("id_sport_item_setting_restore_page"));
            dialog->setImagePath(dialog, "images/sports/dialog/abnormal.png");
            dialog->setPositiveButton(dialog, H_NULL);
            dialog->setNegativeButton(dialog, H_NULL);
            dialog->setClickListener(dialog, restoreOperateListener);
            dialog->showDialog(dialog);
            getAppData()->dialog = dialog;
        }
        return true;
    }
    return false;
}

static void deleteWarningListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    switch (type)
    {
        case BUTTON_LEFT_CLICK:
            getAppData()->dialog->dismissDialog(getAppData()->dialog);
            break;
        case BUTTON_RIGHT_CLICK:
            getAppData()->dialog->dismissDialog(getAppData()->dialog);
            break;
        default:
            break;
    }
}

static void deleteOperateListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    adapter_data_t *adapter_data = getAppData()->adapter_data;
    sport_page_handle *handle = NULL;
    sport_data_page *page_info = NULL;
    sport_data_page *new_page = NULL;
    uint16_t index = 0;
    int cnt = 0;

    switch (type)
    {
        case BUTTON_LEFT_CLICK:
            // Go back to SportItemEditScreen
            getAppData()->initPage = adapter_data->page;
            getScreenManager()->startScreen("SportItemEditScreen", &getAppData()->initPage, sizeof(getAppData()->initPage));
            break;
        case BUTTON_RIGHT_CLICK:
            index = adapter_data->page;
            handle = getSportsDataPresenter()->open(adapter_data->sportItemEdit.sportType);
            cnt = getSportsDataPresenter()->getDataPage(handle, &page_info);

            new_page = hm_mem_malloc(sizeof(sport_data_page) * (cnt - 1));
            H_ASSERT(new_page);

            // 删除第一个页面
            if(index == 0)
            {
                memcpy(&new_page[0], &page_info[1], sizeof(sport_data_page) * (cnt - 1));
            }
            // 删除最后一个页面
            else if(index + 1 == cnt)
            {
                memcpy(&new_page[0], &page_info[0], sizeof(sport_data_page) * (cnt - 1));
            }
            // 删除中间的某个页面
            else
            {
                memcpy(&new_page[0], &page_info[0], sizeof(sport_data_page) * index);
                memcpy(&new_page[index], &page_info[index + 1], sizeof(sport_data_page) * (cnt - index - 1));
            }

            for(uint32_t i = 0; i < cnt - 1; i++)
            {
                switch(new_page[i].pattern)
                {
                    case ITEM_PATTERN_RT:
                        HLOG_E(TAG, "page[%d] : %s\r\n", i, "ITEM_PATTERN_RT");
                        break;
                    case ITEM_PATTERN_CHART:
                        HLOG_E(TAG, "page[%d] : %s\r\n", i, "ITEM_PATTERN_CHART");
                        break;
                    default:
                        HLOG_E(TAG, "page[%d] : %s\r\n", i, "ITEM_PATTERN_UNKNOWN");
                        break;
                }
            }

            getSportsDataPresenter()->setDataPage(handle, new_page, (cnt - 1));
            getSportsDataPresenter()->close(handle);
            hm_mem_free(new_page);

            // Go back to SportItemEditScreen
            getAppData()->initPage = adapter_data->page;
            getScreenManager()->startScreen("SportItemEditScreen", &getAppData()->initPage, sizeof(getAppData()->initPage));

            break;
        default:
            break;
    }
}

static void restoreOperateListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    sport_page_handle *handle = NULL;
    adapter_data_t *adapter_data = getAppData()->adapter_data;
    switch (type)
    {
        case BUTTON_LEFT_CLICK:
            // Go back to SportItemEditScreen
            getAppData()->initPage = adapter_data->page;
            getScreenManager()->startScreen("SportItemEditScreen", &getAppData()->initPage, sizeof(getAppData()->initPage));
            break;
        case BUTTON_RIGHT_CLICK:
            handle = getSportsDataPresenter()->open(adapter_data->sportItemEdit.sportType);
            getSportsDataPresenter()->resetDataPage(handle);
            getSportsDataPresenter()->close(handle);

            // Go back to SportItemEditScreen
            getScreenManager()->startScreen("SportItemEditScreen", NULL, 0);

            break;
        default:
            break;
    }
}