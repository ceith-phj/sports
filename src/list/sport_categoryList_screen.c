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
#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif

#define TAG "SportCategoryList"

#define SCREEN_NAME SportCategoryList
#define TABLE_LIST "ListLayer.ListTable"
#define ID_SPORTLIST_LIST "SportCategoryListLayer.sportCategoryList"
#define SPORTCATEGORYLIST_SCREEN_LAYER "SportCategoryListScreen.SportCategoryListLayer"
#define PATH_SPORTLIST_ARCSCROLLBAR "SportCategoryListScrollLayer.arcScrollBar"
#define ID_SPORTLIST_SCROLLBAR "SportCategoryListLayer.arcScrollBar"


SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    bool isEditStatus;
    sport_category_item *list;
    int type_len;
    int selectsportCategory;
    sportListPresenter *presenter;
    int yoffset;
} AppData;
//--------------------------------------------------------------------
static int _createSportListScrollBar(gr_application_t *app)
{
    int viewHei = gre_get_layer_h(app, SPORTCATEGORYLIST_SCREEN_LAYER);
    int totalHei = 0;
    arcScrollBar *scroll = createArcScrollBarContainer(app);
    if (scroll)
    {

        scroll->init(app, scroll, PATH_SPORTLIST_ARCSCROLLBAR);
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORTLIST_SCROLLBAR, scroll);
        scroll->setArcRange(scroll, -20, 20);
        unequalHeiList *list = (unequalHeiList *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORTLIST_LIST);
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
//--------------------------------------------------------------------
static void sportList_onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath)
{
    if (NULL == list || idx < 0 || NULL == itemGroupPath)
    {
        return;
    }
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    char key[256] = {0};
    snprintf(key, sizeof(key), "%s.item.title", itemGroupPath);
    setSBStringData(getAppInstance(), key, sportType_getCategoryNameChar(data->list[idx].category));

    memset(key, 0, sizeof(key));
    snprintf(key, sizeof(key), "%s.item.subtitle", itemGroupPath);
    char value[100] = {0};
    snprintf(value, sizeof(value), System_getLocaleText("id_sports_number"), data->list[idx].size);
    setSBStringData(getAppInstance(), key, value);
}
//--------------------------------------------------------------------
static void goSportDetailListScreen(ScreenContext *screenContext, sport_category category, bool isEditStatus)
{
    SportlistPageData data = {0};
    data.selectsportCategory = category;
    data.isEditStatus = isEditStatus;
    getScreenManager()->startScreen("SportDetailListScreen", &data, sizeof(SportlistPageData));
}
//--------------------------------------------------------------------
static void sportList_onItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if (NULL == list || idx < 0)
    {
        return;
    }
    if (eventId != TOUCH_SELECT)
    {
        return;
    }
    Screen *screenInstance = getScreen(SCREEN_NAME);

    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);

    data->selectsportCategory = data->list[idx].category;
    HLOG_I(TAG, "goto %s", sportType_getCategoryNameChar(data->selectsportCategory));
    goSportDetailListScreen(getScreenContext(screenInstance), data->selectsportCategory, data->isEditStatus);
}
//--------------------------------------------------------------------
static void _createSportlist(ScreenContext *screenContext)
{
    AppData *data = getScreenData(screenContext, AppData);
    #ifdef HM_DISPLAY_IS_ROUND
    if (data->isEditStatus)
    {
        setSBStringData(getAppInstance(), "SportCategoryListLayer.title.text", System_getLocaleText("id_sport_add_more"));
    }
    else
    {
        setSBStringData(getAppInstance(), "SportCategoryListLayer.title.text", System_getLocaleText("id_more_sports"));
    }
    #endif
    int len = data->type_len;
    unequalHeiList *sportlist = createUnequalHeiListContainer(screenContext->_grAppContext);
    if (sportlist)
    {
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "SportListItemStyleLayer.RichContent"};

        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);

        sportlist->create(screenContext->_grAppContext, sportlist,
                          "SportCategoryListScreen", "SportCategoryListLayer",
                          len, itemTemps, tempCnt, sportList_onItemUpdate, sportList_onItemTouch, "item");

        addContainer(screenContext, ID_SPORTLIST_LIST, sportlist);

        unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORTLIST_LIST);
        if (sportList)
        {
            #ifdef HM_DISPLAY_IS_ROUND
            sportList->setItemSnapToCenter(sportList, true);
            #else
            sportList->setItemSnapToTop(sportList, true);
            sportList->setSnapToTopOffset(sportList, 64);
            #endif
            //sportList->setCircle(sportList, true);
            // sportList->setLastEmptySpace(sportList, 90);
            sportList->setSupportPressEffect(sportList, true);
            sportList->initItems(sportList);
            #ifdef HM_DISPLAY_IS_ROUND
            sportList->moveItemToCenter(sportList, 0, false);
            #endif
        }
    }
}
//--------------------------------------------------------------------
static void _createAppData(ScreenContext *screenContext, void *data)
{
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }
    if (data != H_NULL)
    {
        SportlistPageData *pageData = (SportlistPageData *)data;
        appData->isEditStatus = pageData->isEditStatus;
    }
}
//--------------------------------------------------------------------
static void _getCategoryList(ScreenContext *screenContext)
{
    AppData *data = getScreenData(screenContext, AppData);
    sportListPresenter *presenter = createPresenter_SportList();
    data->presenter = presenter;
    presenter->getCategoryList(presenter, &data->list, &data->type_len);
    data->presenter->openGPS();
}
//--------------------------------------------------------------------
static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _createAppData(screenContext, data);
    _getCategoryList(screenContext);
    _createSportlist(screenContext);
    _createSportListScrollBar(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext);
    #ifndef HM_DISPLAY_IS_ROUND
    if (getScreenData(screenContext, AppData)->isEditStatus){
        HmSysStatusBar_updateTitle(System_getLocaleText("id_sport_add_more"));
    }
    else{
        HmSysStatusBar_updateTitle(System_getLocaleText("id_more_sports"));
    }
    #endif

}
//-------------------------------------------
static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}
//-------------------------------------------
static void _scrollYoffset(ScreenContext *screenContext)
{
    AppData *appdata = getScreenData(screenContext, AppData);
    HLOG_D(TAG, "category list yoffset %d  ", appdata->yoffset);
    if (appdata->yoffset == 0)
    {
        return;
    }
    gre_set_layer_yoffset(screenContext->_grAppContext, SPORTCATEGORYLIST_SCREEN_LAYER, appdata->yoffset);
    arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
    if (scroll)
    {
        int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTCATEGORYLIST_SCREEN_LAYER);
        scroll->setPos(scroll, offset);
    }
    unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORTLIST_LIST);
    if (sportList != H_NULL)
    {
        sportList->updateBotPaddingY(sportList);
        sportList->updateScrolling(sportList);
    }
}
static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}
//------------------------------
static void _restoreYoffset(ScreenContext *screenContext, void *data)
{
    if (data != H_NULL)
    {
        AppData *restoreData = (AppData *)data;
        AppData *appdata = getScreenData(screenContext, AppData);
        appdata->isEditStatus = restoreData->isEditStatus;
        appdata->yoffset = restoreData->yoffset;
    }
}
//------------------------------
static void _saveYoffset(ScreenContext *screenContext)
{
    AppData *appdata = getScreenData(screenContext, AppData);
    AppData *saveData = (AppData *)H_MALLOC(sizeof(AppData));
    if (saveData != H_NULL)
    {
        memset(saveData, 0, sizeof(AppData));
        saveData->isEditStatus = appdata->isEditStatus;
        int yoffset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTCATEGORYLIST_SCREEN_LAYER);
        saveData->yoffset = yoffset;
        getScreenManager()->saveScreenData(getScreen(SCREEN_NAME)->id, saveData, sizeof(AppData));
        H_FREE(saveData);
        saveData = NULL;
    }
}
//--------------------------------------------------------------------
static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _restoreYoffset(screenContext, data);
    if (data != H_NULL)
    {
        _scrollYoffset(screenContext);
        #ifdef HM_DISPLAY_IS_ROUND
        if (getScreenData(screenContext, AppData)->isEditStatus){
            setSBStringData(getAppInstance(), "SportCategoryListLayer.title.text", System_getLocaleText("id_sport_add_more"));
        }
        else{
            setSBStringData(getAppInstance(), "SportCategoryListLayer.title.text", System_getLocaleText("id_more_sports"));
        }
        #endif
    }
}
static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _saveYoffset(screenContext);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    AppData *appData = getScreenData(screenContext, AppData);
    if (appData != H_NULL && appData->presenter != H_NULL)
    {
        appData->presenter->closeGPS();
        appData->presenter->destory(appData->presenter);
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
//--------------------------------------------------------------------
static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    if (eventId == GESTURE_RIGHT)
    {
        getScreenManager()->finishCurrentScreen();
    }
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    if (!strcmp(event, "scroll_change"))
    {
        arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
        if (scroll)
        {
            int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTCATEGORYLIST_SCREEN_LAYER);
            scroll->setPos(scroll, offset);
        }
    }
    else if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORTLIST_LIST);
        if (sportList)
        {
            sportList->container.handleEvent(screenContext, &sportList->container, id, event);
        }
    }
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    //HLOG_D(TAG, "%s: id: %s, event Id:%d\r\n", __func__, id, eventId);
    ScreenContainer *list = findContainer(screenContext, ID_SPORTLIST_LIST);
    if (list)
    {
        list->handleTouchEvent(screenContext, list, id, eventId, info);
        return true;
    }
    return false;
}
