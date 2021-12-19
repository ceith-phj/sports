/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: lizheng1@huami.com
 *  Maintainer: lizheng1@huami.com
 *
 * =====================================================================================
 */

#include <setting/BaseSportSettingLayer.h>
#include <BaseSportView.h>
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sb_screen_util.h>
#include <utils/sports_string_utils.h>
#include <gre/sdk/greal_alloc.h>

#define MOTION_OFFSET   10
#define TAG "BaseSportSettingLayer"

static SportSettingItemStyle itemStyle[SETTING_ITEM_MAX] = {
    {SINGLE_LINE, "sportSettingItemStyleLayer.item_multiLineGroup"},
    {MULTI_LINE, "sportSettingItemStyleLayer.item_multiLineGroup"},
    {RADIO_SINGLE_LINE, "sportSettingItemStyleLayer.item_singleLineRadioButtonGroup"},
    {RADIO_SINGLE_LINE_ARROW, "sportSettingItemStyleLayer.item_multiLineRadioButtonGroup"},
    {RADIO_MULTI_LINE_ARROW, "sportSettingItemStyleLayer.item_multiLineRadioButtonGroup"},
    {CHECKBOX_SINGLE_LINE, "sportSettingItemStyleLayer.item_multiLineCheckboxGroup"},
    {CHECKBOX_MULTI_LINE, "sportSettingItemStyleLayer.item_multiLineCheckboxGroup"},
    {SWITCH_SINGLE_LINE, "sportSettingItemStyleLayer.item_multiLineSwitchGroup"},
    {SWITCH_MULTI_LINE, "sportSettingItemStyleLayer.item_multiLineSwitchGroup"},
    {MAIN_ITEM, "sportSettingItemStyleLayer.item_singleLineTextIconControl"},
    {SINGLE_LINE_ICON_TEXT, "sportSettingItemStyleLayer.item_singleLineTextIconGroup"}};

CONTAINER_FUNCTION_DEFINE();

static H_Int16 getItemId(BaseSportSettingLayer *settingContainer, char *id);
static void initSportSetting(BaseSportSettingLayer *dataView);
static void initData(BaseSportSettingLayer *dataView);

void setStartY(BaseSportSettingLayer *dataView, H_Int16 startY)
{
    dataView->startY = startY;
}

void setTitleEnable(BaseSportSettingLayer *dataView, bool isTitleEnable)
{
    dataView->isTitleEnable = isTitleEnable;
}

static void initContainer(BaseSportSettingLayer *dataView, ScreenContext *screenContext)
{
    dataView->childLayer->parent = dataView;
    initSportSetting(dataView);
    initData(dataView);
    dataView->childLayer->initData(dataView->childLayer);
    dataView->initViews(dataView);
    //addContainer(screenContext, dataView->childLayer->getContainerId(), dataView);
}

static void setChild(BaseSportSettingLayer *dataView, SportSettingLayerImpl *child)
{
    dataView->childLayer = child;
}

static void initData(BaseSportSettingLayer *dataView)
{
    dataView->items = H_MALLOC(sizeof(SportSettingItem) * dataView->childLayer->getDefaultSettingItemSize());
    memset(dataView->items, 0, sizeof(SportSettingItem) * dataView->childLayer->getDefaultSettingItemSize());

    dataView->itemCount = 0;
    for (int i = 0; i < dataView->childLayer->getDefaultSettingItemSize(); i++)
    {
        if (dataView->childLayer->isSettingItemEnable(dataView->childLayer, (dataView->childLayer->getDefaultSettingItem() + i)->type))
        {
            memcpy(dataView->items + dataView->itemCount, dataView->childLayer->getDefaultSettingItem() + i, sizeof(SportSettingItem));
            dataView->itemCount++;
        }
    }

    dataView->pressX = 0;
    dataView->pressY = 0;
    dataView->isMoved = false;
    dataView->isTouched = false;
}

static void initSportSetting(BaseSportSettingLayer *dataView)
{
    dataView->setting = H_MALLOC(sizeof(SportSettingsMessage));
    memset(dataView->setting, 0, sizeof(SportSettingsMessage));
    sport_type_t type = getSportsControlManager()->getType();
    sportSetting_getSettingManager()->getSetting(type, dataView->setting);
}

static void initViews(BaseSportSettingLayer *dataView)
{
    dataView->containerHeight = dataView->startY;
    dataView->initTitleView(dataView);
    dataView->initItems(dataView);
}

static void initTitleView(BaseSportSettingLayer *dataView)
{
    if (!dataView->isTitleEnable)
    {
        return;
    }

    // first, clone objects
    gr_integer_kv_t data[2] =
        {
            {"x", 0},
            {"y", 0},
        };
    char *fqn = NULL;
    gre_clone_object(dataView->app, "sportSettingItemStyleLayer.titleGroup.TitleControl", "titleview", dataView->layer, data, 2, &fqn);
    if (fqn)
    {
        gre_set_control_visible(dataView->app, fqn, 1);
        //the title shoud be sportSettingMainLayer.titleview

        HLOG_D(TAG, "titleview id:%s\r\n", fqn);
        char key[128] = {0};
        //the key shoud be sportSettingMainLayer.titleview.value
        snprintf(key, 128, "%s.%s", fqn, "value");
        char *title_value = getSBStringData(dataView->app, dataView->childLayer->getTitleId(dataView->childLayer));
        setSBStringData(dataView->app, key, title_value);
        H_FREE(title_value);

        // then, set title string
        dataView->containerHeight += gre_get_control_h(dataView->app, fqn);
        greal_free(fqn);
        fqn = NULL;
    }
}

static void initItems(BaseSportSettingLayer *dataView)
{
    // clone items
    for (H_Uint16 i = 0; i < dataView->itemCount; i++)
    {

        gr_integer_kv_t data[2] =
            {
                {"x", 0},
                {"y", dataView->containerHeight},
            };
        char *fqn = NULL;
        char key[128] = {0};
        snprintf(key, 128, "setting_item_%02d", i);
        SportSettingItem *item = dataView->items + i;
        gre_clone_object(dataView->app, dataView->getItemStylePath(dataView, item->displayType), key, dataView->layer, data, 2, &fqn);
        item->path = fqn;
        dataView->initItem(dataView, item);
        dataView->updateItemLines(dataView, item);
        dataView->updatePrimaryText(dataView, fqn, item);
        dataView->updateIcon(dataView, fqn, item);
        if (dataView->childLayer)
        {
            dataView->childLayer->onItemUpdate(dataView->childLayer, item->type, item);
        }
        HLOG_W(TAG, "%s %s\r\n", __func__, fqn);

        gre_set_control_visible(dataView->app, fqn, 1);
        int height = gre_get_control_h(dataView->app, fqn);
        if (height == 0)
        {
            char heightKey[128] = {0};
            snprintf(heightKey, 128, "%s.content.grd_height", fqn);
            height = getSBIntData(dataView->app, heightKey);
        }
        dataView->containerHeight += height;
        //dataView->childLayer->onSettingItemUpdate(dataView->childLayer, i, fqn);
        if (fqn)
        {
            greal_free(fqn);
        }
    }
}

static void onItemCheckedChanged(View *view, bool isChecked)
{
    BaseSportSettingLayer *settingContainer = (BaseSportSettingLayer *)view->getTag(view, "parent");
    ;
    char *id = view->id;
    H_Int16 itemId = getItemId(settingContainer, id);
    if (itemId != -1)
    {
        SportSettingItem *item = settingContainer->items + itemId;
        settingContainer->childLayer->onItemCheckChanged(settingContainer->childLayer, item->type, isChecked, item);
    }
}

static void initItem(BaseSportSettingLayer *dataView, SportSettingItem *item)
{
    // init Checkbox
    char key[128] = {0};
    if (item->displayType == CHECKBOX_SINGLE_LINE || item->displayType == CHECKBOX_MULTI_LINE)
    {
        snprintf(key, sizeof(key), "%s.check", item->path);
        item->checkBox = newCheckBoxContainer(dataView->app, key);
    }
    // init radio button
    else if (item->displayType == RADIO_SINGLE_LINE ||
             item->displayType == RADIO_SINGLE_LINE_ARROW ||
             item->displayType == RADIO_MULTI_LINE_ARROW)
    {
        snprintf(key, sizeof(key), "%s.radio", item->path);
        item->checkBox = newCheckBoxContainer(dataView->app, key);
        if (item->checkBox)
        {
            item->checkBox->setButtonImages(item->checkBox,
                                            "images/widget/button/checkbox/radio_normal.png",
                                            "images/widget/button/checkbox/radio_checked.png",
                                            "images/widget/button/checkbox/radio_normal.png");
        }
    }
    // init switch
    else if (item->displayType == SWITCH_SINGLE_LINE || item->displayType == SWITCH_MULTI_LINE)
    {
        // slideSwitch
        snprintf(key, sizeof(key), "%s.switch", item->path);
        item->checkBox = newCheckBoxContainer(dataView->app, key);
        if (item->checkBox)
        {
            item->checkBox->setButtonImages(item->checkBox,
                                            "images/widget/button/checkbox/switch_off.png",
                                            "images/widget/button/checkbox/switch_on.png",
                                            "images/widget/button/checkbox/switch_off.png");
        }
    }
    if (item->checkBox)
    {
        item->checkBox->view->setTag(item->checkBox->view, "parent", dataView);
    }
}

static void updateItemLines(BaseSportSettingLayer *dataView, SportSettingItem *item)
{
    int primaryTextOffsetY = 0;
    int secondaryTextAlpha = 0;
    char key[128] = {0};
    if (item->displayType <= SINGLE_LINE_COUNT && item->displayType >= SINGLE_LINE)
    {
        // set primaryText's y
        snprintf(key, 128, "%s.singleY", item->path);
        primaryTextOffsetY = getSBIntData(dataView->app, key);
        memset(key, 0, 128);
        snprintf(key, 128, "%s.primaryY", item->path);
        setSBIntData(dataView->app, key, primaryTextOffsetY);
        // set secondaryText's alpha
        memset(key, 0, 128);
        snprintf(key, 128, "%s.singleAlpha", item->path);
        secondaryTextAlpha = getSBIntData(dataView->app, key);
        memset(key, 0, 128);
        snprintf(key, 128, "%s.secondaryAlpha", item->path);
        setSBIntData(dataView->app, key, secondaryTextAlpha);
    }
}

static H_Int16 getHeight(BaseSportSettingLayer *dataView)
{
    return dataView->containerHeight;
}

static void updatePrimaryText(BaseSportSettingLayer *dataView, char *path, SportSettingItem *item)
{
    char textId[128] = {0};
    snprintf(textId, 128, "%s.%s", path, "primaryValue");
    char *textValue = getSBStringData(dataView->app, item->name);
    setSBStringData(dataView->app, textId, textValue);
    H_FREE(textValue);
}

static void updateIcon(BaseSportSettingLayer *dataView, char *path, SportSettingItem *item)
{
    if (item->displayType != MAIN_ITEM)
    {
        return;
    }
    char iconId[128] = {0};
    snprintf(iconId, 128, "%s.%s", path, "icon");
    setSBStringData(dataView->app, iconId, item->icon);
}

char *getItemStylePath(BaseSportSettingLayer *dataView, enum SettingItemType type)
{
    for (H_Int16 i = 0; i < SETTING_ITEM_MAX; i++)
    {
        if (itemStyle[i].type == type)
        {
            return itemStyle[i].path;
        }
    }
    return H_NULL;
}

BaseSportSettingLayer *createSettingContainer(gr_application_t *app, char *layer)
{

    BaseSportSettingLayer *baseSettingLayer = H_NULL;
    if (!baseSettingLayer)
    {
        createContainer(baseSettingLayer, BaseSportSettingLayer, baseSettingLayer->container);
        baseSettingLayer->initContainer = initContainer;
        baseSettingLayer->setChild = setChild;
        baseSettingLayer->initViews = initViews;
        baseSettingLayer->initTitleView = initTitleView;
        baseSettingLayer->initItems = initItems;
        baseSettingLayer->initItem = initItem;
        baseSettingLayer->updateItemLines = updateItemLines;
        baseSettingLayer->updatePrimaryText = updatePrimaryText;
        baseSettingLayer->updateIcon = updateIcon;
        baseSettingLayer->getItemStylePath = getItemStylePath;
        baseSettingLayer->layer = layer;
        baseSettingLayer->app = app;
        baseSettingLayer->setStartY = setStartY;
        baseSettingLayer->setTitleEnable = setTitleEnable;
        baseSettingLayer->getHeight = getHeight;
        baseSettingLayer->onItemCheckedChanged = onItemCheckedChanged;

        baseSettingLayer->startY = 0;
        baseSettingLayer->isTitleEnable = true;
    }
    return baseSettingLayer;
}

static H_Int16 getItemId(BaseSportSettingLayer *settingContainer, char *id)
{

    char key[256] = {0};
    snprintf(key, sizeof(key), "%s.setting_item_", settingContainer->layer);
    H_Int16 itemId = getSportsStringUtils()->getStringId((char *)id, key);
    return itemId;
}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    BaseSportSettingLayer *settingContainer = (BaseSportSettingLayer *)container;

    if (TOUCH_DOWN == eventId)
    {
        settingContainer->pressX = info->x;
        settingContainer->pressY = info->y;
        settingContainer->isTouched = true;
    }
    else if (TOUCH_MOVE == eventId && settingContainer->isTouched)
    {
        if (abs(info->x - settingContainer->pressX) > MOTION_OFFSET || abs(info->y - settingContainer->pressY) > MOTION_OFFSET)
        {
            settingContainer->isMoved = true;
        }
    }
    else if (TOUCH_UP == eventId)
    {
        if (settingContainer->isMoved)
        {
            settingContainer->isMoved = false;
            settingContainer->isTouched = false;
        }
    }
    else if (eventId == TOUCH_SELECT)
    {
        if (settingContainer->isMoved)
        {
            settingContainer->isMoved = false;
            settingContainer->isTouched = false;
            return false;
        }
        char key[256] = {0};

        snprintf(key, sizeof(key), "%s.setting_item_", settingContainer->layer);
        H_Int16 itemId = getItemId(settingContainer, (char *)id);
        if (itemId != -1)
        {
            SportSettingItem *item = settingContainer->items + itemId;
            ScreenContainer *widget = H_NULL;
            if (strstr(id, "check") || strstr(id, "switch") || strstr(id, "radio"))
            {
                CheckBox *checkBox = (settingContainer->items + itemId)->checkBox;
                if (checkBox)
                {
                    checkBox->setOnCheckChangeCallback(checkBox, settingContainer->onItemCheckedChanged);
                    widget = (ScreenContainer *)checkBox;
                }
                return widget->handleTouchEvent(screenContext, widget, id, eventId, info);
            }
            else
            {
                settingContainer->childLayer->onItemClick(settingContainer->childLayer, item->type, item);
                return true;
            }
        }
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

// static void init(ScreenContext *screenContext, ScreenContainer *container)
// {
//     HLOG_D("MenuContainer", "%s\r\n", __func__);
// }

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{

    HLOG_W("MenuContainer", "%s\r\n", __func__);
    BaseSportSettingLayer *settingLayer = (BaseSportSettingLayer *)container;
    // delete title
    if (settingLayer->isTitleEnable)
    {
        char titleKey[128] = {0};
        snprintf(titleKey, 128, "%s.titleview", settingLayer->layer);
        gre_delete_object(settingLayer->app, titleKey);
    }
    // delete items
    for (H_Uint16 i = 0; i < settingLayer->itemCount; i++)
    {
        char key[128] = {0};
        snprintf(key, 128, "%s.setting_item_%02d", settingLayer->layer, i);
        gre_delete_object(settingLayer->app, key);
    }
    if (settingLayer->items)
    {
        H_FREE(settingLayer->items);
    }
}