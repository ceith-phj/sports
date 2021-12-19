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

#include "typeInit/sport_typeInit_dialog.h"
#include "system_locale.h"


void removeSportInitAlertDialog(ScreenContext *contenxt, AlertDialog *self)
{
    self->dismissDialog(self);
    removeContainer(contenxt, self->getAlertDialogId(self));
}

AlertDialog *showSportInitAlertDialog(sport_type_t type, ScreenContext *screenContext, DialogClickListener clickListener, const char *screenName)
{
    AlertDialog *alert_dialog = creatAlertDialog(screenContext, (char*)screenName);
    addContainer(screenContext, alert_dialog->mDialogLayerPath, alert_dialog);

    switch (type)
    {
    case SPORT_TYPE_HUNTING:
        alert_dialog->setTitleAndContent(alert_dialog, NULL, System_getLocaleText("id_sport_tips_hunting"));
        break;
    case SPORT_TYPE_DRIVING:
        alert_dialog->setTitleAndContent(alert_dialog, NULL, System_getLocaleText("id_sport_tips_driving"));
        break;
    case SPORT_TYPE_CLIMB_THE_STAIRS:
        alert_dialog->setTitleAndContent(alert_dialog, NULL, System_getLocaleText("id_sport_tips_climb"));
        break;
    case SPORT_TYPE_31:/* 储存空间不足 */
        alert_dialog->setTitleAndContent(alert_dialog, NULL, System_getLocaleText("id_sport_tips_out_of_space"));
        alert_dialog->setImagePath(alert_dialog, "images/sports/dialog/not_enough_space.png");
        alert_dialog->setNegativeButton(alert_dialog, H_NULL);
        break;
    case SPORT_TYPE_32:/* 空间即将用尽 */
        alert_dialog->setTitleAndContent(alert_dialog, NULL, System_getLocaleText("id_sport_tips_space_not_enough"));
        alert_dialog->setImagePath(alert_dialog, "images/sports/dialog/not_enough_space.png");
        break;
    default:
        break;
    }
    alert_dialog->setPositiveButton(alert_dialog, H_NULL);
    alert_dialog->setClickListener(alert_dialog, clickListener);

    alert_dialog->showDialog(alert_dialog);
    return alert_dialog;
}