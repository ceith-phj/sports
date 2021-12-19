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

#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sport/SportOpreationView.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <presenter/SportPresenter.h>

#define TAG "operate"
CONTAINER_FUNCTION_DEFINE();

#define ID_ACTION_PAUSE     0
#define ID_ACTION_FINISH    1

static const MenuContainerItemInfo items[] = {
    {ID_ACTION_PAUSE, "id_sport_operate_pause"},
    {ID_ACTION_FINISH, "id_sport_operate_finish"},
};

static SportOpreationView *opreationView = NULL;
static void initMenuContainer(SportOpreationView *sportOpreationView);
static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id);

static void initContainer(SportOpreationView *sportOpreationView, ScreenContext *screenContext)
{
    sportOpreationView->screenContext = screenContext;
    sportOpreationView->app = screenContext->_grAppContext;
    sportOpreationView->initMenuContainer(sportOpreationView);
    sportOpreationView->path = "sportOpreationLayer";
}

static void setVisiable(SportOpreationView *sportOpreationView, H_Int8 isVisiable)
{
    gre_set_layer_visible(sportOpreationView->app, "SportScreen.sportOpreationLayer", isVisiable);
    sportOpreationView->menuContainer->updateAutoLockState(sportOpreationView->menuContainer);
}

static bool getVisiable(SportOpreationView *sportOpreationView)
{
    int visiable = gre_get_layer_visible(sportOpreationView->app, "SportScreen.sportOpreationLayer");
    return visiable ? true : false;
}

static void initMenuContainer(SportOpreationView *sportOpreationView)
{
    sportOpreationView->menuContainer = createMunuContainer(sportOpreationView->app);
    addContainer(sportOpreationView->screenContext, "sportOpreationMenu", sportOpreationView->menuContainer);
    sportOpreationView->menuContainer->initContainer(sportOpreationView->menuContainer, sportOpreationView->screenContext);
    sportOpreationView->menuContainer->initViews(sportOpreationView->menuContainer,
                                                    "sportOpreationLayer",
                                                    "sportOpreationStyleLayer.TitleGroup",
                                                    (MenuContainerItemInfo *)items, sizeof(items) / sizeof(items[0]));
    sportOpreationView->menuContainer->setOnItemClickListener(sportOpreationView->menuContainer, (OnMenuContainerItemClickListener)sportOpreationView->onMenuItemClick);
    sportOpreationView->menuContainer->setOnItemLockListener(sportOpreationView->menuContainer, (OnMenuContainerItemLockListener)sportOpreationView->onMenuLockClick);
}

/**
 * @brief
 *
 * @param menuContainer
 * @param id
 */
static void onMenuLockClick(MenuContainer *menuContainer, bool isLock)
{
    if(!opreationView->getVisiable(opreationView))
        return;

    menuContainer->setOperateLock(menuContainer, isLock, true);
}

static void onMenuItemClick(MenuContainer *menuContainer, H_Uint8 id)
{
    switch (id)
    {
        case ID_ACTION_PAUSE:
            getSportsScreenPresenter()->requestPause();
            HLOG_I(TAG, "button ID_ACTION_PAUSE pressed\r\n");
            break;
        case ID_ACTION_FINISH:
            getSportsScreenPresenter()->prepareSave();
            HLOG_I(TAG, "button ID_ACTION_FINISH pressed\r\n");
            break;
        default:
            break;
    }
}

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    SportOpreationView *sportOpreationView = (SportOpreationView *)container;
    ScreenContainer *menuContainer = (ScreenContainer *)sportOpreationView->menuContainer;
    return menuContainer->handleTouchEvent(screenContext, menuContainer, id, eventId, info);
}

static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container,  const HmKeyEvent *info)
{
    if ((info->code == HM_KEY_CODE_HOME && info->action == HM_KEY_ACTION_CLICK) ||
        (info->code == HM_KEY_CODE_SELECT && info->action == HM_KEY_ACTION_CLICK))
    {
        /* 暂停继续开始 */
        switch (getSportsScreenPresenter()->requestSportStatus())
        {
        case SPORT_STATE_RUN:
            getSportsScreenPresenter()->requestPause();
            return true;
        case SPORT_STATE_SUSPEND:
            getSportsScreenPresenter()->requestContinue();
            return true;
        default:
            return false;
        }
    }
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
    opreationView = NULL;
}

SportOpreationView *bindSportOpreationView(gr_application_t *app)
{
    SportOpreationView *sportOpreationView = H_NULL;
    if (!sportOpreationView)
    {
        createContainer(sportOpreationView, SportOpreationView, sportOpreationView->container);
        sportOpreationView->initContainer = initContainer;
        sportOpreationView->setVisiable = setVisiable;
        sportOpreationView->getVisiable = getVisiable;
        sportOpreationView->initMenuContainer = initMenuContainer;
        sportOpreationView->onMenuItemClick = onMenuItemClick;
        sportOpreationView->onMenuLockClick = onMenuLockClick;
        opreationView = sportOpreationView;
    }
    return sportOpreationView;
}