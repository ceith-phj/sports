#ifndef SPORT_WIDGET_MENU_CONTAINER_H
#define SPORT_WIDGET_MENU_CONTAINER_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define MENU_CONTAINER_ITEM_NAME_MAX    128
#define MENU_LOCK_BTN_ID                64

    typedef struct _MenuContainerItemInfo
    {
        H_Int8 itemId;
        char itemName[64];
    } MenuContainerItemInfo;

    typedef struct _MenuContainer MenuContainer;
    typedef void (*OnMenuContainerItemClickListener)(MenuContainer *menuContainer, int itemId);
    typedef void (*OnMenuContainerItemLockListener)(MenuContainer *menuContainer, bool isLock);

    struct _MenuContainer
    {
        ScreenContainer container;

        void (*initContainer)(MenuContainer *dataView, ScreenContext *screenContext);
        void (*initViews)(MenuContainer *dataView, char *groupName, char *titlebarGroupName, MenuContainerItemInfo *info, H_Int8 infoLength);

        void (*initTitlebar)(MenuContainer *dataView, char *titlebarGroupName);
        void (*initItems)(MenuContainer *dataView, MenuContainerItemInfo *info, H_Int8 infoLength);
        void (*setScrollable)(MenuContainer *dataView, bool isScrollable);
        void (*setOnItemClickListener)(MenuContainer *menuContainer, OnMenuContainerItemClickListener listener);
        void (*setOnItemLockListener)(MenuContainer *menuContainer, OnMenuContainerItemLockListener listener);
        void (*updateItemContent)(MenuContainer *menuContainer, H_Int8 index, char *content);
        void (*updateItemImage)(MenuContainer *menuContainer, char *img);
        void (*updateAutoLockState)(MenuContainer *menuContainer);
        void (*setOperateLock)(MenuContainer *menuContainer, bool isAutoLockEnable, bool toastShow);

        char *parentLayerName;
        H_Int16 height;
        gr_application_t *app;
        ScreenContext *screenContext;
        OnMenuContainerItemClickListener listener;
        OnMenuContainerItemLockListener lockListener;
        MenuContainerItemInfo *info;
        H_Uint16 size;
    };

    MenuContainer *createMunuContainer(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif