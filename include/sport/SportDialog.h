#ifndef SPORT_DIALOG_H
#define SPORT_DIALOG_H
#include "widget/alertDialog/alertDialog.h"
#include "sport_type_auto.h"
#include <presenter/SportPresenter.h>

typedef struct
{
    char *title;
    char *msg;
    char *icon;
    bool has_positiveButton;
    bool has_negativeButton;
    DialogClickListener clickListener;
    SportRemindModeType remindMode;
    bool autoHide;
} SportDialogInfo;

typedef enum
{
    SPORT_DIALOG_AGNSS_OVERDUE = 0,     /* A-GNSS过期提醒 */
    SPORT_DIALOG_CLOSE_SPORT,           /* 长时间为开启运动关闭运动（GNSS） */
    SPORT_DIALOG_GNSS_FAILED_NO_AGNSS,  /* 定位失败，更新AGNSS可加快 */
    SPORT_DIALOG_GNSS_FAILED_HAS_AGNSS, /* 定位失败，开阔场地重试 */
    SPORT_DIALOG_ABANDON_SPORT,         /* 放弃运动 */
    SPORT_DIALOG_SAVING_SPORT,          /* 保存运动 */
    SPORT_DIALOG_CALIBRATION,           /* 校准值过大 */
    SPORT_DIALOG_SAVE_WARNING,          /* 是否保存*/
    SPORT_DIALOG_CLOSE_GPS,             /* 关闭GPS*/
    SPORT_DIALOG_MAX,
} SportDialogEnum;

SportDialogInfo *SportDialogGetInfo(void);
void showSportDialog(SportDialogEnum type, ScreenContext *screenContext, AlertDialog **handle);
void dismissSportDialog(ScreenContext *contenxt, AlertDialog **self);

#endif