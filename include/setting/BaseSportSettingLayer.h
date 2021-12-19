#ifndef SPORT_BASE_SETTING_CONTAINER_H
#define SPORT_BASE_SETTING_CONTAINER_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <BaseSportView.h>
#include <setting/SportSettingTypeDef.h>
#include <manager/sports_control_manager.h>
#include <manager/sports_setting_manager.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _BaseSportSettingLayer BaseSportSettingLayer;
    typedef struct _SportSettingLayerImpl SportSettingLayerImpl;

    struct _SportSettingLayerImpl
    {
        void (*initData)(SportSettingLayerImpl *child);
        bool (*isSettingItemEnable)(SportSettingLayerImpl *child, H_Uint8 type);
        void (*onItemCheckChanged)(SportSettingLayerImpl *child, H_Uint8 type, bool isChecked, SportSettingItem *item);
        void (*onItemClick)(SportSettingLayerImpl *child, H_Uint8 type, SportSettingItem *item);
        void (*onItemUpdate)(SportSettingLayerImpl *child, H_Uint8 type, SportSettingItem *item);
        H_Int8 (*getDefaultSettingItemSize)(void);
        SportSettingItem *(*getDefaultSettingItem)(void);
        SportSettingItem *(*getSettingItem)(SportSettingLayerImpl *child, H_Uint16 index);
        char *(*getTitleId)();
        char *(*getContainerId)();

        BaseSportSettingLayer *parent;
    };

    struct _BaseSportSettingLayer
    {
        ScreenContainer container;
        SportSettingsMessage *(*getSportSetting)(void);
        void (*setStartY)(BaseSportSettingLayer *dataView, H_Int16 startY);
        void (*setTitleEnable)(BaseSportSettingLayer *dataView, bool isTitleEnable);
        void (*initContainer)(BaseSportSettingLayer *dataView, ScreenContext *screenContext);
        void (*setChild)(BaseSportSettingLayer *dataView, SportSettingLayerImpl *child);
        void (*initData)(BaseSportSettingLayer *dataView);
        void (*initViews)(BaseSportSettingLayer *dataView);
        void (*initTitleView)(BaseSportSettingLayer *dataView);
        void (*initItems)(BaseSportSettingLayer *dataView);
        void (*initItem)(BaseSportSettingLayer *dataView, SportSettingItem *item);
        void (*updateItemLines)(BaseSportSettingLayer *dataView, SportSettingItem *item);
        void (*updatePrimaryText)(BaseSportSettingLayer *dataView, char *path, SportSettingItem *item);
        void (*updateIcon)(BaseSportSettingLayer *dataView, char *path, SportSettingItem *item);
        char *(*getItemStylePath)(BaseSportSettingLayer *dataView, enum SettingItemType type);
        H_Int16 (*getHeight)(BaseSportSettingLayer *dataView);
        void (*onItemCheckedChanged)(View *view, bool isChecked);
        gr_application_t *app;
        ScreenContext *screenContext;
        SportSettingLayerImpl *childLayer;
        char *layer;
        H_Uint16 containerHeight;
        H_Uint16 startY;
        bool isTitleEnable;
        SportSettingsMessage* setting;
        sport_type_t sportType;
        H_Uint16 itemCount;
        SportSettingItem* items;

        H_Int16 pressX, pressY;
        bool isMoved;
        bool isTouched;
    };

    BaseSportSettingLayer *createSettingContainer(gr_application_t *app, char *layer);

#ifdef __cplusplus
}
#endif

#endif