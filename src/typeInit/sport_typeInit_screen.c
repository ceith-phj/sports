/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: wangxiaomeng@huami.com
 *  Maintainer: wangxiaomeng@huami.com
 *
 * =====================================================================================
 */

#include <stdbool.h>
#include <gre/gre.h>
#include <sb_screen_context.h>
#include <presenter/sportTypeInitPresenter.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_translation.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <widget/arcScrollBar/arcScrollBar.h>
#include <widget/picker/pickerWidget.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <hm_toast.h>
#include <system_locale.h>
#include "sport_utils.h"

#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif

#define TAG "SportTypeInit"

#define SCREEN_NAME SportTypeInit
#define ID_LIST "SportTypeInitLayer.list"
#define LIST_SCREEN_LAYER "SportTypeInitScreen.SportTypeInitLayer"
#define PATH_LIST_ARCSCROLLBAR "SportTypeInitScrollLayer.arcScrollBar"
#define ID_LIST_SCROLLBAR "SportTypeInitLayer.arcScrollBar"

SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    sport_type_t sportType;
    uint8_t itemLen;
    const itmeInfo *list;
    void (*saveSuccessCallBack)(void *data);
    PickerWidget *picker;
} AppData;

static void list_onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath);
static void list_onItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info);

static void createList(const itmeInfo *list, int len)
{
    Screen *screenInstance = getScreen(SCREEN_NAME);
    unequalHeiList *userList = createUnequalHeiListContainer(getScreenContext(screenInstance)->_grAppContext);
    if (userList)
    {
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "sportTypeInitStyleLayer.Title",
                "sportTypeInitStyleLayer.RadioButton",
                "sportTypeInitStyleLayer.Custom",
                "sportTypeInitStyleLayer.Tips"};

        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);

        userList->create(getScreenContext(screenInstance)->_grAppContext, userList, "SportTypeInitScreen", "SportTypeInitLayer", len, itemTemps, tempCnt, list_onItemUpdate, list_onItemTouch, "item");

        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_LIST, userList);


        for (int32_t i = 0; i < len; i++)
        {
            #ifndef HM_DISPLAY_IS_ROUND
            if(0 == i){
            HmSysStatusBar_updateTitle(System_getLocaleText(list[i].text));
            continue;
            }
            #endif
            userList->setItemTemp(userList, i, i, list[i].itemType);
        }
        #ifdef HM_DISPLAY_IS_ROUND
        userList->setItemSnapToCenter(userList, true);
        #endif
        userList->setLastEmptySpace(userList, 90);
        userList->setCircle(userList, false);
        userList->initItems(userList);
    }
}
static int createListScrollBar(gr_application_t *app)
{
    int viewHei = gre_get_layer_h(app, LIST_SCREEN_LAYER);
    int totalHei = 0;
    arcScrollBar *scroll = createArcScrollBarContainer(app);
    if (scroll)
    {

        scroll->init(app, scroll, PATH_LIST_ARCSCROLLBAR);
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_LIST_SCROLLBAR, scroll);
        #ifdef HM_DISPLAY_IS_ROUND
        scroll->setArcRange(scroll, -20, 20);
        #endif
        unequalHeiList *list = (unequalHeiList *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_LIST);
        if (list)
        {
            totalHei = list->getTotalHei(list);
        }
        scroll->setPosRange(scroll, totalHei, viewHei);
        #ifndef HM_DISPLAY_IS_ROUND
        gre_set_layer_y(app, LIST_SCREEN_LAYER,64);
        #endif
        int offset = gre_get_layer_yoffset(app, LIST_SCREEN_LAYER) ;
        scroll->setPos(scroll, offset);
    }
    return 0;
}

static void _notifyList(const itmeInfo *list, int len)
{
    if (len <= 0 || !list)
    {
        return;
    }

    Screen *screenInstance = getScreen(SCREEN_NAME);

    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    data->itemLen = len;
    data->list = list;
    createList(list, len);
    createListScrollBar(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext);
}

static void list_onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath)
{
    if (NULL == list || idx < 0 || NULL == itemGroupPath)
    {
        return;
    }
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    char key[256] = {0};
    if (idx <= data->itemLen)
    {
        snprintf(key, sizeof(key), "%s.item.text", itemGroupPath);
        setSBStringData(getAppInstance(), key, System_getLocaleText(data->list[idx].text));
    }

}

#define SWIM_PICKER_INFO 10,200,1
#define PICKER_CONTAINER_NAME "SportTypeInitpickerContainer"
static void _PickerCallback(PickerWidget *picker, pickerEventTypeE Event, pickerUpdateInfo *updateInfo)
{
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    if (PICKER_COMPLETE == Event)
    {
        /* 点击完成，用来保存数据与转跳页面 */
        SportTypeInitPresenter *presenter = (SportTypeInitPresenter *)getScreenPresenter(getScreenContext(screenInstance));
        if (presenter)
        {
            CALL_FINAL_FUNCTION(presenter, SportTypeInitPresenter, saveCustomConfig)
            (data->sportType, picker->tool.getNumberByIndex(picker->index[0], SWIM_PICKER_INFO), picker->index[1]);
        }
        data->saveSuccessCallBack(&data->sportType);
        return;
    }
    else if (PICKER_CANCEL == Event)
    {
        /* 滑动取消，建议不保存，移除container*/
        removeContainer(getScreenContext(screenInstance), PICKER_CONTAINER_NAME);
        data->picker = NULL;
        gre_redraw(getScreenContext(screenInstance)->_grAppContext);
        return;
    }
    else if (PICKER_UPDATE == Event)
    {
        /* 数据更新，用来处理非法值或更新副标题*/
        if (updateInfo->column == 1 && updateInfo->value == 2)
        {
            picker->index[1] = 0;
            picker->setValue(picker, true);
        }
        return;
    }
}

static void _openPicker(ScreenContext *context)
{
    /* 创建picker */
    PickerWidget *picker = creatPickerWidget(context);
    addContainer(context, PICKER_CONTAINER_NAME, picker);
    /* 初始化数据 */
    /* 设置picker初始化数据 subTitle 不需的话要填 NULL */
    pickerInitData data = {
        .type = PICKER_DOUBLE,
        .subTitle = NULL,
        .title = (char *)System_getLocaleText("id_sportInit_swimLengthCustom"),
    };
    /* 初始化数据 */
    uint16_t item1IndexNum = picker->tool.getIndexMax(SWIM_PICKER_INFO);    //使用工具获取第一列项目数，输入参数必须“合法合理”
    #define item2IndexNum 2
    /* 填充列表信息， 支持参数 连接符，单位，循环，是否为字符串 */
    pickerItemInfo item1 = {.length = item1IndexNum, .connector = NULL, .unit = NULL, .supportLoop = true, .isString = false};
    pickerItemInfo item2 = {.length = item2IndexNum, .connector = NULL, .unit = NULL, .supportLoop = false, .isString = true};
    /* 填充列表值 ，直接填充，或使用工具填充 */
    char *itemStrs1 = H_MALLOC(SELECTOR_TEXT_MAX * item1IndexNum);
    char itemStrs2[item2IndexNum][SELECTOR_TEXT_MAX] = {0};
    snprintf(itemStrs2[0], SELECTOR_TEXT_MAX, "%s", System_getLocaleText("id_sport_chart_altitude_matric_unit"));
    snprintf(itemStrs2[1], SELECTOR_TEXT_MAX, "%s", System_getLocaleText("id_sport_unit_yard"));
    if (picker->tool.initNumberData(itemStrs1, SWIM_PICKER_INFO) != item1IndexNum)
        HLOG_W("TAG", "initNumberData Not meeting expectations");

    item1.itemStrs = (char *)itemStrs1;
    item2.itemStrs = (char *)itemStrs2;
    data.itemInfo[0] = item1;
    data.itemInfo[1] = item2;
    /* 必须先填充数据，再初始化 */
    picker->init(context, &data, picker);
    picker->index[0] = picker->tool.getNumberIndex(50, SWIM_PICKER_INFO);
    picker->index[1] = 0;
    picker->setValue(picker, false);
    picker->setCallback(picker, _PickerCallback);
    picker->show(picker);
    /* 保存handle */
    AppData *appData = getScreenData(context, AppData);
    appData->picker = picker;
    /* 数据初始化之后，填充的数据可以释放 */
    H_FREE(itemStrs1);
}

static void list_onItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if (NULL == list || idx <= 0)
    {
        return;
    }
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    if (eventId != TOUCH_SELECT)
    {
        return;
    }
    if (data->list[idx].itemType == SPORTTYPEINITITEM_RADIO)
    {
        SportTypeInitPresenter *presenter = (SportTypeInitPresenter *)getScreenPresenter(getScreenContext(screenInstance));
        char text[256] = {0};
        char item_path[256] = {0};
        list->getItemGroupPath(list, idx, item_path, sizeof(item_path));
        snprintf(text, 256, "%s.item.radio", item_path);
        setSBStringData(getAppInstance(), (const char *)text, "images/widget/radio/radio_select_base.png");
        snprintf(text, 256, "%s.item.radioOval", item_path);
        setSBStringData(getAppInstance(), (const char *)text, "images/widget/radio/radio_select_oval.png");
        if (presenter)
        {
            CALL_FINAL_FUNCTION(presenter, SportTypeInitPresenter, saveConfig)
            (data->sportType, data->list[idx].value);
        }
        data->saveSuccessCallBack(&data->sportType);
        return;
    }
    else if (data->list[idx].itemType == SPORTTYPEINITITEM_CUSTOM)
    {
        /* Toast_show("custom not support");
        HLOG_D(TAG, "custom not support"); */
        _openPicker(getScreenContext(screenInstance));

        return;
    }
}


static SportsTypeInitCallback listCallback = {
    .notifyList = _notifyList,
};

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }
    if(data)
    {
        appData->sportType = ((sportInitData *)data)->type;
        appData->saveSuccessCallBack = ((sportInitData *)data)->callBack;
    }
    else
    {
        HLOG_W(TAG, "%s creat data NULL , goto HOME", __func__);
        getScreenManager()->gotoHome();
        return;
    }

    //传入运动类型，在presenter中返回对应的类型列表
    SportTypeInitPresenter *sportListPresenter = SportTypeInitPresenter_new();
    Presenter *basePresenter = SUPER_PTR(sportListPresenter, Presenter);
    CALL_FUNCTION(basePresenter, Presenter, init)
    (basePresenter);
    CALL_FINAL_FUNCTION(sportListPresenter, SportTypeInitPresenter, setCallback)
    (sportListPresenter, &listCallback);
    setScreenPresenter(screenContext, sportListPresenter);
    CALL_FINAL_FUNCTION(sportListPresenter, SportTypeInitPresenter, requestList)
    (sportListPresenter, appData->sportType);

    //open GPS 接力gps不关
    CALL_FINAL_FUNCTION(sportListPresenter, SportTypeInitPresenter, openGPS)
    (getScreenPresenter(screenContext));
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);

    SportTypeInitPresenter *presenter = (SportTypeInitPresenter *)getScreenPresenter(screenContext);
    if (presenter)
    {
        //close gps
        CALL_FINAL_FUNCTION(presenter, SportTypeInitPresenter, closeGPS)
        (presenter);
        //close list
        //AppData *appData = getScreenData(screenContext, AppData);
        //destory and free presenter
        Presenter *basePresenter = SUPER_PTR(presenter, Presenter);
        CALL_VOID(destory, basePresenter);
        freeScreenPresenter(screenContext, SportTypeInitPresenter);
    }

    //free screen
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
    HLOG_D(TAG, "%s\r\n", __func__);
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    AppData *data = getScreenData(screenContext, AppData);
    if (data->picker != NULL)
    {
        data->picker->container.handleGestureEvent(screenContext, (ScreenContainer *)data->picker, id, eventId);
        return true;
    }
    if (eventId == GESTURE_RIGHT)
    {
        getScreenManager()->finishCurrentScreen();
    }
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    //HLOG_D(TAG, "%s :%s\r\n", __func__,event);

    AppData *data = getScreenData(screenContext, AppData);
    if (data->picker != NULL)
    {
        data->picker->container.handleEvent(screenContext, (ScreenContainer *)data->picker, id, event);
        return true;
    }
    if (!strcmp(event, "scroll_change"))
    {
        arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_LIST_SCROLLBAR);
        if (scroll)
        {
            int offset = gre_get_layer_yoffset(screenContext->_grAppContext, LIST_SCREEN_LAYER);
            scroll->setPos(scroll, offset);
        }
    }
    else if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        unequalHeiList *list = (unequalHeiList *)findContainer(screenContext, ID_LIST);
        if (list)
        {
            list->container.handleEvent(screenContext, &list->container, id, event);
        }
    }
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    AppData *data = getScreenData(screenContext, AppData);
    if (data->picker != NULL)
    {
        data->picker->container.handleTouchEvent(screenContext, (ScreenContainer *)data->picker, id, eventId, info);
        return true;
    }
    ScreenContainer *list = findContainer(screenContext, ID_LIST);
    if (list)
    {
        list->handleTouchEvent(screenContext, list, id, eventId, info);
        return true;
    }
    return false;
}
