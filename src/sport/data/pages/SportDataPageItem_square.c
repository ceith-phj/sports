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

#ifndef HM_DISPLAY_IS_ROUND

#include <sport/data/pages/SportDataPageItem.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include "core/display_metrics.h"
#include <sb_screen_util.h>
#include <grelib.h>
#include "packages/services/sport/misc/sport_misc.h"
#include <hm_toast.h>

#include <hm_system_status_bar.h>

#define TAG "SportDataPageItem_square"

CONTAINER_FUNCTION_DEFINE();

typedef enum _item_pos_type_t
{
    TYPE_PREFIX = 0,
    TYPE_SUFFIX,
}item_pos_type_t;

typedef struct item_control_attr_
{
    H_Int16 x;
    H_Int16 y;
    H_Int16 w;
    H_Int16 h;
}item_control_attr;

typedef struct item_text_and_control_attr_
{
    H_Int16 valueWidth;
    H_Int16 titleWidth;
    item_control_attr value;
    item_control_attr title;
}item_text_and_control_attr;

#define ICON_RT_HR_ANAMI_DEFAULT    "images/sports/hr/icon_rt_hr_animation_0.png"
#define ICON_RT_HR_ANAMI_PATH       "images/sports/hr"
#define ICON_RT_HR_ANAMI_PREFIX     "icon_rt_hr_animation"

#define SPORT_VALUE_FONT_NAME "fonts/huamifont.ttf"
#define SPORT_TITLE_FONT_NAME "fonts/allfont-Medium.ttf"

#define BLOCK_WIDTH_DELTA           48
#define BLOCK_TOP_PADDING           6
#define PRE_SUF_FIX_TOP_PADDING     15
#define PRE_SUF_FIX_LR_PADDING      6

void greal_free(void *ptr);

#define gre_clone_free(X)           do  \
                                    {   \
                                        if(X) \
                                        {   \
                                            greal_free(X);    \
                                            X = NULL;       \
                                        }   \
                                    }while(0)

static void prepareItemInit(const char * itemPath, gr_integer_kv_t *config);
static void updataData(SportDataPageItem *page);
static void updateItemData(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static void updateItemBlock(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data, item_text_and_control_attr *titleValueAttr);
static int  msUpdateCallback(struct gr_timer *timer, void *data, int dropped);
static void updateItemMSPos(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr);
static void updateItemHourValue(SportDataPageItem *page, H_Int16 position, int hour);
static void updateItemHourPos(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr);
static void updateSuffixIcon(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr, int32_t value);
static void updatePrefixIcon(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr);
static void getControlAttr(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr);
static void getTitleValueTextWidth(SportDataPageItem *page, H_Int16 position, sport_item_data *data, item_text_and_control_attr *titleValueAttr);
static void updateMainData(SportDataPageItem *page,  H_Int16 position, sport_item_data *data);

static bool isNeedHeadSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedTailSubscript(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedColorBlock(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedPrefixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);
static bool isNeedSuffixIcon(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position);

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
    return page->pageData->count;
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


//通过一套机制来获取SB控件中的相应坐标
static void prepareItemInit(const char * itemPath, gr_integer_kv_t *config)
{
    if (NULL == itemPath || NULL == config)
    {
        HLOG_E(TAG, "prepareItemInit: arg fail \r\n");
        return;
    }
    config[0].key   = "x";
    config[1].key   = "y";
    config[0].value = gre_get_control_x(getAppInstance(), itemPath);
    config[1].value = gre_get_control_y(getAppInstance(), itemPath);
    return;
}

static void initItem(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    if(NULL == page)
    {
        HLOG_E(TAG, "initItem: arg fail \r\n");
        return;
    }

    char *fqn           = NULL;
    char *blockPath     = "sportDataItemStyleLayer.block";
    char *iconPath      = "sportDataItemStyleLayer.iconRT";
    char *subPath       = "sportDataItemStyleLayer.subscript";
    char itemPath[64]   = {0};
    char newPath[64]    = {0};
    char itemKey[64]    = {0};
    gr_integer_kv_t groupPos[2] = {0};

    //获取应该绘制group的x , y坐标
    hm_snprintf(itemPath, sizeof(itemPath), "sportDataItemStyleLayer_square.page%d_item%d", getItemCount(page), position);
    prepareItemInit(itemPath, groupPos);

    //色块
    if(isNeedColorBlock(page, type, position))
    {
        hm_snprintf(newPath, sizeof(newPath), "block_%02d", position);
        gre_clone_object(page->app, blockPath, newPath, page->layerId, groupPos, 2, &fqn);
        gre_clone_free(fqn);
    }

    //前缀、后缀 图标
    if(isNeedSuffixIcon(page, type, position) || isNeedPrefixIcon(page, type, position))
    {
        hm_snprintf(newPath, sizeof(newPath), "icon_%02d", position);
        gre_clone_object(page->app, iconPath, newPath, page->layerId, groupPos, 2, &fqn);
        gre_clone_free(fqn);
    }
    //前缀、后缀 文字标题
    else if(isNeedHeadSubscript(page, type, position) || isNeedTailSubscript(page, type, position))
    {
        hm_snprintf(newPath, sizeof(newPath), "sub_%02d", position);
        gre_clone_object(page->app, subPath, newPath, page->layerId, groupPos, 2, &fqn);
        gre_clone_free(fqn);
        page->isStarted = false;
    }

    //绘制group
    hm_snprintf(itemKey, sizeof(itemKey), "item_%02d", position);
    gre_clone_object(page->app, itemPath, itemKey, page->layerId, groupPos, 2, &fqn);
    gre_clone_free(fqn);

    //判断是否编辑状态，编辑状态蓝框显示
    hm_snprintf(itemKey, sizeof(itemKey), "%s.item_%02d.box.alpha", page->layerId, position);
    setSBIntData(page->app, itemKey, page->isEditMode ? 255 : 0);

}

// ui创建时初始化控件
static void initItems(SportDataPageItem *page)
{
    for (size_t i = 0; i < getItemCount(page); i++)
    {
        initItem(page, page->pageData->item[i], i);
    }
    updataData(page);
    initBackground(page);
}

// 根据page项的个数来更新数据，并且刷新Ui
static void updataData(SportDataPageItem *page)
{
    for (size_t i = 0; i < getItemCount(page); i++)
    {
        updateItemData(page, page->pageData->item[i], i);
    }
}

static void updateItemData(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position)
{
    if(NULL == page)
    {
        HLOG_E(TAG, "prepareItemInit: arg fail \r\n");
        return;
    }
    char hintKey[64] = {0};
    item_text_and_control_attr titleValueAttr = {0};
    sport_item_data itemInfo = {0};

    //获取数据
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_RT, type, &itemInfo);

    //更新value 和 title 的数据
    updateMainData(page, position, &itemInfo);

    //获取value和title control的x、y、w、h
    getControlAttr(page, position, &titleValueAttr);

    //获取title和value的文本宽度  !需要先获取控件的属性
    getTitleValueTextWidth(page, position, &itemInfo, &titleValueAttr);

    //更新前缀图标
    if(isNeedPrefixIcon(page, type, position))
    {
        updatePrefixIcon(page, position, &titleValueAttr);
    }

    //更新后缀图标
    if(isNeedSuffixIcon(page, type, position))
    {
        updateSuffixIcon(page, position, &titleValueAttr,  (int32_t)itemInfo.rt.integer);
    }

    //更新色块
    if(isNeedColorBlock(page, type, position))
    {
        updateItemBlock(page, type, position, &itemInfo, &titleValueAttr);
    }

    //获取时间
    uint32_t sec = itemInfo.rt.integer / 1000;
    uint32_t tms = itemInfo.rt.integer;

    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d", page->layerId, position);
    //超过1小时，设置前缀
    if(isNeedHeadSubscript(page, type, position) && sec / 3600)
    {
        //设置值，在更新坐标
        gre_set_control_visible(page->app, hintKey, 1);
        //设置小时内容
        updateItemHourValue(page, position, (int)(sec/3600));
        //设置小时位置
        updateItemHourPos(page, position, &titleValueAttr);
    }
    else if(isNeedTailSubscript(page, type, position) && tms <= TIME_MS_TIMEOUT)
    {
        //需要显示后缀毫秒，时间不超过5秒

        //设置显示毫秒并且刷新判断是否需要启动定时器刷新
        if(!page->isStarted && SportContrlService_getOps()->getSportState() == SPORT_STATE_RUN &&
            page->timerHandle == NULL)
        {
            page->timerHandle = gre_timer_create_timeout(page->app, 45, 45, page, msUpdateCallback);
        }
        //设置后缀的位置 显示毫秒控件
        updateItemMSPos(page, position, &titleValueAttr);
        gre_set_control_visible(page->app, hintKey, 1);
    }
    else if(isNeedTailSubscript(page, type, position) && tms > TIME_MS_TIMEOUT)
    {
        //超过5秒之后如果定时器没有关闭就关闭定时器
        if(page->timerHandle != NULL)
        {
            gre_timer_clear_timeout(page->app, &page->timerHandle);
            page->timerHandle = NULL;
            page->isStarted = true;
        }
        //设置隐藏后缀
        gre_set_control_visible(page->app, hintKey, 0);
    }
}

//更新色块
static void updateItemBlock(SportDataPageItem *page, sport_rt_attr_type_t type, H_Int16 position, sport_item_data *data, item_text_and_control_attr *titleValueAttr)
{
    //设置title内容的颜色
    char hintKey[64] = {0};
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d", page->layerId, position);

    //设置隐藏色块重置title颜色
    if(!data->rt.hr_zone)
    {
        gre_set_control_visible(page->app, hintKey, 0);
        hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title.color", page->layerId, position);
        setSBIntData(page->app, hintKey, 0x999999); // #999999
        return;
    }

    //设置显示色块重置title颜色
    gre_set_control_visible(page->app, hintKey, 1);
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title.color", page->layerId, position);
    setSBIntData(page->app, hintKey, 0x000000); // #000000

    //设置色块宽度
    int16_t block_width = BLOCK_WIDTH_DELTA;
    block_width += titleValueAttr->titleWidth;
    block_width = block_width >= titleValueAttr->title.w ? titleValueAttr->title.w : block_width;
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d.width", page->layerId, position);
    setSBIntData(page->app, hintKey, block_width);

    //设置色块的颜色
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d.color", page->layerId, position);
    uint32_t color_value = hrColor[data->rt.hr_zone];
    setSBIntData(page->app, hintKey, color_value);

    //设置色块的x坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d.x", page->layerId, position);
    int16_t x = (titleValueAttr->title.w - block_width) / 2;
    setSBIntData(page->app, hintKey, x);

    //设置色块的y坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.block_%02d.y", page->layerId, position);
    int16_t y = titleValueAttr->title.y;
    setSBIntData(page->app, hintKey, y + BLOCK_TOP_PADDING);
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
    item->pageData = H_MALLOC(sizeof(sport_data_page));
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
    item->pageData = H_MALLOC(sizeof(sport_data_page));
    memset(item->pageData, 0, sizeof(sport_data_page));
    memcpy(item->pageData, pageData, sizeof(sport_data_page));
    item->layerId = layerId;
    item->update = update;
    item->isEditMode = isEditMode;
    HmSysStatusBar_setVisible(H_False);
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
        Toast_show(name);
        hm_mem_free(name);
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
    isNeed = true;
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
    isNeed = true;
    return isNeed;
}

static void updateMainData(SportDataPageItem *page,  H_Int16 position, sport_item_data *data)
{
    //更新value数据
    char valueKey[64] = {0};
    hm_snprintf(valueKey, sizeof(valueKey), "%s.item_%02d.value.text", page->layerId, position);
    setSBStringData(page->app, valueKey, data->rt.value);

    //更新title数据
    char titleKey[64] = {0};
    hm_snprintf(titleKey, sizeof(titleKey), "%s.item_%02d.title.text", page->layerId, position);
    setSBStringData(page->app, titleKey, data->rt.title);
}

static void getTitleValueTextWidth(SportDataPageItem *page, H_Int16 position, sport_item_data *data, item_text_and_control_attr *titleValueAttr)
{
    char hintKey[64] = {0};
    //获取value字体大小
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.value.font_size", page->layerId, position);
    uint16_t font_size = getSBIntData(page->app, hintKey);

    //获取value字体宽度
    gr_string_size_t stringSize;
    gre_get_string_size(page->app, data->rt.value, SPORT_VALUE_FONT_NAME, font_size, titleValueAttr->value.w, &stringSize, false, 0);
    titleValueAttr->valueWidth = stringSize.width;

    //获取title字体大小
    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title.fontSize", page->layerId, position);
    font_size = getSBIntData(page->app, hintKey);

    // 获取title宽度
    memset(&stringSize, 0, sizeof(stringSize));
    gre_get_string_size(page->app, data->rt.title, SPORT_TITLE_FONT_NAME, font_size, titleValueAttr->title.w, &stringSize, false, 0);
    titleValueAttr->titleWidth = stringSize.width;
}

static void getControlAttr(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr)
{
    char hintKey[64] = {0};

    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.value", page->layerId, position);
    titleValueAttr->value.w = gre_get_control_w(getAppInstance(), hintKey);
    titleValueAttr->value.h = gre_get_control_h(getAppInstance(), hintKey);
    titleValueAttr->value.x = gre_get_control_x(getAppInstance(), hintKey);
    titleValueAttr->value.y = gre_get_control_y(getAppInstance(), hintKey);

    hm_snprintf(hintKey, sizeof(hintKey), "%s.item_%02d.title", page->layerId, position);
    titleValueAttr->title.w = gre_get_control_w(getAppInstance(), hintKey);
    titleValueAttr->title.h = gre_get_control_h(getAppInstance(), hintKey);
    titleValueAttr->title.x = gre_get_control_x(getAppInstance(), hintKey);
    titleValueAttr->title.y = gre_get_control_y(getAppInstance(), hintKey);
    return;
}

static void updatePrefixIcon(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr)
{
    char hintKey[64] = {0};
    int16_t x = 0;
    int16_t y = 0;

    //显示图标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d", page->layerId, position);
    gre_set_control_visible(page->app, hintKey, 1);

    //前缀设置心率图片
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.image", page->layerId, position);
    setSBStringData(page->app, hintKey, ICON_RT_HR_ANAMI_DEFAULT);

    //获取图片尺寸
    int icon_w = 0;
    int icon_h = 0;
    gre_get_image_size(page->app, ICON_RT_HR_ANAMI_DEFAULT, &icon_w, &icon_h);
    icon_w = !icon_w ? getDimension(54) : icon_w;
    icon_h = !icon_h ? getDimension(54) : icon_h;

    //设置图片x坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.x", page->layerId, position);
    x = ((titleValueAttr->value.w - titleValueAttr->valueWidth) / 2) - icon_w - PRE_SUF_FIX_LR_PADDING;
    x = x < 0 ? 0 : x;
    setSBIntData(page->app, hintKey, x);

    //设置图片y坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.y", page->layerId, position);
    y = (titleValueAttr->value.h - icon_w) / 2;
    setSBIntData(page->app, hintKey, y);
    return;
}

static void updateSuffixIcon(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr, int32_t value)
{
    char hintKey[64] = {0};
    int16_t x = 0;
    int16_t y = 0;

    //显示图标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d", page->layerId, position);
    gre_set_control_visible(page->app, hintKey, 1);
    //后缀设置图片
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.image", page->layerId, position);
    //value < 0下降
    if(value < 0)
    {
        setSBStringData(page->app, hintKey, "images/sports/arrow/icon_rt_arrow_down.png");
    }
    else    //否则上升
    {
        setSBStringData(page->app, hintKey, "images/sports/arrow/icon_rt_arrow_up.png");
    }

    //设置图片x坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.x", page->layerId, position);
    x = (titleValueAttr->value.w + titleValueAttr->valueWidth) / 2 + PRE_SUF_FIX_LR_PADDING;
    setSBIntData(page->app, hintKey, x);

    //设置图片y坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.icon_%02d.y", page->layerId, position);
    y = PRE_SUF_FIX_TOP_PADDING;
    setSBIntData(page->app, hintKey, y);
    return;
}

static void updateItemHourPos(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr)
{
    char hintKey[64] = {0};

    // 获取小时字体大小
    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.fontSize", page->layerId, position);
    uint16_t font_size = getSBIntData(page->app, hintKey);

    // 获取小时文字宽度
    gr_string_size_t stringSize;
    gre_get_string_size(page->app, "00", "fonts/huamifont.ttf", font_size, titleValueAttr->value.w, &stringSize, false, 0);

    // 设置 ms x坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.x", page->layerId, position);
    int16_t x = ((titleValueAttr->value.w - titleValueAttr->valueWidth) / 2) - stringSize.width - PRE_SUF_FIX_LR_PADDING;
    x = x < 0 ? 0 : x;
    setSBIntData(page->app, hintKey, x);

    // 设置 ms y坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.y", page->layerId, position);
    int16_t y = PRE_SUF_FIX_TOP_PADDING;
    setSBIntData(page->app, hintKey, y);
}

static void updateItemHourValue(SportDataPageItem *page, H_Int16 position, int hour)
{
    char valueKey[64] = {0};
    char subValue[16] = {0};
    // hour
    hm_snprintf(subValue, sizeof(subValue), "%d", hour);
    hm_snprintf(valueKey, sizeof(valueKey), "%s.sub_%02d.sub", page->layerId, position);
    setSBStringData(page->app, valueKey, subValue);
}

static void updateItemMSPos(SportDataPageItem *page, H_Int16 position, item_text_and_control_attr *titleValueAttr)
{
    char hintKey[64] = {0};
    // 设置 ms x坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.x", page->layerId, position);
    int16_t x = ((titleValueAttr->value.w + titleValueAttr->valueWidth) / 2) + PRE_SUF_FIX_LR_PADDING;
    setSBIntData(page->app, hintKey, x);

    // 设置 ms y坐标
    hm_snprintf(hintKey, sizeof(hintKey), "%s.sub_%02d.y", page->layerId, position);
    int16_t y = PRE_SUF_FIX_TOP_PADDING;
    setSBIntData(page->app, hintKey, y);
}

static int msUpdateCallback(struct gr_timer *timer, void *data, int dropped)
{
    SportDataPageItem *page = (SportDataPageItem *)data;
    if(!page)
        return 0;
    char key[64] = {0};
    char subValue[64] = {0};
    sport_item_data itemInfo;

    for (size_t i = 0; i < getItemCount(page); i++)
    {
        //获取数据
        getSportsDataPresenter()->getItemData(ITEM_PATTERN_RT, page->pageData->item[i], &itemInfo);
        //计算毫秒
        uint32_t ms = (itemInfo.rt.integer % 1000) / 10;
        hm_snprintf(key, sizeof(key), "%s.sub_%02d.sub", page->layerId, i);
        // 设置 ms 内容
        hm_snprintf(subValue, sizeof(subValue), "%02d", (int)ms);
        setSBStringData(getAppInstance(), key, subValue);
    }

    return 0;
}

#endif