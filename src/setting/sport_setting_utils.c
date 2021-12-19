/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: lizheng1@huami.com
 *  Maintainer: lizheng1@huami.com
 *
 * =====================================================================================
 */

#include "ui/Sports/include/setting/sport_setting_utils.h"
#include "sports/include/manager/sports_manager.h"

#define NAME_UNDEFINE               "name not found"                            /**< 名字未找到. */

#include <hlog.h>
#define TAG "[LG_SSU]"

/*============================================================================================================ 00' 列表工具 ==== */
/*****************************************************************************************************
* @brief  : lw设置工具初始化
*
* @param  : utils
* @param  : item列表, 需要外部分配空间
* @return : None
******************************************************************************************************/
void lwUtils_Init(lw_set_utils *lsu, lw_item *itemList, uint16_t amount)
{
    lsu->amount = amount;
    lsu->list   = itemList;
    lsu->mapIndex = 0;
    lsu->sportState = getSportsControlManager()->getState();
}

/*****************************************************************************************************
* @brief  : 通过工具分配一个item
*
* @param  : lsu工具
* @param  : 是否存在
* @param  : 是否运动前独有
* @param  : item实际的index
* @return : None
******************************************************************************************************/
lw_item *lwUtils_InitItem(lw_set_utils *lsu, uint16_t index, bool exist, bool preStartOnly)
{
    lw_item *item;

    /* item index超过最大数量, 直接返回 */
    if(index > lsu->amount)
        return NULL;

    /* 不存在, 直接返回 */
    if(exist == false)
        return NULL;

    /* 运动中, 且为运动前独有, 直接返回 */
    if(lsu->sportState != SPORT_STATE_PRESTART && preStartOnly == true)
        return NULL;

    /* 建立映射 */
    item = &lsu->list[lsu->mapIndex];
    lsu->map[lsu->mapIndex] = index;
    lsu->mapIndex++;

    return item;
}

/*****************************************************************************************************
* @brief  : 获取映射关系
*
* @param  : 映射
* @return : map
******************************************************************************************************/
uint8_t inline *lwUtils_GetMap(lw_set_utils *lsu)
{
    return lsu->map;
}

/*****************************************************************************************************
* @brief  : 映射实际的Index
*
* @param  : lsu
* @return : map大小
******************************************************************************************************/
int lwUtils_MapIndex(lw_set_utils *lsu, int index)
{
    uint8_t *map = lsu->map;

    if(index < 0)
        return index;

    /* 通过Index反推映射时的Index */
    for(uint8_t i = 0 ; i < lsu->mapIndex ; i++)
    {
        if(map[i] == index)
            return i; 
    }

    return -1;
}

/*****************************************************************************************************
* @brief  : 获取map大小
*
* @param  : lsu
* @return : map大小
******************************************************************************************************/
uint8_t lwUtils_GetMapSize(lw_set_utils *lsu)
{
    return lsu->mapIndex;
}

/*============================================================================================================ 01' 选择器工具 ==== */
/*****************************************************************************************************
* @brief  : 设置选择器头部信息
*
* @param  : None
* @return : None
******************************************************************************************************/
PickerWidget *createPicker(ScreenContext *sc, pickerInitData *pdata, pickerTypeE type, const char *title, const char *subTitle)
{
    PickerWidget *wgt = creatPickerWidget(sc);
    addContainer(sc, SPORT_PICKER_CONTAINER_NAME, wgt);

    pdata->type = type;
    pdata->title = (char *)title;
    pdata->subTitle = (char *)subTitle;

    return wgt;
}

/*****************************************************************************************************
* @brief  : 设置选择器信息
*
* @param  : None
* @return : None
******************************************************************************************************/
void setPicker_ItemAttr(PickerWidget *wgt, pickerInitData *pdata, int id, char *connector, const char *unit, bool loop, bool str)
{
    pickerItemInfo *item = &pdata->itemInfo[id];

    /* 设置连接器 */
    item->connector = connector;
    item->unit = (char *)unit;
    item->supportLoop = loop;
    item->isString = str;
}

/*****************************************************************************************************
* @brief  : 设置数字类型的元素
*
* @param  : None
* @return : None
******************************************************************************************************/
char *setPicker_ItemNum(PickerWidget *wgt, pickerInitData *pdata, int id, int start, int end, int step, int init)
{
    uint16_t amount;
    char *itemlist;
    pickerItemInfo *item = &pdata->itemInfo[id];

    if(init > end)
        init = end;

    /* 创建item列表 */
    amount = wgt->tool.getIndexMax(start, end, step);
    itemlist = H_MALLOC(SELECTOR_TEXT_MAX * amount);
    if(itemlist == NULL)
        return NULL;

    /* 初始化列表内容 */
    if(wgt->tool.initNumberData(itemlist, start, end, step) != amount)
        goto err0;

    /* 设置内容和初始值 */
    item->itemStrs = itemlist;
    item->length = amount;
    wgt->index[id] = wgt->tool.getNumberIndex(init, start, end, step);
    return itemlist;

err0:
    H_FREE(itemlist);
    return NULL;
}

/*****************************************************************************************************
* @brief  : 设置数字类型的元素, 可以自定义格式化符号
*
* @param  : 选择器控件
* @param  : picker初始化数据
* @param  : 第几列
* @param  : 选择器开始
* @param  : 选择器结束
* @param  : 选择器步长
* @param  : 初始值
* @param  : 自定义格式化符号
* @return : 完整的格式化后的ITEM列表
******************************************************************************************************/
char *setPicker_ItemNumEx(PickerWidget *wgt, pickerInitData *pdata, int id, int start, int end, int step, int init, char *dataCustom)
{
    uint16_t amount;
    char *itemlist;
    pickerItemInfo *item = &pdata->itemInfo[id];

    if(init > end)
        init = end;

    /* 创建item列表 */
    amount = wgt->tool.getIndexMax(start, end, step);
    itemlist = H_MALLOC(SELECTOR_TEXT_MAX * amount);
    if(itemlist == NULL)
        return NULL;

    /* 初始化列表内容 */
    if(wgt->tool.initNumberDataCustom(itemlist, start, end, step, dataCustom) != amount)
        goto err0;

    /* 设置内容和初始值 */
    item->itemStrs = itemlist;
    item->length = amount;
    wgt->index[id] = wgt->tool.getNumberIndex(init, start, end, step);
    return itemlist;

err0:
    H_FREE(itemlist);
    return NULL;
}

/*****************************************************************************************************
* @brief  : 设置文本类型的元素
*
* @param  : None
* @return : None
******************************************************************************************************/
void setPicker_ItemStr(PickerWidget *wgt,  pickerInitData *pdata, int id, char *text, int len, int init)
{
    pickerItemInfo *item = &pdata->itemInfo[id];

    item->itemStrs = (char *)text;
    item->length = len;
    wgt->index[id] = init;
}

/*****************************************************************************************************
* @brief  : 设置选择器ITEM
*
* @param  : None
* @return : None
******************************************************************************************************/
void setPicker_ItemText(PickerWidget *wgt,  pickerInitData *pdata, int id, const char **textID, char **text, int init)
{
    int i = 0;
    const char *temp_id;
    char *temp_text = (char *)text;

    do {
        temp_id = textID[i];
        strncpy(temp_text, _GT(temp_id), SELECTOR_TEXT_MAX);
        temp_text += SELECTOR_TEXT_MAX;
        i++;
    }while(textID[i]);

    strncpy(temp_text, " ", SELECTOR_TEXT_MAX);
    setPicker_ItemStr(wgt, pdata, id, (char *)text, i, init);
}

/*****************************************************************************************************
* @brief  : 启动选择器
*
* @param  : None
* @return : None
******************************************************************************************************/
void startPicker(ScreenContext *sc, PickerWidget *wgt, pickerInitData *pdata, pickerCallback cb)
{
    wgt->init(sc, pdata, wgt);
    wgt->setValue(wgt, false);
    wgt->setCallback(wgt, cb);
    wgt->show(wgt);
}

/*============================================================================================================ 02' 切换页面工具 ==== */
/*****************************************************************************************************
* @brief  : 切换设置屏幕
*
* @param  : 字符串关键字
* @return : 字符串真实信息
******************************************************************************************************/
void switchSettingScreen(const char *screen, SportSettingsMessage *ssm, bool change)
{
    saveSportSetting(ssm, change);

    /* 屏幕为空回退, 否则切换页面 */
    if(screen == NULL)
        getScreenManager()->finishCurrentScreen();
    else
        getScreenManager()->startScreen(screen, H_NULL, 0);
}

/*****************************************************************************************************
* @brief  : 切换选择器屏幕
*
* @param  : 字符串关键字
* @return : 字符串真实信息
******************************************************************************************************/
void switchPickerScreen(picker_id id, SportSettingsMessage *ssm, bool change)
{

    saveSportSetting(ssm, change);
    getScreenManager()->startScreen("SportPickerScreen", &id, sizeof(int));
}

/*****************************************************************************************************
* @brief  : 切换到DLG
*
* @param  : None
* @return : None
******************************************************************************************************/
void inline switchDialogScreen(dialog_id id, SportSettingsMessage *ssm, bool change)
{
    saveSportSetting(ssm, change);
    getScreenManager()->startScreen("SportSettingDialogScreen", &id, sizeof(int));
}

/*============================================================================================================ 03' 数据工具 ==== */
/*****************************************************************************************************
* @brief  : 检测是否匹配
*
* @param  : 类型
* @param  : 类型组
* @param  : 类型组大小
* @return : None
******************************************************************************************************/
bool checkTypeIsMatch(int type, const int *group, uint16_t groupSize)
{
    for (uint16_t i = 0; i < groupSize; i++)
    {
        if (group[i] == type)
            return true;
    }

    return false;
}

/*****************************************************************************************************
* @brief  : 检测当前分组
*
* @param  : 类型
* @param  : 类型组
* @param  : 类型组大小
* @return : None
******************************************************************************************************/
sport_group_t getSportGroup(void)
{
    sport_type_t sport = sportManager()->ctrlManager()->getType();
    return sportGroup_get(sport);
}

/*============================================================================================================ 03' 其他工具 ==== */
/*****************************************************************************************************
* @brief  : 获取运动设置中的文本
*
* @param  : 字符串关键字
* @return : 字符串真实信息
******************************************************************************************************/
const char *getSettingText(const char *key)
{
    char name[100];
    const char *str, *text;

    /* 获取字符串 */
    snprintf(name, sizeof(name), "id_spset_%s", key);
    str = System_getLocaleText(name);

    /* 是否返回默认值 */
    (str == NULL) ? (text = NAME_UNDEFINE) : (text = str);
    return text;
}

/*****************************************************************************************************
* @brief  : 获取文本
*
* @param  : 字符串关键字
* @return : 字符串真实信息
******************************************************************************************************/
const char *getText(const char *id)
{
    char name[100];
    const char *str, *text;

    /* 获取字符串 */
    snprintf(name, sizeof(name), "id_%s", id);
    str = System_getLocaleText(name);

    /* 是否返回默认值 */
    (str == NULL) ? (text = NAME_UNDEFINE) : (text = str);
    return text;
}

/*****************************************************************************************************
* @brief  : 保存设置
*
* @param  : 字符串关键字
* @return : 字符串真实信息
******************************************************************************************************/
void saveSportSetting(SportSettingsMessage *ssm, bool change)
{
    sport_type_t sport = getSportsControlManager()->getType();

    /* 如果有修改进行保存设置 */
    if(change)
        sportSetting_getSettingManager()->setSetting(sport, ssm);
}

/*****************************************************************************************************
* @brief  : 获取运动设置
*
* @param  : 字符串关键字
* @return : 字符串真实信息
******************************************************************************************************/
void getSportSetting(SportSettingsMessage *ssm)
{
    sport_type_t sport = getSportsControlManager()->getType();
    sportSetting_getSettingManager()->getSetting(sport, ssm);
}

/*****************************************************************************************************
* @brief  : 获取心率区间信息
*
* @param  : 字符串关键字
* @return : 字符串真实信息
******************************************************************************************************/
void getSafeHrInfo(SportSettingTypePicker *set, sport_set_info *info)
{
    sport_hr_range_setting_t hrInfo;
    sportSetting_getSettingManager()->hrZone->getRange(&hrInfo);

    info->min = hrInfo.value[5] * 0.55;
    info->max = hrInfo.value[5];

    /* 计算心率阈值 */
    if(set->data == 0)
        info->val = info->max * 0.9;
    else if(set->data > info->max)
        info->val = info->max;
    else if(set->data < info->min)
        info->val = info->min;
    else
        info->val = set->data;

    HLOG_I(TAG, "min = %d, max = %d, val = %d, set->data = %d", info->min, info->max, info->val, set->data);
}

/*****************************************************************************************************
* @brief  : 获取心率阈值
*           规则:
*               1) 如果心率值为0, 则使用默认心率
*               2) 如果心率值大于最大心率, 使用最大心率
*               3) 如果心率值小于最小心率, 使用最小心率
*               4) 心率值在最大心率与最小心率之间, 采用当前心率值
*
*           计算公式:
*               心率区间 : [hr_max*0.55, hr_max]
*               默认心率 : hr_max * 0.9
*
* @param  : 运动状态
* @return : None
******************************************************************************************************/
uint8_t getHeartRateThreshold(SportSettingTypePicker *hrSet)
{
    sport_set_info info;
    getSafeHrInfo(hrSet, &info);

    return info.val;
}

/*****************************************************************************************************
* @brief  : 选择公英制
*
* @param  : 文本KEY
* @return : 单位文本
******************************************************************************************************/
inline const char *choiceUnitText(const char *metric, const char *imperial)
{
    system_unit unit = sportManager()->utilsManager()->common()->getSystemUnit();
    return (!unit) ? metric : imperial;
}

/*****************************************************************************************************
* @brief  : 获取单位文本
*
* @param  : 文本KEY
* @return : 单位文本
******************************************************************************************************/
const char *getUnitText(text_unit_key key)
{
    const char *text;

    switch(key)
    {
    case KEY_KM_MILE:
        text = choiceUnitText(_GT(TEXT_ID_KM), _GT(TEXT_ID_MILE));
        break;

    case KEY_KPH_MIPH:
        text = choiceUnitText(_GT(TEXT_ID_HOUR_KM), _GT(TEXT_ID_HOUR_MILE));
        break;
    
    default:
        return _ST(TK_TEMP);
    }

    return text;
}
