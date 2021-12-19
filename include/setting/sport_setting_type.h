/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  lizheng1@huami.com
 *  Time : 2021-03-27
 *
 *  Describe : sport setting type
 *
 * ======================================================
 */
#ifndef __SPORT_SETTING_TYPE_H__
#define __SPORT_SETTING_TYPE_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _sport_setting_type {
    SETTING_TYPE_LIST,                                                          /**< 00' 列表. */
    SETTING_TYPE_RADIO_LIST,                                                    /**< 01' 单选列表. */
    SETTING_TYPE_PICKER,                                                        /**< 02' 选择器. */
}sport_setting_type;

#ifdef __cplusplus
}
#endif
#endif
