
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
#include "widget/alertDialog/alertDialog.h"
#include "uiframework/ui_adapter/storyboard/engine/include/widget/sortList/sortList.h"
#include "packages/apps/sports/include/utils/sports_list_page_data.h"
#include "framework/base/data/system_property.h"
#ifndef HM_DISPLAY_IS_ROUND
#include <hm_system_status_bar.h>
#endif
//================================================
#define TAG "SportEdit"
#define SCREEN_NAME SportEditSort
SCREEN_DECLARE(SCREEN_NAME);
//================================================
//================================================
#define ID_SPORT_EDIT_TITLE "sort_title_layer.title.title"
#define ID_SPROT_EDIT_DELETE_BTN "sort_btm_layer.delete_btn"
#define ID_SPORT_EDIT_SURE_BTN "sort_btm_layer.sure_btn"
#define ID_SPORT_EDIT_SAVE_BTN "sort_list_save_layer.save_btn"
#define ID_SPORT_EDIT_DISCARD_BTN "sort_list_save_layer.discard_btn"
#define SPORT_EDIT_SORT_NORMAL_ITEM_STYLE "sort_list_tmp_style_layer.normal_item" //.item
#define SPORT_EDIT_SORT_FOCUS_ITEM_STYLE "sort_list_tmp_style_layer.focus_item"   //.item

#define ID_SPORT_EDIT_SORT_LIST_LAYER "sort_list_layer"
#define ID_SPORT_EDIT_SORT_LIST "sort_list_layer.listlayer"
#define SPORTS_LIST_MIN_NUM (1)
//================================================
//================================================
// extern bool _SportEdit_showDialog(ScreenContext *scontext, const char *screenName, const char *content, DialogClickListener listener);
//================================================
//================================================
typedef struct
{
    // bool isStart;
    bool isEditStatus;
    sport_category category;
    int *types;
    int type_len;
    int select_sport_type;
    sportListPresenter *presenter;
    AlertDialog *dialogHandle;
    int selectIndex;
    int selectType;
    int yoffset;
} AppData;
//===============================================
#define SP_EDIT_ARRAY_MAX (50)
static int sSpTypeArr[SP_EDIT_ARRAY_MAX] = {0};
//===============================================
//------------------------------------------------
static void _onItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath)
{
    if (NULL == list || idx < 0 || NULL == itemGroupPath)
    {
        return;
    }
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
static H_VOID _onSortItemUpdate(SortList *list, int idx, const char *itemGroupPath)
{
    HLOG_D(TAG, "!!!! idx:%d itemgroupath:%s \r\n", idx, itemGroupPath);
    Screen *screenInstance = getScreen(SCREEN_NAME);
    AppData *data = getScreenData(getScreenContext(screenInstance), AppData);
    data->selectIndex = idx;
    data->selectType = *(data->types + idx);
    char path[256];
    snprintf(path, sizeof(path), "%s.item.content", itemGroupPath);
    setSBStringData(getAppInstance(), path, sportType_getTypeNameChar(*(data->types + idx)));

    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "%s.item.img", itemGroupPath);
    setSBStringData(getAppInstance(), path, sportType_getTypeIconSizeId(*(data->types + idx), 64));
}
//------------------------------------------------
static void _createSortList(ScreenContext *sc)
{
    int len = 0;
    AppData *data = getScreenData(sc, AppData);
    len = data->type_len;
    if (data->isEditStatus)
    {
        sSpTypeArr[0] = data->select_sport_type;
        data->selectIndex = 0;
        len++;
    }
    SortList *sortList = createSortContainer(sc->_grAppContext);
    //设置每个item更新的listener，用来填充每行内容
    sortList->setItemUpdateListener(sortList, _onItemUpdate);
    //设置每个item的样式
    // sortList->setItemTemplate(sortList, SPORT_EDIT_SORT_NORMAL_ITEM_STYLE);
    sortList->setItemTemplate(sortList, "sport_edit_tmp_style_layer.normal_item_shade");
    //设置list的item总数量
    sortList->setItemCount(sortList, len);
    //设置作为编辑排序的item的index值
    sortList->setSortIndex(sortList, data->selectIndex);
    //设置中间排序item的样式
    // sortList->setSortItemTemplate(sortList, SPORT_EDIT_SORT_FOCUS_ITEM_STYLE);
    sortList->setSortItemTemplate(sortList, "sport_edit_tmp_style_layer.focus_item");
    //设置排序item更新的listener， 用来填充排序item的内容显示
    sortList->setOnSortItemUpdateListener(sortList, _onSortItemUpdate);
    //初始化排序list，传入要包含sortlist的Screen和layer的名称
    sortList->init(sortList, "SportEditSortScreen", ID_SPORT_EDIT_SORT_LIST_LAYER);
    addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_EDIT_SORT_LIST, sortList);
}
//---------------------------------------------
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
        if (appData->isEditStatus)
        {
            appData->selectIndex = 0;
        }
        else
        {
            appData->selectIndex = pageData->selectIndex;
        }
        appData->select_sport_type = pageData->selectSportType;
        appData->category = pageData->selectsportCategory;
    }
}
static void _getMainListData(ScreenContext *screenContext)
{
    memset(sSpTypeArr, 0, SP_EDIT_ARRAY_MAX);
    AppData *appData = getScreenData(screenContext, AppData);
    sportListPresenter *presenter = createPresenter_SportList();
    appData->presenter = presenter;
    int *spTypes;
    int size;
    presenter->getMainList(presenter, &spTypes, &size);
    for (int id = 0; id < size; id++)
    {
        if (appData->isEditStatus)
        {
            sSpTypeArr[id + 1] = *(spTypes + id);
        }
        else
        {
            sSpTypeArr[id] = *(spTypes + id);
        }
    }
    appData->types = sSpTypeArr;
    appData->type_len = size;
    appData->presenter->openGPS();
}
//---------------------------------------------
static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    #ifdef HM_DISPLAY_IS_ROUND
    setSBStringData(getAppInstance(), ID_SPORT_EDIT_TITLE, System_getLocaleText("id_sport_edit_list"));
    #endif
    _createAppData(screenContext, data);
    if (data != H_NULL)
    {
        _getMainListData(screenContext);
        _createSortList(screenContext);
    }
    #ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_updateTitle(System_getLocaleText("id_sport_edit"));
    #endif
    gre_set_layer_visible(getAppInstance(), "SportEditSortScreen.sort_list_save_layer", false);
    SortList *sortList = (SortList *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_EDIT_SORT_LIST);
    if(sortList){
        sortList->setVisibility(sortList, true);
    }

}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
    HLOG_D(TAG, " %s\r\n", __func__);
}
static void _scrollYoffset(ScreenContext *screenContext)
{
    AppData *appdata = getScreenData(screenContext, AppData);
    HLOG_D(TAG, "category list yoffset %d  ", appdata->yoffset);
    if (appdata->yoffset == 0)
    {
        return;
    }
    gre_set_layer_yoffset(screenContext->_grAppContext, ID_SPORT_EDIT_SORT_LIST_LAYER, appdata->yoffset);
    // arcScrollBar *scroll = (arcScrollBar *)findContainer(screenContext, ID_SPORTLIST_SCROLLBAR);
    // if (scroll)
    // {
    //     int offset = gre_get_layer_yoffset(screenContext->_grAppContext, SPORTLIST_SCREEN_LAYER);
    //     scroll->setPos(scroll, offset);
    // }
    // SortList *sportList = (SortList *)findContainer(screenContext, ID_SPORT_EDIT_SORT_LIST);
    // if (sportList != H_NULL)
    // {
    //     sportList->updateBotPaddingY(sportList);
    //     sportList->updateScrolling(sportList);
    // }
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_D(TAG, " %s\r\n", __func__);
    System_setKeyFilter(HM_KEY_CODE_HOME, HM_KEY_ACT_FILTER_CLICK);
}

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
        //memcpy(appdata, restoreData, sizeof(AppData));
        appdata->isEditStatus = restoreData->isEditStatus;
        appdata->select_sport_type = restoreData->select_sport_type;
        appdata->selectIndex = restoreData->selectIndex;
        appdata->category = restoreData->category;
        appdata->yoffset = restoreData->yoffset;
    }
}
//------------------------------
static void _saveYoffset(ScreenContext *screenContext)
{
    AppData *appdata = getScreenData(screenContext, AppData);
    AppData *saveData = (AppData *)H_MALLOC(sizeof(AppData));
    if (saveData != H_NULL){
        memset(saveData, 0, sizeof(AppData));
        saveData->isEditStatus = appdata->isEditStatus;
        saveData->select_sport_type = appdata->select_sport_type;
        saveData->selectIndex = appdata->selectIndex;
        saveData->category = appdata->category;
        int yoffset = gre_get_layer_yoffset(screenContext->_grAppContext, ID_SPORT_EDIT_SORT_LIST_LAYER);
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
        _getMainListData(screenContext);
        _createSortList(screenContext);
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
    AppData *appData = getScreenData(screenContext, AppData);

    if (appData->dialogHandle != H_NULL && appData->dialogHandle->isShowing(appData->dialogHandle) == true)
    {
        appData->dialogHandle->dismissDialog(appData->dialogHandle);
    }
    // removeContainer(contenxt, data->dialogHandle->getAlertDialogId(data->dialogHandle));
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
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
}

static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    SortList *listContainer = (SortList *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_EDIT_SORT_LIST);
    if (listContainer != H_NULL)
    {
        return ((ScreenContainer *)listContainer)->handleKeyEvent(screenContext, (ScreenContainer *)listContainer, info);
    }

    //if(info->code == HM_KEY_CODE_SHORTCUT && info->action == HM_KEY_ACTION_CLICK){
    //    return true;
    //}else if(info->code == HM_KEY_CODE_HOME && info->action == HM_KEY_ACTION_CLICK){
    //    return true;
    //}
    return true;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    SortList *listContainer = (SortList *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_EDIT_SORT_LIST);

    if (GESTURE_RIGHT == eventId)
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
        }else if(!gre_get_layer_visible(screenContext->_grAppContext, "SportEditSortScreen.sort_list_save_layer")){
            listContainer->setVisibility(listContainer, false);
            gre_set_layer_visible(getAppInstance(), "SportEditSortScreen.sort_list_save_layer", true);
            setSBStringData(screenContext->_grAppContext, "sort_list_save_layer.save_text.value", System_getLocaleText("id_sport_edit_save_or_not"));
        }else if(gre_get_layer_visible(screenContext->_grAppContext, "SportEditSortScreen.sort_list_save_layer")){
            getScreenManager()->finishCurrentScreen();
        }
       // else
       // {
       //     getScreenManager()->finishCurrentScreen();
       // }
    }
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        SortList *listContainer = (SortList *)findContainer(screenContext, ID_SPORT_EDIT_SORT_LIST);
        if (listContainer)
        {
            listContainer->container.handleEvent(screenContext, (ScreenContainer *)listContainer, id, event);
        }
    }
    return true;
}
static void _goBack()
{
    //go to other screen
    ScreenPathTranslation transition = createPathTransition(200, LINEAR, DELTA, LEFT, BOTH);
    getScreenManager()->setPriority(PRIORITY_SCREEN_LOW);
    getScreenManager()->setScreenTransition(&transition, sizeof(ScreenPathTranslation));
    // getScreenManager()->startScreenByFlag("SportCategoryListScreen", FLAG_SCREEN_REMOVE_CURRENT, &pageData, sizeof(SportlistPageData));
    getScreenManager()->startScreen("SportEditScreen", H_NULL, 0);
}
static void _Dialog_clickCallback(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    AppData *data = getScreenData(contenxt, AppData);
    data->dialogHandle->dismissDialog(data->dialogHandle);
    removeContainer(contenxt, data->dialogHandle->getAlertDialogId(data->dialogHandle));
}
static bool _showDialog(ScreenContext *scontext, const char *screenName, const char *content, DialogClickListener listener)
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
static void _saveListData(SortList *listContainer, AppData *data)
{
    H_Uint8 arrayLen = 0;
    H_Uint8 *array = listContainer->getItemArray(listContainer, &arrayLen);

    int ids[arrayLen];
    for (int i = 0; i < arrayLen; i++)
    {
        int index = array[i];
        ids[i] = data->types[index];
    }
    data->presenter->setMainList(data->presenter, ids, arrayLen);
    _goBack();
}
static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    AppData *data = getScreenData(screenContext, AppData);
    SortList *listContainer = NULL;
    int ret = 0;
    if (data->dialogHandle != H_NULL && strstr(id, data->dialogHandle->getAlertDialogId(data->dialogHandle)))
    {
        data->dialogHandle->container.handleTouchEvent(screenContext, &data->dialogHandle->container, id, eventId, info);
        return true;
    }
    if (!strcmp(id, ID_SPROT_EDIT_DELETE_BTN))
    {
        listContainer = (SortList *)findContainer(screenContext, ID_SPORT_EDIT_SORT_LIST);
        H_Uint8 arrayLen = 0;
        listContainer->getItemArray(listContainer, &arrayLen);
        if (arrayLen == SPORTS_LIST_MIN_NUM)
        {
            return _showDialog(screenContext, "SportEditSortScreen", System_getLocaleText("id_sport_tips_minum"), _Dialog_clickCallback);
        }
        listContainer->deleteSortItem(listContainer);
    }else if (!strcmp(id, ID_SPORT_EDIT_SURE_BTN)
              || !strcmp(id, ID_SPORT_EDIT_SAVE_BTN)){
        listContainer = (SortList *)findContainer(screenContext, ID_SPORT_EDIT_SORT_LIST);
        _saveListData(listContainer, data);
        ret = Sysprop_setInt("sportlist.edit.item.type", data->selectType);
        HLOG_I(TAG, "setprop edit prop ret:%d, value:%d", ret, data->selectType);
    }else if(!strcmp(id, ID_SPORT_EDIT_DISCARD_BTN)){
		getScreenManager()->finishCurrentScreen();
    }else{
        listContainer = (SortList *)findContainer(screenContext, ID_SPORT_EDIT_SORT_LIST);
        if (listContainer != H_NULL)
        {
            return ((ScreenContainer *)listContainer)->handleTouchEvent(screenContext, (ScreenContainer *)listContainer, id, eventId, info);
        }
    }
    return true;
}
