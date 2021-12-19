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

#include <containers/MenuContainer.h>
#include <BaseSportView.h>
#include <gre/gre.h>
#include <gre/sdk/grelib.h>
#include <sb_screen_util.h>
#include <utils/sports_string_utils.h>
#include <system_locale.h>
#include "packages/services/sport/misc/sport_misc.h"
#include "packages/apps/sports/include/presenter/SportPresenter.h"
#include "framework/base/core/display_metrics.h"
#include <hm_toast.h>

CONTAINER_FUNCTION_DEFINE();

#define TAG "MenuContainer"

#ifdef HM_DISPLAY_IS_ROUND
#define LOCK_BTN_OFFSET_X   getDimension(190)
#define LOCK_BTN_OFFSET_Y   getDimension(06)
#else
#define ID_LOCK_BTN_OFFSET_X "sportOpreationStyleLayer.LockGroup.iconLock.lock_btn_offset_x"
#define ID_LOCK_BTN_OFFSET_Y "sportOpreationStyleLayer.LockGroup.iconLock.lock_btn_offset_y"
static H_Int32 getInt(const char * path)
{
    gr_application_t *app = getAppInstance();
    if(NULL == app)
    {
        return 0;
    }
    H_Int32 value = getSBIntData(app, path);
    return value;
}
#define LOCK_BTN_OFFSET_X   getInt(ID_LOCK_BTN_OFFSET_X)
#define LOCK_BTN_OFFSET_Y   getInt(ID_LOCK_BTN_OFFSET_Y)
#endif

#define OPERATE_LOCK_ID     "sport_menu_lock"

#define OPERATE_LOCK_ICON   "images/sports/lock/icon_btn_lock.png"
#define OPERATE_UNLOCK_ICON "images/sports/lock/icon_btn_unlock.png"

static void initContainer(MenuContainer *menuContainer, ScreenContext *screenContext);
static void initViews(MenuContainer *menuContainer, char *parentLayerName, char *titlebarGroupName, MenuContainerItemInfo *info, H_Int8 infoLength);
static void initItems(MenuContainer *dataView, MenuContainerItemInfo *info, H_Int8 infoLength);
static void setOperateLock(MenuContainer *menuContainer, bool isAutoLockEnable, bool toastShow);

static const char *lockIcons[] =
{
    OPERATE_UNLOCK_ICON,
    OPERATE_LOCK_ICON,
};

static const char *lockTips[] =
{
    "id_sport_lock_disable_tips",
    "id_sport_lock_enable_tips",
};


static void initContainer(MenuContainer *menuContainer, ScreenContext *screenContext)
{
    menuContainer->screenContext = screenContext;
    menuContainer->app = screenContext->_grAppContext;
}

static void initViews(MenuContainer *menuContainer, char *parentLayerName, char *titlebarGroupName, MenuContainerItemInfo *info, H_Int8 infoLength)
{
    menuContainer->parentLayerName = parentLayerName;
    menuContainer->initTitlebar(menuContainer, titlebarGroupName);
    menuContainer->initItems(menuContainer, info, infoLength);
}

static void initTitlebar(MenuContainer *menuContainer, char *titlebarGroupName)
{
    // "sportOpreationStyleLayer.TitleGroup"
    char *fqn = NULL;
    gr_integer_kv_t data[2] =
        {
            {"x", 0},
            {"y", 0},
        };
    gre_clone_object(menuContainer->app, titlebarGroupName, "titlebarGroup",
                     menuContainer->parentLayerName, data, 2, &fqn);

    char ctrlPath[256] = {0};

    hm_snprintf(ctrlPath, sizeof(ctrlPath), "%s.%s.height", menuContainer->parentLayerName, "titlebarGroup");
    menuContainer->height = getSBIntData(menuContainer->app, ctrlPath);

    HLOG_D(TAG, "key:%s, fqn %s %d\r\n", ctrlPath, fqn, menuContainer->height);

    hm_gre_clone_free(fqn);
}

static void initItems(MenuContainer *menuContainer, MenuContainerItemInfo *info, H_Int8 infoLength)
{
    char key[256] = {0};
    hm_snprintf(key, sizeof(key), "sportOpreationStyleLayer.ItemGroup.item.x_offset");
    uint32_t x_offset = getSBIntData(menuContainer->app, key);

    hm_snprintf(key, sizeof(key), "sportOpreationStyleLayer.ItemGroup.item.height");
    uint32_t height = getSBIntData(menuContainer->app, key);

    uint8_t size = infoLength;
    menuContainer->size = size;
    menuContainer->info = hm_mem_malloc(sizeof(MenuContainerItemInfo) * menuContainer->size);
    memcpy(menuContainer->info, info, sizeof(MenuContainerItemInfo) * size);

    char *fqn = NULL;
    for (int i = 0; i < menuContainer->size; i++)
    {
        gr_integer_kv_t data[2] =
        {
            {"x", x_offset},
            {"y", menuContainer->height},
        };
        hm_snprintf(key, sizeof(key), "sport_menu_item_%d", menuContainer->info[i].itemId);
        gre_clone_object(menuContainer->app, "sportOpreationStyleLayer.ItemGroup.item", key,
                         menuContainer->parentLayerName, data, 2, &fqn);
        menuContainer->height += height;

        hm_snprintf(key, sizeof(key), "%s.value", fqn);
        hm_gre_clone_free(fqn);
        setSBStringData(menuContainer->app, key, System_getLocaleText(menuContainer->info[i].itemName));
    }

    if (!getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
    {
        gr_integer_kv_t lockData[2] =
        {
            {"x", LOCK_BTN_OFFSET_X},
            {"y", menuContainer->height + LOCK_BTN_OFFSET_Y},
        };

        gre_clone_object(menuContainer->app, "sportOpreationStyleLayer.LockGroup.iconLock",
                                            OPERATE_LOCK_ID,
                                            menuContainer->parentLayerName, lockData, 2, &fqn);
        HLOG_D(TAG, "fqn : %s\r\n", fqn);
        hm_gre_clone_free(fqn);

        setOperateLock(menuContainer, getSportsControlManager()->getLockScreenState(), false);
    }
}

static void updateItemContent(MenuContainer *menuContainer, H_Int8 index, char *content)
{
    char key[256] = {0};
    char value[256] = {0};
    hm_snprintf(key, sizeof(key), "%s.sport_menu_item_%d.value", menuContainer->parentLayerName, menuContainer->info[(uint8_t)index].itemId);

    hm_snprintf(value, sizeof(value), "%s%s", System_getLocaleText(menuContainer->info[(uint8_t)index].itemName), content);
    setSBStringData(menuContainer->app, key, value);
}

static void updateItemImage(MenuContainer *menuContainer, char *img)
{
    char key[256] = {0};
    hm_snprintf(key, sizeof(key), "%s.titlebarGroup.iconSport.image", menuContainer->parentLayerName);
    setSBStringData(menuContainer->app, key, img);
}

static void setOperateLock(MenuContainer *menuContainer, bool isAutoLockEnable, bool toastShow)
{
    if (!getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
    {
        char key[256] = {0};
        hm_snprintf(key, sizeof(key), "%s.%s.image", menuContainer->parentLayerName, OPERATE_LOCK_ID);
        setSBStringData(menuContainer->app, key, lockIcons[isAutoLockEnable]);
        HLOG_D(TAG, "key : %s\r\n", key);

        if(toastShow)
        {
            Toast_show(System_getLocaleText(lockTips[isAutoLockEnable]));
        }
        getSportsControlManager()->setLockScreenState(isAutoLockEnable);
    }
    else
    {
        if(toastShow)
        {
            Toast_show(System_getLocaleText(lockTips[1]));
        }
    }
}

static void updateAutoLockState(MenuContainer *menuContainer)
{
    setOperateLock(menuContainer, getSportsControlManager()->getLockScreenState(), false);
}

void setOnItemClickListener(MenuContainer *menuContainer, OnMenuContainerItemClickListener listener)
{
    menuContainer->listener = listener;
}

void setOnLockClickListener(MenuContainer *menuContainer, OnMenuContainerItemLockListener listener)
{
    menuContainer->lockListener = listener;
}

void setScrollable(MenuContainer *dataView, bool isScrollable)
{
    gre_set_layer_attr(dataView->app, dataView->parentLayerName, "scroll_enabled", isScrollable);
}

MenuContainer *createMunuContainer(gr_application_t *app)
{
    MenuContainer *menuContainer = H_NULL;
    if (!menuContainer)
    {
        createContainer(menuContainer, MenuContainer, menuContainer->container);

        menuContainer->initContainer = initContainer;
        menuContainer->initViews = initViews;
        menuContainer->initTitlebar = initTitlebar;
        menuContainer->initItems = initItems;
        menuContainer->setOnItemClickListener = setOnItemClickListener;
        menuContainer->setOnItemLockListener = setOnLockClickListener;
        menuContainer->listener = 0;
        menuContainer->lockListener = 0;
        menuContainer->setScrollable = setScrollable;
        menuContainer->updateItemContent = updateItemContent;
        menuContainer->updateItemImage = updateItemImage;
        menuContainer->updateAutoLockState = updateAutoLockState;
        menuContainer->setOperateLock = setOperateLock;
    }
    return menuContainer;
}

static char *searchButton(const char *id, char *name)
{
    return strstr(id, name);
}

static int g_dragType = DRAG_NONE;
static int g_pressDown = false;
static SPoint g_pressPos = {.x = 0, .y = 0};

static bool handleTouchEvent(ScreenContext *screenContext, ScreenContainer *container, const char *id, const SBTouchEvent eventId, const SBEventInfo *info)
{
    //HLOG_D(TAG, "touch id : %s\r\n", id);
    MenuContainer *menuContainer = (MenuContainer *)container;
    if (eventId == TOUCH_SELECT)
    {
        char key[256] = {0};
        hm_snprintf(key, sizeof(key), "%s.sport_menu_item_", menuContainer->parentLayerName);
        H_Int16 itemId = hm_get_string_digital((char *)id, key);
        if (itemId != -1)
        {
            //HLOG_D(TAG, "touch id : %d\r\n", itemId);
            if (menuContainer->listener)
            {
                menuContainer->listener(menuContainer, itemId);
            }
            return true;
        }
        else if(searchButton(id, OPERATE_LOCK_ID))
        {
            if (menuContainer->lockListener)
            {
                menuContainer->lockListener(menuContainer, !getSportsControlManager()->getLockScreenState());
            }
        }
    }
    if (eventId == TOUCH_DOWN)
    {
        g_pressDown = true;
        g_pressPos.x = info->x;
        g_pressPos.y = info->y;
    }
    else if (eventId == TOUCH_MOVE)
    {
        if (g_pressDown)
        {
            int deltaX = info->x - g_pressPos.x;
            int deltaY = info->y - g_pressPos.y;

            if (abs(deltaY) > SCROLL_DRAG_THREHOLD && abs(deltaY) > abs(deltaX))
            {
                g_dragType = DRAG_VER;
            }
            if (abs(deltaX) > SCROLL_DRAG_THREHOLD && g_dragType != DRAG_VER)
            {
                g_dragType = DRAG_HOR;
                menuContainer->setScrollable(menuContainer, false);
            }
        }

        if (g_dragType == DRAG_VER)
        {
            return true;
        }
    }
    else if (eventId == TOUCH_UP)
    {
        g_pressDown = false;
        if (g_dragType == DRAG_VER)
        {
            g_dragType = DRAG_NONE;
            return true;
        }
        else if (g_dragType == DRAG_HOR)
        {
            menuContainer->setScrollable(menuContainer, true);
        }
    }
    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext, ScreenContainer *container,  const HmKeyEvent *info)
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
    char key[256] = {0};
    MenuContainer *menuContainer = (MenuContainer *)container;

    memset(key, 0, sizeof(key));
    hm_snprintf(key, sizeof(key), "%s.titlebarGroup", menuContainer->parentLayerName);
    gre_delete_object(menuContainer->app, key);

    for(uint32_t i = 0; i < menuContainer->size; i++)
    {
        memset(key, 0, sizeof(key));
        hm_snprintf(key, sizeof(key), "sport_menu_item_%d", menuContainer->info[i].itemId);
        gre_delete_object(menuContainer->app, key);
    }

    memset(key, 0, sizeof(key));
    hm_snprintf(key, sizeof(key), "%s.%s", menuContainer->parentLayerName, OPERATE_LOCK_ID);
    gre_delete_object(menuContainer->app, key);

    hm_mem_free(menuContainer->info);

    HLOG_D("MenuContainer", "%s\r\n", __func__);
}