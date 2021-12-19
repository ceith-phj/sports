/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: wangxiaomeng@huami.com
 *  Maintainer: wangxiaomeng@huami.com
 *
 * =====================================================================================
 */

#ifndef SPORT_TYPEINIT_DIALOG_H
#define SPORT_TYPEINIT_DIALOG_H
#include "widget/alertDialog/alertDialog.h"
#include "sport_type_auto.h"

void removeSportInitAlertDialog(ScreenContext *contenxt, AlertDialog *self);
AlertDialog *showSportInitAlertDialog(sport_type_t type, ScreenContext *screenContext, DialogClickListener clickListener, const char *screenName);

#endif