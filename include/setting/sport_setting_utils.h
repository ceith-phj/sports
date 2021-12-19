/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  lizheng1@huami.com
 *  Time : 2021-04-08
 *
 *  Describe : sport setting untils
 *
 * ======================================================
 */
#ifndef __SPORT_SETTING_UTILS_H__
#define __SPORT_SETTING_UTILS_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <widget/list/list_widget.h>
#include <widget/picker/pickerWidget.h>
#include <widget/list/radio_list_widget.h>
#include "manager/sports_setting_manager.h"
#include "sports/include/common/sport_app_group.h"
#include "sports/include/manager/sports_manager.h"
#include <packages/services/syssevice/manager/system_locale.h>
#include "ui/Sports/include/setting/sport_setting_picker_type.h"
#include "ui/Sports/include/setting/sport_setting_dialog_type.h"
#include "ui/Sports/include/setting/sport_setting_text.h"


#define _ST(x)                      getSettingText(x)                           /**< 文本KEY => 文本. */
#define _GT(x)                      getText(x)                                  /**< 文本ID => 文本. */
#define _UT(x)                      getUnitText(x)                              /**< 单位KEY => 文本. */
#define SZ_SET_BUF                  50                                          /**< BUF默认大小. */

typedef struct _lw_set_utils {
    SportStateType sportState;                                                  /**@ 当前的运动状态. */
    lw_item *list;                                                              /**@ item列表. */
    uint16_t amount;                                                            /**@ item数量. */
    uint16_t mapIndex;                                                          /**@ 当前map的Index. */
    uint8_t map[50];                                                            /**@ id映射关系. */
}lw_set_utils;

typedef struct _sport_set_info {
    int min;                                                                    /**@ 最小值. */
    int max;                                                                    /**@ 最大值. */
    int val;                                                                    /**@ 当前值. */
}sport_set_info;

typedef struct _hr_zone_info {
    sport_hr_range_setting_e type;                                              /**@ 区间类型. */
    sport_set_info min;                                                         /**@ 上界. */
    sport_set_info max;                                                         /**@ 下界. */
}hr_zone_info;

typedef enum _text_unit_key {
    KEY_KM_MILE,                                                                /**@ 公里 | 英里. */
    KEY_KPH_MIPH,                                                               /**@ 公里/时 | 英里/时. */
}text_unit_key;

/* 列表工具 */
void lwUtils_Init(lw_set_utils *lsu, lw_item *itemList, uint16_t amount);                       /**# 初始化工具. */
lw_item *lwUtils_InitItem(lw_set_utils *lsu, uint16_t index, bool exist, bool preStartOnl);     /**# 通过lwUtils分配lw_item. */
uint8_t *lwUtils_GetMap(lw_set_utils *lsu);                                                     /**# 获取映射关系. */
uint8_t lwUtils_GetMapSize(lw_set_utils *lsu);                                                  /**# 获取map大小. */
int lwUtils_MapIndex(lw_set_utils *lsu, int index);                                             /**# 映射Index. */

/* 选择器工具 */
PickerWidget *createPicker(ScreenContext *sc, pickerInitData *pdata, pickerTypeE type, const char *title, const char *subTitle);
void setPicker_ItemAttr(PickerWidget *wgt, pickerInitData *pdata, int id, char *connector, const char *unit, bool loop, bool str);
char *setPicker_ItemNumEx(PickerWidget *wgt, pickerInitData *pdata, int id, int start, int end, int step, int init, char *dataCustom);
char *setPicker_ItemNum(PickerWidget *wgt, pickerInitData *pdata, int id, int start, int end, int step, int init);
void setPicker_ItemStr(PickerWidget *wgt,  pickerInitData *pdata, int id, char *text, int len, int init);
void setPicker_ItemText(PickerWidget *wgt,  pickerInitData *pdata, int id, const char **textID, char **text, int init);
void startPicker(ScreenContext *sc, PickerWidget *wgt, pickerInitData *pdata, pickerCallback cb);

/* 切换页面 */
void switchSettingScreen(const char *screen, SportSettingsMessage *ssm, bool change);       /**# 切换到设置页. */
void switchPickerScreen(picker_id id, SportSettingsMessage *ssm, bool change);              /**# 切换到选择器. */
void switchDialogScreen(dialog_id id, SportSettingsMessage *ssm, bool change);              /**# 切换到DLG. */

/* 数据工具 */
bool checkTypeIsMatch(int type, const int *group, uint16_t groupSize);          /**# 检测类型是否匹配. */
sport_group_t getSportGroup(void);                                              /**# 获取当前的分组. */

/* 其他工具 */
const char *getSettingText(const char *key);                                    /**# 获取设置文本. */
const char *getText(const char *id);                                            /**# 获取文本. */
void getSportSetting(SportSettingsMessage *ssm);                                /**# 读取设置. */
void saveSportSetting(SportSettingsMessage *ssm, bool change);                  /**# 保存设置. */

/* 心率工具 */
uint8_t getHeartRateThreshold(SportSettingTypePicker *hrSet);                   /**# 获取安全心率. */
void getSafeHrInfo(SportSettingTypePicker *set, sport_set_info *info);          /**# 获取安全信息. */

/* 公英制文本 */
const char *getUnitText(text_unit_key key);                                     /**# 获取单位文本. */
const char *choiceUnitText(const char *metric, const char *imperial);           /**# 选择公英制. */

#ifdef __cplusplus
}
#endif
#endif
