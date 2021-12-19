#include <stdbool.h>
#include <gre/gre.h>
#include <sb_screen_context.h>
#include <presenter/sports_list_presenter.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include <gre/sdk/grelib.h>
#include <presenter/SportPresenter.h>
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
#include "framework/base/data/system_property.h"

#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif
#include "hmstatistics.h"

//=======================================
//=======================================
#define TAG "SportList"
#define SCREEN_NAME SportList
SCREEN_DECLARE(SCREEN_NAME);
//=======================================
//=======================================
#define TABLE_LIST "ListLayer.ListTable"
#define ID_SPORTLIST_LIST "sportListLayer.sportlist"
#define SPORTLIST_SCREEN_LAYER "SportListScreen.SportListLayer"
#define PATH_SPORTLIST_ARCSCROLLBAR "SportListScrollLayer.arcScrollBar"
#define ID_SPORTLIST_SCROLLBAR "SportListLayer.arcScrollBar"
#define DETAIL_ICON_SIZE "SportListItemStyleLayer.list_content.item.detaillisticonsize"
//=======================================
//=======================================
//================================
//================================
typedef struct
{
    bool isEditStatus;
    int *types;
    int type_len;
    int select_sport_type;
    AlertDialog *dialogHandle;
    sportListPresenter *presenter;
    int yoffset;
} AppData;

static void sportList_onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath);
static void sportList_onItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info);

static void sportList_scroll(unequalHeiList *list, int idx, const char *itemGroupPath, float percent)
{
    if (NULL == list)
    {
        return;
    }

    int itemHei = list->getItemHei(list, idx);
    float scaleFull = itemHei * 2.0f / list->layerViewHei;
    float percentMin = 0.8f;
    float percentMax = 1.0f;
    float percentCur = 0;
    char buffer[256] = {0};

    if (percent > scaleFull)
    {
        percentCur = 1.0f;
    }
    else
    {
        // [0, 1-full] --> mapto [0, 1]
        percent *= (1 / scaleFull);
        percentCur = percentMin + percent * (percentMax - percentMin);
    }

    snprintf(buffer, sizeof(buffer), "%s.item", itemGroupPath);
    gre_control_set_text_render_imagescale(list->app, buffer, percentCur * 100);
    snprintf(buffer, sizeof(buffer), "%s.item.image_width", itemGroupPath);
    setSBIntData(list->app, buffer, (int)(percentCur * 90));
    snprintf(buffer, sizeof(buffer), "%s.item.image_height", itemGroupPath);
    setSBIntData(list->app, buffer, (int)(percentCur * 90));
    arcScrollBar *scroll = (arcScrollBar *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORTLIST_SCROLLBAR);

    if (scroll)
    {
        int offset = gre_get_layer_yoffset(list->app, SPORTLIST_SCREEN_LAYER);
        scroll->setPos(scroll, offset);
    }
}

static void _createSportlist(ScreenContext *screenContext)
{
    #ifdef HM_DISPLAY_IS_ROUND
    setSBStringData(getAppInstance(), "SportListLayer.title.text", System_getLocaleText("id_select_sports"));
    #endif
    AppData *data = getScreenData(screenContext, AppData);
    int len = data->type_len;
    unequalHeiList *sportlist = createUnequalHeiListContainer(screenContext->_grAppContext);
    if (sportlist)
    {
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "SportListItemStyleLayer.list_content",
                "SportListItemStyleLayer.Icon"};

        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);

        sportlist->create(screenContext->_grAppContext, sportlist, "SportListScreen", "SportListLayer",
                          len + 2, itemTemps, tempCnt, sportList_onItemUpdate, sportList_onItemTouch, "item");

        addContainer(screenContext, ID_SPORTLIST_LIST, sportlist);

        unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORTLIST_LIST);
        if (sportList)
        {
            // sportList->setItemSupportCircle(sportList, 0, false);
            sportList->setItemTemp(sportList, len + 1, len + 1, 1);
            #ifdef HM_DISPLAY_IS_ROUND
            sportList->setItemSupportCircle(sportList, len + 1, false);
            sportList->setItemSnapToCenter(sportList, true);
            sportList->setCircle(sportList, true);
            #else
            sportList->setItemSnapToTop(sportList, true);
            sportList->setSnapToTopOffset(sportList, 64);
            #endif
            sportList->setSupportPressEffect(sportList, true);

            sportList->setScrollCallBack(sportList, sportList_scroll);
            sportList->setToInfiniteList(sportList, false);

            sportList->initItems(sportList);
            #ifdef HM_DISPLAY_IS_ROUND
            sportList->moveItemToCenter(sportList, 0, false);
            #endif
        }
    }
}
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
            //totalHei = list->getTotalHei(list);
            totalHei = list->getTotalHeightIncludePadding(list);
        }
        scroll->setPosRange(scroll, totalHei, viewHei);
        int offset = gre_get_layer_yoffset(app, SPORTLIST_SCREEN_LAYER);
        scroll->setPos(scroll, offset);
    }
    return 0;
}

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
        #ifndef HM_FEATURE_BLED_UI
        setSBStringData(getAppInstance(), key, sportType_getTypeIconSizeId(*(data->types + idx), 88));
        #else
        int detaillisticonsize = getSBIntData(getAppInstance(),DETAIL_ICON_SIZE);
        setSBStringData(getAppInstance(), key, sportType_getTypeIconSizeId(*(data->types + idx), detaillisticonsize));
        #endif
    }
    else if (idx == data->type_len)
    {
        snprintf(key, sizeof(key), "%s.item.text", itemGroupPath);
        setSBStringData(getAppInstance(), key, System_getLocaleText("id_more_sports"));

        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.item.image", itemGroupPath);
        setSBStringData(getAppInstance(), key, "images/sports/list/sport_icon_more.png");
    }
    else if (idx == data->type_len + 1)
    {
        snprintf(key, sizeof(key), "%s.item.image", itemGroupPath);
        setSBStringData(getAppInstance(), key, "images/sports/list/sport_icon_setting.png");
    }

    snprintf(key, sizeof(key), "%s.item", itemGroupPath);
    gre_control_set_text_render_toimage(getAppInstance(), key);
    gre_control_set_text_render_imagescale(getAppInstance(), key, 100);
}

static void _sportShowExemptionDialogCallback(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    AppData *data = getScreenData(contenxt, AppData);
    getSportUtils()->start(data->select_sport_type);
    //removeSportInitAlertDialog(contenxt, self);
    //data->dialogHandle = NULL;
}

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
        data->dialogHandle = showSportInitAlertDialog(type, getScreenContext(screenInstance), _sportShowExemptionDialogCallback, "SportListScreen");
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
    char text[128] = {0};
    if (idx == data->type_len)
    {
        getScreenManager()->startScreen("SportCategoryListScreen", H_NULL, 0);
        Sysprop_setInt("sportlist.list.item.type", -1);

        HLOG_D(TAG, "goto more");
    }
    else if (idx == data->type_len + 1)
    {
        getScreenManager()->startScreen("SportEditScreen", H_NULL, 0);
    }
    else
    {
        data->select_sport_type = *(data->types + idx);

        Sysprop_setInt("sportlist.list.item.type", data->select_sport_type);
        snprintf(text, sizeof(text), "%d clicked.", *(data->types + idx));
        HLOG_D(TAG, "%s touch index:%d, type: %d, type name: %s\r\n", "sportList_onItemTouch", idx, data->select_sport_type, sportType_getTypeNameChar(data->select_sport_type));
        //goSportScreen(getScreenContext(screenInstance), data->select_sport_type);
        SportStateType sport_state = getSportsScreenPresenter()->requestSportStatus();
        if (sport_state != SPORT_STATE_STOP && sport_state != SPORT_STATE_PRESTART)
        {
            HLOG_I(TAG, "sport sevice busy %d", sport_state);
            return;
        }
        if (_sportShowDialog(data->select_sport_type) == false)
        {
            getSportUtils()->start(data->select_sport_type);
            hm_statis_data_add_one_kv(SPORT_ITEM_IN_EID,SPORT_TYPE_KEY_ID,data->select_sport_type,true);
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
}
//--------------------------------------------------------------------
static void _getMainList(ScreenContext *screenContext)
{
    AppData *data = getScreenData(screenContext, AppData);
    sportListPresenter *presenter = createPresenter_SportList();
    data->presenter = presenter;
    presenter->getMainList(presenter, &data->types, &data->type_len);
    data->presenter->openGPS();
}
//--------------------------------------------------------------------
#if 0
static void _scrollYoffset(ScreenContext *screenContext)
{
    AppData *appdata = getScreenData(screenContext, AppData);
    int editType = -1;
    int idx = 0;
    unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORTLIST_LIST);

    Sysprop_getInt("sportlist.edit.item.type", &editType, -1);
    if(editType!=-1 && sportList){
        Sysprop_setInt("sportlist.edit.item.type", -1);
        for(idx=0; idx<appdata->type_len; idx++){
            if( *(appdata->types+idx)==editType){
                break;
            }
        }
        if(idx>=appdata->type_len){
            idx = 0;
        }
        sportList->moveItemToCenter(sportList, idx, false);
        appdata->yoffset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
    }

    HLOG_D(TAG, "list yoffset %d, editIndex:%d ", appdata->yoffset, editType);

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
    if (sportList != H_NULL)
    {
        sportList->updateBotPaddingY(sportList);
        sportList->updateScrolling(sportList);
    }
}
#endif

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _createAppData(screenContext, data);
    _getMainList(screenContext);
    _createSportlist(screenContext);
    _createSportListScrollBar(screenContext->_grAppContext);
    #ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_updateTitle(System_getLocaleText("id_select_sports"));
    #endif
    hm_statis_data_add_no_kv(SPORT_HOME_PAGE_EID, true);

    unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORTLIST_LIST);

    AppData *appData = getScreenData(screenContext, AppData);
    appData->yoffset = 0;

    if(sportList){
        int itemType = -1;
        int idx = 0;

        Sysprop_getInt("sportlist.edit.item.type", &itemType, -1);
        if(itemType<0){
            Sysprop_getInt("sportlist.list.item.type", &itemType, -1);
        }

        if(itemType>=0){
            Sysprop_setInt("sportlist.edit.item.type", -1);
            for(idx=0; idx<appData->type_len; idx++){
                if( *(appData->types+idx)==itemType){
                    break;
                }
            }
            if(idx>=appData->type_len){
                idx = 0;
            }
            sportList->moveItemToCenter(sportList, idx, false);
            appData->yoffset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
        }
        HLOG_D(TAG, "list yoffset %d, editIndex:%d ", appData->yoffset, itemType);
    }

    gre_set_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER, appData->yoffset);
    arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
    if (scroll)
    {
        int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
        scroll->setPos(scroll, offset);
    }
    if (sportList != H_NULL)
    {
        sportList->updateBotPaddingY(sportList);
        sportList->updateScrolling(sportList);
    }

    #ifdef HM_DISPLAY_IS_ROUND
    gre_set_layer_active(screenContext->_grAppContext, "SportListScreen.ShadeLayer", 0);
    #endif
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{

}
//------------------------------
static void onDataSave(ScreenContext *screenContext)
{

}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    hm_statis_data_add_no_kv(SPORT_QUIT_EID, true);
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
        return true;
    }
    return false;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    // HLOG_D(TAG, "%s: id: %s, event Id:%d\r\n", __func__, id, eventId);
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
