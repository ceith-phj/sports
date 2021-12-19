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

#define TAG "billy"
#define SCREEN_NAME SportItemEdit

#define ID_PAGESCROLLER_MULTILAYERS     "SportItemEditScreen.pagesContainer"
#define ID_ITEM_SETTING_BAR_LAYER       "SportItemEditScreen.sportItemSettingBarLayer"
#define ID_ITEM_SETTING_SCRLL_LAYER     "sportEditPageScrollLayer"
#define ID_ITEM_SETTING_DATA1_LAYER     "sportItemDataLayer1"
#define ID_ITEM_SETTING_DATA2_LAYER     "sportItemDataLayer2"
#define ID_ITEM_SETTING_DATA3_LAYER     "sportItemDataLayer3"

#define ID_PAGER_VIEW                   "SportItemEditScreen.viewPager"
#define ID_ADAPTER_VIEW                 "SportItemEditScreen.viewPagerAdapter"
#define ID_LISTENER_VIEW                "SportItemEditScreen.listener"

#define ITEM_KEY_PREFIX     "item_"
#define SETTING_KEY_SETTING "iconSetting"

#define RT_EDIT_TIMEOUT_SEC 1000

SCREEN_DECLARE(SCREEN_NAME);
typedef struct
{
    gr_application_t *app;
    pageScrollerMultiLayers *scroller;
    ViewPager *viewPager;
    ViewPagerAdapter *viewPagerAdapter;
    OnPageChangeListener *onPageChangeListener;
    void *timerHandle;
    int32_t initPage;
} appData_t;

static appData_t *getAppData(void);
static ScreenContext *getSportsScreenContext(void);
static void createPageView(ScreenContext *screenContext);
static void initDataContainer(ScreenContext *screenContext);
static void initDataTimer(ScreenContext *screenContext);

static appData_t *getAppData(void)
{
    appData_t *data = getScreenData(getSportsScreenContext(), appData_t);
    return data;
}

static void onPageViewScrolled(pageScrollerMultiLayers *scroll, int page)
{
    HLOG_D(TAG, "%s page index: %d \r\n", __func__, page);
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    appData_t *appData = (appData_t *)hm_mem_malloc(sizeof(appData_t));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(appData_t));
        setScreenData(screenContext, appData);
    }

    if (getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_SUSPEND ||
        getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_RUN)
    {
        System_setKeyFilter(HM_KEY_CODE_HOME, HM_KEY_ACT_FILTER_CLICK | KEY_EVENT_LONG_PRESS);
        System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
    }

    if(!data)
    {
        appData->initPage = 0;
    }
    else
    {
        appData->initPage = *((int32_t *)data);
    }
    appData->app = screenContext->_grAppContext;
    initDataContainer(screenContext);
    createPageView(screenContext);
}

static H_Int32 handleTickEvent(struct gr_timer *timer, void *data, int dropped)
{
    getAppData()->viewPager->invalidate(getAppData()->viewPager);
    return 0;
}

static void initDataTimer(ScreenContext *screenContext)
{
    getAppData()->timerHandle = gre_timer_create_timeout(screenContext->_grAppContext, 100, RT_EDIT_TIMEOUT_SEC, screenContext, handleTickEvent);
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}

static void onPageScrollStateChanged(ViewPager *pager, ViewPagerScrollState state)
{
    // HLOG_I(TAG, "onPageScrollStateChanged: %d\r\n", state);
}

static void onPageScrolled(ViewPager *pager, H_Int16 pageIndex, float positionOffset, H_Int16 positionOffsetPixels)
{
    // HLOG_I(TAG, "onPageScrolled: %d\r\n", pageIndex);
}

static void onPageSelected(ViewPager *pager, H_Int16 pageIndex)
{
    if(pager->pagerCount == pageIndex + 1)
    {
        gre_set_layer_visible(pager->app, ID_ITEM_SETTING_BAR_LAYER, 0);
    }
    else
    {
        gre_set_layer_visible(pager->app, ID_ITEM_SETTING_BAR_LAYER, 1);
    }
    HLOG_I(TAG, "onPageSelected: %d\r\n", pageIndex);
}

static OnPageChangeListener *newOnPageChangeListener(void)
{
    OnPageChangeListener *onPageChangeListener = H_NULL;
    onPageChangeListener = hm_mem_malloc(sizeof(OnPageChangeListener));
    H_ASSERT(onPageChangeListener);
    onPageChangeListener->onPageScrollStateChanged = onPageScrollStateChanged;
    onPageChangeListener->onPageScrolled = onPageScrolled;
    onPageChangeListener->onPageSelected = onPageSelected;
    return onPageChangeListener;
}

static void initDataContainer(ScreenContext *screenContext)
{
    OnPageChangeListener *onPageChangeListener = newOnPageChangeListener();
    H_ASSERT(onPageChangeListener);
    ViewPagerAdapter *viewPagerAdapter = newViewEditPagerAdapter(screenContext);
    H_ASSERT(viewPagerAdapter);
    ViewPager *viewPager = newViewPager(screenContext,
                                        "SportItemEditScreen",
                                        "sportItemDataLayer1",
                                        "sportItemDataLayer2",
                                        "sportItemDataLayer3");
    H_ASSERT(viewPager);
    addContainer(screenContext, ID_PAGER_VIEW, viewPager);

    viewPager->initPage = getAppData()->initPage;
    viewPager->setScrollLayer(viewPager, ID_ITEM_SETTING_SCRLL_LAYER);
    viewPager->setScrollVisiable(viewPager, true);
    viewPager->setAdapter(viewPager, viewPagerAdapter);
    viewPager->setOnPageChangeListener(viewPager, onPageChangeListener);

    getAppData()->viewPager = viewPager;
    getAppData()->viewPagerAdapter = viewPagerAdapter;
    getAppData()->onPageChangeListener = onPageChangeListener;
}

static void createPageView(ScreenContext *screenContext)
{
    pageScrollerMultiLayers *scroll = createPageScrollerMultiLayers(screenContext->_grAppContext);
    if (scroll)
    {
        addContainer(screenContext, ID_PAGESCROLLER_MULTILAYERS, scroll);
        scroll->init(screenContext->_grAppContext, scroll, getDisplayMetrics()->width, 0, 150, onPageViewScrolled);
        struct _pageScrollerPage pages[] =
        {
            {3, {"SportItemEditScreen.sportItemDataLayer1", "SportItemEditScreen.sportItemDataLayer2", "SportItemEditScreen.sportItemDataLayer3"}},
        };

        scroll->initPages(scroll, pages, sizeof(pages) / sizeof(pages[0]));
        scroll->setCycle(scroll, false);
        scroll->sync(scroll, 0);
        getAppData()->scroller = scroll;
    }
}

static ScreenContext *getSportsScreenContext(void)
{
    return getScreenContext(getScreen(SCREEN_NAME));
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    initDataTimer(screenContext);
    if (getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_SUSPEND ||
        getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_RUN)
    {
        HLOG_I(TAG, "%s : %d\r\n", __func__, __LINE__);
        System_setKeyFilter(HM_KEY_CODE_HOME, HM_KEY_ACT_FILTER_CLICK | KEY_EVENT_LONG_PRESS);
        System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
    }
    HLOG_I(TAG, "%s : %d\r\n", __func__, __LINE__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    System_resetKeyFilter(HM_KEY_CODE_HOME);
    System_resetKeyFilter(HM_KEY_CODE_SHORTCUT);
    if(getAppData()->timerHandle != NULL)
    {
        gre_timer_clear_timeout(screenContext->_grAppContext, &(getAppData()->timerHandle));
        getAppData()->timerHandle = H_NULL;
    }
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    if(getAppData()->viewPager)
    {
        getAppData()->viewPager->destroyAllItems(getAppData()->viewPager);
    }

    if(getAppData()->viewPagerAdapter)
    {
        if(getAppData()->viewPagerAdapter->data)
        {
            adapter_data_t *data = (adapter_data_t *)getAppData()->viewPagerAdapter->data;
            if(data)
            {
                hm_mem_free(data->data);
            }
            hm_mem_free(getAppData()->viewPagerAdapter->data);
        }
        hm_mem_free(getAppData()->viewPagerAdapter);
    }
    if(getAppData()->onPageChangeListener)
    {
        hm_mem_free(getAppData()->onPageChangeListener);
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

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    getScreenManager()->finishCurrentScreen();
    return false;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    ScreenContainer *viewpager = (ScreenContainer *)getAppData()->viewPager;
    if (strcmp(event, GR_EVENT_KEY_SPIN) == 0 && viewpager != NULL)
    {
        viewpager->handleEvent(screenContext, viewpager, id, event);
        return false;
    }

    return false;
}

static char *searchButton(const char *id, char *name)
{
    return strstr(id, name);
}

static uint16_t getItemInfo(const char *id)
{
    char keyID[64] = {0};
    strncpy(keyID, id, sizeof(keyID));
    char *ret = searchButton(keyID, ITEM_KEY_PREFIX);
    ret += strlen(ITEM_KEY_PREFIX);
    ret[2] = '\0';
    return atoi(ret);
}

static void handleItemEditButton(ScreenContext *screenContext, const char *id)
{
    ViewPager *viewPager = getAppData()->viewPager;
    uint16_t index = getItemInfo(id);

    adapter_data_t *adapter_data = (adapter_data_t *)getAppData()->viewPagerAdapter->data;
    sport_data_page *page = NULL;
    adapter_data->handle = getSportsDataPresenter()->open(adapter_data->sportItemEdit.sportType);
    getSportsDataPresenter()->getDataPage(adapter_data->handle, &page);
    adapter_data->sportItemEdit.sportItem = page[viewPager->index].item[index];
    adapter_data->page = viewPager->index;
    adapter_data->pos = index;
    getSportsDataPresenter()->close(adapter_data->handle);
    getScreenManager()->startScreen("SportItemListScreen", &adapter_data, sizeof(adapter_data));
}

static int g_dragType = DRAG_NONE;
static int g_pressDown = false;
static SPoint g_pressPos = {.x = 0, .y = 0};

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    ScreenContainer *scroller = (ScreenContainer *)getAppData()->viewPager;
    if (!scroller)
    {
        return false;
    }

    if (eventId == TOUCH_SELECT)
    {
        if(searchButton(id, ITEM_KEY_PREFIX))
        {
            handleItemEditButton(screenContext, id);
        }
        else if(searchButton(id, SETTING_KEY_SETTING))
        {
            ViewPager *viewPager = getAppData()->viewPager;
            adapter_data_t *adapter_data = (adapter_data_t *)getAppData()->viewPagerAdapter->data;
            adapter_data->page = viewPager->index;
            getScreenManager()->startScreen("SportItemSettingScreen", &adapter_data, sizeof(adapter_data));
        }
        else
        {
            getAppData()->viewPager->container.handleTouchEvent(screenContext, scroller, id, eventId, info);
        }
        return true;
    }
    else if (eventId == TOUCH_DOWN)
    {
        g_pressDown = true;
        g_pressPos.x = info->x;
        g_pressPos.y = info->y;
        scroller->handleTouchEvent(screenContext, scroller, id, eventId, info);
    }
    else if (eventId == TOUCH_MOVE)
    {
        if (g_pressDown)
        {
            int deltaY = info->y - g_pressPos.y;

            if (abs(deltaY) > SCROLL_DRAG_THREHOLD)
            {
                g_dragType = DRAG_VER;
            }
        }

        if (g_dragType == DRAG_VER)
        {
            scroller->handleTouchEvent(screenContext, scroller, id, eventId, info);
            return true;
        }
    }
    else if (eventId == TOUCH_UP)
    {
        scroller->handleTouchEvent(screenContext, scroller, id, eventId, info);
        g_pressDown = false;
        if (g_dragType == DRAG_VER)
        {
            g_dragType = DRAG_NONE;
        }
    }
    return false;
}
