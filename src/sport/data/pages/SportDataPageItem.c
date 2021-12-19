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
#ifdef HM_DISPLAY_IS_ROUND
#include <sport/data/pages/SportDataPageItem.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include "core/display_metrics.h"
#include <sb_screen_util.h>
#include <grelib.h>
#include "packages/services/sport/misc/sport_misc.h"
#include <hm_toast.h>
#include <hlog.h>

CONTAINER_FUNCTION_DEFINE();

typedef enum _item_pos_type_t
{
    TYPE_PREFIX = 0,
    TYPE_SUFFIX,
}item_pos_type_t;

#define TAG     "dataItem"

#define ICON_RT_HR_ANAMI_DEFAULT    "images/sports/hr/icon_rt_hr_animation_0.png"
#define ICON_RT_HR_ANAMI_PATH       "images/sports/hr"
#define ICON_RT_HR_ANAMI_PREFIX     "icon_rt_hr_animation"

void greal_free(void *ptr);

#define gre_clone_free(X)           do  \
                                    {   \
                                        if(X) \
                                        {   \
                                            greal_free(X);    \
                                            X = NULL;       \
                                        }   \
                                    }while(0)

static void updataData(SportDataPageItem *page);
static void updateItemData(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static void updateItemBlock(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data);
static void updateItemFixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data, item_pos_type_t pos_type);
static void updateItemSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data, item_pos_type_t pos_type);
static bool isNeedHeadSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedTailSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedColorBlock(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedPrefixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedSuffixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static char *prepareInit(SportDataPageItem *page, H_Int16 position, gr_integer_kv_t *config);
static int msUpdateCallback(struct gr_timer *timer, void *data, int dropped);
static void updateItemSubscriptMS(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);

// ##
#define TIME_MS_TIMEOUT     (5000)

const uint32_t hrColor[] =
{
    0xFFFFFF,   // #FFFFFF
    0x229DBE,   // #229DBE
    0x17C06E,   // #17C06E
    0xDFA032,   // #DFA032
    0xCE581A,   // #CE581A
    0xC42742,   // #C42742
};

static H_Int16 getItemCount(SportDataPageItem *page)
{
    if(page && page->pageData)
    {
        return page->pageData->count;
    }
    else
    {
        return 0;
    }
}

static void initBackground(SportDataPageItem *page)
{
    char *fqn = NULL;
    gr_integer_kv_t valuePos[2] = {
        {"x", 0},
        {"y", 0}};
    char *itemPath = H_NULL;
    switch (getItemCount(page))
    {
    case 4:
        itemPath = "sportDataItemStyleLayer.divide";
        break;

    default:
        break;
    }
    if (itemPath)
    {
        gre_clone_object(page->app, itemPath, "item_bg", page->layerId, valuePos, 0, &fqn);
        gre_clone_free(fqn);
    }
}

static void removeBackground(SportDataPageItem *page, H_Int16 itemCount)
{
    if (itemCount != 4)
    {
        return;
    }
    char key[256];
    hm_snprintf(key, 255, "%s.item_bg", page->layerId);
    gre_delete_object(page->app, key);
}

// ##
#define TYPE1_ROW1_X_OFFSET     getDimension(50)
#define TYPE1_ROW1_Y_OFFSET     getDimension(170)
#define TYPE1_ROW1_X_WIDTH      (getDisplayMetrics()->width - TYPE1_ROW1_X_OFFSET * 2)
#define TYPE1_ROW1_X_STEP(X)    TYPE1_ROW1_X_OFFSET

#define TYPE2_ROW1_X_OFFSET     getDimension(50)
#define TYPE2_ROW1_Y_OFFSET     getDimension(113)
#define TYPE2_ROW1_X_WIDTH      (getDisplayMetrics()->width - TYPE2_ROW1_X_OFFSET * 2)
#define TYPE2_ROW1_X_STEP(X)    TYPE2_ROW1_X_OFFSET

#define TYPE2_ROW2_X_OFFSET     getDimension(50)
#define TYPE2_ROW2_Y_OFFSET     getDimension(295)
#define TYPE2_ROW2_X_WIDTH      (getDisplayMetrics()->width - TYPE2_ROW2_X_OFFSET * 2)
#define TYPE2_ROW2_X_STEP(X)    TYPE2_ROW2_X_OFFSET

#define TYPE3_ROW1_X_OFFSET     getDimension(40)
#define TYPE3_ROW1_Y_OFFSET     getDimension(80)
#define TYPE3_ROW1_X_WIDTH      (getDisplayMetrics()->width - TYPE3_ROW1_X_OFFSET * 2)
#define TYPE3_ROW1_X_STEP(X)    TYPE3_ROW1_X_OFFSET

#define TYPE3_ROW2_X_OFFSET     getDimension(40)
#define TYPE3_ROW2_Y_OFFSET     getDimension(207)
#define TYPE3_ROW2_X_WIDTH      (getDisplayMetrics()->width -TYPE3_ROW2_X_OFFSET* 2)
#define TYPE3_ROW2_X_STEP(X)    TYPE3_ROW2_X_OFFSET

#define TYPE3_ROW3_X_OFFSET     getDimension(40)
#define TYPE3_ROW3_Y_OFFSET     getDimension(333)
#define TYPE3_ROW3_X_WIDTH      (getDisplayMetrics()->width - TYPE3_ROW3_X_OFFSET* 2)
#define TYPE3_ROW3_X_STEP(X)    TYPE3_ROW3_X_OFFSET

#define TYPE4_ROW1_X_OFFSET     getDimension(40)
#define TYPE4_ROW1_Y_OFFSET     getDimension(77)
#define TYPE4_ROW1_X_STEP(X)    TYPE4_ROW1_X_OFFSET
#define TYPE4_ROW1_X_WIDTH      (getDisplayMetrics()->width - TYPE4_ROW1_X_OFFSET * 2)

#define TYPE4_ROW2_X_OFFSET     getDimension(10)
#define TYPE4_ROW2_Y_OFFSET     getDimension(205)
#define TYPE4_ROW2_X_DELTA      getDimension(10)
#define TYPE4_ROW2_X_WIDTH      ((getDisplayMetrics()->width - (TYPE4_ROW2_X_OFFSET * 2 + TYPE4_ROW2_X_DELTA)) / 2)
#define TYPE4_ROW2_X_STEP(X)    (TYPE4_ROW2_X_OFFSET + (X) * (TYPE4_ROW2_X_DELTA + TYPE4_ROW2_X_WIDTH))

#define TYPE4_ROW3_X_OFFSET     getDimension(40)
#define TYPE4_ROW3_Y_OFFSET     getDimension(331)
#define TYPE4_ROW3_X_STEP(X)    TYPE4_ROW1_X_OFFSET
#define TYPE4_ROW3_X_WIDTH      (getDisplayMetrics()->width - TYPE4_ROW3_X_OFFSET * 2)

#define TYPE5_ROW1_X_OFFSET     getDimension(40)
#define TYPE5_ROW1_Y_OFFSET     getDimension(84)
#define TYPE5_ROW1_X_STEP(X)    TYPE5_ROW1_X_OFFSET
#define TYPE5_ROW1_X_WIDTH      (getDisplayMetrics()->width - TYPE5_ROW1_X_OFFSET * 2)

#define TYPE5_ROW2_X_OFFSET     getDimension(20)
#define TYPE5_ROW2_X_DELTA      getDimension(20)
#define TYPE5_ROW2_X_WIDTH      ((getDisplayMetrics()->width - (TYPE5_ROW2_X_OFFSET * 2 + TYPE5_ROW2_X_DELTA * 2)) / 3)
#define TYPE5_ROW2_X_STEP(X)    (TYPE5_ROW2_X_OFFSET + (X) * (TYPE5_ROW2_X_DELTA + TYPE5_ROW2_X_WIDTH))
#define TYPE5_ROW2_Y_OFFSET     getDimension(211)

#define TYPE5_ROW3_X_OFFSET     getDimension(40)
#define TYPE5_ROW3_Y_OFFSET     getDimension(331)
#define TYPE5_ROW3_X_STEP(X)    TYPE5_ROW3_X_OFFSET
#define TYPE5_ROW3_X_WIDTH      (getDisplayMetrics()->width - TYPE5_ROW3_X_OFFSET * 2)

#define TYPE6_ROW1_X_OFFSET     getDimension(40)
#define TYPE6_ROW1_X_DELTA      getDimension(20)
#define TYPE6_ROW1_X_WIDTH      ((getDisplayMetrics()->width - (TYPE6_ROW1_X_OFFSET * 2 + TYPE6_ROW1_X_DELTA)) / 2)
#define TYPE6_ROW1_X_STEP(X)    (TYPE6_ROW1_X_OFFSET + (X) * (TYPE6_ROW1_X_DELTA + TYPE6_ROW1_X_WIDTH))
#define TYPE6_ROW1_Y_OFFSET     getDimension(90)

#define TYPE6_ROW2_X_OFFSET     getDimension(20)
#define TYPE6_ROW2_X_DELTA      getDimension(20)
#define TYPE6_ROW2_X_WIDTH      ((getDisplayMetrics()->width - (TYPE6_ROW2_X_OFFSET * 2 + TYPE6_ROW2_X_DELTA * 2)) / 3)
#define TYPE6_ROW2_X_STEP(X)    (TYPE6_ROW2_X_OFFSET + (X) * (TYPE6_ROW2_X_DELTA + TYPE6_ROW2_X_WIDTH))
#define TYPE6_ROW2_Y_OFFSET     getDimension(211)

#define TYPE6_ROW3_X_OFFSET     getDimension(40)
#define TYPE6_ROW3_Y_OFFSET     getDimension(331)
#define TYPE6_ROW3_X_STEP(X)    TYPE6_ROW3_X_OFFSET
#define TYPE6_ROW3_X_WIDTH      (getDisplayMetrics()->width - TYPE6_ROW3_X_OFFSET * 2)

#define ATTR_GET_Y_VALUE(N, R, P)   (TYPE##N##_ROW##R##_Y_OFFSET)
#define ATTR_GET_W_VALUE(N, R, P)   (TYPE##N##_ROW##R##_X_WIDTH)
#define ATTR_GET_X_VALUE(N, R, P)   (TYPE##N##_ROW##R##_X_STEP(P))

static char *prepareInit(SportDataPageItem *page, H_Int16 position, gr_integer_kv_t *config)
{

    char *itemPath = NULL;
    config[0].key = "x";
    config[1].key = "y";
    config[2].key = "width";
    switch (getItemCount(page))
    {
        case 1:
            itemPath = "sportDataItemStyleLayer.r1_c1_type_p1";
            config[0].value = ATTR_GET_X_VALUE(1, 1, position);
            config[1].value = ATTR_GET_Y_VALUE(1, 1, position);
            config[2].value = getDimension(132);
            break;
        case 2:
            switch (position)
            {
                case 0:
                    itemPath = "sportDataItemStyleLayer.r1_c1_type_p2";
                    config[0].value = ATTR_GET_X_VALUE(2, 1, position);
                    config[1].value = ATTR_GET_Y_VALUE(2, 1, position);
                    config[2].value = getDimension(108);
                    break;
                case 1:
                    itemPath = "sportDataItemStyleLayer.r1_c1_l2_type_p2";
                    config[0].value = ATTR_GET_X_VALUE(2, 2, position);
                    config[1].value = ATTR_GET_Y_VALUE(2, 2, position);
                    config[2].value = getDimension(108);
                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch (position)
            {
                case 0:
                    itemPath = "sportDataItemStyleLayer.r1_c1_l1_type_p3";
                    config[0].value = ATTR_GET_X_VALUE(3, 1, position);
                    config[1].value = ATTR_GET_Y_VALUE(3, 1, position);
                    config[2].value = getDimension(84);
                    break;
                case 1:
                    itemPath = "sportDataItemStyleLayer.r1_c1_l2_type_p3";
                    config[0].value = ATTR_GET_X_VALUE(3, 2, position);
                    config[1].value = ATTR_GET_Y_VALUE(3, 2, position);
                    config[2].value = getDimension(84);
                    break;
                case 2:
                    itemPath = "sportDataItemStyleLayer.r1_c1_l3_type_p3";
                    config[0].value = ATTR_GET_X_VALUE(3, 3, position);
                    config[1].value = ATTR_GET_Y_VALUE(3, 3, position);
                    config[2].value = getDimension(84);
                    break;
                default:
                    break;
            }
            break;
        case 4:
            switch (position)
            {
                case 0:
                    itemPath = "sportDataItemStyleLayer.r1_c1_type_page4";
                    config[0].value = ATTR_GET_X_VALUE(4, 1, position);
                    config[1].value = ATTR_GET_Y_VALUE(4, 1, position);
                    config[2].value = getDimension(84);
                    break;
                case 1:
                    itemPath = "sportDataItemStyleLayer.r2_c2_type_page4";
                    config[0].value = ATTR_GET_X_VALUE(4, 2, position - 1);
                    config[1].value = ATTR_GET_Y_VALUE(4, 2, position);
                    config[2].value = getDimension(84);
                    break;
                case 2:
                    itemPath = "sportDataItemStyleLayer.r2_c2_type_page4";
                    config[0].value = ATTR_GET_X_VALUE(4, 2, position - 1);
                    config[1].value = ATTR_GET_Y_VALUE(4, 2, position);
                    config[2].value = getDimension(84);
                    break;
                case 3:
                    itemPath = "sportDataItemStyleLayer.r1_c1_l2_type_page4";
                    config[0].value = ATTR_GET_X_VALUE(4, 3, position);
                    config[1].value = ATTR_GET_Y_VALUE(4, 3, position);
                    config[2].value = getDimension(84);
                    break;
                default:
                    break;
            }
            break;

        case 5:
            switch (position)
            {
                case 0:
                    itemPath = "sportDataItemStyleLayer.r1_c1_type";
                    config[0].value = ATTR_GET_X_VALUE(5, 1, position);
                    config[1].value = ATTR_GET_Y_VALUE(5, 1, position);
                    config[2].value = getDimension(75);
                    break;
                case 1:
                    itemPath = "sportDataItemStyleLayer.r2_c3_type";
                    config[0].value = ATTR_GET_X_VALUE(5, 2, position - 1);
                    config[1].value = ATTR_GET_Y_VALUE(5, 2, position);
                    config[2].value = getDimension(64);
                    break;
                case 2:
                    itemPath = "sportDataItemStyleLayer.r2_c3_type";
                    config[0].value = ATTR_GET_X_VALUE(5, 2, position - 1);
                    config[1].value = ATTR_GET_Y_VALUE(5, 2, position);
                    config[2].value = getDimension(64);
                    break;
                case 3:
                    itemPath = "sportDataItemStyleLayer.r2_c3_type";
                    config[0].value = ATTR_GET_X_VALUE(5, 2, position - 1);
                    config[1].value = ATTR_GET_Y_VALUE(5, 2, position);
                    config[2].value = getDimension(64);
                    break;
                case 4:
                    itemPath = "sportDataItemStyleLayer.r1_c1_l3_type";
                    config[0].value = ATTR_GET_X_VALUE(5, 3, position);
                    config[1].value = ATTR_GET_Y_VALUE(5, 3, position);
                    config[2].value = getDimension(75);
                    break;
                default:
                    break;
            }
            break;

        case 6:
            switch (position)
            {
                case 0:
                    itemPath = "sportDataItemStyleLayer.r1_c2_type";
                    config[0].value = ATTR_GET_X_VALUE(6, 1, position);
                    config[1].value = ATTR_GET_Y_VALUE(6, 1, position);
                    config[2].value = getDimension(75);
                    break;
                case 1:
                    itemPath = "sportDataItemStyleLayer.r1_c2_type";
                    config[0].value = ATTR_GET_X_VALUE(6, 1, position);
                    config[1].value = ATTR_GET_Y_VALUE(6, 1, position);
                    config[2].value = getDimension(75);
                    break;
                case 2:
                    itemPath = "sportDataItemStyleLayer.r2_c3_type";
                    config[0].value = ATTR_GET_X_VALUE(6, 2, position - 2);
                    config[1].value = ATTR_GET_Y_VALUE(6, 2, position);
                    config[2].value = getDimension(64);
                    break;
                case 3:
                    itemPath = "sportDataItemStyleLayer.r2_c3_type";
                    config[0].value = ATTR_GET_X_VALUE(6, 2, position - 2);
                    config[1].value = ATTR_GET_Y_VALUE(6, 2, position);
                    config[2].value = getDimension(64);
                    break;
                case 4:
                    itemPath = "sportDataItemStyleLayer.r2_c3_type";
                    config[0].value = ATTR_GET_X_VALUE(6, 2, position - 2);
                    config[1].value = ATTR_GET_Y_VALUE(6, 2, position);
                    config[2].value = getDimension(64);
                    break;
                case 5:
                    itemPath = "sportDataItemStyleLayer.r1_c1_l3_type";
                    config[0].value = ATTR_GET_X_VALUE(6, 3, position - 2);
                    config[1].value = ATTR_GET_Y_VALUE(6, 3, position);
                    config[2].value = getDimension(75);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return itemPath;
}

static void initItem(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    char *fqn = NULL;
    char *blockPath = "sportDataItemStyleLayer.block";
    char *iconPath = "sportDataItemStyleLayer.iconRT";
    char *subPath = "sportDataItemStyleLayer.subscript";
    gr_integer_kv_t valuePos[3];
    gr_integer_kv_t iconPos[2];
    gr_integer_kv_t blockPos[2];
    gr_integer_kv_t subPos[2];
    char newPath[64] = {0};
    char itemKey[64] = {0};
    char *itemPath = prepareInit(page, position, valuePos);

    if(!itemPath)
        return;

    memcpy(subPos, valuePos, sizeof(subPos));
    memcpy(iconPos, valuePos, sizeof(iconPos));
    memcpy(blockPos, valuePos, sizeof(blockPos));

    if(isNeedSuffixIcon(page, type, position))
    {
        hm_snprintf(newPath, sizeof(newPath), "icon_%02d", position);
        gre_clone_object(page->app, iconPath, newPath, page->layerId, iconPos, 2, &fqn);
        gre_clone_free(fqn);
    }

    if(isNeedColorBlock(page, type, position))
    {
        const uint8_t _y[6][6] =
        {
            {8},
            {8, 8},
            {7, 7, 7},
            {7, 7, 7, 7},
            {7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7},
        };

        blockPos[0].value = 0;
        blockPos[1].value += valuePos[2].value + _y[getItemCount(page) - 1][position];

        hm_snprintf(newPath, sizeof(newPath), "block_%02d", position);
        gre_clone_object(page->app, blockPath, newPath, page->layerId, blockPos, 2, &fqn);

        hm_snprintf(newPath, sizeof(newPath), "%s.block_%02d", page->layerId, position);
        gre_set_control_visible(page->app, fqn, 1);
        gre_clone_free(fqn);
    }

    if(isNeedHeadSubscript(page, type, position) || isNeedTailSubscript(page, type, position))
    {
        hm_snprintf(newPath, sizeof(newPath), "sub_%02d", position);
        gre_clone_object(page->app, subPath, newPath, page->layerId, subPos, 2, &fqn);
        gre_clone_free(fqn);
    }
    else if(isNeedPrefixIcon(page, type, position))
    {
        hm_snprintf(newPath, sizeof(newPath), "icon_%02d", position);
        gre_clone_object(page->app, iconPath, newPath, page->layerId, iconPos, 2, &fqn);
        gre_clone_free(fqn);
    }

    hm_snprintf(itemKey, sizeof(itemKey), "item_%02d", position);
    gre_clone_object(page->app, itemPath, itemKey, page->layerId, valuePos, 2, &fqn);
    gre_clone_free(fqn);

    hm_snprintf(itemKey, sizeof(itemKey), "%s.item_%02d.title.y", page->layerId, position);
    setSBIntData(page->app, itemKey, valuePos[2].value);

    hm_snprintf(itemKey, sizeof(itemKey), "%s.item_%02d.box.alpha", page->layerId, position);
    setSBIntData(page->app, itemKey, page->isEditMode ? 255 : 0);
}

static void initItems(SportDataPageItem *page)
{
    for (size_t i = 0; i < getItemCount(page); i++)
    {
        initItem(page, page->pageData->item[i], i);
    }
    updataData(page);
    initBackground(page);
}

static void updataData(SportDataPageItem *page)
{
    for (size_t i = 0; i < getItemCount(page); i++)
    {
        updateItemData(page, page->pageData->item[i], i);
    }
}

static int msUpdateCallback(struct gr_timer *timer, void *data, int dropped)
{
    SportDataPageItem *page = (SportDataPageItem *)data;
    if(!page)
        return 0;

    for (size_t i = 0; i < getItemCount(page); i++)
    {
        if(isNeedTailSubscript(page, page->pageData->item[i], i))
        {
            updateItemSubscriptMS(page, page->pageData->item[i], i);
        }
    }

    return 0;
}

static void updateMainData(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data)
{
    char valueKey[64] = {0};
    char value[64] = {0};
    uint32_t hour = (data->rt.integer / 1000) / 3600;
    uint32_t min = ((data->rt.integer / 1000) % 3600) / 60;
    uint32_t sec = (data->rt.integer / 1000) % 60;
    hm_snprintf(valueKey, sizeof(valueKey), "%s.item_%02d.value.text", page->layerId, position);
    if(isNeedHeadSubscript(page, type, position) && hour)
    {
        hm_snprintf(value, sizeof(value), "%d:%02d", min, sec);
        setSBStringData(page->app, valueKey, value);
    }
    else
    {
        setSBStringData(page->app, valueKey, data->rt.value);
    }
}

static void updateItemSubscriptMS(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    char hintKey[64] = {0};
    sport_item_data itemInfo;
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_RT, type, &itemInfo);

    HmTime time;
    hmTime_getTime(&time);
    int32_t delta = time.sec * 1000 + time.usec / 1000 - page->latestSec;

    if(isNeedTailSubscript(page, type, position) && delta <= TIME_MS_TIMEOUT + 2000)
    {
        // updata sub data, ms
        updateItemSubscript(page, type, position, &itemInfo, TYPE_SUFFIX);

        // Set subscript as visiable
        hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d", page->layerId, position);
        gre_set_control_visible(page->app, hintKey, 1);
    }
    else
    {
        hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d", page->layerId, position);
        gre_set_control_visible(page->app, hintKey, 0);

        gre_timer_clear_timeout(page->app, &page->timerHandle);
        page->timerHandle = NULL;
    }
}

static void updateItemData(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    char hintKey[64] = {0};
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title.text", page->layerId, position);

    sport_item_data itemInfo;
    memset(&itemInfo, 0, sizeof(sport_item_data));
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_RT, type, &itemInfo);
    setSBStringData(page->app, hintKey, itemInfo.rt.title);

    uint32_t sec = itemInfo.rt.integer / 1000;
    uint32_t tms = itemInfo.rt.integer;

    if(tms == 0 && isNeedTailSubscript(page, type, position))
    {
        page->isPreStarted = true;
        hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d", page->layerId, position);
        gre_set_control_visible(page->app, hintKey, 0);
    }

    if(page->isPreStarted && !page->isStarted && SportContrlService_getOps()->getSportState() == SPORT_STATE_RUN)
    {
        page->isStarted = true;
    }

    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d", page->layerId, position);
    if(isNeedHeadSubscript(page, type, position) && sec / 3600)
    {
        // Set subscript as visiable
        gre_set_control_visible(page->app, hintKey, 1);

        // updata sub data, hour
        updateItemSubscript(page, type, position, &itemInfo, TYPE_PREFIX);
    }
    else if(isNeedTailSubscript(page, type, position) && tms <= TIME_MS_TIMEOUT)
    {
        if(page->isStarted)
        {
            page->isStarted = false;
            if(page->timerHandle == NULL)
            {
                // start a timer to updata sub data, ms
                page->timerHandle = gre_timer_create_timeout(page->app, 45, 45, page, msUpdateCallback);
            }

            HmTime time;
            hmTime_getTime(&time);
            page->latestSec = time.sec * 1000 + time.usec / 1000;
        }
        updateItemSubscriptMS(page, type, position);
    }
    else if(isNeedTailSubscript(page, type, position) && tms > TIME_MS_TIMEOUT)
    {
        if(page->timerHandle != NULL)
        {
            gre_timer_clear_timeout(page->app, &page->timerHandle);
            page->timerHandle = NULL;
            page->latestSec = 0;
            page->isStarted = false;
        }

        // Set subscript as invisiable
        gre_set_control_visible(page->app, hintKey, 0);
    }

    // update main data
    updateMainData(page, type, position, &itemInfo);

    if(isNeedColorBlock(page, type, position))
    {
        updateItemBlock(page, type, position, &itemInfo);
    }

    if(isNeedPrefixIcon(page, type, position))
    {
        updateItemFixIcon(page, type, position, &itemInfo, TYPE_PREFIX);
    }
    else if(isNeedSuffixIcon(page, type, position))
    {
        updateItemFixIcon(page, type, position, &itemInfo, TYPE_SUFFIX);
    }
}

static uint16_t getTextAreaWidth(SportDataPageItem *page, H_Int16 position, int16_t *x_pos)
{
    int16_t width = 0;
    switch (getItemCount(page))
    {
        case 1:
            *x_pos = ATTR_GET_X_VALUE(1, 1, position);
            width  = ATTR_GET_W_VALUE(1, 1, position);
            break;
        case 2:
            switch (position)
            {
                case 0:
                    *x_pos = ATTR_GET_X_VALUE(2, 1, position);
                    width  = ATTR_GET_W_VALUE(2, 1, position);
                    break;
                case 1:
                    *x_pos = ATTR_GET_X_VALUE(2, 2, position);
                    width  = ATTR_GET_W_VALUE(2, 2, position);
                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch (position)
            {
                case 0:
                    *x_pos = ATTR_GET_X_VALUE(3, 1, position);
                    width  = ATTR_GET_W_VALUE(3, 1, position);
                    break;
                case 1:
                    *x_pos = ATTR_GET_X_VALUE(3, 2, position);
                    width  = ATTR_GET_W_VALUE(3, 2, position);
                    break;
                case 2:
                    *x_pos = ATTR_GET_X_VALUE(3, 3, position);
                    width  = ATTR_GET_W_VALUE(3, 3, position);
                    break;
                default:
                    break;
            }
            break;
        case 4:
            switch (position)
            {
                case 0:
                    *x_pos = ATTR_GET_X_VALUE(4, 1, position);
                    width  = ATTR_GET_W_VALUE(4, 1, position);
                    break;
                case 1:
                    *x_pos = ATTR_GET_X_VALUE(4, 2, position - 1);
                    width  = ATTR_GET_W_VALUE(4, 2, position - 1);
                    break;
                case 2:
                    *x_pos = ATTR_GET_X_VALUE(4, 2, position - 1);
                    width  = ATTR_GET_W_VALUE(4, 2, position - 1);
                    break;
                case 3:
                    *x_pos = ATTR_GET_X_VALUE(4, 3, position);
                    width  = ATTR_GET_W_VALUE(4, 3, position);
                    break;
                default:
                    break;
            }
            break;

        case 5:
            switch (position)
            {
                case 0:
                    *x_pos = ATTR_GET_X_VALUE(5, 1, position);
                    width  = ATTR_GET_W_VALUE(5, 1, position);
                    break;
                case 1:
                    *x_pos = ATTR_GET_X_VALUE(5, 2, position - 1);
                    width  = ATTR_GET_W_VALUE(5, 2, position - 1);
                    break;
                case 2:
                    *x_pos = ATTR_GET_X_VALUE(5, 2, position - 1);
                    width  = ATTR_GET_W_VALUE(5, 2, position - 1);
                    break;
                case 3:
                    *x_pos = ATTR_GET_X_VALUE(5, 2, position - 1);
                    width  = ATTR_GET_W_VALUE(5, 2, position - 1);
                    break;
                case 4:
                    *x_pos = ATTR_GET_X_VALUE(5, 3, position);
                    width  = ATTR_GET_W_VALUE(5, 3, position);
                    break;
                default:
                    break;
            }
            break;

        case 6:
            switch (position)
            {
                case 0:
                    *x_pos = ATTR_GET_X_VALUE(6, 1, position);
                    width  = ATTR_GET_W_VALUE(6, 1, position);
                    break;
                case 1:
                    *x_pos = ATTR_GET_X_VALUE(6, 1, position);
                    width  = ATTR_GET_W_VALUE(6, 1, position);
                    break;
                case 2:
                    *x_pos = ATTR_GET_X_VALUE(6, 2, position - 2);
                    width  = ATTR_GET_W_VALUE(6, 2, position - 2);
                    break;
                case 3:
                    *x_pos = ATTR_GET_X_VALUE(6, 2, position - 2);
                    width  = ATTR_GET_W_VALUE(6, 2, position - 2);
                    break;
                case 4:
                    *x_pos = ATTR_GET_X_VALUE(6, 2, position - 2);
                    width  = ATTR_GET_W_VALUE(6, 2, position - 2);
                    break;
                case 5:
                    *x_pos = ATTR_GET_X_VALUE(6, 3, position);
                    width  = ATTR_GET_W_VALUE(6, 3, position - 2);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return width;
}

static bool isNeedColorBlock(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    if(type == SPORT_RT_ATTR_TYPE_HR && !page->isEditMode)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool isNeedPrefixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    bool isNeed = false;
    uint16_t itemCnt = getItemCount(page);
    if( (itemCnt == 5 && (position > 0 && position < 4)) ||
        (itemCnt == 6 && (position > 1 && position < 5)))
    {
        isNeed = false;
    }
    else
    {
        isNeed = true;
    }

    if(type == SPORT_RT_ATTR_TYPE_HR && isNeed == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool isNeedSuffixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    if(type == SPORT_RT_ATTR_TYPE_SPEED_VERTICAL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool isNeedTailSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    if(type == SPORT_RT_ATTR_TYPE_DURATION_TOTAL ||
        type == SPORT_RT_ATTR_TYPE_DURATION_NET)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool isNeedHeadSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    if( type != SPORT_RT_ATTR_TYPE_DURATION_TOTAL &&
        type != SPORT_RT_ATTR_TYPE_DURATION_NET &&
        type != SPORT_RT_ATTR_TYPE_DURATION_CUR_SECTION &&
        type != SPORT_RT_ATTR_TYPE_DURATION_PREV_SECTION &&
        type != SPORT_RT_ATTR_TYPE_DURATION_AVG_SECTION &&
        type != SPORT_RT_ATTR_TYPE_DURATION_CUR_GROUP)
    {
        return false;
    }

    bool isNeed = false;
    uint16_t itemCnt = getItemCount(page);
    if( (itemCnt <= 3) ||
        (itemCnt == 4 && (position == 0 || position == 3)) ||
        (itemCnt == 5 && (position == 0 || position == 4)) ||
        (itemCnt == 6 && position == 5))
    {
        isNeed = false;
    }
    else if(itemCnt == 4 && (position == 1 || position == 2))
    {
        isNeed = true;
    }
    else if(itemCnt == 6 && (position == 0 || position == 1))
    {
        isNeed = true;
    }
    else
    {
        isNeed = true;
    }

    return isNeed;
}

static void updateItemSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data, item_pos_type_t pos_type)
{
    char hintKey[64] = {0};
    char valueKey[64] = {0};
    char value[16] = {0};
    char subValue[16] = {0};
    int16_t x_pos = 0;
    int16_t width = getTextAreaWidth(page, position, &x_pos);
    uint32_t ms = (data->rt.integer % 1000) / 10;
    uint32_t sec = data->rt.integer / 1000;

    // min : sec
    hm_snprintf(value, sizeof(value), "%d:%02d", (int)((sec % 3600) / 60), (int)((sec % 3600) % 60));

    // ##
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.value.font_size", page->layerId, position);
    uint16_t font_size = getDimension(getSBIntData(page->app, hintKey));

    gr_string_size_t stringSize;
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.value", page->layerId, position);
    gre_get_string_size(page->app, value, "fonts/huamifont.ttf", font_size, width, &stringSize, false, 0);

    // set subscript font size
    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.fontSize", page->layerId, position);
    setSBIntData(page->app, hintKey, font_size / 3);

    // ##
    if(pos_type == TYPE_PREFIX)
    {
        // hour
        hm_snprintf(subValue, sizeof(subValue), "%d", (int)(sec / 3600));

        hm_snprintf(valueKey, sizeof(valueKey), "%s.sub_%02d.sub", page->layerId, position);
        setSBStringData(page->app, valueKey, subValue);
    }
    else
    {
        // ms
        hm_snprintf(subValue, sizeof(subValue), "%02d", (int)ms);

        hm_snprintf(valueKey, sizeof(valueKey), "%s.sub_%02d.sub", page->layerId, position);
        setSBStringData(page->app, valueKey, subValue);
    }

    int16_t x = 0;
    gr_string_size_t stringSize1;
    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.sub", page->layerId, position);
    gre_get_string_size(page->app, subValue, "fonts/huamifont.ttf", font_size / 2, width, &stringSize1, false, 0);

    if(pos_type == TYPE_PREFIX)
    {
        const uint8_t _x[6][6] =
        {
            {0},
            {0, 0},
            {0, 0, 0},
            {0, 6, 6, 0},
            {0, 6, 6, 6, 0},
            {6, 6, 6, 6, 6, 0},
        };

        hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.x", page->layerId, position);
        x = (width - stringSize.width) / 2 - stringSize1.width + _x[getItemCount(page) - 1][position];;
        x = x < 0 ? 0 : x;
        setSBIntData(page->app, hintKey, x);
    }
    else
    {
        const uint8_t _x[6][6] =
        {
            {8},
            {8, 8},
            {0, 0, 0},
            {0, 0, 0, 0},
            {0, 6, 6, 6, 0},
            {0, 0, 6, 6, 6, 0},
        };

        hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.x", page->layerId, position);
        x = ((width + stringSize.width) / 2) + _x[getItemCount(page) - 1][position];
        setSBIntData(page->app, hintKey, x);
    }

    const uint8_t _y[6][6] =
    {
        {font_size / 5},
        {font_size / 8, font_size / 8},
        {font_size / 10, font_size / 10, font_size / 10},
        {font_size / 8, font_size / 10, font_size / 10, font_size / 8},
        {font_size / 8, font_size / 4, font_size / 4, font_size / 4, font_size / 8},
        {font_size / 8, font_size / 8, font_size / 4, font_size / 4, font_size / 4, font_size / 8},
    };

    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.y", page->layerId, position);
    int16_t y = _y[getItemCount(page) - 1][position];
    setSBIntData(page->app, hintKey, y);
}

static void updateItemFixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data, item_pos_type_t pos_type)
{
    int16_t x = 0;
    int16_t y = 0;
    char hintKey[64] = {0};
    int16_t x_pos = 0;
    int16_t width = getTextAreaWidth(page, position, &x_pos);

    int32_t value = (int32_t)data->rt.integer;

    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d", page->layerId, position);
    gre_set_control_visible(page->app, hintKey, 1);

    // ##
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.value.font_size", page->layerId, position);
    uint16_t font_size = getSBIntData(page->app, hintKey);

    gr_string_size_t stringSize;
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.value", page->layerId, position);
    gre_get_string_size(page->app, data->rt.value, "fonts/huamifont.ttf", font_size, width, &stringSize, false, 0);

    if(pos_type == TYPE_PREFIX)
    {
        const uint8_t _y[6][6] =
        {
            {0},
            {0, 0},
            {0, 0, 0},
            {0, 0, 0, 0},
            {0, font_size / 10, font_size / 10, font_size / 10, 0},
            {0, 0, font_size / 10, font_size / 10, font_size / 10, 0},
        };

        hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.image", page->layerId, position);
        setSBStringData(page->app, hintKey, ICON_RT_HR_ANAMI_DEFAULT);

        int icon_w = 0;
        int icon_h = 0;
        gre_get_image_size(page->app, ICON_RT_HR_ANAMI_DEFAULT, &icon_w, &icon_h);
        icon_w = !icon_w ? getDimension(54) : icon_w;
        icon_h = !icon_h ? getDimension(54) : icon_h;

        hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.x", page->layerId, position);
        x = ((width - stringSize.width) / 2) - icon_w;
        x = x < 0 ? 0 : x;
        setSBIntData(page->app, hintKey, x);

        hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.y", page->layerId, position);
        y = (font_size - icon_w) / 2;
        y += _y[getItemCount(page) - 1][position];
        setSBIntData(page->app, hintKey, y);
    }
    else if(pos_type == TYPE_SUFFIX)
    {
        hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.image", page->layerId, position);
        if(value < 0)
        {
            setSBStringData(page->app, hintKey, "images/sports/arrow/icon_rt_arrow_down.png");
        }
        else
        {
            setSBStringData(page->app, hintKey, "images/sports/arrow/icon_rt_arrow_up.png");
        }

        hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.x", page->layerId, position);
        x = (width + stringSize.width) / 2 + getDimension(10);
        setSBIntData(page->app, hintKey, x);

        hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.y", page->layerId, position);
        setSBIntData(page->app, hintKey, font_size / 8);
    }
}

static void updateItemBlock(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data)
{
    char hintKey[64] = {0};
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d", page->layerId, position);

    if(!data->rt.hr_zone)
    {
        gre_set_control_visible(page->app, hintKey, 0);
        hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title.color", page->layerId, position);
        setSBIntData(page->app, hintKey, 0x999999); // #999999
        return;
    }

    gre_set_control_visible(page->app, hintKey, 1);

    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title.color", page->layerId, position);
    setSBIntData(page->app, hintKey, 0x000000); // #000000

    int16_t x_pos = 0;
    int16_t width = getTextAreaWidth(page, position, &x_pos);

    // ##
    gr_string_size_t stringSize;
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title", page->layerId, position);
    gre_get_string_size(page->app, data->rt.title, "fonts/allfont-Medium.ttf", 24, width, &stringSize, false, 0);

    #define BLOCK_WIDTH_DELTA       getDimension(48)
    int16_t block_width = (stringSize.width >= width) ? width: stringSize.width;
    block_width += BLOCK_WIDTH_DELTA;
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d.width", page->layerId, position);
    setSBIntData(page->app, hintKey, block_width);

    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d.color", page->layerId, position);
    uint32_t color_value = hrColor[data->rt.hr_zone];
    setSBIntData(page->app, hintKey, color_value);

    const int8_t _x[6][6] =
    {
        {8},
        {8, 8},
        {-2, -2, -2},
        {-2, -2, -2, -2},
        {-2, 6, 6, 6, -2},
        {0, 0, 6, 6, 6, -2},
    };
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d.x", page->layerId, position);
    int16_t x = (width - block_width) / 2 + x_pos + _x[getItemCount(page) - 1][position];
    setSBIntData(page->app, hintKey, x);
}

static void update(SportDataPageItem *item)
{
    updataData(item);
}

SportDataPageItem *newSportDataPageItemView(ScreenContext *screenContext, char *layerId, sport_data_page *pageData)
{
    SportDataPageItem *item = H_NULL;
    createContainer(item, SportDataPageItem, item->container);
    H_ASSERT(item);
    item->screenContext = screenContext;
    item->app = screenContext->_grAppContext;
    item->pageData = hm_mem_malloc(sizeof(sport_data_page));
    memset(item->pageData, 0, sizeof(sport_data_page));
    memcpy(item->pageData, pageData, sizeof(sport_data_page));
    item->layerId = layerId;
    item->update = update;
    item->isEditMode = false;
    initItems(item);
    return item;
}

SportDataPageItem *newDataEditPageItemView(ScreenContext *screenContext, char *layerId, sport_data_page *pageData, bool isEditMode)
{
    SportDataPageItem *item = H_NULL;
    createContainer(item, SportDataPageItem, item->container);
    H_ASSERT(item);
    item->screenContext = screenContext;
    item->app = screenContext->_grAppContext;
    item->pageData = hm_mem_malloc(sizeof(sport_data_page));
    memset(item->pageData, 0, sizeof(sport_data_page));
    memcpy(item->pageData, pageData, sizeof(sport_data_page));
    item->layerId = layerId;
    item->update = update;
    item->isEditMode = isEditMode;
    initItems(item);
    return item;
}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if (eventId == TOUCH_SELECT)
    {
        int32_t item_id = hm_get_string_digital(id, "item_");
        SportDataPageItem *rtItem = (SportDataPageItem *)container;

        char hintKey[64] = {0};
        hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title.text", rtItem->layerId, item_id);
        char *name  = getSBStringData(rtItem->app, hintKey);
        if(name)
        {
            Toast_show(name);
            hm_mem_free(name);
        }
        else
        {
            HLOG_E(TAG, "%s not found\r\n", hintKey);
        }
        return true;
    }
    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container, const HmKeyEvent *info)
{
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const char *event)
{
    return false;
}

static void removeAllItems(SportDataPageItem *page)
{
    char key[256];
    H_Int16 pagecount = getItemCount(page);
    removeBackground(page, pagecount);
    for (int i = 0; i < pagecount; i++)
    {
        hm_snprintf(key, sizeof(key), "%s.item_%02d", page->layerId, i);
        gre_delete_object(page->app, key);

        sport_rt_attr_type_t type = page->pageData->item[i];
        if(isNeedSuffixIcon(page, type, i) || isNeedPrefixIcon(page, type, i))
        {
            hm_snprintf(key, sizeof(key), "%s.icon_%02d", page->layerId, i);
            gre_delete_object(page->app, key);
        }

        if(isNeedColorBlock(page, type, i))
        {
            hm_snprintf(key, sizeof(key), "%s.block_%02d", page->layerId, i);
            gre_delete_object(page->app, key);

            hm_snprintf(key, sizeof(key), "%s.icon_%02d.image", page->layerId, i);
            removeContainer(page->screenContext, key);
        }

        if(isNeedHeadSubscript(page, type, i) || isNeedTailSubscript(page, type, i))
        {
            hm_snprintf(key, sizeof(key), "%s.sub_%02d", page->layerId, i);
            gre_delete_object(page->app, key);
        }
    }

    if(page->timerHandle != NULL)
    {
        gre_timer_clear_timeout(page->app, &page->timerHandle);
        page->timerHandle = NULL;
    }
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportDataPageItem *page = (SportDataPageItem *)container;

    if(page && page->pageData)
    {
        removeAllItems(page);
        hm_mem_free(page->pageData);
    }
}

#endif