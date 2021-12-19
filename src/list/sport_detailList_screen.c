#include <stdbool.h>
#include <gre/gre.h>
#include <grelib.h>
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
#include "sport_utils.h"
#include "typeInit/sport_typeInit_dialog.h"
#include "packages/apps/sports/include/utils/sports_list_page_data.h"
#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif


#define TAG "SportList"

#define SCREEN_NAME SportDetailList
#define ID_SPORTLIST_LIST "SportDetailListLayer.sportlist"
#define SPORTLIST_SCREEN_LAYER "SportDetailListScreen.SportDetailListLayer"
#define PATH_SPORTLIST_ARCSCROLLBAR "SportDetailListScrollLayer.arcScrollBar"
#define ID_SPORTLIST_SCROLLBAR "SportDetailListLayer.arcScrollBar"
#define DETAIL_ICON_SIZE "SportListItemStyleLayer.list_content.item.detaillisticonsize"

SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    // bool isStart;
    bool isEditStatus;
    sport_category category;
    int *types;
    int type_len;
    sportListPresenter *presenter;
    int select_sport_type;
    AlertDialog *dialogHandle;
    int yoffset;
} AppData;

typedef struct _listConfig
{
    sport_category sportCategory; /**@ 列表配置. */
    list_type listType;           /**@ 列表类型. */
} listConfig;

static const listConfig sportsListConfig[SPORT_CATEGORY_MAX] = {
    {SPORT_CATEGORY_RUNAWAY, SPORT_RUNWAY_LIST},  /**< 00' 跑走类. */
    {SPORT_CATEGORY_RIDING, SPORT_RIDING_LIST},   /**< 01' 骑行类. */
    {SPORT_CATEGORY_SWIM, SPORT_SWIM_LIST},       /**< 02' 游泳类. */
    {SPORT_CATEGORY_OUTDOOR, SPORT_OUTDOOR_LIST}, /**< 03' 户外类. */
    {SPORT_CATEGORY_INDOOR, SPORT_INDOOR_LIST},   /**< 04' 室内类. */
    {SPORT_CATEGORY_DANCE, SPORT_DANCE_LIST},     /**< 05' 舞蹈类. */
    {SPORT_CATEGORY_FIGHTWITH, SPORT_FIGHT_LIST}, /**< 06' 搏击类. */
    {SPORT_CATEGORY_BALL, SPORT_BALL_LIST},       /**< 07' 球类类. */
    {SPORT_CATEGORY_WATER, SPORT_WATER_LIST},     /**< 08' 水上类. */
    {SPORT_CATEGORY_ICESNOW, SPORT_ICESNOW_LIST}, /**< 09' 冰雪类. */
    {SPORT_CATEGORY_EXTREME, SPORT_EXTREME_LIST}, /**< 10' 极限类. */
    {SPORT_CATEGORY_LEISURE, SPORT_LEISURE_LIST}, /**< 11' 休闲类. */
    {SPORT_CATEGORY_CHESS, SPORT_CHESS_LIST},     /**< 12' 棋牌类. */
    {SPORT_CATEGORY_OTHER, SPORT_OTHER_LIST},     /**< 13' 其他类. */
};

static void sportList_onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath);
static void sportList_onItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info);

static list_type _getListType(sport_category category)
{
    for (int i = 0; i < SPORT_CATEGORY_MAX; i++)
    {
        if (sportsListConfig[i].sportCategory == category)
        {
            return sportsListConfig[i].listType;
        }
    }
    return SPORT_MAIN_LIST;
}
//--------------------------------------------------------------------
static void _createSportlist(ScreenContext *screenContext)
{
    AppData *data = getScreenData(screenContext, AppData);
    #ifdef HM_DISPLAY_IS_ROUND
    setSBStringData(getAppInstance(), "SportDetailListLayer.title.text", sportType_getCategoryNameChar(data->category));
    #endif
    int len = data->type_len;
    unequalHeiList *sportlist = createUnequalHeiListContainer(screenContext->_grAppContext);
    if (sportlist)
    {
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "SportListItemStyleLayer.Content",
                "SportListItemStyleLayer.AddItem"};

        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);

        sportlist->create(screenContext->_grAppContext, sportlist, "SportDetailListScreen", "SportDetailListLayer",
                          len, itemTemps, tempCnt, sportList_onItemUpdate, sportList_onItemTouch, "item");

        addContainer(screenContext, ID_SPORTLIST_LIST, sportlist);

        if (data->isEditStatus)
        {
            sportlist->setItemTemp(sportlist, 0, len - 1, 1);
        }
        // sportlist->setItemSupportCircle(sportlist, 0, false);
        #ifdef HM_DISPLAY_IS_ROUND
        sportlist->setItemSnapToCenter(sportlist, true);
        #else
        sportlist->setItemSnapToTop(sportlist, true);
        sportlist->setSnapToTopOffset(sportlist, 64);
        #endif
        // if (!data->isEditStatus)
        // {
        //     sportlist->setCircle(sportlist, true);
        // }
        // sportlist->setLastEmptySpace(sportlist, 90);
        sportlist->setSupportPressEffect(sportlist, true);
        sportlist->initItems(sportlist);
        #ifdef HM_DISPLAY_IS_ROUND
        sportlist->moveItemToCenter(sportlist, 0, false);
        #endif
    }
}
//--------------------------------------------------------------------
static int _createSportListScrollBar(gr_application_t *app)
{
    int viewHei = gre_get_layer_h(app, SPORTLIST_SCREEN_LAYER);
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
        int offset = gre_get_layer_yoffset(app, SPORTLIST_SCREEN_LAYER);
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
    if (idx < data->type_len)
    {
        snprintf(key, sizeof(key), "%s.item.text", itemGroupPath);
        setSBStringData(getAppInstance(), key, sportType_getTypeNameChar(*(data->types + idx)));

        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.item.image", itemGroupPath);
        #ifdef HM_DISPLAY_IS_ROUND
        setSBStringData(getAppInstance(), key, sportType_getTypeIconSizeId(*(data->types + idx), 64));
        #else
        #ifndef HM_FEATURE_BLED_UI
        setSBStringData(getAppInstance(), key, sportType_getTypeIconSizeId(*(data->types + idx), 88));
        #else
        int detaillisticonsize = getSBIntData(getAppInstance(),DETAIL_ICON_SIZE);
        setSBStringData(getAppInstance(), key, sportType_getTypeIconSizeId(*(data->types + idx), detaillisticonsize));
        #endif
        #endif
        bool isExit = data->presenter->existItem(data->presenter, data->types[idx]);
        if (isExit)
        {
            memset(key, 0, sizeof(key));
            snprintf(key, sizeof(key), "%s.item.image_alpha", itemGroupPath);
            setSBIntData(getAppInstance(), key, 122);

            memset(key, 0, sizeof(key));
            snprintf(key, sizeof(key), "%s.item.text_alpha", itemGroupPath);
            setSBIntData(getAppInstance(), key, 97);

            memset(key, 0, sizeof(key));
            snprintf(key, sizeof(key), "%s.item.add_img_alpha", itemGroupPath);
            setSBIntData(getAppInstance(), key, 0);
        }
        else
        {
            memset(key, 0, sizeof(key));
            snprintf(key, sizeof(key), "%s.item.image_alpha", itemGroupPath);
            setSBIntData(getAppInstance(), key, 255);

            memset(key, 0, sizeof(key));
            snprintf(key, sizeof(key), "%s.item.text_alpha", itemGroupPath);
            setSBIntData(getAppInstance(), key, 255);

            memset(key, 0, sizeof(key));
            snprintf(key, sizeof(key), "%s.item.add_img_alpha", itemGroupPath);
            setSBIntData(getAppInstance(), key, 255);
        }
    }
}
//--------------------------------------------------------------------
static void _sportShowExemptionDialogCallback(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    AppData *data = getScreenData(contenxt, AppData);
    getSportUtils()->start(data->select_sport_type);
    /* removeSportInitAlertDialog(contenxt, self);
    data->dialogHandle = NULL; */
}
//--------------------------------------------------------------------
static bool _sportShowExemptionDialog(sport_type_t type)
{
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    switch (type)
    {
    case SPORT_TYPE_HUNTING: //打猎免责
    case SPORT_TYPE_DRIVING: //驾车免责
        if (data->dialogHandle != NULL)
        {
            removeSportInitAlertDialog(getScreenContext(screenInstance), data->dialogHandle);
            data->dialogHandle = NULL;
        }
        char screenname[256] = {0};
        getScreenManager()->getCurrentScreen(screenname, 256);
        data->dialogHandle = showSportInitAlertDialog(type, getScreenContext(screenInstance), _sportShowExemptionDialogCallback, screenname);
        return true;
    case SPORT_TYPE_CLIMB_THE_STAIRS:
    {
        if (data->presenter->isClimbTips())
        {
            if (data->dialogHandle != NULL)
            {
                removeSportInitAlertDialog(getScreenContext(screenInstance), data->dialogHandle);
                data->dialogHandle = NULL;
            }
            char screenname[256] = {0};
            getScreenManager()->getCurrentScreen(screenname, 256);
            data->dialogHandle = showSportInitAlertDialog(type, getScreenContext(screenInstance), _sportShowExemptionDialogCallback, screenname);
            return true;
        }
        else
        {
            return false;
        }
    }
    default:
        return false;
    }
}
//--------------------------------------------------------------------
static void _sportisOutOfSpaceDialogCallback(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    AppData *data = getScreenData(contenxt, AppData);
    Screen *screenInstance = getScreen(SCREEN_NAME);
    if (type == BUTTON_LEFT_CLICK)
    {
        removeSportInitAlertDialog(getScreenContext(screenInstance), data->dialogHandle);
        data->dialogHandle = NULL;
    }
    else if (type == BUTTON_RIGHT_CLICK)
    {
        if (_sportShowExemptionDialog(data->select_sport_type) == false)
            getSportUtils()->start(data->select_sport_type);
    }
}
static void _sportisNotEnoughSpaceDialogCallback(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    AppData *data = getScreenData(contenxt, AppData);
    if (_sportShowExemptionDialog(data->select_sport_type) == false)
        getSportUtils()->start(data->select_sport_type);
}
static bool _sportShowDialog(sport_type_t type)
{
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    if (data->presenter->isOutOfSpace())
    {
        /* 储存空间不足 */
        data->dialogHandle = showSportInitAlertDialog(SPORT_TYPE_31, getScreenContext(screenInstance), _sportisOutOfSpaceDialogCallback, "SportListScreen");
        return true;
    }
    if (data->presenter->isNotEnoughSpace())
    {
        /* 空间即将用尽 */
        data->dialogHandle = showSportInitAlertDialog(SPORT_TYPE_32, getScreenContext(screenInstance), _sportisNotEnoughSpaceDialogCallback, "SportListScreen");
        return true;
    }

    return _sportShowExemptionDialog(type);
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
    //TODO 判断ITEM是否存在主列表中
    if (data->isEditStatus)
    {
        bool isExit = data->presenter->existItem(data->presenter, data->types[idx]);
        if (isExit)
        {
            Toast_show(System_getLocaleText("id_sport_tips_have"));
            return;
        }
    }

    char text[128] = {0};
    {
        data->select_sport_type = *(data->types + idx);
        snprintf(text, sizeof(text), "%d clicked.", *(data->types + idx));
        HLOG_D(TAG, "%s touch index:%d, type: %d, type name: %s\r\n", "sportList_onItemTouch", idx, data->select_sport_type, sportType_getTypeNameChar(data->select_sport_type));
        if (!data->isEditStatus)
        {
            if (_sportShowDialog(data->select_sport_type) == false)
            {
                getSportUtils()->start(data->select_sport_type);
            }
        }
        else
        {
            SportlistPageData pagedata = {0};
            pagedata.isEditStatus = H_True;
            pagedata.selectIndex = 0;
            pagedata.selectSportType = data->select_sport_type;
            pagedata.selectsportCategory = data->category;
            getScreenManager()->startScreen("SportEditSortScreen", &pagedata, sizeof(SportlistPageData));
        }
    }
}
static void _createAppData(ScreenContext *screenContext, void *data)
{
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));
    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }
    if (data == NULL)
    {
        appData->category = SPORT_CATEGORY_MAX;
    }
    else
    {
        SportlistPageData *pageData = (SportlistPageData *)data;
        appData->category = pageData->selectsportCategory;
        appData->isEditStatus = pageData->isEditStatus;
    }
}
static void _getDetailList(ScreenContext *screenContext)
{
    AppData *appData = getScreenData(screenContext, AppData);
    sportListPresenter *presenter = createPresenter_SportList();
    appData->presenter = presenter;
    presenter->getDetailList(presenter, _getListType(appData->category),
                             &appData->types, &appData->type_len);
    appData->presenter->openGPS();
}
//--------------------------------------------------------------------
static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _createAppData(screenContext, data);
    if (data != H_NULL)
    {
        _getDetailList(screenContext);
        _createSportlist(screenContext);
        _createSportListScrollBar(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext);
        #ifndef HM_DISPLAY_IS_ROUND
        AppData *data = getScreenData(screenContext, AppData);
        HmSysStatusBar_updateTitle(sportType_getCategoryNameChar(data->category));
        #endif

    }
}

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
    gre_set_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER, appdata->yoffset);
    arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
    if (scroll)
    {
        int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
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
        appdata->category = restoreData->category;
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
        saveData->category = appdata->category;
        saveData->isEditStatus = appdata->isEditStatus;
        int yoffset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
        saveData->yoffset = yoffset;
        getScreenManager()->saveScreenData(getScreen(SCREEN_NAME)->id, saveData, sizeof(AppData));
        H_FREE(saveData);
        saveData = NULL;
    }
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _restoreYoffset(screenContext, data);
    if (data != H_NULL)
    {
        _getDetailList(screenContext);
        _createSportlist(screenContext);
        _createSportListScrollBar(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext);
        _scrollYoffset(screenContext);
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
// static void _gotoCategoryScreen(ScreenContext *screenContext, AppData *data)
// {
//     SportlistPageData pageData = {0};
//     pageData.isEditStatus = data->isEditStatus;
//     ScreenPathTranslation transition = createPathTransition(200, LINEAR, DELTA, LEFT, BOTH);
//     getScreenManager()->setPriority(PRIORITY_SCREEN_LOW);
//     getScreenManager()->setScreenTransition(&transition, sizeof(ScreenPathTranslation));
//     getScreenManager()->startScreenByFlag("SportCategoryListScreen", FLAG_SCREEN_REMOVE_CURRENT, &pageData, sizeof(SportlistPageData));
// }
//--------------------------------------------------------------------
static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    if (eventId == GESTURE_RIGHT)
    {
        AppData *data = getScreenData(screenContext, AppData);
        if (data->dialogHandle != H_NULL && data->dialogHandle->isShowing(data->dialogHandle) == true)
        {
            if (data->dialogHandle->container.handleGestureEvent(screenContext, &data->dialogHandle->container, id, eventId) == true)
            {
                removeSportInitAlertDialog(screenContext, data->dialogHandle);
            }
            return true;
        }
        else
        {
            getScreenManager()->finishCurrentScreen();
            return true;
        }
    }
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    //HLOG_D(TAG, "%s :%s\r\n", __func__,event);
    if (!strcmp(event, "scroll_change"))
    {
        arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
        if (scroll)
        {
            int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
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

    AppData *data = getScreenData(screenContext, AppData);
    if (data->dialogHandle != H_NULL && strstr(id, data->dialogHandle->getAlertDialogId(data->dialogHandle)))
    {
        data->dialogHandle->container.handleTouchEvent(screenContext, &data->dialogHandle->container, id, eventId, info);
        return true;
    }
    ScreenContainer *settingList = findContainer(screenContext, ID_SPORTLIST_LIST);
    if (settingList)
    {
        settingList->handleTouchEvent(screenContext, settingList, id, eventId, info);
        return true;
    }
    return false;
}
