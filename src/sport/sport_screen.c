#include <BaseSportView.h>
#include <stdbool.h>
#include <gre/gre.h>
#include <sb_screen_context.h>
#include <presenter/SportPresenter.h>
#include <sport_type_utils.h>
#include <hlog.h>
#include <stdio.h>
#include <string.h>
#include "hm_time.h"
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <core/power/hm_power_manager.h>
#include <widget/pageScrollerMultiLayers/pageScrollerMultiLayers.h>
#include <sport/SportPrestartContainer.h>
#include <sport/data/SportDataContainer.h>
#include <sport/SportOpreationView.h>
#include <sport/SportAutoStopView.h>
#include <sport/SportPauseOpreationView.h>
#include <sport/SportHalfOpreationView.h>
#include <sport/SportGroupOpreationView.h>
#include "core/display_metrics.h"
#include "packages/services/syssevice/screen_status/system_screen_status.h"
#include <record/record_detail_jump_utils.h>
#include "widget/alertDialog/alertDialog.h"
#include <sport/SportStateBarContainer.h>
#include <system_locale.h>
#include "sport/SportDialog.h"
#include "ui/Sports/include/reminder/sport_reminder_container.h"
#include "ui/Sports/include/transAnima/sport_trains_anima.h"
#include <sport/SportCalibrationPicker.h>
#include "framework/base/core/ui_handle.h"
#include <sport/SportLockScreenContainer.h>
#include <framework/base/battery/battery_manager.h>
#include "SportsMusicContainer.h"
#include "ui/Sports/include/group/sport_group_container.h"
#include "sport_group_auto.h"
#include "framework/base/core/device/device_info.h"
#include <sport/SportGpsNoticeContainer.h>
#include "packages/services/sport/voicebroad/sport_voice_service.h"
#include "ui/SportRecord/include/record/record_detail_data_utils.h"
#include <display/screen_display.h>

#define TAG "SportScreen"

#define ID_PAGESCROLLER_MULTILAYERS     "SportScreen.pagesContainer"
#define ID_PRESTART_CONTAINER           "SportScreen.sportPreStartContainer"
#define ID_NOTICE_CONTAINER             "SportScreen.sportGpsNoticeLayer"
#define ID_DATA_VIEW                    "SportScreen.sportDataLayer"
#define ID_STATEBAR_CONTAINER           "SportScreen.sportStateBarContainer"
#define ID_LOCK_CONTAINER               "SportScreen.sportLockContainer"
#define ID_SPORT_REMINDER_CONTAINER     "SportScreen.sport_reminder"
#define ID_SPORT_MUSIC_CONTAINER        "SportScreen.sportMusicContainer"
#define ID_SPORT_GROUP_CONTAINER        "SportScreen.sportGroupLayer"

#define ID_OPREATION_CONTAINER          "SportScreen.sportOpreationContainer"
#define ID_GROUP_OPREATION_CONTAINER    "SportScreen.sportGroupOpreationContainer"
#define ID_PAUSE_OPREATION_CONTAINER    "SportScreen.sportPauseOpreationContainer"
#define ID_HALF_OPREATION_CONTAINER     "SportScreen.sportHalfOpreationContainer"
#define ID_AUTO_OPREATION_CONTAINER     "SportScreen.sportAutoOpreationContainer"

typedef enum _operate_id_t
{
    ID_OPERATE_PRE_INDEX = 0,
    ID_OPERATE_NORMAL_INDEX,
    ID_OPERATE_PAUSE_INDEX,
    ID_OPERATE_AUTO_INDEX,
    ID_OPERATE_HALF_INDEX,
    ID_OPERATE_GROUP_INDEX,
    ID_OPERATE_INDEX_CNT,
} operate_id_t;

#define OPERATE_PAGE_CNT    ID_OPERATE_INDEX_CNT

#define TIMEOUT_SEC     800
#define LOCKWAKE_2MIN   (2 * 60)

//SCREEN_DECLARE(SCREEN_SPORT_NAME);
H_VOID API_setTpQuickSwitch(H_Bool enable);
static void onScreenPreCreate(ScreenContext *screenContext, void *data);
static void onScreenPostCreate(ScreenContext *screenContext);
static void onScreenPreDestory(ScreenContext *screenContext);
static void onScreenPostDestory(ScreenContext *screenContext);
static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event);
static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId);
static bool handleKeyEvent(ScreenContext *screenContext,
                           const HmKeyEvent *info);
static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info);
static void onAnimationStop(ScreenContext *screenContext, const char *id);
static void onAnimationComplete(ScreenContext *screenContext, const char *id);
static void onDataSave(ScreenContext *screenContext);
static void onDataRestore(ScreenContext *screenContext, void *data);
static void onScreenResume(ScreenContext *screenContext);
static void onScreenPause(ScreenContext *screenContext);
static void onNewScreenData(ScreenContext *screenContext, void *data);
static void processPauseView(ScreenContext *screenContext, void *data);
static void overView(ScreenContext *screenContext, bool isVisible);

static bool goto_sport_record = H_False;

DEFINE_SB_DATA_START(sCustomData)
.isHideSystemBar = H_True,
DEFINE_SB_DATA_END

INTERFACE_DEFINED_START(sScreenInterface)
.onScreenPreCreate = onScreenPreCreate,
.onScreenPostCreate = onScreenPostCreate,
.onScreenPreDestory = onScreenPreDestory,
.onScreenPostDestory = onScreenPostDestory,
.onNewScreenData = onNewScreenData,
.onDataRestore = onDataRestore,
.onDataSave = onDataSave,
.onAnimationComplete = onAnimationComplete,
.onAnimationStop = onAnimationStop,
.handleTouchEvent = handleTouchEvent,
.handleKeyEvent = handleKeyEvent,
.handleGestureEvent = handleGestureEvent,
.handleEvent = handleEvent,
.onScreenResume = onScreenResume,
.onScreenPause = onScreenPause,
INTERFACE_DEFINED_END

SCREEN_DECLARE_CUSTOM_START(SCREEN_SPORT_NAME)
.priority = PRIORITY_SCREEN_LOW,
.data = &sCustomData,
.screenInterface = &sScreenInterface,
SCREEN_DECLARE_CUSTOM_END

typedef struct
{
    bool isStartAnimating;
    bool isStartAnimEnd;
    H_Uint8 mPageIndex;
    H_Uint8 sportSaveSuccessNum;
    void *timerHandle;
    float choiceDistance;
    WakeLockHandle preStartWakeLockHandle;
    AlertDialog *dialogHandle;
    PickerWidget *pickerHandle;

    sport_type_t sportType;
    pageScrollerMultiLayers *viewpager;
    SportPrestartContainer *prestartContainer;
    SportGpsNoticeContainer *noticeContainer;
    SportDataContainer *sportDataContainer;
    ViewPager *sportDataViewPager;
    SportOpreationView *sportOpreationView;
    SportAutoStopView *sportAutoStopView;
    SportStateBarContainer *sportStateBarContainer;
    SportPauseOpreationView *sportPauseOpreationView;
    SportHalfOpreationView *sportHalfOpreationView;
    SportGroupOpreationView *sportGroupOpreationView;
    SportReminderContainer *reminderContainer;
    SportGroupContainer *groupContainer;
    SportTransAnima *transAnima;
    ScreenContainer *screenContainers[3][OPERATE_PAGE_CNT];
    SportLockScreenContainer *sportLockContainer;
    SportsMusicContainer *sportsMusicContainer;
    bool toShowNumber;
    uint32_t swimLockedTime;
    ScreenContext *screenContext;
    uint32_t dialogSec;
    bool isCloseDialogTrigger;
} AppData;

typedef struct
{
    SportStateBarStatus stateBarData;
    SportPrestartStatus prestartData;
    uint32_t horizonPage;
    uint32_t verticalPage;
    bool isGroupDetailShow;
    //int16_t groupNumber;
    bool isGroupPause;
    int groupTicket;
    bool toShowNumber;
    uint32_t choiceDistance;
    bool showTransAnima;
} SaveData;

static AppData *getAppData(void);
static ScreenContext *getSportsScreenContext(void);
static void createPageView(ScreenContext *screenContext);
static void initPrestartView(ScreenContext *screenContext);
static void initGpsNoticeView(ScreenContext *screenContext);
static void initDataContainer(ScreenContext *screenContext);
static void initOpreationView(ScreenContext *screenContext);
static void initAutoStopView(ScreenContext *screenContext);
static void initHalfOpreationView(ScreenContext *screenContext);
static void initPauseOpreationView(ScreenContext *screenContext);
static void initGroupOpreationView(ScreenContext *screenContext);
static void initReminderContainer(ScreenContext *screenContext);
static void initGroupContainer(ScreenContext *screenContext);
static void initTransAnima(ScreenContext *screenContext);
static void initStatusBarView(ScreenContext *screenContext);
static void showPauseView(ScreenContext *screenContext);
static void hidePauseView(ScreenContext *screenContext);
static void hidePrestartView(ScreenContext *screenContext,
                             SportStateType SportState);
static void hideStatusBarView(ScreenContext *screenContext);
static void showStatusBarView(ScreenContext *screenContext);
static void initPresenter(ScreenContext *screenContext);
static void initData(ScreenContext *screenContext);
static void onSportStartAnimEnd(void);
static void onSportPause(ScreenContext *screenContext);
static void onSportRunning(ScreenContext *screenContext);
static void onSportEnd(ScreenContext *screenContext);
static void onSportSave(ScreenContext *screenContext);
static void onSportResume(ScreenContext *screenContext,
                          SportStateType SportState);
static void onSportSaveDone(ScreenContext *screenContext);
static bool isPreStartAnimating();
static void setPrestartViewVisiable(ScreenContext *screenContext, bool show);
static void setOpreationViewVisiable(ScreenContext *screenContext, bool show);
static void setPauseOpreationViewVisiable(ScreenContext *screenContex,
        bool show);
static void setHalfOpreationViewVisiable(ScreenContext *screenContex,
        bool show);
static void setGroupOpreationViewVisiable(ScreenContext *screenContex,
        bool show);
static void _creatScreenTimer(ScreenContext *screenContext);
static void _destroyTimer(ScreenContext *screenContext);
static void _showSaveConfirmDialog(ScreenContext *screenContext);
static void _pauseTimer(ScreenContext *screenContext);
static void _showCalibrationPicker(ScreenContext *screenContext);
static void initLockContainer(ScreenContext *screenContext);
static void initSportsMusicContainer(ScreenContext *screenContext);
static void onPageViewScrolled(pageScrollerMultiLayers *scroll, int page);

static bool isTouchEventToScrollerPages(const char *id)
{
    if (STR_MATCH(id, TAG))
    {
        return true;
    }

    pageScrollerMultiLayers *scroll = (pageScrollerMultiLayers *)findContainer(
                                          getSportsScreenContext(), ID_PAGESCROLLER_MULTILAYERS);

    if (scroll)
    {
        if (scroll->isLayerInclude(scroll, id))
        {
            return true;
        }
    }

    return false;
}

static void notifySportType(sport_type_t type)
{
    HLOG_I(TAG, "notifySportType: %d\r\n", type);
    AppData *data = getScreenData(getSportsScreenContext(), AppData);
    data->sportType = type;
    /* SportPrestartContainer *prestartContainer = data->prestartContainer;
    prestartContainer->setSportType(prestartContainer, type); */
}

static void _notifySaveSuccessCallbackNum(H_Uint8 value)
{
    HLOG_I(TAG, "%s: %d\r\n", __func__, value);
    AppData *data = getScreenData(getSportsScreenContext(), AppData);
    data->sportSaveSuccessNum = value;
}

static void _unlockWakeLockHandle(ScreenContext *screenContext)
{
    if (getAppData()->preStartWakeLockHandle)
    {
        getPowerManager()->release(getAppData()->preStartWakeLockHandle);
        getAppData()->preStartWakeLockHandle = 0;
    }
}

static void _lockWakeLockHandle(ScreenContext *screenContext)
{
    if (getAppData()->preStartWakeLockHandle)
    {
        return;
    }

    WakeLockHandle handle = getPowerManager()->acquire("sport pre start",
                            SCREEN_BRIGHT_WAKE_LOCK | ON_AFTER_RELEASE);
    getAppData()->preStartWakeLockHandle = handle;
}

static void _lockWakeLockHandleWithTimeout(ScreenContext *screenContext,
        uint32_t time_s)
{
    if (getAppData()->preStartWakeLockHandle)
    {
        _unlockWakeLockHandle(screenContext);
    }

    WakeLockHandle handle = getPowerManager()->acquireWithTimeout("sport pre start",
                            SCREEN_BRIGHT_WAKE_LOCK | ON_AFTER_RELEASE, time_s);
    getAppData()->preStartWakeLockHandle = handle;
}

static void _SwitchToActivePage(bool anim)
{
    if (getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_RUN)
    {
        if (getAppData()->mPageIndex != 1)
        {
            getAppData()->viewpager->scrollToPage(getAppData()->viewpager, 1, anim);

            if (anim == 0)
            {
                onPageViewScrolled(getAppData()->viewpager, 1);
            }

            /* Todo viewpager 增加锁屏回调，锁屏时可能要调整页面，如不能锁在指南针页面 */
        }
    }
    else if (getSportsScreenPresenter()->requestSportStatus() ==
             SPORT_STATE_SUSPEND)
    {
        if (getAppData()->mPageIndex != 0)
        {
            getAppData()->viewpager->scrollToPage(getAppData()->viewpager, 0, anim);

            if (anim == 0)
            {
                onPageViewScrolled(getAppData()->viewpager, 0);
            }
        }
    }
}

static void _notifySportUIEvent(SportUIEventType event)
{
    HLOG_I(TAG, "notifySportUIEvent: %d\r\n", event);

    switch (event)
    {
        case SPORTUI_EVENT_HIDE_STATUS_BAR:
            hideStatusBarView(getSportsScreenContext());
            break;

        case SPORTUI_EVENT_SHOW_STATUS_BAR:
            showStatusBarView(getSportsScreenContext());
            break;

        case SPORTUI_EVENT_GOHOME:
            getScreenManager()->gotoHome();
            break;

        case SPORTUI_EVENT_SHOW_GNSS_FAILED_NO_AGNSS:
            showSportDialog(SPORT_DIALOG_GNSS_FAILED_NO_AGNSS, getSportsScreenContext(),
                            &getAppData()->dialogHandle);
            _lockWakeLockHandleWithTimeout(getSportsScreenContext(), LOCKWAKE_2MIN);
            break;

        case SPORTUI_EVENT_SHOW_GNSS_FAILED_HAS_AGNSS:
            showSportDialog(SPORT_DIALOG_GNSS_FAILED_HAS_AGNSS, getSportsScreenContext(),
                            &getAppData()->dialogHandle);
            _lockWakeLockHandleWithTimeout(getSportsScreenContext(), LOCKWAKE_2MIN);
            break;

        case SPORTUI_EVENT_SHOW_NO_AGNSS:
            showSportDialog(SPORT_DIALOG_AGNSS_OVERDUE, getSportsScreenContext(),
                            &getAppData()->dialogHandle);
            break;

        case SPORTUI_EVENT_PRESTART_EXITSPORT:
            showSportDialog(SPORT_DIALOG_CLOSE_SPORT, getSportsScreenContext(),
                            &getAppData()->dialogHandle);
            _unlockWakeLockHandle(getSportsScreenContext());
            getSportsScreenPresenter()->requestExit();
            _pauseTimer(getSportsScreenContext());
            break;

        case SPORTUI_EVENT_DISMISS_DIALOG:
            dismissSportDialog(getSportsScreenContext(), &getAppData()->dialogHandle);

            if (getAppData()->prestartContainer->status.AGPSTipsIsShow !=
                    SPORT_APGS_TIPS_DISMISS)
            {
                getAppData()->prestartContainer->status.AGPSTipsIsShow =
                    SPORT_APGS_TIPS_DISMISS;
            }

            break;

        case SPORTUI_EVENT_RESET_PRESTART_TIME:
            getAppData()->prestartContainer->resetPresatrtTime(
                getAppData()->prestartContainer);
            _lockWakeLockHandle(getSportsScreenContext());
            break;

        case SPORTUI_EVENT_UNLOCK_WAKELOCK: /* 释放屏幕锁，允许黑屏*/
            _unlockWakeLockHandle(getSportsScreenContext());
            break;

        case SPORTUI_EVENT_SHOW_DIALOG_CALIBRATION:
            showSportDialog(SPORT_DIALOG_CALIBRATION, getSportsScreenContext(),
                            &getAppData()->dialogHandle);
            break;

        case SPORTUI_EVENT_SAVE_CALIBRATION:
            getSportsScreenPresenter()->requestCalibration(sportCalibrationPickerGetData(
                        getAppData()->pickerHandle));
            getAppData()->choiceDistance = 0;
            break;

        case SPORTUI_EVENT_UPDATE_CALIBATION:
            getAppData()->choiceDistance = sportCalibrationPickerGetData(
                                               getAppData()->pickerHandle);
            HLOG_I(TAG, "SPORTUI_EVENT_UPDATE_CALIBATION %d\r\n",
                   getAppData()->choiceDistance);
            break;

        case SPORTUI_EVENT_LOCK_SCREEN:
            getAppData()->sportLockContainer->setLock(getAppData()->sportLockContainer, true);
            getAppData()->sportStateBarContainer->setLock(getAppData()->sportStateBarContainer, true);

            if (getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
            {
                API_setTpQuickSwitch(false);
            }

            break;

        case SPORTUI_EVENT_UNLOCK_SCREEN:
            getAppData()->sportStateBarContainer->setLock(getAppData()->sportStateBarContainer, false);
            if (getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
            {
                API_setTpQuickSwitch(true);
            }

            break;
        case SPORTUI_EVENT_SHOW_PRESTART:
            getAppData()->prestartContainer->setVisiable(getAppData()->prestartContainer, 1);
            getAppData()->noticeContainer->setVisiable(getAppData()->noticeContainer, 0);
            break;

        case SPORTUI_EVENT_SHOW_GPS_NOTICE:
            getAppData()->prestartContainer->setVisiable(getAppData()->prestartContainer, 0);
            getAppData()->noticeContainer->setVisiable(getAppData()->noticeContainer, 1);
            break;

        case SPORTUI_EVENT_START_SPORT:
            getAppData()->noticeContainer->setVisiable(getAppData()->noticeContainer, 0);
            System_setKeyFilter(HM_KEY_CODE_HOME,
                                HM_KEY_ACT_FILTER_CLICK | HM_KEY_ACT_FILTER_LONG_PRESS);
            System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
            getSportsScreenPresenter()->requestStartAnim();
            getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
            break;
        case SPORTUI_EVENT_CLOSE_GPS:
            showSportDialog(SPORT_DIALOG_CLOSE_GPS, getSportsScreenContext(), &getAppData()->dialogHandle);
            break;
        default:
            break;
    }
    gre_redraw(getSportsScreenContext()->_grAppContext);
}

static void notifySportEvent(SportEventType event)
{
    HLOG_I(TAG, "notifySportEvent: %d\r\n", event);

    switch (event)
    {
        case SPORT_EVENT_PRESTART: // prestart
            //open lcd hold on
            _lockWakeLockHandle(getSportsScreenContext());
            break;

        case SPORT_EVENT_RUN: // sporting
            sportVoice_broadSportControl(SPORT_CMD_START); /* 语音播报：开始运动*/
            onSportRunning(getSportsScreenContext());
            getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_ACTION);
            break;

        case SPORT_EVENT_CONTINUE:
            if (getAppData()->sportAutoStopView->isAutoStop)
            {
                getAppData()->sportAutoStopView->eventHandle(getAppData()->sportAutoStopView,
                        event);
            }
            else
            {
                if (getSportsScreenPresenter()->isHalfSupported(
                            getSportsScreenPresenter()->getSportType()) &&
                        !getAppData()->sportHalfOpreationView->isSecondHalfStart(
                            getAppData()->sportHalfOpreationView))
                {
                    getAppData()->sportHalfOpreationView->eventHandle(
                        getAppData()->sportHalfOpreationView, event);
                }
                else
                {
                    getAppData()->sportPauseOpreationView->eventHandle(
                        getAppData()->sportPauseOpreationView, event);
                }
            }

            sportVoice_broadSportControl(SPORT_CMD_RESUME); /* 语音播报：继续运动*/
            onSportRunning(getSportsScreenContext());
            getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_ACTION);
            getPowerManager()->userActivity();

            // swin-like type, screen need be locked when continued
            if (getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
            {
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_LOCK_SCREEN);
            }
            break;

        case SPORT_EVENT_SUSPEND: // pause
            if (getAppData()->sportAutoStopView->isAutoStop)
            {
                getAppData()->sportAutoStopView->eventHandle(getAppData()->sportAutoStopView,
                        event);
            }
            else
            {
                if (getSportsScreenPresenter()->isHalfSupported(sportGroup_get(
                            getSportsScreenPresenter()->getSportType())) &&
                        !getAppData()->sportHalfOpreationView->isSecondHalfStart(
                            getAppData()->sportHalfOpreationView))
                {
                    getAppData()->sportHalfOpreationView->eventHandle(
                        getAppData()->sportHalfOpreationView, event);
                }
                else
                {
                    getAppData()->sportPauseOpreationView->eventHandle(
                        getAppData()->sportPauseOpreationView, event);
                }
            }

            sportVoice_broadSportControl(SPORT_CMD_PAUSE); /* 语音播报：运动已暂停*/
            onSportPause(getSportsScreenContext());
            getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_ACTION);
            getPowerManager()->userActivity();
            break;

        case SPORT_EVENT_STOP:
            getAppData()->sportHalfOpreationView->isNextHalfStarted = false;
            onSportEnd(getSportsScreenContext());
            break;

        case SPORT_EVENT_SAVE:
            getAppData()->sportHalfOpreationView->isNextHalfStarted = false;
            onSportSave(getSportsScreenContext());
            break;

        case SPORT_EVENT_SAVEDONE:
            onSportSaveDone(getSportsScreenContext());
            getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_WARN);
            break;

        case SPORT_EVENT_CALIBRATION:
            _showCalibrationPicker(getSportsScreenContext());
            break;

        case SPORT_EVENT_SAVE_WARNING:
            _showSaveConfirmDialog(getSportsScreenContext());
            break;

        case SPORT_EVENT_ABANDON:
            if (getAppData()->transAnima)
            {
                getAppData()->transAnima->finish(getAppData()->transAnima);
            }

            break;

        default:
            break;
    }

    if (getAppData()->sportDataContainer != H_NULL)
    {
        getAppData()->sportDataContainer->sportStatus(getAppData()->sportDataContainer,
                (void *)event);
    }

    if (getAppData()->groupContainer &&
            getAppData()->groupContainer->isSptGroupType(getAppData()->groupContainer))
    {
        getAppData()->groupContainer->eventHandle(getAppData()->groupContainer, event);
    }
}

static void notifyStartAnim(void)
{
    HLOG_I(TAG, "notifyPreStart!\r\n");
//    bool isGpShow = getAppData()->groupContainer->isSptGroupType(getAppData()->groupContainer);
//    if (isGpShow)
//    {
//       gre_set_layer_visible(getAppInstance(), "SportScreen.sportDataLayer2", false);
//    }
    getAppData()->viewpager->scrollToPage(getAppData()->viewpager, 1, true);
    getAppData()->isStartAnimating = true;

    if(sportSetting_getSettingManager()->swimTouch->get(getSportsScreenPresenter()->getSportType())){
        getAppData()->swimLockedTime = 0;
        setSBStringData(getAppInstance(),"swimLockHiteLayer.lockHite.lockHiteText",System_getLocaleText("id_sport_swim_lock_hite"));
        gre_set_layer_visible(getAppInstance(),"SportScreen.swimLockHiteLayer",1);
    }else{
         gre_set_layer_visible(getAppInstance(),"SportScreen.swimLockHiteLayer",0);
    }
}

static SportCallback sportCallback =
{
    .notifySportType = notifySportType,
    .notifySportEvent = notifySportEvent,
    .notifyStartAnim = notifyStartAnim,
    .notifySaveSuccessCallbackNum = _notifySaveSuccessCallbackNum,
    .notifySportUIEvent = _notifySportUIEvent,

};

static AppData *getAppData()
{
    AppData *data = getScreenData(getSportsScreenContext(), AppData);
    return data;
}

static void onPageViewScrolled(pageScrollerMultiLayers *scroll, int page)
{
    if (getAppData() == NULL)
    {
        HLOG_I(TAG, "%s but sport screen destroy\r\n", __func__);
        return;
    }

    getAppData()->mPageIndex = page;
    getAppData()->sportStateBarContainer->setScheduleCurrent(getAppData()->sportStateBarContainer, page);
    getAppData()->sportDataViewPager->setScrollVisiable(getAppData()->sportDataViewPager, page == 1 ? 1 : 0);

    if (getAppData()->screenContext)
    {
        gre_set_layer_yoffset(getAppData()->screenContext->_grAppContext, "SportScreen.sportOpreationLayer", 0);
        gre_set_layer_yoffset(getAppData()->screenContext->_grAppContext, "SportScreen.sportPauseOpreationLayer", 0);
        gre_set_layer_yoffset(getAppData()->screenContext->_grAppContext, "SportScreen.sportAutoStopLayer", 0);
        gre_set_layer_yoffset(getAppData()->screenContext->_grAppContext, "SportScreen.sportHalfOpreationLayer", 0);
        gre_set_layer_yoffset(getAppData()->screenContext->_grAppContext, "SportScreen.sportGroupOpreationLayer", 0);
    }

    if (getAppData()->mPageIndex == 1 && isPreStartAnimating())
    {
        onSportStartAnimEnd();
    }

    if (getAppData()->transAnima && getAppData()->transAnima->showTransAnima)
    {
        getAppData()->transAnima->showTransAnima = false;
        getAppData()->transAnima->start(getAppData()->transAnima);
    }

    HLOG_I(TAG, "%s page index: %d \r\n", __func__, page);

    if (2 == getAppData()->mPageIndex && H_NULL != getAppData()->sportsMusicContainer)
    {
        getAppData()->sportsMusicContainer->updateSportsMusicStatus(getAppData()->sportsMusicContainer);
    }
    else if (1 == getAppData()->mPageIndex && H_NULL != getAppData()->sportsMusicContainer)
    {
        getAppData()->sportsMusicContainer->setSportsMusicGrpVisible(getAppData()->sportsMusicContainer);
        getAppData()->sportsMusicContainer->unRegisterCB(getAppData()->sportsMusicContainer);
    }
    else if (0 == getAppData()->mPageIndex && sportManager()->ctrlManager()->getState() == SPORT_STATE_PRESTART)
    {
        if(getSportsScreenPresenter()->getGpsStatus())
            getSportsScreenPresenter()->playReminder(SPORT_REMIND_MODE_SUCCESS);
    }
}

static void onSportStartAnimEnd(void)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    getAppData()->prestartContainer->setVisiable(getAppData()->prestartContainer, 0);
    getAppData()->noticeContainer->setVisiable(getAppData()->noticeContainer, 0);
    getAppData()->isStartAnimEnd = true;
    getSportsScreenPresenter()->requestStart();
    if (getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_LOCK_SCREEN);
    }
}

static void onSportPause(ScreenContext *screenContext)
{
    showPauseView(screenContext);
}

static void onSportEnd(ScreenContext *screenContext)
{
    if (getAppData()->prestartContainer->status.ExitSportIsShow == true)
    {
        return;
    }

    getScreenManager()->finishCurrentScreen();
}
static void onSportSaveDone(ScreenContext *screenContext)
{
    getAppData()->sportSaveSuccessNum--;
    getAppData()->transAnima->finish(getAppData()->transAnima);
    HLOG_I(TAG, "onSportSaveDone:%d", getAppData()->sportSaveSuccessNum);

    if (getAppData()->sportSaveSuccessNum == 0)
    {
        //goto sport record
        singleSportjumpDetail(0, FLAG_SCREEN_REMOVE_CURRENT, FROM_SPORT_END_DETAIL);
    }
}

static void _showSaveConfirmDialog(ScreenContext *screenContext)
{
    showSportDialog(SPORT_DIALOG_SAVE_WARNING, screenContext,
                    &getAppData()->dialogHandle);
}

static void _showSavingDialog(ScreenContext *screenContext)
{
    showSportDialog(SPORT_DIALOG_SAVING_SPORT, screenContext,
                    &getAppData()->dialogHandle);
}
static void _showCalibrationPicker(ScreenContext *screenContext)
{
    sportCalibrationPickerOpen(screenContext, &getAppData()->pickerHandle,
                               "sportPicker");
    HLOG_I(TAG, "_showCalibrationPicker %d\r\n",
           getAppData()->choiceDistance);

    if (getAppData()->choiceDistance > 0)
    {
        sportCalibrationPickerRestore(getAppData()->pickerHandle,
                                      getAppData()->choiceDistance);
    }
}
static void onSportSave(ScreenContext *screenContext)
{
    // dial-state-dot should be unregistered when sport finished
    getSportsScreenPresenter()->requestDialState(NULL, 0);

    // ##
    _showSavingDialog(screenContext);
    _pauseTimer(screenContext);
}

static bool autoRecogEventListener(EventData *eventData)
{
    if (eventData->arg1 == EVT_AUTO_DETECT)
    {
        getAppData()->sportAutoStopView->autoRecogEventHandle(
            getAppData()->sportAutoStopView, (SportAutoRecogStatus)eventData->arg2);
    }

    return true;
}

static bool screenEventListener(EventData* eventData)
{
    if(getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_PRESTART)
    {
        // 运动前，禁止首次按键分发，只亮屏
        System_dispatchFirstKeyOnScreenOff(H_False);
        return true;
    }

    switch ((enum ScreenStatus)screen_parserScreenStatus(eventData))
    {
        case SCREEN_ON:
            // 亮屏状态下，按键即刻分发
            System_dispatchFirstKeyOnScreenOff(H_True);
            HLOG_I(TAG, "screen state : SCREEN_ON\r\n");
            break;
        case SCREEN_OFF:
            if (getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
            {
                // 游泳运动，灭屏后，禁止首次按键分发，只亮屏
                System_dispatchFirstKeyOnScreenOff(H_False);
            }
            else
            {
                // 非游泳运动，灭屏后，按键即刻分发
                System_dispatchFirstKeyOnScreenOff(H_True);
            }
            HLOG_I(TAG, "screen state : SCREEN_OFF\r\n");
            break;
        case SCREEN_IDLE:
            if (getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
            {
                // 游泳运动，进入息屏后，禁止首次按键分发，只亮屏
                System_dispatchFirstKeyOnScreenOff(H_False);
            }
            else
            {
                // 非游泳运动，进入息屏后，按键即刻分发
                System_dispatchFirstKeyOnScreenOff(H_True);
            }
            HLOG_I(TAG, "screen state : SCREEN_IDLE\r\n");
            break;
        default:
            break;
    }
    return true;
}

static void onSportResume(ScreenContext *screenContext,
                          SportStateType SportState)
{
    H_Uint8 horPageIndex = 1;

    if (SportState == SPORT_STATE_SUSPEND)
    {
        horPageIndex = 0;
    }
    else
    {
        horPageIndex = 1;
    }

    getAppData()->viewpager->scrollToPage(getAppData()->viewpager, horPageIndex,
                                          false);
    getAppData()->mPageIndex = horPageIndex;
    getAppData()->sportStateBarContainer->setScheduleCurrent(
        getAppData()->sportStateBarContainer, horPageIndex);
    hidePrestartView(screenContext, SportState);
    getAppData()->isStartAnimEnd = true;
    SportService_getInstance()->addTaskEventListener(getUIHandle(),
            SPORT_SERVICE_EVT_REPORT_EVENT, autoRecogEventListener);
}

static void onSportRunning(ScreenContext *screenContext)
{
    getAppData()->transAnima->showTransAnima = true;
    getAppData()->viewpager->scrollToPage(getAppData()->viewpager, 1, true);
    hidePauseView(screenContext);
    getAppData()->sportDataViewPager->setScrollVisiable(
        getAppData()->sportDataViewPager, 1);
    _unlockWakeLockHandle(screenContext);
}

static bool isPreStartAnimating()
{
    return getAppData()->isStartAnimating && !getAppData()->isStartAnimEnd;
}

static bool _isInSetting(void)
{
    return getAppData()->prestartContainer->status.isInSetting;
}

// close-gps listener
static void sport_event_closeGPS(void)
{
    HmTime time;
    hmTime_getTime(&time);
    getAppData()->dialogSec = time.sec;
    getAppData()->isCloseDialogTrigger = true;
    HLOG_I(TAG, "%s shows closing-gps @ %d\r\n", __func__, getAppData()->dialogSec);
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    // dial-state-dot should be unregistered when sport screen entered
    getSportsScreenPresenter()->requestDialState(NULL, 0);

    // register screen changed listener
    screen_registerScreenStatusListener((Hm_ThreadHandle)getUIHandle(), screenEventListener);

    if (getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_STOP &&
        SportAutoRecogService_getOps()->getAndClearAutoStopStaus())
    {
        HLOG_I(TAG, "%s, last sport closed by ar service, goto sport record!",
               __func__);
        overView(screenContext, true);
        goto_sport_record = H_True;
        singleSportjumpDetail(0, FLAG_SCREEN_REMOVE_CURRENT, FROM_SPORT_END_DETAIL);
        return;
    }
    else
    {
        overView(screenContext, false);
    }

    if (getSportsScreenPresenter()->getSportType() == SPORT_TYPE_NONE)
    {
        HLOG_I(TAG, "%s [SPTERR] invalid sport type!\r\n", __func__);
        getSportsControlManager()->setType(SPORT_TYPE_OUTDOOR_RUNNING);
    }

    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));

    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        appData->toShowNumber = true;
        setScreenData(screenContext, appData);
    }

    initPresenter(screenContext);
    initPrestartView(screenContext);
    initGpsNoticeView(screenContext);
    initStatusBarView(screenContext);
    initDataContainer(screenContext);
    initOpreationView(screenContext);
    initAutoStopView(screenContext);
    initPauseOpreationView(screenContext);
    initHalfOpreationView(screenContext);
    initGroupOpreationView(screenContext);
    initReminderContainer(screenContext);
    initGroupContainer(screenContext);
    initTransAnima(screenContext);
    initLockContainer(screenContext);
    createPageView(screenContext);
    initData(screenContext);
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}

static void initPresenter(ScreenContext *screenContext)
{
    getSportsScreenPresenter()->init();
    getSportsScreenPresenter()->setCallback(&sportCallback);
}

static void initData(ScreenContext *screenContext)
{
    getSportsScreenPresenter()->requestSportType();
    SportStateType SportState = getSportsScreenPresenter()->requestSportStatus();

    if (getAppData()->sportAutoStopView != NULL)
    {
        getAppData()->sportAutoStopView->containerRecover(
            getAppData()->sportAutoStopView);
    }

    if (SportState == SPORT_STATE_RUN || SportState == SPORT_STATE_SUSPEND)
    {
        onSportResume(screenContext, SportState);
    }
}

static void initPrestartView(ScreenContext *screenContext)
{
    SportPrestartContainer *prestartContainer = bindSportPrestartView(
                screenContext->_grAppContext);
    addContainer(screenContext, ID_PRESTART_CONTAINER, prestartContainer);
    getAppData()->screenContainers[0][ID_OPERATE_PRE_INDEX] =
        (ScreenContainer *)prestartContainer;
    getAppData()->prestartContainer = prestartContainer;
    prestartContainer->initContainer(prestartContainer, screenContext);
}

static void initGpsNoticeView(ScreenContext *screenContext)
{
    SportGpsNoticeContainer *noticeContainer = bindSportGpsNoticeView(screenContext->_grAppContext);
    addContainer(screenContext, ID_NOTICE_CONTAINER, noticeContainer);
    getAppData()->noticeContainer = noticeContainer;
    noticeContainer->initContainer(noticeContainer, screenContext);
}

static void initDataContainer(ScreenContext *screenContext)
{
    SportDataContainer *sportDataContainer = bindSportDataView(screenContext);
    getScreenData(screenContext, AppData)->sportDataContainer = sportDataContainer;
    addContainer(screenContext, ID_DATA_VIEW, sportDataContainer);
    sportDataContainer->initData(sportDataContainer, "sportDataLayer", TAG);
    sportDataContainer->initContainer(sportDataContainer, screenContext);
    getAppData()->sportDataViewPager = sportDataContainer->dataPageView;
    sportDataContainer->dataPageView->setScrollVisiable(
        sportDataContainer->dataPageView, 0);
    getAppData()->screenContainers[1][0] = (ScreenContainer *)sportDataContainer;
}

static void initOpreationView(ScreenContext *screenContext)
{
    SportOpreationView *sportOpreationView = bindSportOpreationView(
                screenContext->_grAppContext);
    getScreenData(screenContext, AppData)->sportOpreationView = sportOpreationView;
    addContainer(screenContext, ID_OPREATION_CONTAINER, sportOpreationView);
    getAppData()->screenContainers[0][ID_OPERATE_NORMAL_INDEX] =
        (ScreenContainer *)sportOpreationView;
    sportOpreationView->initContainer(sportOpreationView, screenContext);
    sportOpreationView->setVisiable(sportOpreationView, 0);
    getAppData()->sportOpreationView = sportOpreationView;
}

static void initPauseOpreationView(ScreenContext *screenContext)
{
    if (getAppData()->sportPauseOpreationView)
    {
        return;
    }

    SportPauseOpreationView *sportPauseOpreationView =
        bindSportPauseOpreationContainer(screenContext->_grAppContext);
    getScreenData(screenContext,
                  AppData)->sportPauseOpreationView = sportPauseOpreationView;
    addContainer(screenContext, ID_PAUSE_OPREATION_CONTAINER,
                 sportPauseOpreationView);
    getAppData()->screenContainers[0][ID_OPERATE_PAUSE_INDEX] =
        (ScreenContainer *)sportPauseOpreationView;
    sportPauseOpreationView->initContainer(sportPauseOpreationView, screenContext);
    sportPauseOpreationView->setVisiable(sportPauseOpreationView, 0);
    getAppData()->sportPauseOpreationView = sportPauseOpreationView;
}

static void initAutoStopView(ScreenContext *screenContext)
{
    SportAutoStopView *sportAutoStopView = bindSportAutoStopView(
            screenContext->_grAppContext);
    getScreenData(screenContext, AppData)->sportAutoStopView = sportAutoStopView;
    addContainer(screenContext, ID_AUTO_OPREATION_CONTAINER, sportAutoStopView);
    getAppData()->screenContainers[0][ID_OPERATE_AUTO_INDEX] =
        (ScreenContainer *)sportAutoStopView;
    sportAutoStopView->initContainer(sportAutoStopView, screenContext);
    sportAutoStopView->setVisiable(sportAutoStopView, 0);
    getAppData()->sportAutoStopView = sportAutoStopView;
}

static void initHalfOpreationView(ScreenContext *screenContext)
{
    if (getAppData()->sportHalfOpreationView)
    {
        return;
    }

    SportHalfOpreationView *sportHalfOpreationView =
        bindSportHalfOpreationContainer(screenContext->_grAppContext);
    getScreenData(screenContext,
                  AppData)->sportHalfOpreationView = sportHalfOpreationView;
    addContainer(screenContext, ID_HALF_OPREATION_CONTAINER,
                 sportHalfOpreationView);
    getAppData()->screenContainers[0][ID_OPERATE_HALF_INDEX] =
        (ScreenContainer *)sportHalfOpreationView;
    sportHalfOpreationView->initContainer(sportHalfOpreationView, screenContext);
    sportHalfOpreationView->setVisiable(sportHalfOpreationView, 0);
    getAppData()->sportHalfOpreationView = sportHalfOpreationView;
}

static void startNextGroupHandler(void *data)
{
    SportGroupOpreationView *sportGroupOpreationView = (SportGroupOpreationView *)
            data;
    sportGroupOpreationView->isBreakStarted = false;
}

static void initGroupOpreationView(ScreenContext *screenContext)
{
    if (getAppData()->sportGroupOpreationView)
    {
        return;
    }

    SportGroupOpreationView *sportGroupOpreationView =
        bindSportGroupOpreationContainer(screenContext->_grAppContext);
    getScreenData(screenContext,
                  AppData)->sportGroupOpreationView = sportGroupOpreationView;
    addContainer(screenContext, ID_GROUP_OPREATION_CONTAINER,
                 sportGroupOpreationView);
    getAppData()->screenContainers[0][ID_OPERATE_GROUP_INDEX] =
        (ScreenContainer *)sportGroupOpreationView;
    sportGroupOpreationView->initContainer(sportGroupOpreationView, screenContext);
    sportGroupOpreationView->setVisiable(sportGroupOpreationView, 0);
    getAppData()->sportGroupOpreationView = sportGroupOpreationView;
}

static void initStatusBarView(ScreenContext *screenContext)
{
    SportStateBarContainer *sportStateBarContainer = bindSportStateBarView(
                screenContext->_grAppContext);

    if (sportStateBarContainer)
    {
        addContainer(screenContext, ID_STATEBAR_CONTAINER, sportStateBarContainer);
        getAppData()->sportStateBarContainer = sportStateBarContainer;
        sportStateBarContainer->initContainer(sportStateBarContainer, screenContext);
    }
}


static void initLockContainer(ScreenContext *screenContext)
{
    SportLockScreenContainer *sportLockContainer = bindSportLockScreenView(
                screenContext->_grAppContext);

    if (sportLockContainer)
    {
        addContainer(screenContext, ID_LOCK_CONTAINER, sportLockContainer);
        getAppData()->sportLockContainer = sportLockContainer;
        sportLockContainer->initContainer(sportLockContainer, screenContext);
    }
}

static void initReminderContainer(ScreenContext *screenContext)
{
    if (getAppData()->reminderContainer)
    {
        return;
    }

    SportReminderContainer *reminderContainer = createReminderContainer(
                screenContext);
    reminderContainer->screenContext = screenContext;
    reminderContainer->app = screenContext->_grAppContext;
    getScreenData(screenContext, AppData)->reminderContainer = reminderContainer;
    addContainer(screenContext, ID_SPORT_REMINDER_CONTAINER, reminderContainer);
    getAppData()->reminderContainer = reminderContainer;
}

static void initSportsMusicContainer(ScreenContext *screenContext)
{
    if (getAppData()->sportsMusicContainer)
    {
        HLOG_E(TAG, "%s, sportsMusicContainer is not null", __func__);
        return;
    }

    SportsMusicContainer *sportsMusicContainer = createSportsMusicContainer(
                screenContext);

    if (H_NULL != sportsMusicContainer)
    {
        sportsMusicContainer->initWithClone(sportsMusicContainer,
                                            "sportMusicControlLayer");
        sportsMusicContainer->screenContext = screenContext;
        addContainer(screenContext, ID_SPORT_MUSIC_CONTAINER, sportsMusicContainer);
        getAppData()->sportsMusicContainer = sportsMusicContainer;
        getAppData()->screenContainers[2][0] = (ScreenContainer *)sportsMusicContainer;
    }
}

static void _transAnima_timerOutCallBack()
{
    bool isGpShow = getAppData()->groupContainer->isSptGroupType(
                        getAppData()->groupContainer);

    if (isGpShow && getAppData()->toShowNumber)
    {
        getAppData()->groupContainer->SptGroupUIShow(getAppData()->groupContainer,
                SPTGP_NUMBER);
        gre_set_layer_visible(getAppInstance(), "SportScreen.sportDataLayer2", false);
        getAppData()->sportDataViewPager->setScrollVisiable(
            getAppData()->sportDataViewPager, 0);
        getAppData()->toShowNumber = false;
    }

    getAppData()->transAnima->animaTimerOutCallBack = NULL;
}

static void initTransAnima(ScreenContext *screenContext)
{
    if (getAppData()->transAnima)
    {
        return;
    }

    SportTransAnima *transAnima = createSportTransAnima(screenContext);
    getScreenData(screenContext, AppData)->transAnima = transAnima;
    transAnima->animaTimerOutCallBack = _transAnima_timerOutCallBack;
}

static void _SptGroup_timerOutCallBack()
{
    gre_set_layer_visible(getAppInstance(), "SportScreen.sportDataLayer2", true);
    getAppData()->viewpager->scrollToPage(getAppData()->viewpager, 1, false);
    getAppData()->sportDataViewPager->setScrollVisiable(
        getAppData()->sportDataViewPager, 1);
    getAppData()->sportDataViewPager->setCureentItem(
        getAppData()->sportDataViewPager, 1);

    if (isPreStartAnimating())
    {
        onSportStartAnimEnd();
    }
}

static void _SptGroup_ScreenDataInit()
{
    getAppData()->sportDataViewPager->setScrollVisiable(
        getAppData()->sportDataViewPager, 0);
}

static void initGroupContainer(ScreenContext *screenContext)
{
    if (getAppData()->groupContainer)
    {
        return;
    }

    SportGroupContainer *_groupContainer = createGroupContainer(screenContext,
                                           getAppData()->reminderContainer->presenter);
    _groupContainer->screenContext = screenContext;
    _groupContainer->app = screenContext->_grAppContext;
    _groupContainer->timerOutCallBack = _SptGroup_timerOutCallBack;
    _groupContainer->parentScreenDataInit = _SptGroup_ScreenDataInit;
    _groupContainer->setNextGroupHandler(_groupContainer,
                                         getAppData()->sportGroupOpreationView, startNextGroupHandler);
    getScreenData(screenContext, AppData)->groupContainer = _groupContainer;
    addContainer(screenContext, ID_SPORT_GROUP_CONTAINER, _groupContainer);
    // getAppData()->screenContainers[0][2] = (ScreenContainer *)sportPauseOpreationContainer;
    getAppData()->groupContainer = _groupContainer;
}

/* 处理和运动相关的计时逻辑，比如超时退出，弹框 */
static void _sportScreenTickEvent(void)
{
    HLOG_I(TAG, "%s ...", __func__);
    #define SPORT_UI_REMINDER_STAY_SEC  (6)
    AlertDialog *dialog = getAppData()->dialogHandle;
    if (getAppData()->isCloseDialogTrigger)
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_CLOSE_GPS);
        getAppData()->isCloseDialogTrigger = false;
    }
    else if (dialog && SportDialogGetInfo()->autoHide && dialog->isShowing(dialog) && getAppData()->dialogSec)
    {
        HmTime time;
        hmTime_getTime(&time);
        int32_t diff = time.sec - getAppData()->dialogSec;
        HLOG_I(TAG, "%s diff : %d %d %d", __func__, diff, time.sec, getAppData()->dialogSec);
        if(diff >= SPORT_UI_REMINDER_STAY_SEC)
        {
            dialog->dismissDialog(dialog);
            getAppData()->dialogSec = 0;
        }
    }
}

static H_Int32 _handleTickEvent(struct gr_timer *timer, void *data, int dropped)
{
    if (getAppData()->timerHandle == H_NULL)
    {
        return 0;
    }
    if(getAppData()->swimLockedTime == 3){
        gre_set_layer_visible(getAppInstance(),"SportScreen.swimLockHiteLayer",0);
    }else{
        getAppData()->swimLockedTime++;
    }

    ScreenContext *screenContext = (ScreenContext *)data;
    //run tickEvent
    _sportScreenTickEvent();

    if (getAppData()->noticeContainer->getVisiable(getAppData()->noticeContainer))
    {
        getAppData()->noticeContainer->secTickEvent(getAppData()->noticeContainer);
    }
    else
    {
        getAppData()->sportStateBarContainer->secTickEvent(getAppData()->sportStateBarContainer);

        if (getAppData()->prestartContainer->getVisiable(getAppData()->prestartContainer))
        {
            getAppData()->prestartContainer->secTickEvent(getAppData()->prestartContainer);
        }

        if (getAppData()->sportDataContainer->getVisiable(getAppData()->sportDataContainer))
        {
            getAppData()->sportDataContainer->secTickEvent(getAppData()->sportDataContainer);
        }

        if (getAppData()->sportPauseOpreationView->getVisiable(getAppData()->sportPauseOpreationView))
        {
            getAppData()->sportPauseOpreationView->secTickEvent(getAppData()->sportPauseOpreationView);
        }
        else if (getAppData()->sportHalfOpreationView->getVisiable(getAppData()->sportHalfOpreationView))
        {
            getAppData()->sportHalfOpreationView->secTickEvent(getAppData()->sportHalfOpreationView);
        }
        else if (getAppData()->sportAutoStopView->getVisiable(getAppData()->sportAutoStopView))
        {
            getAppData()->sportAutoStopView->secTickEvent(getAppData()->sportAutoStopView);
        }
    }

    //set nex timeout time
    HmTime timestamp = {0};
    hmTime_getTime(&timestamp);
    uint16_t timeout_ms = 1000 - ((timestamp.usec / 1000) % 1000);
    gre_timer_set_repeat(screenContext->_grAppContext, timer, timeout_ms,
                         0); /* 为了保险，重复可以置成1000 TIMEOUT_SEC*/
    return 0;
}
static void _pauseTimer(ScreenContext *screenContext)
{
    if (getAppData()->timerHandle)
    {
        gre_timer_pause(screenContext->_grAppContext, getAppData()->timerHandle);
        HLOG_I(TAG, "_pauseTimer");
    }
}

static void _destroyTimer(ScreenContext *screenContext)
{
    if (getAppData()->timerHandle)
    {
        gre_timer_clear_timeout(screenContext->_grAppContext,
                                &(getAppData()->timerHandle));
        getAppData()->timerHandle = H_NULL;
        HLOG_I(TAG, "destroyTimer");
    }
}

static void _creatScreenTimer(ScreenContext *screenContext)
{
    if (getAppData()->timerHandle)
    {
        return;
    }

    getAppData()->timerHandle = gre_timer_create_timeout(
                                    screenContext->_grAppContext, TIMEOUT_SEC, 0, screenContext, _handleTickEvent);

    if (getAppData()->timerHandle == H_NULL)
    {
        HLOG_W(TAG, "initScreenTimer failed");
        return;
    }
}

static void createPageView(ScreenContext *screenContext)
{
    pageScrollerMultiLayers *scroll = createPageScrollerMultiLayers(
                                          screenContext->_grAppContext);

    if (scroll)
    {
        addContainer(screenContext, ID_PAGESCROLLER_MULTILAYERS, scroll);
        scroll->init(screenContext->_grAppContext, scroll, getDisplayMetrics()->width,
                     0, 250, onPageViewScrolled);
        struct _pageScrollerPage pages[] =
        {
            {
                OPERATE_PAGE_CNT,
                {
                    "SportScreen.sportPreStartLayer",
                    "SportScreen.sportOpreationLayer",
                    "SportScreen.sportPauseOpreationLayer",
                    "SportScreen.sportAutoStopLayer",
                    "SportScreen.sportHalfOpreationLayer",
                    "SportScreen.sportGroupOpreationLayer"
                }
            },
            {
                3, {"SportScreen.sportDataLayer1",
                    "SportScreen.sportDataLayer2",
                    "SportScreen.sportDataLayer3"
                }
            },
            {1, {"SportScreen.sportMusicControlLayer"}},
        };
        scroll->initPages(scroll, pages, sizeof(pages) / sizeof(pages[0]));
        scroll->setCycle(scroll, false);
        scroll->sync(scroll, 0);
        getAppData()->viewpager = scroll;
        getAppData()->screenContext = screenContext;
    }
}

static void setPrestartViewVisiable(ScreenContext *screenContext, bool show)
{
    if (getAppData()->prestartContainer)
    {
        getAppData()->prestartContainer->setVisiable(getAppData()->prestartContainer,
                show);
    }
}

static void setOpreationViewVisiable(ScreenContext *screenContext, bool show)
{
    if (getAppData()->sportOpreationView)
    {
        getAppData()->sportOpreationView->setVisiable(getAppData()->sportOpreationView,
                show);
    }
}

static void setAutoStopViewVisiable(ScreenContext *screenContext, bool show)
{
    if (getAppData()->sportAutoStopView)
    {
        getAppData()->sportAutoStopView->setVisiable(getAppData()->sportAutoStopView,
                show);
    }
}

static void setPauseOpreationViewVisiable(ScreenContext *screenContex,
        bool show)
{
    if (getAppData()->sportPauseOpreationView)
    {
        getAppData()->sportPauseOpreationView->setVisiable(
            getAppData()->sportPauseOpreationView, show);
    }
}

static void setHalfOpreationViewVisiable(ScreenContext *screenContex, bool show)
{
    if (getAppData()->sportHalfOpreationView)
    {
        getAppData()->sportHalfOpreationView->setVisiable(
            getAppData()->sportHalfOpreationView, show);
    }
}

static void setGroupOpreationViewVisiable(ScreenContext *screenContex,
        bool show)
{
    if (getAppData()->sportGroupOpreationView)
    {
        getAppData()->sportGroupOpreationView->setVisiable(
            getAppData()->sportGroupOpreationView, show);
    }
}

static void showPauseView(ScreenContext *screenContext)
{
    if (getAppData()->reminderContainer->isShowing)
    {
        getAppData()->reminderContainer->presenter->closeReminder();
    }

    getAppData()->transAnima->pause(getAppData()->transAnima, true);
    getAppData()->sportDataViewPager->setScrollVisiable(
        getAppData()->sportDataViewPager, 0);
    setOpreationViewVisiable(screenContext, false);
    setGroupOpreationViewVisiable(screenContext, false);
    setHalfOpreationViewVisiable(screenContext, false);
    setPauseOpreationViewVisiable(screenContext, false);
    processPauseView(screenContext, getAppData());

    if (getAppData()->viewpager->curPage != 0)
    {
        getAppData()->viewpager->scrollToPage(getAppData()->viewpager, 0, true);
    }
}

static void processOperateView(ScreenContext *screenContext, void *data)
{
    // AppData *appData = (AppData *)data;
    bool isAutoStop =  getAppData()->sportAutoStopView->isAutoStop;
    setAutoStopViewVisiable(screenContext, isAutoStop);

    if (getSportsScreenPresenter()->isGroupSupported(
                getSportsScreenPresenter()->getSportType()) &&
            !getAppData()->sportGroupOpreationView->isBreakStart(
                getAppData()->sportGroupOpreationView))
    {
        setGroupOpreationViewVisiable(screenContext, !isAutoStop);
    }
    else
    {
        setOpreationViewVisiable(screenContext, !isAutoStop);
    }
}

static void processPauseView(ScreenContext *screenContext, void *data)
{
    // AppData *appData = (AppData *)data;
    bool isAutoStop = getAppData()->sportAutoStopView->isAutoStop;
    setAutoStopViewVisiable(screenContext, isAutoStop);

    if (getSportsScreenPresenter()->isHalfSupported(
                getSportsScreenPresenter()->getSportType()) &&
            !getAppData()->sportHalfOpreationView->isSecondHalfStart(
                getAppData()->sportHalfOpreationView))
    {
        setHalfOpreationViewVisiable(screenContext, !isAutoStop);
    }
    else
    {
        setPauseOpreationViewVisiable(screenContext, !isAutoStop);
    }
}

static void overView(ScreenContext *screenContext, bool isVisible)
{
    gre_set_layer_visible(screenContext->_grAppContext, "SportScreen.overLayer",
                          isVisible);
    setSBStringData(screenContext->_grAppContext, "overLayer.overHite.over",
                    System_getLocaleText("id_sport_saving"));
}

static void hidePauseView(ScreenContext *screenContext)
{
    setAutoStopViewVisiable(screenContext, false);
    setPauseOpreationViewVisiable(screenContext, false);
    setHalfOpreationViewVisiable(screenContext, false);
    processOperateView(screenContext, getAppData());
}

static void hideStatusBarView(ScreenContext *screenContext)
{
    getAppData()->sportStateBarContainer->setVisiable(
        getAppData()->sportStateBarContainer, false);
}

static void showStatusBarView(ScreenContext *screenContext)
{
    getAppData()->sportStateBarContainer->setVisiable(
        getAppData()->sportStateBarContainer, true);
}

static void hidePrestartView(ScreenContext *screenContext,
                             SportStateType SportState)
{
    setPrestartViewVisiable(screenContext, false);

    if (SportState == SPORT_STATE_RUN)
    {
        processOperateView(screenContext, getAppData());
    }
    else
    {
        processPauseView(screenContext, getAppData());
    }
}

static ScreenContext *getSportsScreenContext()
{
    return getScreenContext(getScreen(SCREEN_SPORT_NAME));
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    if (goto_sport_record)
    {
        return;
    }

    SaveData *saveData = data;
    memcpy(&getAppData()->prestartContainer->status, &saveData->prestartData, sizeof(saveData->prestartData));
    getAppData()->prestartContainer->restoreContainer(getAppData()->prestartContainer);

    // onDataRestore runs after onScreenPreCreate, so view should be restored here
    getAppData()->sportDataViewPager->initPage = saveData->verticalPage;
    getAppData()->sportDataViewPager->setCureentItem(getAppData()->sportDataViewPager, saveData->verticalPage);

    memcpy(&getAppData()->sportStateBarContainer->status, &saveData->stateBarData, sizeof(saveData->stateBarData));
    getAppData()->sportStateBarContainer->restoreContainer(getAppData()->sportStateBarContainer);

    getAppData()->groupContainer->isGroupDetailShow = saveData->isGroupDetailShow;
    getAppData()->groupContainer->isPause = saveData->isGroupPause;
    //getAppData()->groupContainer->_gGoupNumber = saveData->groupNumber;
    getAppData()->groupContainer->detailTick = saveData->groupTicket;
    getAppData()->toShowNumber = saveData->toShowNumber;
    getAppData()->choiceDistance = saveData->choiceDistance;
    // getAppData()->transAnima->showTransAnima = saveData->showTransAnima;

    if (getAppData()->groupContainer->isGroupDetailShow)
    {
        getAppData()->groupContainer->presenter->triggerLastGroupReminder();
        gre_set_layer_visible(getAppInstance(), "SportScreen.sportDataLayer2", false);
        getAppData()->sportDataViewPager->setScrollVisiable(getAppData()->sportDataViewPager, 0);
    }
    else if (getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_SUSPEND)
    {
        getAppData()->transAnima->pause(getAppData()->transAnima, false);
        getAppData()->transAnima->showTransAnima = false;
    }
}

static bool _batteryChange(EventData *data)
{
    BatteryStatus status;
    Battery_extractEventData(data, &status);

    if (status.battery_level <= 1 && status.charge_status == BATTERY_NOT_CHARGING)
    {
        SportStateType SportState = getSportsScreenPresenter()->requestSportStatus();

        if (SportState == SPORT_STATE_RUN || SportState == SPORT_STATE_SUSPEND)
        {
            HLOG_W(TAG, "battery low at sports, auto save");
            getPowerManager()->userActivity();
            getSportsScreenPresenter()->requestSave();
        }
    }

    return true;
}
static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    if (goto_sport_record)
    {
        return;
    }

    initSportsMusicContainer(screenContext);
    _creatScreenTimer(screenContext);

    System_setKeyFilter(HM_KEY_CODE_HOME,
                            HM_KEY_ACT_FILTER_CLICK | KEY_EVENT_LONG_PRESS);
    System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);

    getScreenManager()->setIdleScreen("SportScreen");
    System_enableGoDefaultScreen(H_False);
    Battery_registerListner(BATTERY_EVENT_LEVEL, getUIHandle(), _batteryChange);

    // register gps listener
    getSportsEventManager()->regCloseGPS(sport_event_closeGPS);
    getAppData()->isCloseDialogTrigger = false;
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    if (goto_sport_record)
    {
        return;
    }

    System_dispatchFirstKeyOnScreenOff(H_False);
    System_resetKeyFilter(HM_KEY_CODE_HOME);
    System_resetKeyFilter(HM_KEY_CODE_SHORTCUT);
    System_enableGoDefaultScreen(H_True);
    _unlockWakeLockHandle(screenContext);
    Battery_unregisterListner(getUIHandle(), _batteryChange);
    SportService_getInstance()->delTaskEventListener(getUIHandle(),
            autoRecogEventListener);
    SportService_getInstance()->getCtrlService()->exitSportView();
    getScreenManager()->removeIdleScreen("SportScreen");
    getSportsScreenPresenter()->destory();
    _destroyTimer(screenContext);

    if (getAppData() && getAppData()->sportsMusicContainer)
    {
        getAppData()->sportsMusicContainer->deinit(getAppData()->sportsMusicContainer);
    }

    // unregister gps listener
    getSportsEventManager()->unregCloseGPS();
}

static void onDataSave(ScreenContext *screenContext)
{
    if (goto_sport_record)
    {
        return;
    }

    HLOG_I(TAG, "%s\r\n", __func__);
    SaveData *data = H_MALLOC(sizeof(SaveData));
    memset(data, 0, sizeof(SaveData));
    memcpy(&data->prestartData, &getAppData()->prestartContainer->status,
           sizeof(data->prestartData));
    memcpy(&data->stateBarData, &getAppData()->sportStateBarContainer->status,
           sizeof(data->stateBarData));
    data->verticalPage = getAppData()->sportDataViewPager->index;;
    data->horizonPage = getAppData()->mPageIndex;
    data->isGroupDetailShow = getAppData()->groupContainer->isGroupDetailShow;
    //data->groupNumber = getAppData()->groupContainer->_gGoupNumber;
    data->isGroupPause = getAppData()->groupContainer->isPause;
    data->groupTicket = getAppData()->groupContainer->detailTick;
    data->toShowNumber = getAppData()->toShowNumber;
    data->choiceDistance = getAppData()->choiceDistance;
    // data->showTransAnima = getAppData()->transAnima->isPause(
    //                            getAppData()->transAnima);
    getScreenManager()->saveScreenData(TAG, data, sizeof(SaveData));
    H_FREE(data);
    data = NULL;
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    if (goto_sport_record)
    {
        goto_sport_record = H_False;
        return;
    }

    getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_DISMISS_DIALOG);

    if (getAppData() && getAppData()->transAnima)
    {
        getAppData()->transAnima->destory(getAppData()->transAnima);
        getAppData()->transAnima = NULL;
    }

    if (getSportsScreenPresenter()->isSportSupportDisbaleTP(
                getSportsScreenPresenter()->getSportType()))
    {
        API_setTpQuickSwitch(true);
    }

    if (getAppData()->sportPauseOpreationView)
    {
        getAppData()->sportPauseOpreationView->setVisiable(
            getAppData()->sportPauseOpreationView, 0);
    }

    if (getAppData()->sportAutoStopView)
    {
        getAppData()->sportAutoStopView->setVisiable(getAppData()->sportAutoStopView,
                0);
    }

    if (getAppData()->sportHalfOpreationView)
    {
        getAppData()->sportHalfOpreationView->setVisiable(
            getAppData()->sportHalfOpreationView, 0);
    }

    freeScreenData(screenContext);
    removeAllContainers(screenContext);
}

static void onScreenResume(ScreenContext *screenContext)
{
    if (goto_sport_record)
    {
        return;
    }

    getAppData()->sportDataViewPager->setUpdateMode(
        getAppData()->sportDataViewPager, VIEWPAGE_SCROLL_UPDATE_ALL);

    switch (getSportsScreenPresenter()->requestSportStatus())
    {
        case SPORT_STATE_PRESTART:
            /* 持回锁，防止锁屏 */
            getScreenManager()->setIdleScreen("SportScreen");
            System_dispatchFirstKeyOnScreenOff(H_True);
            System_enableGoDefaultScreen(H_False);

            if (getAppData()->prestartContainer->status.GPSFailedIsShow)
            {
                return;
            }

            _lockWakeLockHandle(getSportsScreenContext());
            return;

        case SPORT_STATE_RUN:
        {
            HLOG_I(TAG, "%s enterSportView SPORT_STATE_RUN\r\n", __func__);
            System_setKeyFilter(HM_KEY_CODE_HOME,
                                HM_KEY_ACT_FILTER_CLICK | KEY_EVENT_LONG_PRESS);
            System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
            SportService_getInstance()->getCtrlService()->enterSportView();
            break;
        }

        case SPORT_STATE_SUSPEND:
        {
            HLOG_I(TAG, "%s enterSportView SPORT_STATE_SUSPEND\r\n", __func__);
            System_setKeyFilter(HM_KEY_CODE_HOME,
                                HM_KEY_ACT_FILTER_CLICK | KEY_EVENT_LONG_PRESS);
            System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
            getAppData()->transAnima->pause(getAppData()->transAnima, false);
            getAppData()->transAnima->showTransAnima = false;
            /* 标记进入运动 */
            SportService_getInstance()->getCtrlService()->enterSportView();
            break;
        }

        default:
            return;
    }
}

static void onScreenPause(ScreenContext *screenContext)
{
    if (goto_sport_record)
    {
        return;
    }

    getAppData()->sportDataViewPager->setUpdateMode(
        getAppData()->sportDataViewPager, VIEWPAGE_SCROLL_UPDATE_CUREENT);

    switch (getSportsScreenPresenter()->requestSportStatus())
    {
        case SPORT_STATE_PRESTART:
            /* 释放锁，允许回表盘 */
            System_enableGoDefaultScreen(H_True);
            System_dispatchFirstKeyOnScreenOff(H_False);
            _unlockWakeLockHandle(screenContext);
            getScreenManager()->removeIdleScreen("SportScreen");
            return;

        default:
            /* 锁屏 */
            if (getSportsScreenPresenter()->getSupportScreenLock())
            {
                getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_LOCK_SCREEN);
            }

            /* 回到活动页 */
            _SwitchToActivePage(false);
            return;
    }
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    HLOG_I(TAG, "%s, key : %d, action : %d\r\n", __func__, info->code, info->action);

    if(gre_get_layer_visible(getAppInstance(),"SportScreen.swimLockHiteLayer") == 1){
        return true;
    }

    if (getAppData()->pickerHandle != NULL &&
            info->code == HM_KEY_CODE_HOME &&
            info->action == HM_KEY_ACTION_CLICK)
    {
        HLOG_I(TAG,
               "%s, key : %d, action : %d, SportCalibrationPicker is shwoing, ignored\r\n",
               __func__, info->code, info->action);
        return true;
    }

    if (getAppData()->dialogHandle != NULL &&
            getAppData()->dialogHandle->isShowing(getAppData()->dialogHandle) &&
            info->code == HM_KEY_CODE_HOME &&
            info->action == HM_KEY_ACTION_CLICK)
    {
        HLOG_I(TAG, "%s, key : %d, action : %d, dialogHandle is shwoing, ignored\r\n",
               __func__, info->code, info->action);
        return getAppData()->dialogHandle->container.handleKeyEvent(screenContext,
                &getAppData()->dialogHandle->container, info);
    }

    if (info->code == HM_KEY_CODE_HOME && info->action == HM_KEY_ACTION_CLICK)
    {
        if (getSportsScreenPresenter()->requestSportStatus() == SPORT_STATE_PRESTART)
        {
            getSportsScreenPresenter()->requestGoHome();
            return true;
        }
    }

    if (getAppData()->reminderContainer->isShowing)
    {
        if (getAppData()->reminderContainer->container.handleKeyEvent(screenContext,
                &getAppData()->reminderContainer->container, info))
        {
            return true;
        }
    }

    if (getAppData()->sportLockContainer->status.isLock)
    {
        if (!getSportsScreenPresenter()->isSportSupportDisbaleTP(getSportsScreenPresenter()->getSportType()))
        {
            return getAppData()->sportLockContainer->container.handleKeyEvent(screenContext, &getAppData()->sportLockContainer->container, info);
        }
        else
        {
            if(info->action == HM_KEY_ACTION_CLICK)
            {
                // 操作页面，按home键，执行解锁
                if(info->code == HM_KEY_CODE_HOME && getAppData()->mPageIndex == 0)
                {
                    return getAppData()->sportLockContainer->container.handleKeyEvent(screenContext, &getAppData()->sportLockContainer->container, info);
                }
                // 数据页面，按home键，执行解锁
                if(info->code == HM_KEY_CODE_HOME && getAppData()->mPageIndex == 1)
                {
                    getAppData()->sportLockContainer->container.handleKeyEvent(screenContext, &getAppData()->sportLockContainer->container, info);

                    // 不支持数字表冠，只执行解锁
                    if(!device_hasCrown())
                    {
                        return true;
                    }
                }
                // 操作页面，按back键，执行解锁并其他
                else if(info->code == HM_KEY_CODE_SHORTCUT && getAppData()->mPageIndex == 0)
                {
                    getAppData()->sportLockContainer->container.handleKeyEvent(screenContext, &getAppData()->sportLockContainer->container, info);
                }
                HLOG_I(TAG, "%s %d\r\n", __func__, getAppData()->mPageIndex);
            }
        }
    }

    if (getAppData()->groupContainer->isGroupDetailShow || getAppData()->groupContainer->isGroupNumberShowing)
    {
        if (getAppData()->groupContainer->container.handleKeyEvent(screenContext,
                &getAppData()->groupContainer->container, info))
        {
            return true;
        }
    }

    for (H_Uint8 i = 0; i < OPERATE_PAGE_CNT; i++)
    {
        ScreenContainer *container = (ScreenContainer *)
                                     getAppData()->screenContainers[getAppData()->mPageIndex][i];

        if (container &&
                !container->handleKeyEvent(screenContext, container, info))
        {
            continue;
        }
        else
        {
            if (container)
            {
                return true;
            }
            else
            {
                break;
            }
        }
    }

    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    if (getAppData()->mPageIndex != 0
            && !getAppData()->reminderContainer->isShowing)
    {
        return false;
    }

    // SportGpsNoticeLayer on the top layer
    if (eventId == GESTURE_LEFT &&
        getAppData()->noticeContainer != 0  &&
        getAppData()->noticeContainer->getVisiable(getAppData()->noticeContainer))
    {
        return true;
    }

    if (eventId == GESTURE_RIGHT &&
        getAppData()->noticeContainer != 0  &&
        getAppData()->noticeContainer->getVisiable(getAppData()->noticeContainer))
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_STATUS_BAR);
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SHOW_PRESTART);
        return true;
    }

    if (getAppData()->dialogHandle != H_NULL
            && getAppData()->dialogHandle->isShowing(getAppData()->dialogHandle))
    {
        return true;
    }

    if (eventId == GESTURE_RIGHT)
    {
        // ## not support GESTURE_RIGHT
        /* if (getAppData()->dialogHandle && getAppData()->dialogHandle->isShowing(getAppData()->dialogHandle))
        {
            getAppData()->dialogHandle->container.handleGestureEvent(screenContext, &getAppData()->dialogHandle->container, id, eventId);
            return true;
        } */
        if (getAppData()->reminderContainer->isShowing
                && getAppData()->reminderContainer->isFullScreen)
        {
            getAppData()->reminderContainer->container.handleGestureEvent(screenContext,
                    &getAppData()->reminderContainer->container, id, eventId);
            return true;
        }

        if (getAppData()->pickerHandle != NULL)
        {
            return true;
        }

        HLOG_I(TAG, "sport status:%d _isInSetting:%d\r\n",
               getSportsScreenPresenter()->requestSportStatus(), _isInSetting());

        switch (getSportsScreenPresenter()->requestSportStatus())
        {
            case SPORT_STATE_PRESTART:

                // only in prestart page top
                if (_isInSetting() == false)
                {
                    getSportsScreenPresenter()->requestExit();
                }

                return true;

            default:
                return false;
        }
    }
    else if (eventId == GESTURE_UP)
    {
        if (getAppData()->reminderContainer->isShowing
                && !getAppData()->reminderContainer->isFullScreen)
        {
            getAppData()->reminderContainer->container.handleGestureEvent(screenContext,
                    &getAppData()->reminderContainer->container, id, eventId);
            return true;
        }
    }

    return false;
}

static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    HLOG_I(TAG, "%s, id: %s, event: %s\r\n", __func__, id, event);

    if(gre_get_layer_visible(getAppInstance(),"SportScreen.swimLockHiteLayer") == 1){
        return true;
    }

    if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        if (getAppData()->pickerHandle != NULL)
        {
            getAppData()->pickerHandle->container.handleEvent(screenContext,
                    (ScreenContainer *)getAppData()->pickerHandle, id, event);
            return true;
        }

        if (getAppData()->mPageIndex == 0)
        {
            //运动状态页表冠
            return false;
        }
        else if (getAppData()->mPageIndex == 1)
        {
            //运动数据页表冠
            if (getAppData()->sportDataContainer != NULL)
            {
                ScreenContainer *container = (ScreenContainer *)
                                             getAppData()->sportDataContainer;
                container->handleEvent(screenContext,
                                       (ScreenContainer *)getAppData()->sportDataContainer, id, event);
                return false;
            }
        }
        else if (getAppData()->mPageIndex == 2)
        {
            //音乐控制表冠
            if (getAppData()->sportsMusicContainer != NULL)
            {
                ScreenContainer *container = (ScreenContainer *)
                                             getAppData()->sportsMusicContainer;
                container->handleEvent(screenContext,
                                       (ScreenContainer *)getAppData()->sportsMusicContainer, id, event);
                return false;
            }
        }
    }

    if (getAppData()->pickerHandle != NULL)
    {
        getAppData()->pickerHandle->container.handleEvent(screenContext,
                &getAppData()->pickerHandle->container, id, event);
        return true;
    }
    for (H_Uint8 i = 0; i < OPERATE_PAGE_CNT; i++)
    {
        ScreenContainer *container = (ScreenContainer *)
                                     getAppData()->screenContainers[getAppData()->mPageIndex][i];

        if (container &&
                !container->handleEvent(screenContext, container, id, event))
        {
            continue;
        }
        else
        {
            if (container)
            {
                return true;
            }
            else
            {
                break;
            }
        }
    }

    return false;
}

static int g_dragType = DRAG_NONE;
static int g_pressDown = false;
static SPoint g_pressPos = {.x = 0, .y = 0};

static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    if(gre_get_layer_visible(getAppInstance(),"SportScreen.swimLockHiteLayer") == 1){
        return true;
    }
    //HLOG_I(TAG, "%s id : %s\r\n", __func__, id);
    if ((isPreStartAnimating() && eventId != TOUCH_UP)
            || getAppData()->reminderContainer->isShowing)
    {
        return false;
    }

    if ((eventId == TOUCH_SELECT || eventId == TOUCH_MOVE) &&
        getAppData()->noticeContainer &&
        getAppData()->noticeContainer->getVisiable(getAppData()->noticeContainer))
    {
        getAppData()->noticeContainer->container.handleTouchEvent(screenContext,
                &getAppData()->noticeContainer->container, id, eventId, info);
        return true;
    }

    if (getAppData()->dialogHandle != H_NULL
            && getAppData()->dialogHandle->isShowing(getAppData()->dialogHandle))
    {
        getAppData()->dialogHandle->container.handleTouchEvent(screenContext,
                &getAppData()->dialogHandle->container, id, eventId, info);
        return true;
    }

    if (getAppData()->pickerHandle != NULL)
    {
        getAppData()->pickerHandle->container.handleTouchEvent(screenContext,
                &getAppData()->pickerHandle->container, id, eventId, info);
        return true;
    }

    // if (getAppData()->groupContainer->groupPicker != NULL )
    // {
    //     getAppData()->groupContainer->groupPicker->container.handleTouchEvent(screenContext, &getAppData()->groupContainer->groupPicker->container, id, eventId, info);
    //     return true;
    // }
    if (getAppData()->sportLockContainer->status.isLock
            || getAppData()->sportLockContainer->status.isShow)
    {
        if (getAppData()->sportLockContainer->container.handleTouchEvent(screenContext,
                &getAppData()->sportLockContainer->container, id, eventId, info))
        {
            return true;
        }
    }

    if (getAppData()->groupContainer->isGroupDetailShow)
    {
        if (getAppData()->groupContainer->container.handleTouchEvent(screenContext,
                &getAppData()->groupContainer->container, id, eventId, info))
        {
            return true;
        }
    }

    if (g_dragType != DRAG_HOR)
    {
        for (H_Uint8 i = 0; i < OPERATE_PAGE_CNT; i++)
        {
            ScreenContainer *container = (ScreenContainer *)
                                         getAppData()->screenContainers[getAppData()->mPageIndex][i];

            if (container &&
                    !container->handleTouchEvent(screenContext, container, id, eventId, info))
            {
                continue;
            }
            else
            {
                if (container)
                {
                    return true;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if (eventId == TOUCH_SELECT)
    {
        return true;
    }

    if (isTouchEventToScrollerPages(id))
    {
        pageScrollerMultiLayers *scroll = (pageScrollerMultiLayers *)findContainer(
                                              getSportsScreenContext(), ID_PAGESCROLLER_MULTILAYERS);

        if ((scroll == NULL)
                || (&scroll->container == NULL)) //if (!scroll && !(&scroll->container))
        {
            return false;
        }

        if (eventId == TOUCH_DOWN)
        {
            g_pressDown = true;
            g_pressPos.x = info->x;
            g_pressPos.y = info->y;
            scroll->container.handleTouchEvent(getSportsScreenContext(), &scroll->container,
                                               id, eventId, info);
        }
        else if (eventId == TOUCH_MOVE)
        {
            if (g_pressDown)
            {
                int deltaX = info->x - g_pressPos.x;
                int deltaY = info->y - g_pressPos.y;

                if ((abs(deltaX) > SCROLL_DRAG_THREHOLD) && (g_dragType == DRAG_NONE))
                {
                    g_dragType = DRAG_HOR;
                    getAppData()->sportDataViewPager->setScrollVisiable(
                        getAppData()->sportDataViewPager, 0);
                }
                else if ((abs(deltaY) > SCROLL_DRAG_THREHOLD) && (g_dragType == DRAG_NONE))
                {
                    g_dragType = DRAG_VER;
                }
            }

            if (g_dragType == DRAG_HOR && _isInSetting() == false)
            {
                scroll->container.handleTouchEvent(getSportsScreenContext(), &scroll->container,
                                                   id, eventId, info);
            }
        }
        else if (eventId == TOUCH_UP)
        {
            if (g_dragType == DRAG_HOR && _isInSetting() == false)
            {
                scroll->container.handleTouchEvent(getSportsScreenContext(), &scroll->container,
                                                   id, eventId, info);
            }

            g_dragType = DRAG_NONE;
            g_pressDown = false;
        }
    }

    return true;
}
