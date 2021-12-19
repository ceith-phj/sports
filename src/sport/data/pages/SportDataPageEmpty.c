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

#include <sport/data/pages/SportDataPageEmpty.h>
#include <packages/apps/sports/include/presenter/SportPresenter.h>
#include "packages/services/sport/misc/sport_misc.h"
#include <hm_toast.h>
#include "uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h"

CONTAINER_FUNCTION_DEFINE();

#define SPORT_ITEM_PAGE_MAX_CNT     (10)

static void updatePageData(SportDataPageEmpty *page);

#define ID_SPORT_ADD_PAGE_BTN       "sportEmptyPageLayer.emptyPageGroup.icon_add_page"
#define ID_SPORT_ADD_PAGE_BTN_TEXT  "sportEmptyPageLayer.emptyPageGroup.text_add_page.text"
#define ID_SPORT_EMPTY_PAGE         "sportEmptyPageLayer.emptyPageGroup"
#define ID_SPORT_EMPTY_CONTROL      "emptyPageGroupID"
#define ID_SPORT_EMPTY_ADD_BTN      "icon_add_page"

SportDataPageEmpty *newSportEmptyView(ScreenContext *screenContext, char *layerId)
{
    SportDataPageEmpty *page = H_NULL;
    createContainer(page, SportDataPageEmpty, page->container);
    H_ASSERT(page);
    page->screenContext = screenContext;
    page->app = screenContext->_grAppContext;
    page->layerId = layerId;
    page->update = updatePageData;

    char *fqn = NULL;
    gre_clone_object(page->app, ID_SPORT_EMPTY_PAGE, ID_SPORT_EMPTY_CONTROL, page->layerId, NULL, 0, &fqn);

    char key[64] = {0};
    char *addTips = (char *)System_getLocaleText("id_sport_item_add_page");
    if(!strcmp(addTips, "NULL"))
    {
        addTips = "Add page";
    }

    hm_snprintf(key, sizeof(key), "%s.text_add_page.text", fqn);
    setSBStringData(page->app, key, addTips);
    hm_gre_clone_free(fqn);

    return page;
}

static void updatePageData(SportDataPageEmpty *page)
{

}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    if(eventId == TOUCH_SELECT)
    {
        SportDataPageEmpty *page = (SportDataPageEmpty *)container;
        char key[128];
        hm_snprintf(key, sizeof(key), "%s.%s.%s", page->layerId, ID_SPORT_EMPTY_CONTROL, ID_SPORT_EMPTY_ADD_BTN);

        if(!strcmp(id, key))
        {
            sport_page_handle *handle = getSportsDataPresenter()->open(getSportsControlManager()->getType());
            sport_data_page *page_info = NULL;
            int cnt = getSportsDataPresenter()->getDataPage(handle, &page_info);
            getSportsDataPresenter()->close(handle);

            if(cnt < SPORT_ITEM_PAGE_MAX_CNT)
            {
                getScreenManager()->startScreen("SportItemPageListScreen", NULL, 0);
            }
            else
            {
                Toast_show(System_getLocaleText("id_sport_rt_page_add_tips"));
            }
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

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportDataPageEmpty *page = (SportDataPageEmpty *)container;
    char key[128];
    hm_snprintf(key, sizeof(key), "%s.%s", page->layerId, ID_SPORT_EMPTY_CONTROL);
    gre_delete_object(page->app, key);
}