#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <sport_type_utils.h>
#include <sb_screen_context.h>
#include <gre/sdk/grelib.h>

#include "presenter/sports_course_list_presenter.h"

#define TAG "SC_LIST"
#include "log.h"

#define ID_SPORT_COURSE_LIST            "SportListLayer.SportCourseList"
#define SCREEN_NAME                     SportCourseList


SCREEN_DECLARE(SCREEN_NAME);

static SportCourseListPresenter *thisPresenter;

char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] = {
    "SportCourseListStyleLayer.Content",
    "SportCourseListStyleLayer.Title",
    "SportCourseListStyleLayer.Icon"
};

typedef struct _resource_info {
    course_category category;
    char *str;
    char *img;
}resource_info;

static resource_info resourceTable[] = {
    {COURSE_CATEGORY_RUN, "translations.id_course_type_sport_course", "images/course/icon_run.png"},
    {COURSE_CATEGORY_RUN, "translations.id_course_type_run", "images/course/icon_train.png"},
    {COURSE_CATEGORY_TRAIN, "translations.id_course_type_train", "images/course/icon_run.png"},
    {COURSE_CATEGORY_WARM_UP, "translations.id_course_type_warm_up", "images/course/icon_warm_up.png"},
    {COURSE_CATEGORY_RELAX, "translations.id_course_type_relax", "images/course/icon_relax.png"},
};

static void eventItemUpdate(unequalHeiList *list, int idx, const char *itemGroupPath)
{
    char key[256], *itemName;
    Screen *screenInstance;
    gr_application_t *appContext;
    resource_info *resource;

    screenInstance = getScreen(SCREEN_NAME);
    appContext = getScreenContext(screenInstance)->_grAppContext;
    resource = &resourceTable[idx];

    /* 设置文字 */
    itemName = getSBStringData(appContext, resource->str);
    snprintf(key, sizeof(key), "%s.item.text", itemGroupPath);
    setSBStringData(appContext, key, itemName);
    H_FREE(itemName);

    /* 设置背景 */
    snprintf(key, sizeof(key), "%s.item.image", itemGroupPath);
    setSBStringData(appContext, key, resource->img);
}

static void eventItemTouch(unequalHeiList *list, int idx, const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    getScreenManager()->startScreen("SportCourseDetailScreen", &idx, sizeof(int));
}

static void createList(int *list, int count)
{
    Screen *screenInstance;
    unequalHeiList *listWidget;
    gr_application_t *appContext;
    ScreenContext *screenContext;

    /* 创建列表控件 */
    screenInstance = getScreen(SCREEN_NAME);
    appContext = getScreenContext(screenInstance)->_grAppContext;
    screenContext = getScreenContext(screenInstance);
    listWidget = createUnequalHeiListContainer(appContext);

    /* 创建部件 */
    listWidget->create(appContext, listWidget, "SportCourseListScreen", "SportCourseListLayer",
                            5, itemTemps, ARRAY_SIZE(itemTemps), eventItemUpdate, eventItemTouch, "item");

    addContainer(screenContext, ID_SPORT_COURSE_LIST, listWidget);
    listWidget = findContainer(screenContext, ID_SPORT_COURSE_LIST);

    /* 初始化部件 */
    listWidget->setItemTemp(listWidget, 0, 0, 1);
    //listWidget->setItemTemp(listWidget, count, count, 2);
    listWidget->setCircle(listWidget, true);
    listWidget->initItems(listWidget);
}

static void createCourseList(int *list, int count)
{
    createList(list, count);
    // createScrollBar();
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    LOG_I("%s\n", __func__);
    void *list;
    int count;
    Presenter *basePresenter;

    /* 创建presenter */
    thisPresenter = SportCourseListPresenter_new();
    basePresenter = SUPER_PTR(thisPresenter, Presenter);

    /* 初始化presenter */
    CALL_FUNCTION(basePresenter, Presenter, init)(basePresenter);
    setScreenPresenter(screenContext, thisPresenter);

    /* 获取课程列表 */
    CALL_FINAL_FUNCTION(thisPresenter, SportCourseListPresenter, getCourseList)(thisPresenter, &list, &count);
    createCourseList(list, count);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    LOG_I("%s\n", __func__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    LOG_I("%s\n", __func__);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    LOG_I("%s\n", __func__);

    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

static void onNewScreenData(ScreenContext* screenContext, void* data)
{
    LOG_I("%s\n", __func__);
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    LOG_I("%s\n", __func__);
}

static void onDataSave(ScreenContext *screenContext)
{
    LOG_I("%s\n", __func__);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
    LOG_I("%s\n", __func__);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    LOG_I("%s\n", __func__);
}

static bool handleKeyEvent(ScreenContext *screenContext,  const HmKeyEvent *info)
{
    LOG_I("%s\n", __func__);
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id, const SBGestureEvent eventId)
{
    LOG_I("%s\n", __func__);
    return true;
}

static bool handleEvent(ScreenContext *screenContext, const char *id, const char *event)
{
    LOG_I("%s\n", __func__);
    return true;
}

static bool handleTouchEvent(ScreenContext *screenContext, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    LOG_I("%s\n", __func__);

    ScreenContainer *settingList = findContainer(screenContext, ID_SPORT_COURSE_LIST);
    if(settingList != NULL)
    {
        settingList->handleTouchEvent(screenContext, settingList, id, eventId, info);
        return true;
    }

    return false;
}