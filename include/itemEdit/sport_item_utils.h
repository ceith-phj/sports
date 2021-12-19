/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  liubingyan@huami.com
 *  Time : 2021-05-14
 *
 *  Describe : sports presenter for sports app
 *
 * ======================================================
 */

#ifndef SPORT_ITEM_UTILS_H
#define SPORT_ITEM_UTILS_H

#include <hformat.h>
#include "packages/apps/sports/include/manager/sports_item_type.h"

#ifdef __cplusplus
"C"
{
#endif

#define SPORT_ITEM_GROUP_SIZE (SPORT_ITEM_GROUP_CNT)

    const char *sportItem_getTypeNameId(sport_item_type_t sportItem);
    const char *sportItem_getTypeName(sport_item_type_t sportItem);
    const char *sportItem_getGroupNameId(sport_item_group_t groupType);
    const char* sportItem_getTypeNameChar(sport_item_type_t sportItem);
    const char *sportItem_getGroupNameChar(sport_item_group_t group);
    const SportItemGroup *sportItem_getGroupList(uint16_t group);
    const sport_item_t *sportItem_getSubList(uint16_t item);
    bool sportItem_isSupport(sport_item_type_t itemType, sport_type_t sportType);
    bool sportItem_isGroupSupport(sport_item_group_t itemGroup, sport_type_t sportType);

    // time
    extern const int timeDL[6];

    // distance
    extern const int distanceDL[3];

    // count
    extern const int countDL[6];

    // downhill
    extern const int downhillDL[5];

    // climbstair
    extern const int climbstairsDL[13];

    // swimming
    extern const int swimmingDL[15];

    // pace
    extern const int paceDL[4];

    // step
    extern const int stepDL[9];

    // speed
    extern const int speedDL[8];

    // altitude
    extern const int altitudeDL[13];

    // slope
    extern const int slopeDL[8];

    // swing
    extern const int swingDL[4];

    // consume
    extern const int consumeDL[2];

    // boating
    extern const int boatingDL[5];

    // freq
    extern const int freqDL[3];

    // golf
    extern const int golfDL[8];

    // hr
    extern const int hrDL[14];

    // pressure
    extern const int pressureDL[4];

    // temperatue
    extern const int temperatueDL[3];

    // other
    extern const int otherDataDL[8];

    /* 默认数据项类型列表 */
    extern const sport_item_descrip_t ItemDL[20];

#ifdef __cplusplus
}
#endif

#endif