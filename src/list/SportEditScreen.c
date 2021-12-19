
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
#include "sport_utils.h"
#include "uiframework/ui_adapter/storyboard/engine/include/widget/sortList/sortList.h"
#include "widget/alertDialog/alertDialog.h"
#include "packages/apps/sports/include/utils/sports_list_page_data.h"
#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif

//================================================
#define TAG "SportEdit"
#define SCREEN_NAME SportEdit
SCREEN_DECLARE(SCREEN_NAME);
//================================================
//================================================
#define ID_SPORT_EDIT_LIST_LAYER "SportEditScreen.sport_edit_list_layer"
#define ID_SPORT_EDIT_LIST "sport_edit_list_layer.list_layer"
#define ID_SPROT_EDIT_ADD_BTN "sport_edit_btm_layer.add_btn"
#define SPORTS_LIST_MAX_NUM (50)
//================================================
//================================================
typedef struct
{
    bool isStart;
    int *types;
    int type_len;
    int select_sport_type;
    AlertDialog *dialogHandle;
    sportListPresenter *presenter;
    int yoffset;
} AppData;
//------------------------------------------------
static void _edit_itemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath)
{
    char path[256];
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);

    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "%s.item.content", itemGroupPath);
    setSBStringData(getAppInstance(), path, sportType_getTypeNameChar(*(data->types + idx)));

    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "%s.item.img", itemGroupPath);
    setSBStringData(getAppInstance(), path, sportType_getTypeIconSizeId(*(data->types + idx), 64));
}
//------------------------------------------------
static void _edit_itemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if (NULL == list || idx < 0)
    {
        return;
    }
    if (eventId != TOUCH_SELECT)
    {
        return;
    }
    SportlistPageData pageData = {0};
    pageData.isEditStatus = H_False;
    pageData.selectIndex = idx;
    getScreenManager()->startScreen("SportEditSortScreen", &pageData, sizeof(SportlistPageData));
}
//------------------------------------------------
static void _createSportList(ScreenContext *sc)
{
    #ifdef HM_DISPLAY_IS_ROUND
    setSBStringData(getAppInstance(), "sport_edit_list_layer.title.text", System_getLocaleText("id_sport_edit"));
    #endif
    AppData *data = getScreenData(sc, AppData);
    int len = data->type_len;
    unequalHeiList *editlist = createUnequalHeiListContainer(sc->_grAppContext);
    if (editlist)
    {
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "sport_edit_tmp_style_layer.normal_item"}; // "sport_edit_tmp_style_layer.focus_item",

        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);

        editlist->create(sc->_grAppContext, editlist, "SportEditScreen", "sport_edit_list_layer", len,
                         itemTemps, tempCnt, _edit_itemUpdate, _edit_itemTouch, "item");

        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_EDIT_LIST, editlist);

        unequalHeiList *list = (unequalHeiList *)findContainer(sc, ID_SPORT_EDIT_LIST);
        if (list)
        {
            // list->setItemSupportCircle(list, 0, false);
            list->setItemTemp(list, 0, len - 1, 0);
            #ifdef HM_DISPLAY_IS_ROUND
            list->setItemSupportCircle(list, len - 1, false);
            list->setItemSnapToCenter(list, true);
            #else
            list->setItemSnapToTop(list, true);
            list->setSnapToTopOffset(list, 64);
            #endif
            // list->setCircle(list, true);
            // list->setLastEmptySpace(list, 100);
            list->setSupportPressEffect(list, true);
            list->initItems(list);
            #ifdef HM_DISPLAY_IS_ROUND
            list->moveItemToCenter(list, 0, false);
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
//---------------------------------------------
static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    _createAppData(screenContext, data);
    _getMainList(screenContext);
    _createSportList(screenContext);
    #ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_updateTitle(System_getLocaleText("id_sport_edit"));
    #endif

}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, " %s\r\n", __func__);
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
    gre_set_layer_yoffset(screenContext->_grAppContext, ID_SPORT_EDIT_LIST_LAYER, appdata->yoffset);
    // arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
    // if (scroll)
    // {
    //     int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
    //     scroll->setPos(scroll, offset);
    // }
    unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORT_EDIT_LIST);
    if (sportList != H_NULL)
    {
        sportList->updateBotPaddingY(sportList);
        sportList->updateScrolling(sportList);
    }
}
static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_D(TAG, " %s\r\n", __func__);
    System_setKeyFilter(HM_KEY_CODE_HOME, HM_KEY_ACT_FILTER_CLICK);
}
//------------------------------
static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, " %s\r\n", __func__);
    System_resetKeyFilter(HM_KEY_CODE_HOME);
}
//------------------------------
static void _restoreYoffset(ScreenContext *screenContext, void *data)
{
    if (data != H_NULL)
    {
        AppData *restoreData = (AppData *)data;
        AppData *appdata = getScreenData(screenContext, AppData);
        appdata->yoffset = restoreData->yoffset;
    }
}
//------------------------------
static void _saveYoffset(ScreenContext *screenContext)
{
    AppData *saveData = (AppData *)H_MALLOC(sizeof(AppData));
    if (saveData != H_NULL)
    {
        memset(saveData, 0, sizeof(AppData));
        int yoffset = gre_get_layer_yoffset(screenContext->_grAppContext, ID_SPORT_EDIT_LIST_LAYER);
        saveData->yoffset = yoffset;
        getScreenManager()->saveScreenData(getScreen(SCREEN_NAME)->id, saveData, sizeof(AppData));
        H_FREE(saveData);
        saveData = NULL;
    }
}
static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, " %s\r\n", __func__);
    _restoreYoffset(screenContext, data);
    if (data != H_NULL)
    {
        _scrollYoffset(screenContext);
    }
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_D(TAG, " %s\r\n", __func__);
    _saveYoffset(screenContext);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_D(TAG, " %s\r\n", __func__);
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
    HLOG_D(TAG, " %s\r\n", __func__);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    HLOG_D(TAG, " %s\r\n", __func__);
}

static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    if (info->code == HM_KEY_CODE_HOME)
    {
        getScreenManager()->finishCurrentScreen();
        return true;
    }
    return false;
}
static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    if (eventId == GESTURE_RIGHT)
    {
        AppData *data = getScreenData(screenContext, AppData);
        if (data->dialogHandle != H_NULL && data->dialogHandle->isShowing(data->dialogHandle) == true)
        {
            if (data->dialogHandle->container.handleGestureEvent(screenContext, &data->dialogHandle->container, id, eventId) == true)
            {
                data->dialogHandle->dismissDialog(data->dialogHandle);
                removeContainer(screenContext, data->dialogHandle->getAlertDialogId(data->dialogHandle));
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
    HLOG_D(TAG, " %s\r\n", __func__);
    if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        unequalHeiList *sportList = (unequalHeiList *)findContainer(screenContext, ID_SPORT_EDIT_LIST);
        if (sportList)
        {
            sportList->container.handleEvent(screenContext, &sportList->container, id, event);
        }
    }
    return true;
}

static void _Dialog_clickCallback(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    AppData *data = getScreenData(contenxt, AppData);
    data->dialogHandle->dismissDialog(data->dialogHandle);
    removeContainer(contenxt, data->dialogHandle->getAlertDialogId(data->dialogHandle));
}
// static bool _Edit_showDialog(ScreenContext *screenContext)
// {
//     AppData *data = getScreenData(screenContext, AppData);
//     AlertDialog *alert_dialog = creatAlertDialog(screenContext, "SportEditScreen");
//     addContainer(screenContext, alert_dialog->mDialogLayerPath, alert_dialog);
//     alert_dialog->setTitleAndContent(alert_dialog, NULL, System_getLocaleText("sport_tips_edit_max_num")); //
//     alert_dialog->setPositiveButton(alert_dialog, H_NULL);
//     alert_dialog->setClickListener(alert_dialog, _Dialog_clickCallback);
//     alert_dialog->showDialog(alert_dialog);
//     data->dialogHandle = alert_dialog;
//     return true;
// }

bool _SportEdit_showDialog(ScreenContext *scontext, const char *screenName, const char *content, DialogClickListener listener)
{
    AppData *data = getScreenData(scontext, AppData);
    AlertDialog *alert_dialog = creatAlertDialog(scontext, (char *)screenName);
    addContainer(scontext, alert_dialog->mDialogLayerPath, alert_dialog);
    alert_dialog->setTitleAndContent(alert_dialog, NULL, content); //System_getLocaleText("sport_tips_edit_max_num")
    alert_dialog->setPositiveButton(alert_dialog, H_NULL);
    alert_dialog->setClickListener(alert_dialog, listener);
    alert_dialog->showDialog(alert_dialog);
    data->dialogHandle = alert_dialog;
    return true;
}
static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    AppData *data = getScreenData(screenContext, AppData);
    if (data->dialogHandle != H_NULL && strstr(id, data->dialogHandle->getAlertDialogId(data->dialogHandle)))
    {
        data->dialogHandle->container.handleTouchEvent(screenContext, &data->dialogHandle->container, id, eventId, info);
        return true;
    }
    if (!strcmp(id, ID_SPROT_EDIT_ADD_BTN) && (eventId == TOUCH_SELECT))
    {
        AppData *tmpData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)), AppData);
        int len = tmpData->type_len;
        HLOG_D(TAG, "len：%d \r\n", len);
        if (len == SPORTS_LIST_MAX_NUM)
        {
            return _SportEdit_showDialog(screenContext, "SportEditScreen", System_getLocaleText("id_sport_tips_maxnum"), _Dialog_clickCallback);
        }
        SportlistPageData pageData = {0};
        pageData.isEditStatus = H_True;
        getScreenManager()->startScreen("SportCategoryListScreen", &pageData, sizeof(SportlistPageData));
    }
    else
    {
        ScreenContainer *settingList = findContainer(screenContext, ID_SPORT_EDIT_LIST);
        if (settingList)
        {
            settingList->handleTouchEvent(screenContext, settingList, id, eventId, info);
            return true;
        }
    }
    return false;
}
