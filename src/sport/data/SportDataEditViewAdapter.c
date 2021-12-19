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

#include <sport/data/SportDataViewAdapter.h>
#include <sport/data/pages/SportDataPageItem.h>
#include <sport/data/pages/SportDataPageChart.h>
#include <sport/data/pages/SportDataRabbitPage.h>
#include <sport/data/pages/SportDataCadenceAssistantPage.h>
#include <sport/data/pages/SportGoalPage.h>
#include "presenter/sport_data_presenter.h"
#include "manager/sports_control_manager.h"
#include <widget/sportChart/sportChart.h>
#include "ui/Sports/include/sport/data/SportDataEditViewAdapter.h"
#include "ui/Sports/include/sport/data/pages/SportTrainingEffe.h"

#define TAG "sportDataEdit"

static void init(ViewPagerAdapter *adapter, ScreenContext *screenContext)
{
    adapter->screenContext = screenContext;
    adapter->app = screenContext->_grAppContext;
    adapter_data_t *info = hm_mem_malloc(sizeof(adapter_data_t));
    H_ASSERT(info);
    adapter->data = info;
    adapter->isStatic = false;

    // ##
    info->handle = getSportsDataPresenter()->open(getSportsControlManager()->getType());
    H_ASSERT(info->handle);
    sport_data_page *page_info = NULL;
    info->page_cnt = getSportsDataPresenter()->getDataPage(info->handle, &page_info);
    H_ASSERT(info->page_cnt);

    for(uint8_t i = 0; i < info->page_cnt; i++)
    {
        switch (page_info[i].pattern)
        {
            case ITEM_PATTERN_RT:
                HLOG_I("billy", "item page list-%d : ITEM_PATTERN_RT\r\n", i);
                break;
            case ITEM_PATTERN_CHART:
                HLOG_I("billy", "item page list-%d : ITEM_PATTERN_CHART, %d\r\n", i, page_info[i].item[0]);
                break;
            case ITEM_PATTERN_TE:
                HLOG_I("billy", "item page list-%d : ITEM_PATTERN_TE\r\n", i);
                break;
            default:
                HLOG_I("billy", "item page list-%d : unknown\r\n", i);
                break;
        }
    }

    // ##
    info->data = hm_mem_malloc(sizeof(sport_data_page) * (info->page_cnt + 1));
    H_ASSERT(info->data);
    sport_data_page *page = (sport_data_page *)info->data;
    memcpy(page, page_info, sizeof(sport_data_page) * info->page_cnt);
    page[info->page_cnt].pattern = ITEM_PATTERN_EMPTY;
    info->sportItemEdit.sportType = getSportsControlManager()->getType();

    // ##
    getSportsDataPresenter()->close(info->handle);
}

static H_Int16 getViewPagerCount(ViewPagerAdapter *adapter)
{
    return ((adapter_data_t *)(adapter->data))->page_cnt + 1;
}

static ScreenContainer *instantiateItem(ViewPagerAdapter *adapter, char *layerId, H_Int16 page)
{
    adapter_data_t *adapter_data = (adapter_data_t *)adapter->data;
    sport_data_page *pageInfo = (sport_data_page *)adapter_data->data;
    sport_data_page *pageData = (sport_data_page *)&pageInfo[page];
    char key[48] = {0};
    char value[48] = {0};
    char hint[48] = {0};
    snprintf(key, sizeof(key), "%s.item.text", layerId);
    snprintf(value, sizeof(value), "data layer %d", page);
    setSBStringData(adapter->app, key, value);
    switch (pageData->pattern)
    {
        case ITEM_PATTERN_RT:
        {
            SportDataPageItem *rtItem = newDataEditPageItemView(adapter->screenContext, layerId, pageData, true);
            snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "rtItem", page);
            addContainer(adapter->screenContext, hint, rtItem);
            return (ScreenContainer *)rtItem;
        }
        case ITEM_PATTERN_CHART:
        {
            SportDataPageChart *chartItem = newSportDataPageChartView(adapter->screenContext, layerId, pageData);
            hm_snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "chartItem", page);
            addContainer(adapter->screenContext, hint, chartItem);
            return (ScreenContainer *)chartItem;
        }
        case ITEM_PATTERN_GOAL:
        {
            SportGoalPage *goalItem = newSportGoalView(adapter->screenContext, layerId);
            hm_snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "goalItem", page);
            addContainer(adapter->screenContext, hint, goalItem);
            return (ScreenContainer *)goalItem;
        }
        case ITEM_PATTERN_RABBITE:
        {
            SportRabbiteContainer *rabbitItem = newSportRabbiteContainer(adapter->screenContext, layerId);
            hm_snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "rabbitItem", page);
            addContainer(adapter->screenContext, hint, rabbitItem);
            return (ScreenContainer *)rabbitItem;
        }
        case ITEM_PATTERN_CADENCE_ASSISTANT:
        {
            CadenceAssistantContainer *assistantItem = newCadenceAssistantContainer(adapter->screenContext, layerId);
            hm_snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "assistantItem", page);
            addContainer(adapter->screenContext, hint, assistantItem);
            return (ScreenContainer *)assistantItem;
        }
        case ITEM_PATTERN_TE:
        {
            TrainingEffeContainer * effetcItem = newTrainingEffeContainer(adapter->screenContext, layerId);
            hm_snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "effetcItem", page);
            addContainer(adapter->screenContext, hint, effetcItem);
            return (ScreenContainer *)effetcItem;
        }
        case ITEM_PATTERN_EMPTY:
        {
            SportDataPageEmpty *emptyItem = newSportEmptyView(adapter->screenContext, layerId);
            hm_snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "emptyItem", page);
            addContainer(adapter->screenContext, hint, emptyItem);
            return (ScreenContainer *)emptyItem;
        }
        default:
            break;
    }
    return H_NULL;
}

static void destroyItem(ViewPagerAdapter *adapter, ScreenContainer *container, H_Int16 page)
{
    if (container)
    {
        container->destroy(adapter->screenContext, container);
    }
}

// static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
// {
//     return false;
// }

// static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container, const HmKeyEvent *info)
// {
//     return false;
// }

// static bool handleGestureEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBGestureEvent eventId)
// {
//     return false;
// }

// static bool handleEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const char *event)
// {
//     return false;
// }

// static void destroy(ScreenContext *screenContext, ScreenContainer *container)
// {
//     ViewPagerAdapter *adapter = (ViewPagerAdapter *)container;
//     adapter_data_t *info = adapter->data;
//     H_FREE(info->data);
// }

static void update(ViewPagerAdapter *adapter, ScreenContainer *container, H_Int16 page)
{
    adapter_data_t *adapter_data = (adapter_data_t *)adapter->data;
    sport_data_page *pageInfo = (sport_data_page *)adapter_data->data;
    sport_data_page *pageData = (sport_data_page *)&pageInfo[page];
    switch (pageData->pattern)
    {
        case ITEM_PATTERN_CHART:
        {
            SportDataPageChart *item = (SportDataPageChart *)container;
            item->update(item);
            break;
        }
        case ITEM_PATTERN_RT:
        {
            SportDataPageItem *item = (SportDataPageItem *)container;
            item->update(item);
            break;
        }
        case ITEM_PATTERN_GOAL:
        {
            SportGoalPage *item = (SportGoalPage *)container;
            item->update(item);
            break;
        }
        case ITEM_PATTERN_RABBITE:
        {
            SportRabbiteContainer *item = (SportRabbiteContainer *)container;
            item->update(item);
            break;
        }
        case ITEM_PATTERN_CADENCE_ASSISTANT:
        {
            CadenceAssistantContainer *item = (CadenceAssistantContainer *)container;
            item->update(item);
            break;
        }
        case ITEM_PATTERN_TE:
        {
            TrainingEffeContainer *item = (TrainingEffeContainer *)container;
            item->update(item);
            break;
        }
        default:
            break;
    }
}

static void clickItemHandle(ViewPagerAdapter *adapter, ScreenContainer *container, H_Int16 page, H_Int16 index)
{
}

ViewPagerAdapter *newViewEditPagerAdapter(ScreenContext *context)
{
    ViewPagerAdapter *adapter = H_NULL;
    adapter = H_MALLOC(sizeof(ViewPagerAdapter));
    H_ASSERT(adapter);
    adapter->getViewPagerCount = getViewPagerCount;
    adapter->instantiateItem = instantiateItem;
    adapter->destroyItem = destroyItem;
    adapter->update = update;
    adapter->init = init;
    adapter->click = clickItemHandle;
    adapter->init(adapter, context);
    return adapter;
}