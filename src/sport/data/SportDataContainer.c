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

#include <BaseSportView.h>
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sport/data/SportDataContainer.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <sb_screen_context.h>
#include "presenter/sport_data_presenter.h"
#include "presenter/SportPresenter.h"

#define TAG "SportDataContainer"
#define ID_DATA_PAGE_VIEW "SportScreen.SportDataContainer.DataScrollPageLayer"

CONTAINER_FUNCTION_DEFINE();

static void initContainer(SportDataContainer *dataContainer,
                          ScreenContext *screenContext)
{
    dataContainer->timerHandle = H_NULL;
    dataContainer->screenContext = screenContext;
    dataContainer->app = screenContext->_grAppContext;
    dataContainer->initPageView(dataContainer);
    dataContainer->dataPresenter = SportsDataPresenter_new();
    Presenter *basePresenter = SUPER_PTR(dataContainer->dataPresenter, Presenter);
    CALL_FUNCTION(basePresenter, Presenter, init)
    (basePresenter);
}

static void initData(SportDataContainer *sportDataContainer, const char *path,
                     const char *screenName)
{
    memset(sportDataContainer->rootPath, 0, sizeof(sportDataContainer->rootPath));
    strncpy(sportDataContainer->rootPath, path,
            sizeof(sportDataContainer->rootPath));
}

static void initPageView(SportDataContainer *sportDataContainer)
{
    sportDataContainer->dataPageView = newViewPager(
                                           sportDataContainer->screenContext, "SportScreen", "sportDataLayer1",
                                           "sportDataLayer2", "sportDataLayer3");
    sportDataContainer->dataPageView->setScrollLayer(
        sportDataContainer->dataPageView, "sportPageScrollLayer");
    sportDataContainer->dataPageView->setAdapter(sportDataContainer->dataPageView,
            sportDataContainer->dataViewPagerAdapter);
    sportDataContainer->dataPageView->setOnPageChangeListener(
        sportDataContainer->dataPageView, sportDataContainer->onPageChangeListener);
    sportDataContainer->dataPageView->view->setTag(
        sportDataContainer->dataPageView->view, TAG, sportDataContainer);
}

static void secTickEvent(SportDataContainer *container)
{
    if (container->dataPageView != H_NULL)
    {
        container->dataPageView->invalidate(container->dataPageView);
    }
}

static void sportStatus(SportDataContainer *dataView, void *event)
{
    if (dataView->dataPageView != H_NULL)
    {
        dataView->dataPageView->setEvent(dataView->dataPageView, event);
    }
}

static void onPageScrollStateChanged(ViewPager *pager,
                                     ViewPagerScrollState state)
{
    HLOG_I(TAG, "onPageScrollStateChanged: %d\r\n", state);
}
static void onPageScrolled(ViewPager *pager, H_Int16 position,
                           float positionOffset, H_Int16 positionOffsetPixels)
{
    HLOG_I(TAG, "onPageScrolled: %d\r\n", position);
    sport_data *sportData = getSportsDataPresenter()->getSportData(
                                getSportsControlManager()->getType());
    sport_data_page *pageData = (sport_data_page *)&sportData->page[position];

    if (pageData->pattern == ITEM_PATTERN_GOAL)
    {
        #ifdef HM_DISPLAY_IS_ROUND
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_HIDE_STATUS_BAR);
        #else
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
        #endif
    }
    else
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
    }
}

static void onPageSelected(ViewPager *pager, H_Int16 position)
{
    HLOG_I(TAG, "onPageSelected: %d\r\n", position);
}

static void setVisiable(SportDataContainer *dataContainer, H_Int8 isVisiable)
{
}

static bool getVisiable(SportDataContainer *dataContainer)
{
    return true;
}

static OnPageChangeListener *newOnPageChangeListener()
{
    OnPageChangeListener *onPageChangeListener = H_NULL;
    onPageChangeListener = H_MALLOC(sizeof(OnPageChangeListener));
    H_ASSERT(onPageChangeListener);
    onPageChangeListener->onPageScrollStateChanged = onPageScrollStateChanged;
    onPageChangeListener->onPageScrolled = onPageScrolled;
    onPageChangeListener->onPageSelected = onPageSelected;
    return onPageChangeListener;
}

SportDataContainer *bindSportDataView(ScreenContext *screenContext)
{
    SportDataContainer *sportDataContainer = H_NULL;

    if (!sportDataContainer)
    {
        createContainer(sportDataContainer, SportDataContainer,
                        sportDataContainer->container);
        sportDataContainer->initContainer = initContainer;
        sportDataContainer->initData = initData;
        sportDataContainer->initPageView = initPageView;
        sportDataContainer->app = screenContext->_grAppContext;
        sportDataContainer->sportStatus = sportStatus;
        sportDataContainer->dataViewPagerAdapter = newViewPagerAdapter(screenContext);
        sportDataContainer->onPageChangeListener = newOnPageChangeListener();
        sportDataContainer->secTickEvent = secTickEvent;
        sportDataContainer->setVisiable = setVisiable;
        sportDataContainer->getVisiable = getVisiable;
    }

    return sportDataContainer;
}

static int g_dragType = DRAG_NONE;
static int g_pressDown = false;
static SPoint g_pressPos = {.x = 0, .y = 0};

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    SportDataContainer *sportDataContainer = (SportDataContainer *)container;
    //HLOG_D("SportDataContainer", "%s id:%s\r\n", __func__, id);
    ScreenContainer *viewpager = (ScreenContainer *)
                                 sportDataContainer->dataPageView;

    if (!viewpager)
    {
        return false;
    }

    if (eventId == TOUCH_DOWN)
    {
        g_pressDown = true;
        g_pressPos.x = info->x;
        g_pressPos.y = info->y;
        viewpager->handleTouchEvent(screenContext, viewpager, id, eventId, info);
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
            viewpager->handleTouchEvent(screenContext, viewpager, id, eventId, info);
            return true;
        }
    }
    else if (eventId == TOUCH_UP)
    {
        viewpager->handleTouchEvent(screenContext, viewpager, id, eventId, info);
        g_pressDown = false;

        if (g_dragType == DRAG_VER)
        {
            g_dragType = DRAG_NONE;
        }
    }
    else if (eventId == TOUCH_SELECT)
    {
        viewpager->handleTouchEvent(screenContext, viewpager, id, eventId, info);
    }

    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container, const HmKeyEvent *info)
{
    SportDataContainer *sportDataContainer = (SportDataContainer *)container;
    ScreenContainer *viewpager = (ScreenContainer *)
                                 sportDataContainer->dataPageView;

    if (!viewpager)
    {
        return false;
    }

    if (info->code == HM_KEY_CODE_SELECT && info->action == HM_KEY_ACTION_CLICK)
    {
        /* 暂停继续开始 */
        switch (getSportsScreenPresenter()->requestSportStatus())
        {
            case SPORT_STATE_RUN:
                getSportsScreenPresenter()->requestPause();
                return true;

            case SPORT_STATE_SUSPEND:
                getSportsScreenPresenter()->requestContinue();
                return true;

            case SPORT_STATE_PRESTART:
                getSportsScreenPresenter()->requestStart();
                return true;

            default:
                return false;
        }
    }

    if (info->code == HM_KEY_CODE_HOME && info->action == HM_KEY_ACTION_CLICK)
    {
        /* 暂停继续 */
        switch (getSportsScreenPresenter()->requestSportStatus())
        {
            case SPORT_STATE_RUN:
                getSportsScreenPresenter()->requestPause();
                return true;

            case SPORT_STATE_SUSPEND:
                getSportsScreenPresenter()->requestContinue();
                return true;

            default:
                return false;
        }
    }

    if (info->code == HM_KEY_CODE_BACK && info->action == HM_KEY_ACTION_CLICK)
    {
        /* 分组分段 */
        getSportsScreenPresenter()->requestLap();
        return true;
    }

    if (info->code == HM_KEY_CODE_SHORTCUT && info->action == HM_KEY_ACTION_CLICK)
    {
        if (getSportsScreenPresenter()->isMannulLapSupported(
                    getSportsScreenPresenter()->getSportType()) ||
                getSportsScreenPresenter()->isGroupSupported(
                    getSportsScreenPresenter()->getSportType()))
        {
            getSportsScreenPresenter()->requestLap();
        }
        else
        {
            viewpager->handleKeyEvent(screenContext, viewpager, info);
        }

        return true;
    }

    if (info->code == HM_KEY_CODE_UP && info->action == HM_KEY_ACTION_CLICK)
    {
        /* TODO 向上翻页 */
        return true;
    }

    if (info->code == HM_KEY_CODE_DOWN && info->action == HM_KEY_ACTION_CLICK)
    {
        /* TODO 向下翻页 */
        return true;
    }

    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext,
                               ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext,
                        ScreenContainer *container, const char *id, const char *event)
{
    SportDataContainer *sportDataContainer = (SportDataContainer *)container;
    if (NULL == sportDataContainer)
    {
        return false;
    }
    ScreenContainer *viewpager = (ScreenContainer *)
                                 sportDataContainer->dataPageView;

    if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        viewpager->handleEvent(screenContext, viewpager, id,
                               event);
        return false;
    }

    return false;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportDataContainer *sportDataContainer = (SportDataContainer *)container;

    if (sportDataContainer->dataPresenter != H_NULL)
    {
        SportsDataPresenter_delete(sportDataContainer->dataPresenter);
        sportDataContainer->dataPresenter = H_NULL;
    }

    if (sportDataContainer->dataPageView)
    {
        ScreenContainer *subContainer = (ScreenContainer *)(sportDataContainer->dataPageView);
        ViewPager *viewPager = sportDataContainer->dataPageView;

        // destroy items on 3 pages, such as dataPage, goal page, rabbit and etc.
        viewPager->destroyAllItems(viewPager);

        // destroy viewPager : timer, screenLayerId, scrollLayerId, view
        subContainer->destroy(screenContext, subContainer);

        // Free viewPager
        H_FREE(viewPager);
    }

    if (sportDataContainer->dataViewPagerAdapter)
    {
        H_FREE(sportDataContainer->dataViewPagerAdapter);
    }

    if (sportDataContainer->onPageChangeListener)
    {
        H_FREE(sportDataContainer->onPageChangeListener);
    }
}