#include "sport/SportDialog.h"
#include "system_locale.h"
#include "screen_manager.h"
#include <core/power/hm_power_manager.h>
#include <hlog.h>
#define TAG "sport_dialog"

static void _dismissDialogListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);
static void _exitSportListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);
static void _retryGPSListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);
static void _calibrationListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);
static void _saveConfirmListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type);

static uint32_t currentDialog = 0;

static const SportDialogInfo sportCategoryArray[SPORT_DIALOG_MAX] =
{
    [SPORT_DIALOG_AGNSS_OVERDUE] = {"id_sd_agnss_overdue_title", "id_sd_agnss_overdue_msg", "images/sports/dialog/gnss_failed.png", true, false, _dismissDialogListener, 0, false},
    [SPORT_DIALOG_CLOSE_SPORT] = {NULL, "id_sd_close_sport_msg", "images/sports/dialog/gnss_close.png", true, false, _exitSportListener, SPORT_REMIND_MODE_FAILED, false},
    [SPORT_DIALOG_GNSS_FAILED_NO_AGNSS] = {"id_gnss_failed_no_agnss_title", "id_gnss_failed_no_agnss_msg", "images/sports/dialog/gnss_failed.png", true, false, _retryGPSListener, SPORT_REMIND_MODE_FAILED, false},
    [SPORT_DIALOG_GNSS_FAILED_HAS_AGNSS] = {"id_gnss_failed_has_agnss_title", "id_gnss_failed_has_agnss_msg", "images/sports/dialog/gnss_failed.png", true, false, _retryGPSListener, SPORT_REMIND_MODE_FAILED, false},
    [SPORT_DIALOG_SAVING_SPORT] = {"id_sport_saving", NULL, NULL, false, false, NULL, 0, false},
    [SPORT_DIALOG_CALIBRATION] = {NULL, "id_sport_calibration_dialog", NULL, true, true, _calibrationListener, 0, false},
    [SPORT_DIALOG_SAVE_WARNING] = {NULL, "id_sport_save_warnning", NULL, true, true, _saveConfirmListener, 0, false},
    [SPORT_DIALOG_CLOSE_GPS] = {"id_sport_close_gps_hite", NULL, NULL, false, false, NULL, SPORT_REMIND_MODE_REMIND, true},
};

static void _calibrationListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    HLOG_I(TAG, "%s %s pressed\r\n", __func__, type == BUTTON_LEFT_CLICK ? "BUTTON_LEFT_CLICK" : "BUTTON_RIGHT_CLICK");
    switch (type)
    {
    case BUTTON_LEFT_CLICK:
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
        break;
    case BUTTON_RIGHT_CLICK:
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SAVE_CALIBRATION);
        break;
    default:
        break;
    }
}

static void _saveConfirmListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    HLOG_I(TAG, "%s %s pressed\r\n", __func__, type == BUTTON_LEFT_CLICK ? "BUTTON_LEFT_CLICK" : "BUTTON_RIGHT_CLICK");
    switch (type)
    {
    case BUTTON_LEFT_CLICK:
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
        getSportsScreenPresenter()->requestAbandon();
        getScreenManager()->startScreen("SportListScreen", H_NULL, 0);
        break;
    case BUTTON_RIGHT_CLICK:
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
        getSportsScreenPresenter()->requestSave();
        break;
    default:
        break;
    }
}


static void _exitSportListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    HLOG_I(TAG, "%s %s pressed\r\n", __func__, type == BUTTON_LEFT_CLICK ? "BUTTON_LEFT_CLICK" : "BUTTON_RIGHT_CLICK");
    switch (type)
    {
    case BUTTON_LEFT_CLICK:
    case BUTTON_RIGHT_CLICK:
        getScreenManager()->finishCurrentScreen();
        break;
    default:
        break;
    }
}

static void _dismissDialogListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    HLOG_I(TAG, "%s %s pressed\r\n", __func__, type == BUTTON_LEFT_CLICK ? "BUTTON_LEFT_CLICK" : "BUTTON_RIGHT_CLICK");
    switch (type)
    {
    case BUTTON_LEFT_CLICK:
    case BUTTON_RIGHT_CLICK:
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
        break;
    default:
        break;
    }
}

static void _retryGPSListener(ScreenContext *contenxt, AlertDialog *self, AlertButtonType type)
{
    HLOG_I(TAG, "%s %s pressed\r\n", __func__, type == BUTTON_LEFT_CLICK ? "BUTTON_LEFT_CLICK" : "BUTTON_RIGHT_CLICK");
    switch (type)
    {
    case BUTTON_LEFT_CLICK:
    case BUTTON_RIGHT_CLICK:
        HLOG_I(TAG, "%s \r\n", __func__);
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_RESET_PRESTART_TIME);
        break;
    default:
        break;
    }
}

void dismissSportDialog(ScreenContext *contenxt, AlertDialog **dialog)
{
    if (*dialog == NULL)
    {
        return;
    }
    AlertDialog *self = *dialog;

    self->dismissDialog(self);
    removeContainer(contenxt, self->mDialogLayerPath);
    *dialog = NULL;

    HLOG_I(TAG, "dialog dismiss.\r\n");
}

// 获取当前dialog信息
SportDialogInfo *SportDialogGetInfo(void)
{
    return (SportDialogInfo *)&sportCategoryArray[currentDialog];
}


void showSportDialog(SportDialogEnum type, ScreenContext *screenContext, AlertDialog **dialog)
{
    AlertDialog *alert_dialog = *dialog;

    if (type >= SPORT_DIALOG_MAX)
    {
        HLOG_I(TAG, "%s dialog type invalid\r\n", __func__, type);
        return;
    }

    currentDialog = type;

    if (alert_dialog == NULL)
    {
        char screenname[256] = {0};
        getScreenManager()->getCurrentScreen(screenname, 256);
        alert_dialog = creatAlertDialog(screenContext, screenname);
        addContainer(screenContext, alert_dialog->mDialogLayerPath, alert_dialog);
        *dialog = alert_dialog;
    }

    HLOG_I(TAG, "%s %s showed up\r\n", __func__,
                sportCategoryArray[type].title == NULL ? NULL : System_getLocaleText(sportCategoryArray[type].title));

    if(alert_dialog->isShowing(alert_dialog))
        return;

    alert_dialog->setTitleAndContent(alert_dialog,
                                     sportCategoryArray[type].title == NULL ? NULL : System_getLocaleText(sportCategoryArray[type].title),
                                     sportCategoryArray[type].msg == NULL? NULL : System_getLocaleText(sportCategoryArray[type].msg));

    if (sportCategoryArray[type].has_positiveButton)
    {
        alert_dialog->setPositiveButton(alert_dialog, H_NULL);
    }
    if (sportCategoryArray[type].has_negativeButton)
    {
        alert_dialog->setNegativeButton(alert_dialog, H_NULL);
    }

    alert_dialog->setClickListener(alert_dialog, sportCategoryArray[type].clickListener);

    alert_dialog->showDialog(alert_dialog);

    getSportsScreenPresenter()->playReminder(sportCategoryArray[type].remindMode);
    getPowerManager()->userActivity();
    return;
}
