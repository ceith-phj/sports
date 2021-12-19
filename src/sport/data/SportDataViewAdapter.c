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
#include "ui/Sports/include/sport/data/pages/SportTrainingEffe.h"
#include "packages/services/sport/misc/sport_misc.h"

#define TAG "sportChart"

static void init(ViewPagerAdapter *adapter, ScreenContext *screenContext)
{
    adapter->screenContext = screenContext;
    adapter->app = screenContext->_grAppContext;
    adapter->isStatic = true;
    adapter->data = getSportsDataPresenter()->getSportData(getSportsControlManager()->getType());
}

static H_Int16 getViewPagerCount(ViewPagerAdapter *adapter)
{
    sport_data *data = (sport_data *)adapter->data;
    return data->pageCount;
}

static ScreenContainer *instantiateItem(ViewPagerAdapter *adapter, char *layerId, H_Int16 page)
{
    sport_data *data = (sport_data *)adapter->data;
    sport_data_page *pageData = (sport_data_page *)&data->page[page];
    char key[128] = {0};
    snprintf(key, 128, "%s.item.text", layerId);
    char value[128] = {0};
    snprintf(value, 128, "data layer %d", page);
    setSBStringData(adapter->app, key, value);

    char hint[48] = {0};

    switch (pageData->pattern)
    {
        case ITEM_PATTERN_RT:
        {
            SportDataPageItem *rtItem = newSportDataPageItemView(adapter->screenContext, layerId, pageData);
            hm_snprintf(hint, sizeof(hint), "%s.%s.%d", layerId, "rtItem", page);
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
        default:
            break;
    }
    return H_NULL;
}

static void destroyItem(ViewPagerAdapter *adapter, ScreenContainer *container, H_Int16 position)
{
    if (container && container->destroy)
    {
        container->destroy(adapter->screenContext, container);
    }
}

static void update(ViewPagerAdapter *adapter, ScreenContainer *container, H_Int16 position)
{
    sport_data *data = (sport_data *)adapter->data;
    sport_data_page *pageData = (sport_data_page *)&data->page[position];

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

static void event(ViewPagerAdapter *adapter, ScreenContainer *container, H_Int16 position, void *event)
{
    sport_data *data = (sport_data *)adapter->data;
    sport_data_page *pageData = (sport_data_page *)&data->page[position];
    HLOG_I(TAG, "%s pageData->pattern:%d SportEventType:%d position:%d\r\n", __func__, pageData->pattern, event, position);
    switch (pageData->pattern)
    {
        case ITEM_PATTERN_CHART:
        {
            break;
        }
        case ITEM_PATTERN_RT:
        {
            break;
        }
        case ITEM_PATTERN_GOAL:
        {
            break;
        }
        case ITEM_PATTERN_RABBITE:
        {
            SportRabbiteContainer *item = (SportRabbiteContainer *)container;
            switch ((SportEventType)event)
            {
            case SPORT_EVENT_STOP:
            case SPORT_EVENT_SUSPEND:
                item->stopSport(item);
                break;
            case SPORT_EVENT_CONTINUE:
            case SPORT_EVENT_RUN:
                item->startSport(item);
                break;

            default:
                break;
            }
            break;
        }
        case ITEM_PATTERN_CADENCE_ASSISTANT:
        {
            break;
        }
        default:
            break;
    }
}

ViewPagerAdapter *newViewPagerAdapter(ScreenContext *context)
{
    ViewPagerAdapter *adapter = H_NULL;
    adapter = H_MALLOC(sizeof(ViewPagerAdapter));
    H_ASSERT(adapter);
    adapter->getViewPagerCount = getViewPagerCount;
    adapter->instantiateItem = instantiateItem;
    adapter->destroyItem = destroyItem;
    adapter->update = update;
    adapter->event = event;
    adapter->init = init;
    adapter->init(adapter, context);
    return adapter;
}