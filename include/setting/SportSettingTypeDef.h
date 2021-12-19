#ifndef SPORT_BASE_SETTING_TYPE_DEF_H
#define SPORT_BASE_SETTING_TYPE_DEF_H

#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include "sport_type_auto.h"
#include <widget/slideSwitch/slideSwitch.h>
#include <widget/radioGroup/radioGroup.h>
#include <widget/checkGroup/checkGroup.h>
#include <widget/button/checkbox/checkbox.h>

#ifdef __cplusplus
extern "C"
{
#endif
    enum SettingItemType
    {
        MAIN_ITEM = 0,
        SINGLE_LINE_ICON_TEXT,
        SINGLE_LINE,
        RADIO_SINGLE_LINE,
        RADIO_SINGLE_LINE_ARROW,
        CHECKBOX_SINGLE_LINE,
        SWITCH_SINGLE_LINE,
        SINGLE_LINE_COUNT,
        MULTI_LINE,
        RADIO_MULTI_LINE_ARROW,
        CHECKBOX_MULTI_LINE,
        SWITCH_MULTI_LINE,
        SETTING_ITEM_MAX
    };

    typedef struct _SportSettingItemStyle
    {
        enum SettingItemType type;
        char *path;
    } SportSettingItemStyle;

    typedef struct _SportSettingItem
    {
        H_Uint8 type;
        char *name;
        char *icon;
        enum SettingItemType displayType;
        bool (*isEnable)(sport_type_t type);
        CheckBox *checkBox;
        char* path;
    } SportSettingItem;

#ifdef __cplusplus
}
#endif

#endif